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
#include "Exceptions.hh"
#include "BGFile.hh"

#include <iostream>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

NcursesTerminal * toDump = NULL;

void sighandle ( int signo ) {
	switch ( signo ) {
		case SIGUSR1:
			SDEBUG << "SIGUSR1, dumping char array to stderr." << std::endl;
			if ( ! toDump )
				return;
			
			for ( int iy = 0; iy < toDump->get_height(); ++iy ) {
				for ( int ix = 0; ix < toDump->get_width(); ++ix ) {
					int offset = ( toDump->get_width() * iy ) + ix;
					std::cerr << toDump->chars[offset].ch;
				}
				std::cerr << std::endl;
			}
			break;
		case SIGWINCH:
			SDEBUG << "Window Resize" << std::endl;
			endwin();
			refresh();
			/* XXX: Handle background re-center */
			break;
		case SIGTERM:
			SDEBUG << "Trapped SIGTERM." << std::endl;
			uninit_screen();
			exit(0);
			break;
		case SIGINT:
			SDEBUG << "Shibuya SIGINT handler called." << std::endl;
			toDump->sigint(); /* Fixme */
			break;
		default:
			SDEBUG << "Defaulted on a signal trap. Fixme! (SIG: " << signo
				<< " )" << std::endl;
	}
}
 
int main ( int argc, char ** argv ) {
	set_clog();    // XXX: This is ugly
	init_screen();
	
	NcursesTerminal nt( 80, 25, 3, 2 );
	nt.fork("/bin/bash");
	toDump = &nt;
	
	std::vector<std::string> * bg = NULL;
	
	if ( argc > 0 ) {
		init_pair(1, COLOR_RED, COLOR_BLACK);
		attron(COLOR_PAIR(1));
		attron(A_BOLD);
		bg = get_bg_vector( argv[1] );
		write_out_bg( bg );
		attroff(COLOR_PAIR(1));
		attroff(A_BOLD);
	}
	
	signal( SIGUSR1, sighandle );
	signal( SIGTERM, sighandle );
	signal( SIGINT,  sighandle );
	signal( SIGWINCH, sighandle );

	try {
		while ( true ) {
			nt.poke();
			if ( nt.render() )
				update_screen();
			timeout(0);
			int ch = getch();

			if ( ch != ERR ) {
				if ( ch < 128 ) {
					nt.type(ch);
				}
			}

			usleep(20);
		}
	} catch ( DeadChildException * e ) {
		SDEBUG << "Dead child. Exiting." << std::endl;
	}
	uninit_screen();
}
