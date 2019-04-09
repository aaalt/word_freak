#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../___.h"
#include "../cfg.h"
#include "../glb.h"

C cs(C c)
{
	// R (C)toupper(c);
	// R (C)c;
	R (C)tolower(c);
}

UJ char_is_ext(C c)
{
	DO(EXT_KEY_AM, 
				P(EXT_KEY[i] == c, i););
	R NIL;
}

S convert_str(S key, I len)
{
	DO(len, 
		key[i] = cs(key[i]);)
	R key;
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