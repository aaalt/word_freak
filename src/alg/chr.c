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
TXT_TYPE mtoupper(TXT_TYPE c)
{
	R (TXT_TYPE)toupper(c);
}
TXT_TYPE mtolower(TXT_TYPE c)
{
	R (TXT_TYPE)tolower(c);
}

C in_alphabet(TXT_TYPE c)
{
	R ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) ? 1 : 0;
}

UJ mfread(FILE* f, TXT_T buf, UJ max)
{
	UJ len = fread(buf, SZ(TXT_TYPE), max - 1, f);
	TEXT_BUF[len] = 0;
	R len + 1;
}
#else
TXT_TYPE mtoupper(TXT_TYPE c)
{
	R (TXT_TYPE)towupper(c);
}
TXT_TYPE mtolower(TXT_TYPE c)
{
	R (TXT_TYPE)towlower(c);
}

C in_alphabet(TXT_TYPE c)
{
	R  (IN(0x41, c, 0x5a) || IN(0x61, c, 0x7a) || 									//<	basic latin		[52]
		// IN(0x400, c, 0x45F) ||	IN(0x460, c, 0x52f) ||	IN(0x1c80, c, 0x1c88) ||	//<	cyrillic		[96+...]
		// 
		IN(0x401, c, 0x451)															//< MAIM cyrilic`	[82]
		// IN(0xc0, c, 0xd6) || IN(0xd8, c, 0xf6) || (IN(0xf8, c, 0x2b8)) ||			//<	extended latin	
		// IN(0x370, c, 0x373) || IN(0x376, c, 0x377) || IN(0x37b, c, 0x37d) ||		//<	greek
		// (c == 0x37f) || (c == 0x386) || IN(0x388, c, 0x38a) || (c == 0x38c) ||		//<	greek
		// IN(0x38e, c, 0x3a1) || IN(0x3a3, c, 0x3ff) || 								//<	greek
		// IN(0x1d00, c, 0x1dbf) ||													//<	phonetics
		// IN(0x1e00, c, 0x1eff) || IN(0x2c60, c, 0x2c7f)								//<	latin
		) ? 1 : 0;
}

UJ mfread(FILE* f, TXT_T buf, UJ max)
{
	UJ i = 0;
	for (i = 0; i < max - 1 && !feof(f); i++) 
		buf[i] = fgetwc(f);
	buf[i] = 0;
	R i + 1;
}
#endif

TXT_TYPE cs(TXT_TYPE c)
{
	// R (C)mtoupper(c);
	// R (C)c;
	R (TXT_TYPE)mtolower(c);
}

UJ char_is_ext(TXT_TYPE c)
{
	DO(EXT_KEY_AM, 
				P(EXT_KEY[i] == c, i););
	R NIL;
}

TXT_T convert_str(TXT_T key, I len)
{
	DO(len, 
		key[i] = cs(key[i]);)
	R key;
}

C valid_key(TXT_TYPE c)
{
	// R (in_alphabet(c)) ? 1 : 0;
	R (char_is_ext(c) != NIL || in_alphabet(c)) ? 1 : 0;
}

//< if par == 0 --> swipe non-alphabetical
//<	if par == 1 --> swipe alphabetical
UJ swipe_buf(TXT_T buf, I ptr, I lim, C par)
{
	I i;

	for (i = 0; i + ptr < lim && !(par^valid_key(buf[i + ptr])); i++);		
		
	R (i + ptr >= lim - 1) ? NIL : i;
}

V clean_buf(TXT_T buf, I len)
{
	buf[0] = buf[len - 1] = buf[len - 2] = 0;
}

UJ sz_buf(TXT_T buf, UJ max)
{
	I i;
	for (i = 0; i < max && buf[i]; i++);
	R i;
}
