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

#ifndef _SHIBUYA_HH_
#define _SHIBUYA_HH_ FOO

/* Attr hooks

  1   2   3   8   16  32  64  128
  BKG BKG BKG BNK FGD FGD FGD BLD
   \   \   \   \   \   \   \   \
    \   \   \   \   \   \   \   +--- Bold
     \   \   \   \   +---+---+------ Foreground
      \   \   \   +----------------- Blink
       +---+---+-------------------- Background
 */

// Single bit defs
#define SHIBUYA_ATTR_BOLD  128
#define SHIBUYA_ATTR_BLINK 8

// Bit mask defs
#define SHIBUYA_ATTR_FG_MASK   0x70
#define SHIBUYA_ATTR_FG_OFFSET 4

#define SHIBUYA_ATTR_BG_MASK   0x07
#define SHIBUYA_ATTR_BG_OFFSET 0

// Access macro defs
#define SHIBUYA_ATTR_GET_FG(x) \
	((x & SHIBUYA_ATTR_FG_MASK) >> SHIBUYA_ATTR_FG_OFFSET)

#define SHIBUYA_ATTR_GET_BG(x) \
	((x & SHIBUYA_ATTR_BG_MASK) >> SHIBUYA_ATTR_BG_OFFSET)

#define SHIBUYA_ATTR_HAS_BOLD(x)  (x & SHIBUYA_ATTR_BOLD)
#define SHIBUYA_ATTR_HAS_BLINK(x) (x & SHIBUYA_ATTR_BLINK)

/* OK. Done with the bitwise stuffs */

#define String std::string
#define SDEBUG std::clog

#define SHIBUYA_DEFAULT_CMODE 0x70

void init_screen();
void set_clog();
void uninit_screen();
void update_screen();

#endif
