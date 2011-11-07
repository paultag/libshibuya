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

#include <panel.h>

#include "Pane.hh"


Pane::Pane(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width  = width;

	this->win = newwin(height, width, y, x);
	this->pan = new_panel(this->win);
	this->draw_outline();
}

Pane::~Pane() {

}

void Pane::draw_outline() {
	//box(this->win, 0, 0);
	// XXX: Fix this
}

void Pane::focus() {
	top_panel(this->pan);
}

void Pane::move_to(int x, int y) {
	move_panel(this->pan, x, y);
}

void Pane::resize( int width, int height ) {
	WINDOW * old_win  = this->win;
	WINDOW * temp_win = newwin(height, width, this->y, this->x);

	replace_panel(this->pan, temp_win);
	this->win = temp_win;
	delwin(old_win);

	this->x = width;
	this->y = height;
	this->draw_outline();
}

WINDOW * Pane::getWindow() {
	return this->win;
}
