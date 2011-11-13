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

#include <ansiescape.hh>
#include <iostream>

#include "ANSITerminal.hh"
#include "Terminal.hh"

ANSITerminal::ANSITerminal() {
	this->_init_ANSITerminal();
	this->_init_Terminal( 80, 25 );
}
ANSITerminal::ANSITerminal( int width, int height ) {
	this->_init_ANSITerminal();
	this->_init_Terminal( width, height );
}
ANSITerminal::~ANSITerminal() {}

void ANSITerminal::_init_ANSITerminal() {
	ansi_escape_parser_reset();
}

void ANSITerminal::_handle_escape( ansi_sequence * last ) {
	/* char mode               = last->mode;
	std::vector<int> * seqs = last->values; */
}

void ANSITerminal::insert( unsigned char c ) {
	ANSI_ESCAPE_PARSE_T res = ansi_escape_parser_feed( c );
	ansi_sequence * last = NULL;
	
	switch ( res ) {
		case ANSI_ESCAPE_PARSE_OK:
			last = ansi_escape_get_last_sequence();
			this->_handle_escape( last );
			break;
		case ANSI_ESCAPE_PARSE_BAD:
			ansi_escape_parser_reset();
			/* If we don't reset, the following bytes are "INCOMPLETE" */
			Terminal::insert(c);
			break;
		case ANSI_ESCAPE_PARSE_INCOMPLETE:
			break;
	}
}
