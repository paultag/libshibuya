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

#include "RenderableTerminal.hh"
#include "Terminal.hh"
#include "Pane.hh"

RenderableTerminal::RenderableTerminal( Terminal * t ) {
	this->terminal = t;
	int width  = t->get_width();
	int height = t->get_height();
	this->width  = width;
	this->height = height;

	Pane * p = new Pane(0, 0, width, height );
	this->pane = p;
}

RenderableTerminal::~RenderableTerminal() {
	delete this->pane;
}

void RenderableTerminal::render() {
	this->terminal->render(this->pane->getWindow());
}

void RenderableTerminal::insert( unsigned char c ) {
	this->terminal->insert(c);
}

void RenderableTerminal::move_to(unsigned int x, unsigned int y ) {
	this->pane->move_to(x, y);
}
