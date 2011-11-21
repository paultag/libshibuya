/*
 * Copyright (C) 2011, Paul Tagliamonte <tag@pault.ag>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <iostream>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <pty.h>

#include "conf/term.hh"
#include "Exceptions.hh"
#include "Terminal.hh"
#include "Shibuya.hh"

void Terminal::_init_Terminal(int width, int height) {
	this->width  = width;
	this->height = height;
	this->cX     = 0;
	this->cY     = 0;
	this->cMode  = SHIBUYA_DEFAULT_CMODE;
	this->scroll_frame_bottom = this->height;
	this->scroll_frame_top   = 0;
	this->pty    = -1;
	this->chars = (TerminalCell*)
		malloc(sizeof(TerminalCell) * (width * height));
	this->erase_to_from( 0, 0, width, height ); 
}

Terminal::~Terminal() {
	free( this->chars );
	/* This segfaults.... */
}

Terminal::Terminal() {
	this->_init_Terminal(80, 25);
}

Terminal::Terminal( int width, int height ) {
	this->_init_Terminal(width, height);
}

void Terminal::erase_to_from( int iX, int iY, int tX, int tY ) {
	int from = GET_OFFSET(iX, iY);
	int to   = GET_OFFSET(tX, tY);

	to = ( ( this->height * this->width ) < to ) ?
		((this->height * this->width) - 1) : to ;

	SDEBUG << "Erasing from/to: " << iX << ", " << iY << "(" << from
		<< ") -> " << tX << ", " << tY
		<< "(" << to << ")" << std::endl;

	for ( int i = from; i <= to; ++i ) {
		this->chars[i].ch   = ' ';
		this->chars[i].attr = SHIBUYA_DEFAULT_CMODE;
	}
}

void Terminal::scroll_up() {
	for (
		int iy = this->scroll_frame_top + 1;
		iy < this->scroll_frame_bottom;
		++iy
	) {
		for ( int ix = 0; ix < this->width; ++ix ) {
			int thisChar = GET_OFFSET(ix, iy);
			int lastChar = GET_OFFSET(ix, (iy - 1));
			this->chars[lastChar].ch   = this->chars[thisChar].ch;
			this->chars[lastChar].attr = this->chars[thisChar].attr;
		}
	}
	for ( int ix = 0; ix < this->width; ++ix ) {
		int offset = GET_OFFSET( ix, (this->scroll_frame_bottom - 1) );
		this->chars[offset].ch = ' ';
		this->chars[offset].attr = SHIBUYA_DEFAULT_CMODE;
	}
}

void Terminal::scroll_down() {
	/*
	 * ----------------------+--------------------+--------+
	 * A A A A A A A A A A A | Blank A line       | Step 5 | < idex 0
	 * B B B B B B B B B B B | Copy  A line over  | Step 4 | < idex 1
	 * C C C C C C C C C C C | Copy  B line over  | Step 3 | < idex 2
	 * D D D D D D D D D D D | Copy  C line over  | Step 2 | < idex 3
	 * E E E E E E E E E E E | Copy  D line over  | Step 1 | < idex 4
	 * ----------------------+--------------------+--------+
	 */
	for (
		int iy = this->scroll_frame_bottom - 1; // line "E"
		iy > this->scroll_frame_top; // line "A"
		--iy
	) {
		for ( int ix = 0; ix < this->width; ++ix ) {
			int thisChar = GET_OFFSET(ix, (iy - 1)); // for "E" this is "D"
			int lastChar = GET_OFFSET(ix, iy);       // for "E" this is "E"
			this->chars[lastChar].ch   = this->chars[thisChar].ch;
			this->chars[lastChar].attr = this->chars[thisChar].attr;
		}
	}
	
	for ( int ix = 0; ix < this->width; ++ix ) {
		int thisChar = GET_OFFSET(ix, this->scroll_frame_top);
		this->chars[thisChar].ch   = ' ';
		this->chars[thisChar].attr = SHIBUYA_DEFAULT_CMODE;
	}
}

void Terminal::sigint() {
	SDEBUG << "Sending SIGINT" << std::endl;
	this->type( 0x03 );
}

pid_t Terminal::fork( const char * command ) {

	struct winsize ws;

	ws.ws_row    = this->height;
	ws.ws_col    = this->width;
	ws.ws_xpixel = 0;
	ws.ws_ypixel = 0;

	pid_t childpid = forkpty(&this->pty, NULL, NULL, &ws);
	if (childpid < 0) return -1;

	if (childpid == 0) {
		setenv("TERM", TERMINAL_ENV_NAME, 1);
		execl(command, command, NULL);
		std::cerr << "Failed to fork." << std::endl;
		exit(127);
	}
	/* if we got here we are the parent process */
	this->childpid = childpid;
	return childpid;
}

