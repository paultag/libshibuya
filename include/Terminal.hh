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

#ifndef _TERMINAL_HH_
#define _TERMINAL_HH_ foo

#include <pty.h>

#define GET_OFFSET(x, y) ( ( y * this->width ) + x )

typedef struct _TerminalCell {
	unsigned char attr;
	unsigned char ch;
} TerminalCell;

class Terminal {
	protected:
		int width;
		int height;

		int scroll_frame_top;
		int scroll_frame_bottom;

		int cX;
		int cY;
		
		unsigned char   cMode;

		pid_t pty;
		pid_t childpid;

		void erase_to_from( int iX, int iY, int tX, int tY );
		void newline();
		void bounds_check();
		void delete_line( int idex );
		void insert_line( int idex );
		void _init_Terminal( int width, int height );

	public:
		TerminalCell * chars;

		Terminal(int width, int height);
		Terminal();
		~Terminal();

		virtual void insert( unsigned char c );
		void type( char c );

		void scroll_up();
		void scroll_down();
		void poke();
		void sigint();

		pid_t fork( const char * command ); /* XXX: Protect this? */

		int get_width();
		int get_height();
};

#endif
