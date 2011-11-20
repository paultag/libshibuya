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
#include "Shibuya.hh"

ANSITerminal::ANSITerminal() {
	SDEBUG << "Default ANSI Constructor" << std::endl;
	this->_init_ANSITerminal();
	this->_init_Terminal( 80, 25 );
}
ANSITerminal::ANSITerminal( int width, int height ) {
	SDEBUG << "Width / Height ANSI Constructor" << std::endl;
	this->_init_ANSITerminal();
	this->_init_Terminal( width, height );
}
ANSITerminal::~ANSITerminal() {
	SDEBUG << "ANSI Destructor" << std::endl;
}

void ANSITerminal::_init_ANSITerminal() {
	ansi_escape_parser_reset();
}

void ANSITerminal::_handle_private_escape( ansi_sequence * last ) {
	SDEBUG << "Rcvd a private mode CSI: " << last->priv << std::endl;
	for ( unsigned int i = 0; i < last->values->size(); ++i )
		SDEBUG << "  " << last->values->at(i) << std::endl;
	SDEBUG << "  Mode: " << last->mode << std::endl;
}

void ANSITerminal::_handle_escape( ansi_sequence * last ) {
	char mode               = last->mode;
	std::vector<int> * seqs = last->values;

	if ( last->priv )
		SDEBUG << last->priv << ", ";
	SDEBUG << last->mode << ": ";
	for ( unsigned int i = 0; i < seqs->size(); ++i )
		SDEBUG << seqs->at(i) << ", ";
	SDEBUG << std::endl;

	int move_steps =  1;
	int nRow       = -1;
	int nCol       = -1; /* Sorry about this hack, friend */
	int nTop       = -1;
	int nBottom    = -1;

	if ( last->priv ) {
		/* We have a private-mode ANSI CSI Sequence. */
		this->_handle_private_escape( last );
		return; /* Don't drop to normal handlers */
	}

	switch ( mode ) {
		case CSI_CUU:
		case CSI_CUD:
		case CSI_CUF:
		case CSI_CUB:
			/* Moves the cursor n (default 1) cells in the given direction. If
			 * the cursor is already at the edge of the screen, this has no
			 * effect. */
			move_steps = ( seqs->at(0) > 0 ) ? seqs->at(0) : 1;
			for ( int i = 0; i < move_steps; ++i ) {
				switch ( mode ) {
					case CSI_CUU: this->cY--; break;
					case CSI_CUD: this->cY++; break;
					case CSI_CUF: this->cX++; break;
					case CSI_CUB: this->cX--; break;
				}
			}
			this->cX = ( this->cX < this->width  )
				? this->cX : this->width;
			this->cY = ( this->cY < this->height )
				? this->cY : this->height;
			break;
		case CSI_CHA:
			/* Moves the cursor to column n. */
			this->cX = ( seqs->at(0) < 1 ) ? 0 : seqs->at(0) - 1;
			break;
		case 'd': // XXX: Fixme
			/* moves the cursor to row n. */
			this->cY = ( seqs->at(0) < 1 ) ? 0 : seqs->at(0) - 1;
			break;
		case 'M': // XXX: Fixme (CSI_DL)
			/* DL | Delete the indicated # of lines. */
			move_steps = ( seqs->at(0) > 0 ) ? seqs->at(0) : 0;
			for ( int i = 0; i < move_steps; ++i )
				this->delete_line( this->cY );
			break;
		case 'L': // XXX: FIXME
			move_steps = ( seqs->at(0) > 0 ) ? seqs->at(0) : 0;
			for ( int i = 0; i < move_steps; ++i )
				this->insert_line( this->cY );
			break;
		case 'm': // XXX: FIXME
			/* 0     Reset / Normal	all attributes off
			 * 1     Bright (increased intensity) or Bold	
			 * 2     Faint (decreased intensity) not widely supported
			 * 5     Blink: Slow less than 150 per minute
			 * 6     Blink: Rapid MS-DOS ANSI.SYS; 150 per minute or more; not
			 *       widely supported
			 * 7     Image: Negative inverse or reverse; swap foreground and
			 *       background
			 * 8     Conceal not widely supported
			 * 9     Crossed-out Characters legible, but marked for deletion.
			 *       Not widely supported.
			 * 10    Primary(default) font
			 * 22    Normal color or intensity neither bright, bold nor faint
			 * 25    Blink: off
			 * 27    Image: Positive
			 * 28    Reveal	conceal off
			 * 30–37 Set text colo
			 * 39    Default text color	implementation defined
			 *       (according to standard)
			 * 40–47 Set background color 40 + x, where x is from the color
			 *       table below
			 * 49    Default background color implementation defined
			 *       (according to standard)
			 */
			for ( unsigned int i = 0; i < seqs->size(); ++i ) {
				switch ( seqs->at(i) ) {
					case 1:
						/* bold global attr */
						if ( SHIBUYA_ATTR_HAS_BOLD(this->cMode) == 0 ) {
							this->cMode += SHIBUYA_ATTR_BOLD;
							SDEBUG << "Set the bold bit" << std::endl;
						}
						break;
					case 0:
						/* Reset global attr */
						this->cMode = SHIBUYA_DEFAULT_CMODE;
						break;
					case 30: case 31: case 32:
					case 33: case 34: case 35:
					case 36: case 37:
						this->cMode -= (this->cMode & SHIBUYA_ATTR_FG_MASK);
						/* Now that the flags are unset, let's set them again */
						this->cMode += ((seqs->at(i) - 30) << SHIBUYA_ATTR_FG_OFFSET);
						/* Great. All set. */
						SDEBUG << "FG: " << SHIBUYA_ATTR_GET_FG(this->cMode) << std::endl;
					break;
					default:
						/* Unknown m sequence id */
						break;
				}
			}
			break;
		case 'r':
			nTop = seqs->at(0);
			
			if ( nTop < 0 )
				nTop = 1;
			
			if ( seqs->size() >= 2 )
				nBottom = ( seqs->at(1) > 0 ) ? seqs->at(1) : this->height;
			else
				nBottom = this->height;
			
			nBottom = ( nBottom < 1 )         ? 0            : nBottom;
			nTop    = ( nTop > this->height ) ? this->height : nTop - 1;
			
			this->scroll_frame_bottom = nBottom;
			this->scroll_frame_top    = nTop;
			
			this->cX = 0;
			this->cY = nTop;
			
			SDEBUG << "rTop / rBottom: "
				<< nTop << ", " << nBottom << std::endl;
			
			break;
		case CSI_CUP:
			/* Moves the cursor to row n, column m. The values are 1-based, and
			 * default to 1 (top left corner) if omitted. A sequence such as CSI
			 * ;5H is a synonym for CSI 1;5H as well as CSI 17;H is the same as
			 * CSI 17H and CSI 17;1H */
			nRow = seqs->at(0);
			
			if ( nRow < 0 )
				nRow = 1;
			
			if ( seqs->size() >= 2 )
				nCol = seqs->at(1);
			
			nRow = ( nRow < 1 ) ? 1 : nRow;
			nCol = ( nCol < 1 ) ? 1 : nCol;
			nRow = ( nRow > this->height ) ? this->height : nRow;
			nCol = ( nCol > this->width  ) ? this->width  : nCol;
			
			this->cX = (nCol - 1);
			this->cY = (nRow - 1);
			
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
				case -1:
				case  0:
					this->erase_to_from( this->cX, this->cY,
						this->width - 1, this->height - 1 );
					break;
				case 1:
					this->erase_to_from( 0, 0, this->cX + 1, this->cY );
					break;
				case 2:
					this->erase_to_from( 0, 0,
						this->width - 1, this->height - 1 );
					break;
			}
			break;
		default:
			SDEBUG << "(Unhandled)" << std::endl;
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
			Terminal::insert(c);
			break;
		case ANSI_ESCAPE_PARSE_INCOMPLETE:
			break;
	}
}
