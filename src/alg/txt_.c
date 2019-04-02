//< text processing
#include <stdio.h>
#include <stdlib.h>

#include "../___.h"
#include "../cfg.h"
#include "../glb.h"

#include "str.h"
#include "txt.h"

#include "../adt/tri.h"


//< TODO
//<	tri_in(TRIE tri, S word)
//<	hsh_proc(HT hsh, S word)
//< DEFINE TEXT_BUF WORD_BUF SZ_WBUF SZ_TBUF
//< ADT STOP_TRIE TEXT_HSH


C in_alphabet(C c)
{
	R ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '\'') ? 1 : 0;
}

UJ txt_swipe(S buf, I ptr, I lim)
{
	for (I i = 0; i + ptr < lim && !in_alphabet(buf[i+ptr]); i++);
	R (i + ptr >= lim - 1) ? NIL : i;
}

V txt_clean_buf(S buf, I len)
{
	DO(len, buf[i] = 0);
}

UJ txt_get_word(S dir, S source, I max_d, I max_s, I ptr)
{
	LOG("txt_get_word");
	UJ j, i;

	for (j = 0; in_alphabet(dir[j]) && j < max_d; j++);

	X(j == max_d, T(FATAL, "WORD_BUF will be overflowed (max. capacity %d)", max_d), NIL);

	for (i = 0; in_alphabet(source[ptr + i]) && i < max_s && j + i < max_d; i++) 
		dir[j + i] = source[ptr + i];
	dir[j + i] = 0;

	R (i == max_s || j + i == max_d) ? NIL : i;
} 

UJ txt_process_buf(S buf, TRIE tri, HT hsh, I len)
{
	I i = 0, var;
	if (in_alphabet(WORD_BUF[0])) {
		if (in_alphabet(buf[0])) {
			var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);		
			P (var == NIL, NIL);									//< WORD_BUF or buf is overflowed
			i += var;
		}
		goto INCLUDE;
	}

	W(i < len - 1) {
		var = txt_swipe(buf, i, len);							//< how many chars swiped or NIL
		P(var == NIL, 0);										//< end of buf
		i += var;												//< move pointer

		var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);		//< length of word or NIL
		P(var == NIL, NIL);										//< problems with buf's capacity
		i += var;

		P(i >= len - 1, 0); 

		INCLUDE:
		P(str_word_inclusion(WORD_BUF, hsh, tri) == NIL, NIL);
		txt_clean_buf(WORD_BUF, SZ_WBUF);
	}

	fflush(stdout);
	R0;
}

//< FILE* f into TRIE and HSH
UJ txt_process(FILE* f)
{
	LOG("txt_process");
	UJ len;

	LOOP:
	len = fread(TEXT_BUF, SZ(C), SZ_TBUF - 1, f);
	TEXT_BUF[len] = 0;
	P(txt_process_buf(TEXT_BUF, STOP_TRIE, TEXT_HSH, len + 1) == NIL, NIL);

	if (!feof(f)) goto LOOP;

	R 0;
}
