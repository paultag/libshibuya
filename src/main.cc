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

#include <iostream>
#include <string.h>

int main ( int argc, char ** argv ) {
	init_screen();
	
	NcursesTerminal nct;
	
	const char * foo = "Hello, World!\n\0";
	
	for ( unsigned int i = 0; i < 5; ++i ) {
		nct.insert( foo[i] );
	}
	
	for ( int iy = 0; iy < nct.get_height(); ++iy ) {
		for ( int ix = 0; ix  < nct.get_width(); ++ix ) {
			int offset = (( nct.get_width() * iy ) + ix );
			std::cerr << nct.chars[offset].ch;
		}
		std::cerr << std::endl;
	}
	
	nct.poke();
	nct.render();
	
	update_screen();
	
	usleep(2000000);
	uninit_screen();
}
