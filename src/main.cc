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

#include "Terminal.hh"
#include "Shibuya.hh"
#include "NcursesTerminal.hh"
#include "BGFile.hh"

#include <iostream>
#include <string.h>

int main ( int argc, char ** argv ) {
	init_screen();
	
	
	if ( argc > 0 ) {
		start_color();
		init_pair(1, COLOR_RED, COLOR_BLACK);
		
		attron(COLOR_PAIR(1));
		attron(A_BOLD);
		
		std::vector<std::string> * bg = get_bg_vector( argv[1] );
		write_out_bg( bg );
		
		attroff(COLOR_PAIR(1));
		attroff(A_BOLD);
	}
	
	move(0, 0);
	
	update_screen();
	usleep(2000000);
	uninit_screen();
}
