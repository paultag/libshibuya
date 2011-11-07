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

#include <ncurses.h>
#include <pty.h>

typedef struct _TerminalCell {
	unsigned char attr;
	unsigned char ch;
} TerminalCell;

class Terminal {
	protected:

	/* iVars */

		bool graph;
		bool special;

		int width;
		int height;

		int scrollframe_top;
		int scrollframe_floor;

		int cX;
		int cY;
		
		unsigned char   cMode;
		TerminalCell  * chars;

		char  *  escape;
		int      maxesc;

		pid_t  pty;
		pid_t  childpid;

	/* Functions */
		void erase_to_from( int iX, int iY, int tX, int tY );
		
		void advance_curs();
		void _init_Terminal( int width, int height );

	public:
		Terminal(int width, int height);
		Terminal();
		~Terminal();
		virtual void insert( unsigned char c );
		void type( char c );
		void scroll_up();
		void render( WINDOW * win );
		void poke();
		pid_t fork( const char * command ); /* XXX: Protect this? */

		int get_width();
		int get_height();
};

#endif