void Terminal::poke() {
	/*
	 *  A good deal of this was inspired by `librote'. Mad gangster-props to
	 * librote.
	 */
	if (this->pty < 0)
		return;
	
	fd_set         ifs;
	struct timeval tvzero;
	
	char  buf[512];
	int   bytesread;
	int   n = 5; // XXX: Fix?
	int   status;
	
	pid_t result = waitpid(this->childpid, &status, WNOHANG);
	
	switch ( result ) {
		case 0:
			/* If waitpid() was invoked with WNOHANG set in options, and there
			 * are children specified by pid for which status is not available,
			 * waitpid() returns 0 */
			break;
		case -1:
			/* Otherwise, it returns -1 and sets errno to one of the following
			 * values */
			SDEBUG << "Error in PID wait." << std::endl;
			switch ( errno ) {
				case ECHILD:
					/* The process or process group specified by pid does not
					 * exist or is not a child of the calling process. */
					SDEBUG << " => pid (" << this->childpid <<
						") does not exist." << std::endl;
					break;
				case EFAULT:
					/* stat_loc is not a writable address. */
					SDEBUG << " => pid is out of our segment" << std::endl;
					break;
				case EINTR:
					/* The function was interrupted by a signal. The value of
					 * the location pointed to by stat_loc is undefined. */
					SDEBUG << " => wait() hit with a signal" << std::endl;
					break;
				case EINVAL:
					/* The options argument is not valid. */
					SDEBUG << " => wait() arguments not valid" << std::endl;
					break;
				case ENOSYS:
					/* pid specifies a process group (0 or less than -1), which
					 * is not currently supported. */
					SDEBUG << " => pid is less then 1" << std::endl;
					break;
			}
			break;
		default:
			/* Our child died :'( */
			SDEBUG << "Child PID: " << this->childpid << " has died."
				<< std::endl;
			throw new DeadChildException();
			// ^^ This will force whatever else to clean up the mess
			//                       ( if you will pardon the term )
			break;
	}
	
	/* This bit here taken from librote almost directly */
	while (n--) {
		FD_ZERO(&ifs);
		FD_SET(this->pty, &ifs);
		tvzero.tv_sec  = 0;
		tvzero.tv_usec = 0;
		
		if (select(this->pty + 1, &ifs, NULL, NULL, &tvzero) <= 0)
			return;
		
		bytesread = read(this->pty, buf, 512);
		if (bytesread <= 0)
			return;

		for ( int i = 0; i < bytesread; ++i )
			this->insert( buf[i] );
	}
}


void Terminal::insert( unsigned char c ) {
	switch ( c ) {
		case '\r':
			this->cX = 0;
			break;
		case '\n':
			this->newline();
			break;
		case 8: /* Backspace */
			SDEBUG << "Backspace " << this->cX << " -> ";
			if ( this->cX > 0 )
				--this->cX;
			SDEBUG << this->cX << std::endl;
			break;
		case 9: /* Tab */
			SDEBUG << "TAB!" << std::endl;
			while ( ( this->cX % 8 ) != 0 )
				this->insert(' ');
			break;
	}
	
	/* No need to return above, because of below :) */
	
	if ( c < 32 )
		return;
	
	/* Alright. We've got something printable. Let's deal with it. */
	int ix = this->cX;
	int iy = this->cY;
	/*
	 * XXX: Why was the math using this->cX failing?
	 * for some reason we have to bring it into the local
	 * scope...
	 */
	int offset = GET_OFFSET(ix, iy);

	this->chars[offset].ch   = c;
	this->chars[offset].attr = this->cMode;

	this->cX++;
	this->bounds_check();

}

void Terminal::type( char c ) {
	SDEBUG << "Writing: " << (int)c << std::endl;
	write(this->pty, &c, 1);
}

void Terminal::newline() {
	this->cX = 0;
	this->cY++;

	if ( this->scroll_frame_bottom <= this->cY ) {
		this->cY = (this->scroll_frame_bottom - 1);
		this->scroll_up();
	}
}

void Terminal::bounds_check() {
	if ( this->width < this->cX ) {
		this->newline();
	}
	if ( this->cX < 0 ) {
		this->cX = 0;
	}
}

int Terminal::get_width() {
	return this->width;
}

int Terminal::get_height() {
	return this->height;
}

void Terminal::delete_line( int idex ) {
	int oldfloor = scroll_frame_top;
	this->scroll_frame_top = idex;
	this->scroll_up();
	scroll_frame_top = oldfloor;
}

void Terminal::insert_line( int idex ) {
	int oldfloor = scroll_frame_top;
	this->scroll_frame_top = idex;
	this->scroll_down();
	scroll_frame_top = oldfloor;
}
