//< text processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../___.h"
#include "../cfg.h"

#include "str.h"
#include "txt.h"
#include "chr.h"

#include "../glb.h"


//< TODO
//<	tri_in(TRIE tri, S word)
//<	hsh_proc(HT hsh, S word)
//< DEFINE TEXT_BUF WORD_BUF SZ_WBUF SZ_TBUF
//< ADT STOP_TRIE TEXT_HSH

// C valid_key(C c)
// {
	// R ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '\'') ? 1 : 0;
// }

/*UJ txt_swipe(S buf, I ptr, I lim)
{
	I i;
	for (i = 0; i + ptr < lim && !valid_key(buf[i+ptr]); i++);
	R (i + ptr >= lim - 1) ? NIL : i;
}

V txt_clean_buf(S buf, I len)
{
	DO(len, buf[i] = 0);
}

UJ sz_buf(S buf, UJ max)
{
	I i;
	for (i = 0; i < max && buf[i]; i++);
	R i;
}
*/
UJ txt_get_word(S dir, S source, I max_d, I max_s, I ptr)
{
	LOG("txt_get_word");
	UJ j, i;

	for (j = 0; valid_key(dir[j]) && j < max_d; j++);

	X(j == max_d, T(FATAL, "WORD_BUF will be overflowed (max. capacity %d)", max_d), NIL);

	for (i = 0; valid_key(source[ptr + i]) && i < max_s && j + i < max_d; i++) 
		dir[j + i] = source[ptr + i];
	dir[j + i] = 0;

	R (i == max_s || j + i == max_d) ? NIL : i;
} 

//<	process buf and break it into words 
//<	use fn for each word found
UJ txt_process_buf(S buf, V* tri, V* hsh, I len, WORD_ADD fn, I param)
{
	I i = 0, var;
	if (valid_key(WORD_BUF[0])) {
		if (valid_key(buf[0])) {
			var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);		
			P (var == NIL, NIL);									//< WORD_BUF or buf is overflowed
			i += var;
		}
		goto INCLUDE;
	}

	W(i < len - 1) {
		var = swipe_buf(buf, i, len);								//< how many chars swiped or NIL
		P(var == NIL, 0);											//< end of buf
		i += var;													//< move pointer

		var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);			//< length of word or NIL
		P(var == NIL, NIL);											//< problems with buf's capacity
		i += var;

		P(i >= len - 1 && !param, 0); 								//< it could be an incompleted word 

		INCLUDE:
		P(fn(tri, hsh, WORD_BUF, sz_buf(WORD_BUF, SZ_WBUF)) == NIL, NIL);
		clean_buf(WORD_BUF, SZ_WBUF);
	}

	fflush(stdout);
	R0;
}

//< FILE* f into TRIE and HSH
//< fn: str_hsh_ins or str_tri_ins
//< parsing main loop
UJ txt_process(FILE* f, V* struct_1, V* struct_2, WORD_ADD fn)
{
	LOG("txt_process");
	UJ len, res;

	LOOP:
	len = fread(TEXT_BUF, SZ(C), SZ_TBUF - 1, f);
	TEXT_BUF[len] = 0;
	P(txt_process_buf(TEXT_BUF, struct_1, struct_2, len + 1, fn, feof(f)) == NIL, NIL);
	if (!feof(f)) goto LOOP;

	clean_buf(WORD_BUF, SZ_WBUF);
	clean_buf(TEXT_BUF, SZ_TBUF);
	R 0;
}
