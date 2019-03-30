//< text processing
#include <stdio.h>
#include <stdlib.h>

#include "../___.h"
#include "../cfg.h"

#include "str.h"

#include "../adt/tri.h"

#include "../glb.h"


//< TODO
//<	tri_in(TRIE tri, S word)
//<	hsh_proc(HT hsh, S word)
//< DEFINE TEXT_BUF WORD_BUF SZ_WBUF SZ_TBUF
//< ADT STOP_TRIE TEXT_HSH



UJ cnt_upd(UJ* cnt_0, UJ* cnt_1, UJ* cnt_2, UJ res)
{
	SW(res) {
		CS(0, {*cnt_0++;});
		CS(1, {*cnt_1++;});
		CS(2, {*cnt_2++;});
	}	
	R res;
}


C in_alphabet(C c)
{
	R ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '\'') ? 1 : 0;
}


UJ txt_swipe(S buf, I ptr, I lim)
{
	I i;
	for (i = 0; i + ptr < lim && !in_alphabet(buf[i+ptr]); i++);

	R (i + ptr >= lim - 1) ? NIL : i;
}

V txt_clean_buf(S buf, I len)
{
	for (I i = 0; i < len; i++)
		buf[i] = 0;
}


UJ txt_get_word(S dir, S source, I max_d, I max_s, I ptr)
{
	LOG("txt_get_word");
	UJ j, i;

	for (j = 0; in_alphabet(dir[j]) && j < max_d; j++);

	if (j == max_d) {
		T(FATAL, "WORD_BUF is overflowed (max. capacity %d)", max_d);
		R NIL;
	}

	for (i = 0; in_alphabet(source[ptr + i]) && i < max_s && j + i < max_d; i++) 
		dir[j + i] = source[ptr + i];
	dir[j + i] = 0;

	R (i == max_s || j + i == max_d) ? NIL : i;
	
} 

UJ txt_process_buf(S buf, TRIE tri, HT hsh, I len)
{
	I i = 0, var;
	UJ cnt_repeats = 0, cnt_added = 0, cnt_stops = 0;
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
											//< it could be incompleted words
		// txt_include_word(WORD_BUF);
		P(cnt_upd(&cnt_repeats, &cnt_added, &cnt_stops, 			//< do str_word_inclusion(), update counters 
			str_word_inclusion(WORD_BUF, hsh, tri)) == NIL, NIL);	//< panic if NIL is returned 
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
