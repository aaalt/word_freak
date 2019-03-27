//< text processing
#include <stdio.h>
#include <stdlib.h>

#include "../___.h"
#include "../cfg.h"

#include "../adt/tri.h"
#include "str.h"

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
	R ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'b') || c == '\'') ? 1 : 0;
}


UJ txt_swipe(S buf, I ptr, I lim)
{
	I i;
	for (i = 0; i + ptr < lim && !in_alphabet(buf[i+ptr]); i++);
	R (i + ptr <= lim) ? NIL : i + ptr;
}

V txt_clean_buf(S buf, I len)
{
	for (I i = 0; i < len; i++)
		buf[i] = 0;
}

//< returns last i+1 if success
//< else NIL
UJ txt_get_word(S dir, S source, I i, I max_d, I max_s)
{
	LOG("txt_get_word");
	UJ j, ptr;

	for (j = 0; in_alphabet(dir[j]) && j < max_d; j++);

	if (j == max_d) {
		T(FATAL, "WORD_BUF is overflowed (max. capacity %d)", max_d);
		R NIL;
	}

	for (ptr = 0; in_alphabet(source[i]) && i < max_s && j < max_d;) 
		dir[j++ + ptr++] = source[i++];

	dir[j + ptr] = 0;
	
	R i;
}

//<	return 0 if success
//< else NIL
UJ txt_process_buf(S buf, TRIE tri, HT hsh, I len)
{
	LOG("txt_process_buf");
	UJ i = 0, ptr, res;
	UJ cnt_added = 0, cnt_repeats = 0, cnt_stops = 0;

	if (in_alphabet(WORD_BUF[0])) 									//< if previous word_buf contains a word
		if (!in_alphabet(buf[0])) 									//< and now we see that it's a completed
			goto INCLUDE;											//< word then just go to INCLUDE

	while (i < len - 1) {
		i = txt_swipe(buf, i, len - 1);								//< swipe non-alphabetical symbols
		if (i == NIL) R0;											//< if empty end of buffer
		ptr = i;
		i = txt_get_word(WORD_BUF, buf, i, SZ_WBUF - 1, len -1);	//<	get word form buf into WORD_BUF from buf[i]

		P(i == NIL, NIL);											//<	word is longer than WORD_BUF
		if (i >= len - 1 || ptr == i) R0;							//< if tail may be not empty or there are no
																	//<	words left in a buf
		INCLUDE:
		
		P(cnt_upd(&cnt_repeats, &cnt_added, &cnt_stops, 			//< do str_word_inclusion(), update counters 
			str_word_inclusion(WORD_BUF, hsh, tri)) == NIL, NIL);	//< panic if NIL is returned 

		txt_clean_buf(WORD_BUF, SZ_WBUF);							//< clean WORD_BUF
	}	

	T(DEBUG, "%d words:  %d stops  %d unique  %d repeats", 
		cnt_stops + cnt_added + cnt_repeats, cnt_stops, cnt_added, cnt_repeats);
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