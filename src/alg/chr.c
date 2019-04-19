#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "../___.h"
#include "../cfg.h"
#include "../glb.h"

#if (TXT_DATA_TYPE == 1)
CHAR mtoupper(CHAR c)
{
	R (CHAR)toupper(c);
}
CHAR mtolower(CHAR c)
{
	R (CHAR)tolower(c);
}

C in_alphabet(CHAR c)
{
	R ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) ? 1 : 0;
}

UJ mfread(FILE* f, STR buf, UJ max)
{
	UJ len = fread(buf, SZ(CHAR), max - 1, f);
	TEXT_BUF[len] = 0;
	R len + 1;
}
#else
CHAR mtoupper(CHAR c)
{
	R (CHAR)towupper(c);
}
CHAR mtolower(CHAR c)
{
	R (CHAR)towlower(c);
}

C in_alphabet(CHAR c)
{
	R  (IN(0x41, c, 0x5a) || IN(0x61, c, 0x7a) || 									//<	basic latin		[52]
		IN(0x400, c, 0x45F) ||	IN(0x460, c, 0x52f) ||	IN(0x1c80, c, 0x1c88) ||	//<	cyrillic		[96+...]
		IN(0x401, c, 0x451)	||														//< MAIN cyrilic`	[82]
		IN(0xc0, c, 0xd6) || IN(0xd8, c, 0xf6) || (IN(0xf8, c, 0x2b8)) ||			//<	extended latin	
		IN(0x370, c, 0x373) || IN(0x376, c, 0x377) || IN(0x37b, c, 0x37d) ||		//<	greek
		(c == 0x37f) || (c == 0x386) || IN(0x388, c, 0x38a) || (c == 0x38c) ||		//<	greek
		IN(0x38e, c, 0x3a1) || IN(0x3a3, c, 0x3ff) || 								//<	greek
		IN(0x1d00, c, 0x1dbf) ||													//<	phonetics
		IN(0x1e00, c, 0x1eff) || IN(0x2c60, c, 0x2c7f)								//<	latin
		) ? 1 : 0;
}

UJ mfread(FILE* f, STR buf, UJ max)
{
	UJ i = 0;
	for (i = 0; i < max - 1 && !feof(f); i++) 
		buf[i] = fgetwc(f);
	buf[i] = 0;
	R i;
}
#endif

CHAR cs(CHAR c)
{
	// R (C)mtoupper(c);
	// R (C)c;
	R (CHAR)mtolower(c);
}

UJ char_is_ext(CHAR c)
{
	DO(EXT_KEY_AM, 
				P(EXT_KEY[i] == c, i););
	R NIL;
}

STR convert_str(STR key, I len, I max, C par)
{
	I i;
	for (i = 0; i < len && i < max; i++)
		key[i] = cs(key[i]);
	if (i == max)
		R NULL;
	if (par)
		key[i] = 0;

	R key;
}

C valid_key(CHAR c)
{
	// R (in_alphabet(c)) ? 1 : 0;
	R (char_is_ext(c) != NIL || in_alphabet(c)) ? 1 : 0;
}

//< if par == 0 --> swipe non-alphabetical
//<	if par == 1 --> swipe alphabetical
UJ swipe_buf(STR buf, I ptr, I lim, C par)
{
	I i;
	for (i = 0; i + ptr < lim && !(par^valid_key(buf[i + ptr])); i++);
	R (i + ptr >= lim - 1) ? NIL : i;
}

V clean_buf(STR buf, I len)
{
	buf[0] = buf[len - 1] = buf[len - 2] = 0;
}

UJ sz_buf(STR buf, UJ max)
{
	I i;
	for (i = 0; i < max && buf[i]; i++);
	R i;
}
