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

NcursesTerminal::NcursesTerminal() {
	this->_init_NcursesTerminal(80, 25, 0, 0);
	this->_init_ANSITerminal();
	this->_init_Terminal( 80, 25 );
}

NcursesTerminal::~NcursesTerminal() {}

NcursesTerminal::NcursesTerminal( int width, int height ) {
	this->_init_NcursesTerminal(width, height, 0, 0);
	this->_init_ANSITerminal();
	this->_init_Terminal( width, height );
}

NcursesTerminal::NcursesTerminal( int width, int height, int x, int y ) {
	this->_init_NcursesTerminal(width, height, x, y);
	this->_init_ANSITerminal();
	this->_init_Terminal( width, height );
}

void NcursesTerminal::_init_NcursesTerminal(
	int width,
	int height,
	int x,
	int y
) {
	this->pane = new Pane(width, height, x, y);
}

bool NcursesTerminal::render( WINDOW * win ) {
	if ( ! this->tainted )
		return false;
	for ( int iy = 0; iy < this->height; ++iy ) {
		for ( int ix = 0; ix  < this->width; ++ix ) {
			int offset = GET_OFFSET(ix, iy);
			mvwaddch(win, iy, ix, this->chars[offset].ch);
		}
	}
	this->tainted = false;
	return true;
}

bool NcursesTerminal::render() {
	return this->render(this->pane->getWindow());
}

void NcursesTerminal::insert( unsigned char c ) {
	this->tainted = true;
	ANSITerminal::insert( c );
}
