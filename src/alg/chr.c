#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../___.h"
#include "../cfg.h"
#include "../glb.h"

// #include "chr.h"

C cs(C c)
{
	// R toupper(c);
	// R c;
	R (C)tolower(c);
}

UJ char_is_ext(C c)
{
	I i;
	for (i = 0; i < EXT_KEY_AM; i++) {
		// O("%d!\n", i);
		// O("%c!\n", EXT_KEY[0]);

		if (EXT_KEY[i] == c)
			R i;
		// O("%c at %d NOT EXTRA\n", c, i);
	}


	// DO(EXT_KEY_AM, 
				// P(EXT_KEY[i] == c, i););
	R NIL;
}

C in_alphabet(C c)
{
	R ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) ? 1 : 0;
}

C valid_key(C c)
{
	R (in_alphabet(c) || char_is_ext(c) != NIL) ? 1 : 0;
}

UJ swipe_buf(S buf, I ptr, I lim)
{
	I i;
	for (i = 0; i + ptr < lim && !valid_key(buf[i+ptr]); i++);
	R (i + ptr >= lim - 1) ? NIL : i;
}

V clean_buf(S buf, I len)
{
	DO(len, buf[i] = 0);
}

UJ sz_buf(S buf, UJ max)
{
	I i;
	for (i = 0; i < max && buf[i]; i++);
	R i;
}