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
/*
C in_alphabet(C c)
{
	R ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) ? 1 : 0;
}
*/

C in_alphabet(WC c)
{
	R  (IN(0x41, c, 0x5a) || IN(0x61, c, 0x7a) || 									//<	basic latin
		IN(0x400, c, 0x45F) ||	IN(0x460, c, 0x52f) ||	IN(0x1c80, c, 0x1c88) ||	//<	cyrillic
		IN(0xc0, c, 0xd6) || IN(0xd8, c, 0xf6) || (IN(0xf8, c, 0x2b8)) ||			//<	extended latin	
		IN(0x370, c, 0x373) || IN(0x376, c, 0x377) || IN(0x37b, c, 0x37d) ||		//<	greek
		(c == 0x37f) || (c == 0x386) || IN(0x388, c, 0x38a) || (c == 0x38c) ||		//<	greek
		IN(0x38e, c, 0x3a1) || IN(0x3a3, c, 0x3ff) || 								//<	greek
		IN(0x1d00, c, 0x1dbf) ||													//<	phonetics
		IN(0x1e00, c, 0x1eff) || IN(0x2c60, c, 0x2c7f)
		) ? 1 : 0;
}


C valid_key(C c)
{
	// R (in_alphabet(c)) ? 1 : 0;
	R (in_alphabet(c) || char_is_ext(c) != NIL) ? 1 : 0;
}

//< if par == 0 --> swipe non-alphabetical
//<	if par == 1 --> swipe alphabetical
UJ swipe_buf(S buf, I ptr, I lim, C par)
{
	I i;

	for (i = 0; i + ptr < lim && !(par^valid_key(buf[i + ptr])); i++);		
		
	R (i + ptr >= lim - 1) ? NIL : i;
}

V clean_buf(S buf, I len)
{
	// DO(len, buf[i] = 0);
	// I i;
	// for (i = 0; i < len && buf[i]; i++)
		// buf[i] = 0;
	// buf[0] = 0;
	buf[0] = buf[len - 1] = buf[len - 2] = 0;
}

UJ sz_buf(S buf, UJ max)
{
	I i;
	for (i = 0; i < max && buf[i]; i++);
	R i;
}