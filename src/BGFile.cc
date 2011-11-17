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

#include "Shibuya.hh"
#include "BGFile.hh"

#include <iostream>
#include <fstream>

std::vector<std::string> * get_bg_vector( const char * fd ) {
	std::string line;
	std::ifstream myfile(fd);
	
	SDEBUG << "BG fd: " << fd << std::endl;
	
	std::vector<std::string> * bg = new std::vector<std::string>();
	
	if (myfile.is_open()) {
		while ( myfile.good() ) {
			getline(myfile,line);
			bg->insert( bg->end(), line );
		}
		myfile.close();
	}
	return bg;
}

void write_out_bg( std::vector<std::string> * boo ) {
	
	SDEBUG << "Writing out the BG" << std::endl;
	
	if ( boo == NULL )
		return;
	
	int maxRow = 0;
	int maxCol = 0;
	
	getmaxyx(stdscr, maxRow, maxCol);
	
	for ( int iy = 0; iy < maxRow; ++iy ) {
		for ( int ix = 0; ix < maxCol; ++ix ) {
			mvwaddch(stdscr, iy, ix, ' ');
		}
	}
	
	unsigned int flen = 0;
	unsigned int fmax = 0;
	
	for ( unsigned int i = 0; i < boo->size(); ++i ) {
		fmax = fmax < boo->at(i).length() ? boo->at(i).length() : fmax;
		++flen;
	}
	
	int midX = ( maxCol / 2 );
	int midY = ( maxRow / 2 );
	midX     = midX - ( fmax / 2 );
	midY     = midY - ( flen / 2 );
	
	for ( unsigned int i = 0; i < boo->size(); ++i ) {
		mvprintw( midY, midX, boo->at(i).c_str() );
		midY++;
	}
}
