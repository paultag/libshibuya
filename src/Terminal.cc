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

#include <malloc.h>
#include <iostream>
#include <pty.h>
#include <stdlib.h>
#include <string.h>

#include "boothby.hh"
#include "Terminal.hh"

#include "conf/term.hh"

#define GET_OFFSET(x, y) ( ( y * this->width ) + x )
/* This is in here because it's:
   a) only used in here
   b) very dependent on this exact impl */

void Terminal::_init_Terminal(int width, int height) {
	this->graph  = false;
	this->width  = width;
	this->height = height;
	this->cX     = 0;
	this->cY     = 0;
	this->cMode  = 0x70;
	/* 11100000 = 0x70 (112)
	   FFF BBB */

	this->scrollframe_floor = this->height;
	this->scrollframe_top   = 0;

	this->pty    = -1;

	this->special   = false;
	this->maxesc    = 128; /* Hopefully more then we ever need. */
	this->escape    = (char *) malloc(sizeof(char) * this->maxesc);
	this->escape[0] = '\0';
	
	this->chars = (TerminalCell*)
		malloc(sizeof(TerminalCell) * (width * height));
 
	for ( int i = 0; i < ( height * width ); ++i ) {
		this->chars[i].ch   = ' ';
		this->chars[i].attr = this->cMode;
	}
}

Terminal::~Terminal() {

}

Terminal::Terminal() {
	this->_init_Terminal(80, 25);
}

Terminal::Terminal( int width, int height ) {
	this->_init_Terminal(width, height);
}

void Terminal::erase_to_from( int iX, int iY, int tX, int tY ) {

	iY--;
	tY--;

	int from = GET_OFFSET(iX, iY);
	int to   = GET_OFFSET(tX, tY);

	for ( int i = from - 1; i < to; ++i ) {
		this->chars[i].ch   = ' ';
		this->chars[i].attr = 0x70;
	}
}

void Terminal::scroll_up() {
	for (
		int iy = this->scrollframe_top + 1;
		iy < this->scrollframe_floor;
		iy++
	) {
		for ( int ix = 0; ix < this->width; ++ix ) {
			int thisChar = GET_OFFSET(ix, iy);
			int lastChar = GET_OFFSET(ix, (iy - 1));
			this->chars[lastChar].ch   = this->chars[thisChar].ch;
			this->chars[lastChar].attr = this->chars[thisChar].attr;
		}
	}
	for ( int ix = 0; ix < this->width; ++ix ) {
		int offset = GET_OFFSET( ix, (this->scrollframe_floor - 1) );
		this->chars[offset].ch = ' ';
		this->chars[offset].attr = 0x70;
	}
}

void Terminal::render( WINDOW * win ) {
	for ( int iy = 0; iy < this->height; ++iy ) {
		for ( int ix = 0; ix  < this->width; ++ix ) {
			int offset = GET_OFFSET(ix, iy);
			mvwaddch(win, iy, ix, this->chars[offset].ch);
		}
	}
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
		execl("/bin/sh", "/bin/sh", "-c", command, NULL); // XXX: Choose shell better
		std::cerr << "Oh, crap. Failed to fork." << std::endl;
		exit(127);
	}
	/* if we got here we are the parent process */
	this->childpid = childpid;
	return childpid;
}

void Terminal::poke() {
	fd_set ifs;
	struct timeval tvzero;
	char buf[512];
	int bytesread;
	int n = 5; // XXX: Fix?

	if (this->pty < 0)
		return;
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

	int ix = this->cX;
	int iy = this->cY;
	/*
	 * XXX: Why was the math using this->cX failing?
	 * for some reason we have to bring it into the local
	 * scope...
	 */
	int offset = GET_OFFSET(ix, iy)

	this->chars[offset].ch   = c;
	this->chars[offset].attr = this->cMode;
	this->advance_curs();
}

void Terminal::type( char c ) {
	write(this->pty, &c, 1);
}

void Terminal::advance_curs() {
	this->cX++;

	if ( this->width <= this->cX ) {
		this->cX = 0;
		this->cY++;
	}

	if ( this->height <= this->cY ) {
		this->cY = (this->height - 1);
		this->scroll_up();
	}
}

int Terminal::get_width() {
	return this->width;
}

int Terminal::get_height() {
	return this->height;
}
