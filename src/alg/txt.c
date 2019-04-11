//< text processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../___.h"
#include "../glb.h"
#include "../utl/clk.h"

#include "str.h"
#include "txt.h"
#include "chr.h"

G SWIPE_NEXT_VALS = 0;

UJ SZFILE(FILE* ptr)
{
	I a, b;
	a = ftell(ptr);fseek(ptr, 0, SEEK_END);
	b = ftell(ptr);fseek(ptr, a, SEEK_SET);
	R b;
}

UJ txt_get_word(S dir, S source, I max_d, I max_s, I ptr)
{
	LOG("txt_get_word");
	UJ i, j;
	for (j = 0; j < max_d - 1 && valid_key(dir[j]); j++);		//<	j --> next available pos in dir
	for (i = 0; valid_key(source[ptr + i]) && i < max_s - 1 && j + i < max_d - 1; i++)
		dir[j + i] = source[ptr + i];
	dir[j + i] = 0;
	R i;														//<	i 	-->	amount of copied chars
}

V txt_overflow(S w_buf, I sz_w_buf, S t_buf, I sz_t_buf, UJ* i, I pos)
{
	LOG("txt_overflow");
	UJ var;
	T(WARN, "\t[!]\tword '%.10s...' at %d (max. cap. %d)", w_buf, pos, sz_w_buf);
	var = swipe_buf(t_buf, *i, sz_t_buf, 1);
	*i += var;
	if (var == NIL) {
		SWIPE_NEXT_VALS = 1;
		*i = sz_t_buf -1;
	}
}

UJ txt_proc_buf(FILE* f, S buf, V* tri, V* hsh, I len, WORD_ADD fn, I param)
{
	LOG("txt_proc_buf");
	UJ i = 0, var = 0, cnt = 0;		

	if (SWIPE_NEXT_VALS) {
		var = swipe_buf(buf, i, len, 1);
		P(var == NIL, 0);
		i += var;
		if (i < len - 1)
			SWIPE_NEXT_VALS = 0;	
	}

	if (valid_key(WORD_BUF[0])) {
		if (valid_key(buf[i])) {
			var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);
			i += var;
			if (!var || (valid_key(WORD_BUF[SZ_WBUF - 2]) && valid_key(buf[i]))) {
				txt_overflow(WORD_BUF, SZ_WBUF, buf, len, &i, ftell(f) - len - SZ_WBUF + var);
				goto CLEAN;	
			}
		}
		goto INCLUDE;	
	}

	W (i < len - 1) {
		var = swipe_buf(buf, i, len, 0);								//<	swipe unvalids		
		P(var == NIL, cnt);												//<	it's end of buf and all posiible words were processed
		i += var;
		var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);
		P(!var, cnt);													//<	WORD_BUF was already full --> non-sence || i == len - 1 - 
		i += var;
		P(!param && i >= len - 1, cnt);									//<	it could be an incompleted word

		if (var == SZ_WBUF - 1 && i >= len -1) 							//<	if WORD_BUF is full and it's end of buf
				SWIPE_NEXT_VALS = 1;

		if (valid_key(buf[i])) { 										//<	word is too long
			txt_overflow(WORD_BUF, SZ_WBUF, buf, len, &i, ftell(f) - len + i - SZ_WBUF);
			goto CLEAN;	
		}

		P(i >= len - 1 && !param, cnt); 									//< it can be an incompleted word 

		INCLUDE:
		cnt++;
		P(fn(tri, hsh, WORD_BUF, sz_buf(WORD_BUF, SZ_WBUF)) == NIL, NIL);
		CLEAN:
		clean_buf(WORD_BUF, SZ_WBUF);
	}
	R cnt;
}

//< FILE* f into TRIE and HSH
//< fn: str_hsh_ins or str_tri_ins
//< parsing main loop
UJ txt_process(FILE* f, V* struct_1, V* struct_2, WORD_ADD fn)
{
	LOG("txt_process");
	UJ len, cnt = 0, i = 0;

	T(INFO, "\t______________________________________________________________________");
	X(SZ_TBUF < SZ_WBUF, T(FATAL, "\t[!]\tSZ_TBUF must be >= SZ_WBUF (now SZ_TBUF = %d; SZ_WBUF = %d)", SZ_TBUF, SZ_WBUF), NIL);

	LOOP:
	len = fread(TEXT_BUF, SZ(C), SZ_TBUF - 1, f);
	TEXT_BUF[len] = 0;
	i = txt_proc_buf(f, TEXT_BUF, struct_1, struct_2, len + 1, fn, feof(f));
	P(i == NIL, NIL);
	cnt += i;
	if (!feof(f)) 
		goto LOOP;

	clean_buf(WORD_BUF, SZ_WBUF);
	clean_buf(TEXT_BUF, SZ_TBUF);
	R 0;
}



