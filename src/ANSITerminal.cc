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
#include <ansicsi.hh>
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
	char mode               = last->mode;
	std::vector<int> * seqs = last->values;
	
	int move_steps = 1;
	
	switch ( mode ) {
		case CSI_CUP:
		case CSI_CUD:
		case CSI_CUF:
		case CSI_CUB:
			/* Moves the cursor n (default 1) cells in the given direction. If
			 * the cursor is already at the edge of the screen, this has no
			 * effect. */
			switch ( seqs->at(0) ) {
				case -1:
					move_steps = 1;
				default:
					move_steps = seqs->at(0);
					for ( int i = 0; i < move_steps; ++i ) {
						switch ( mode ) {
							case CSI_CUP:
								this->cY--;
								break;
							case CSI_CUD:
								this->cY++;
								break;
							case CSI_CUF:
								this->cX++;
								break;
							case CSI_CUB:
								this->cX--;
								break;
						}
					}
					
					this->cX = ( this->cX < this->width  )
						? this->cX : this->width;
					this->cY = ( this->cY < this->height )
						? this->cY : this->height;
					
					break;
			}
			break;
			
		case CSI_EL:
			/* Erases part of the line. If n is zero (or missing), clear from
			 * cursor to the end of the line. If n is one, clear from cursor to
			 * beginning of the line. If n is two, clear entire line. Cursor
			 * position does not change. */
			switch ( seqs->at(0) ) {
				case -1:
				case 0:
					this->erase_to_from( this->cX, this->cY,
						this->width, this->cY );
					break;
				case 1:
					this->erase_to_from( 0, this->cY, this->cX, this->cY );
					break;
				case 2:
					this->erase_to_from( 0, this->cY,
						this->width, this->cY );
					break;
			}
			break;
		case CSI_ED:
			/* Clears part of the screen. If n is zero (or missing),
			 * clear from cursor to end of screen. If n is one,
			 * clear from cursor to beginning of the screen. If n is two, clear
			 * entire screen (and moves cursor to upper left on MS-DOS
			 * ANSI.SYS).*/
			switch ( seqs->at(0) ) {
				case -1: /* Missing */
				case  0:
					this->erase_to_from( this->cX, this->cY,
						this->width, this->height );
					break;
				case 1:
					this->erase_to_from( 0, 0, this->cX, this->cY );
					break;
				case 2:
					this->erase_to_from( 0, 0, this->width, this->height );
					break;
			}
			break;
		default:
			/* Unknown sequence */
			break;
	}
	
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
