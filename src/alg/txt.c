//< text processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

#include "../___.h"
#include "../glb.h"
#include "../utl/clk.h"

#include "str.h"
#include "txt.h"
#include "chr.h"

G SWIPE_NEXT_VALS = 0;
G BUF_FULLNESS = 0;		//<	0 buf is empty; 1 buf is full; 2 buf contains something

I cnt_swipe, cnt_ovr_a, cnt_ovr_b, cnt_between, cnt_clean;
UJ WORD_LEN = 0;


UJ SZFILE(FILE* ptr)
{
	I a, b;
	a = ftell(ptr);fseek(ptr, 0, SEEK_END);
	b = ftell(ptr);fseek(ptr, a, SEEK_SET);
	R b;
}

UJ txt_get_word(TXT_T dir, TXT_T source, I max_d, I max_s, I ptr)
{
	LOG("txt_get_word");
	UJ i, j = 0;
	if (BUF_FULLNESS)
		// for (j = 0; j < max_d - 1 && valid_key(dir[j]); j++);		//<	j --> next available pos in dir
		for (j = 0; j < max_d - 1 && valid_key(dir[j]); j++);		//<	j --> next available pos in dir
	for (i = 0; valid_key(source[ptr + i]) && i < max_s - 1 && j + i < max_d - 1; i++) 
		dir[j + i] = source[ptr + i];
	dir[j + i] = 0;
	WORD_LEN += i;

	if (i)														//<	something was written in, in any case
		BUF_FULLNESS = 2;

	if (j + i == max_d - 1)										//<	dir is full
		BUF_FULLNESS = 1;
	
	R i;														//<	i 	-->	amount of copied chars
}

V txt_overflow(TXT_T w_buf, I sz_w_buf, TXT_T t_buf, I sz_t_buf, UJ* i, I pos)
{
	LOG("txt_overflow");
	UJ var;
	T(WARN, "\t[!]\tword '%.10ls...' at %lu\t(max. cap. %d)", w_buf, pos, sz_w_buf);
	var = swipe_buf(t_buf, *i, sz_t_buf, 1);
	*i += var;
	if (var == NIL) {
		SWIPE_NEXT_VALS = 1;
		*i = sz_t_buf -1;
	}
}

UJ txt_proc_buf(TXT_T buf, V* tri, V* hsh, I len, WORD_ADD fn, I param, I prev, I cur)
{
	LOG("txt_proc_buf");
	UJ i = 0, var = 0, cnt = 0, word_ptr = 0;;		

	P(!buf[0], 0);

	if (SWIPE_NEXT_VALS) {
		cnt_swipe++;													//<!!!!!!!
		var = swipe_buf(buf, i, len, 1);
		P(var == NIL, 0);
		i += var;
		SWIPE_NEXT_VALS = 0;	
	}

	if (BUF_FULLNESS) { 														//<	if  WORD_BUF is not empty
		if (valid_key(buf[i])) {

			cnt_between++;															//<!!!!!!!

			var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);
			i += var;

			if (!var || (BUF_FULLNESS == 1 && valid_key(buf[i]))) { 
				cnt_ovr_a++;														//<!!!!!!!
				txt_overflow(WORD_BUF, SZ_WBUF, buf, len, &i, prev - (SZ_WBUF - 1) + var);
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
		P(!var, cnt);													//<	WORD_BUF was already full --> non-sence || i == len - 1 
		i += var;
		P(!param && i >= len - 1, cnt);									//<	it could be an incompleted word

		if (i >= len - 1 && BUF_FULLNESS == 1)							//<	if WORD_BUF is full and it's end of buf
				SWIPE_NEXT_VALS = 1;

		if (valid_key(buf[i])) { 										//<	word is too long
			cnt_ovr_b++;																	//<!!!!!!!
			txt_overflow(WORD_BUF, SZ_WBUF, buf, len, &i, prev + i - var);
			goto CLEAN;	
		}

		P(i >= len - 1 && !param, cnt); 									//< it can be an incompleted word 

		INCLUDE:
		cnt++;
		//<	WORD_BUF gets here perfectly safe and last bytes are 0
				// O("'%ls'!\n", WORD_BUF);
		convert_str(WORD_BUF, WORD_LEN + 1);
		P(fn(tri, hsh, WORD_BUF, WORD_LEN + 1) == NIL, NIL);
		// O("'%ls'!\n", WORD_BUF);

		CLEAN:
		cnt_clean++;																		//<!!!!!!!
		BUF_FULLNESS = 0;													//<	set buf empty
		WORD_LEN = 0;
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
	UJ prev = 0, cur;

	T(INFO, "\t______________________________________________________________________");
	X(SZ_TBUF < SZ_WBUF, T(FATAL, "\t[!]\tSZ_TBUF must be >= SZ_WBUF (now SZ_TBUF = %d; SZ_WBUF = %d)", SZ_TBUF, SZ_WBUF), NIL);
	SWIPE_NEXT_VALS = 0;

	WORD_LEN = BUF_FULLNESS = cnt_swipe = cnt_ovr_a = cnt_ovr_b = cnt_between = cnt_clean = 0;

	LOOP:
	// O("LOOP\n");
	len = mfread(f, TEXT_BUF, SZ_TBUF);

	cur = ftell(f);

	i = txt_proc_buf(TEXT_BUF, struct_1, struct_2, len, fn, feof(f), prev, cur);

	P(i == NIL, NIL);
	cnt += i;
	prev = cur;

	if (!feof(f)) 
		goto LOOP;

	SWIPE_NEXT_VALS = 0;
	T(INFO, "\t[|]\tswipe: %d; beg_ovr: %d; mid_ovr: %d; cutted: %d; clean: %d;", cnt_swipe, cnt_ovr_a, cnt_ovr_b, cnt_between, cnt_clean);
	T(INFO, "\t[|]\t\t%d got to fn\t", cnt);
	// clean_buf(WORD_BUF, SZ_WBUF);
	BUF_FULLNESS = WORD_LEN = 0;
	// clean_buf(TEXT_BUF, SZ_TBUF);
	// exit(0);
	R cnt;
}



