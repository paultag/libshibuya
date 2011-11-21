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

#include "NcursesTerminal.hh"
#include "Shibuya.hh"

#include <iostream>
#include <malloc.h>
#include <signal.h>

NcursesTerminal::NcursesTerminal() {
	SDEBUG << "Default ncurses Constructor" << std::endl;
	this->_init_NcursesTerminal(80, 25, 0, 0);
	this->_init_ANSITerminal();
	this->_init_Terminal( 80, 25 );
}

NcursesTerminal::NcursesTerminal( int width, int height ) {
	SDEBUG << "Width / Height ncurses Constructor" << std::endl;
	this->_init_NcursesTerminal(width, height, 0, 0);
	this->_init_ANSITerminal();
	this->_init_Terminal( width, height );
}

NcursesTerminal::NcursesTerminal( int width, int height, int x, int y ) {
	SDEBUG << "Width / Height / x / y ncurses Constructor" << std::endl;
	this->_init_NcursesTerminal(width, height, x, y);
	this->_init_ANSITerminal();
	this->_init_Terminal( width, height );
}

void NcursesTerminal::_init_NcursesTerminal(
	int width, int height, int x, int y
) {
	SDEBUG << "ncurses init" << std::endl;
	this->pane = new Pane((width + 2), (height + 2), x, y);
	this->pane->setTitle( "Terminal ID: (" + this->pane->getId() + ")" );
	this->tainted = true;
}

NcursesTerminal::~NcursesTerminal() {
	SDEBUG << "ncurses terminal destructor" << std::endl;
	delete this->pane;
}

bool NcursesTerminal::render( WINDOW * win ) {
	if ( ! this->tainted )
		return false;

	SDEBUG << "Tainted display. Rendering window" << std::endl;
	
	this->pane->render_frame();
	
	for ( int iy = 0; iy < this->height; ++iy ) {
		for ( int ix = 0; ix < this->width; ++ix ) {
			int offset = GET_OFFSET(ix, iy);

			/* By default, we'll disable everything. */
			wattroff( win, A_BOLD );

			char attrs = this->chars[offset].attr;
			if ( SHIBUYA_ATTR_HAS_BOLD(attrs) ) {
				wattron(win, A_BOLD);
			}

			mvwaddch(win, ( iy + 1 ), ( ix + 1 ),
				this->chars[offset].ch);
		}
	}
	
	wmove( win, this->cY + 1, this->cX + 1 );
	
	this->tainted = false;
	return true;
}

bool NcursesTerminal::render() {
	return this->render(this->pane->getWindow());
}

void NcursesTerminal::insert( unsigned char c ) {
	// SDEBUG << "Inserting: " << (int)c << std::endl;
	this->tainted = true;
	ANSITerminal::insert( c );
}

void NcursesTerminal::sigwinch() {
	/* XXX: Globalize this */
	SDEBUG << "Sending WINCH " << this->childpid << std::endl;
	pid_t pg = tcgetpgrp( this->pty );
	TerminalSize ts = { 0, 0, 0, 0 };
	
	ts.ws_row    = this->height;
	ts.ws_col    = this->width;
	ts.ws_xpixel = this->height;
	ts.ws_ypixel = this->width;
	
	this->scroll_frame_bottom = this->height;
	this->scroll_frame_top    = 0;
	
	int r = ioctl(this->pty, TIOCSWINSZ, (char * )&ts);
	/* This pseudo code taken from xterm. */
	kill( pg, SIGWINCH );
	SDEBUG << "Sent SIGWINCH to child. IO output is: " << r << std::endl;
}

void NcursesTerminal::resize( int x, int y ) {
	TerminalCell * tcTmp =
		(TerminalCell*) malloc(sizeof(TerminalCell) * (x * y));
	
	/* Let's do some costly allocation */
	
	for ( int i = 0; i < ( x * y ); ++i ) {
		tcTmp[i].ch   = ' ';
		tcTmp[i].attr = SHIBUYA_DEFAULT_CMODE;
	}
	
	/* And, now, let's copy over stuff in-range. */
	/* XXX: Fix this when the cX / cY stuff is fixed.
	
	int resizeXMin = ( x < this->width )  ? x : this->width;
	int resizeYMin = ( y < this->height ) ? y : this->height;
	
	SDEBUG << "Limiting XY: " << resizeXMin << ", " << resizeYMin << std::endl;
	
	for ( int iy = 0; iy < resizeYMin; ++iy ) {
		for ( int ix = 0; ix < resizeXMin; ++ix ) {
			int hostOffset = GET_OFFSET( ix, iy );
			int nextOffset = (( x * iy ) + ix );
			SDEBUG << "XY: " << ix << "/" << iy << " -- " <<
				hostOffset << ", " << nextOffset << std::endl;
			tcTmp[nextOffset] = this->chars[hostOffset];
		}
	} */
	
	free( this->chars );
	this->chars = tcTmp;
	this->width  = x;
	this->height = y;
	
	this->cX = 0;
	this->cY = 0;/* Handle this smarter? */
	
	this->pane->resize( ( x + 2 ), (y + 2) );
	this->tainted = true;
	
	/* Now, let's tell the process group what's up */
	this->sigwinch();
}

void NcursesTerminal::move_to( int x, int y ) {
	this->pane->move_to( x, y );
}
