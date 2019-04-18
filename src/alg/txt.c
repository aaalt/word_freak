//< text processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

#include "../___.h"
#include "../glb.h"
#include "../utl/clk.h"
#include "../utl/trc.h"

#include "str.h"
#include "txt.h"
#include "chr.h"

UJ SWIPE_NEXT_VALS = 0;

I cnt_swipe, cnt_ovr_a, cnt_ovr_b, cnt_between, cnt_clean;

J WORD_LEN = 0;
STR WORD_PTR = 0;
J PTR = 0;
J CP;

UJ SZFILE(FILE* ptr)
{
	I a, b;
	a = ftell(ptr);fseek(ptr, 0, SEEK_END);
	b = ftell(ptr);fseek(ptr, a, SEEK_SET);
	R b;
}

Z inline V set_glb(UJ* var_1, J par_1, UJ* var_2, J par_2, UJ* var_3, J par_3)
{	
	if (var_1 != NULL)
		*var_1 += par_1;
	if (var_2 != NULL)
		*var_2 += par_2;
	if (var_3 != NULL)
		*var_3 += par_3;
}

V txt_overflow(STR w_buf, I sz_w_buf, STR t_buf, I sz_t_buf, UJ* i, I pos)
{
	LOG("txt_overflow");
	UJ var;
	T(WARN, "\t[!]\tword '%.20ls...' at %lu\t(max. cap. %d)", w_buf, pos, sz_w_buf);
	var = swipe_buf(t_buf, *i, sz_t_buf, 1);
	*i += var;
	if (var == NIL) {
		SWIPE_NEXT_VALS = 1;
		*i = sz_t_buf -1;
	}
}

UJ cp_buf(STR dir, STR source, I max_d, I max_s, I ptr_d, STR ptr_s, I am)
{
	I i;
	for (i = 0; i < am && i + ptr_s < &source[max_s] && ptr_d - am + i < max_d; i++)
		dir[ptr_d - am + i] = *(ptr_s + i);
	R i;
}

UJ txt_get_word(STR dir, STR source, UJ max_d, UJ max_s, UJ ptr, C param)
{
	LOG("txt_get_word");
	UJ i, j;

	if (param || (max_d != NIL && dir != NULL)) {
		for (i = 0, j = WORD_LEN; valid_key(source[ptr + i]) && i < max_s - 1 && j + i < max_d - 1; i++) 
				dir[j + i] = source[ptr + i];
		dir[j + i] = 0;
	}
	else 
		for (i = 0; valid_key(source[ptr + i]) && i < max_s - 1; i++);

	R i;														//<	i 	-->	amount of copied chars
}

//<		enter stats:
//<		1.	prev buf was normal; start from scratch
//<		2.	prev buf had an incompeleted word (len <= SZ_WBUF); read the rest of a word ; if word end at the end of buf, then don't write, just swipe next buf
//<		3.	prev buf had an incompleted word (len > SZ_WBUF); just swipe this buf 
UJ txt_proc_buf(STR buf, V* hsh, V* stop, I len, WORD_ADD fn, I param, sz el)
{
	LOG("txt_process");
	//<	WORD_LEN, WORD_PTR set, CONT
	UJ var = 0, cnt = 0;
	PTR = 0;

	if (WORD_LEN)
		goto GET_TXT;

	if (SWIPE_NEXT_VALS) {														//<	if previous word was too long and all next valids should be swiped (till first unvalid)
		cnt_swipe++;
		var = swipe_buf(buf, PTR, len, 1);
		P(var == NIL, 0);														//<	if the whole buf consits of unvalids
		set_glb(&PTR, var, &SWIPE_NEXT_VALS, -1, NULL, 0);
	}

	W(PTR < len - 1) {
		var = swipe_buf(buf, PTR, len, 0);
		P(var == NIL, cnt);
		PTR += var;
		if (!WORD_LEN)
			WORD_PTR = &buf[PTR];

		GET_TXT:
		var = txt_get_word((STR)NIL, buf, NIL, len, PTR, 0);				//< var == 0:  ptr  == len -1 	--> end of buf
		set_glb(&PTR, var, &WORD_LEN, var, NULL, 0);

		if (PTR >= len - 1) {												//<	end of buf
			if (param) {													//<	end of file
				goto INCLUDE; 								//<	from WORD_PTR
			}
			else {
				if (WORD_LEN <= SZ_WBUF) {		//< var 1 || var 2
					cp_buf(WORD_BUF, WORD_BUF, SZ_WBUF, len - 1 + SZ_WBUF, SZ_WBUF, WORD_PTR, WORD_LEN);
					WORD_PTR = &WORD_BUF[SZ_WBUF - WORD_LEN];
					R cnt;
				}
				else 
				if (WORD_LEN > SZ_WBUF || WORD_PTR < &buf[0]) {		//< var 2.2	[too long for everything, just swipe it]
					txt_overflow(WORD_PTR, SZ_WBUF, buf, len, &PTR, 0);
					set_glb(&SWIPE_NEXT_VALS, 1, &WORD_LEN, (WORD_LEN * -1), NULL, 0);
					R cnt;
				}
			}
		}

		INCLUDE:
		cnt++;
		//<	WORD_BUF gets here perfectly safe and last bytes are 0
		P(convert_str(WORD_PTR, WORD_LEN, &buf[len - 1] - WORD_PTR, 1) == NULL, NIL);
		P(fn(hsh, stop, WORD_PTR, WORD_LEN + 1, el) == NIL, NIL);

		CLEAN:
		cnt_clean++;			
		set_glb(&WORD_LEN, (WORD_LEN * -1), (UJ*)WORD_PTR, ((J)WORD_PTR * -1), NULL, 0);
	}
	R cnt;
}

//< FILE* f into TRIE and HSH
//< fn: str_hsh_ins or str_tri_ins
//< parsing main loop
UJ txt_process(FILE* f, V* struct_1, V* struct_2, WORD_ADD fn, sz el)
{
	LOG("txt_process");
	UJ len, cnt = 0, i = 0;
	UJ prev = 0;

	T(INFO, "\t______________________________________________________________________");
	X(SZ_TBUF < SZ_WBUF, T(FATAL, "\t[!]\tSZ_TBUF must be >= SZ_WBUF (now SZ_TBUF = %d; SZ_WBUF = %d)", SZ_TBUF, SZ_WBUF), NIL);
	SWIPE_NEXT_VALS = 0;

	WORD_LEN = PTR = cnt_swipe = cnt_ovr_a = cnt_ovr_b = cnt_between = cnt_clean = 0;
	WORD_PTR  = 0;
	LOOP:
	len = mfread(f, TEXT_BUF, SZ_TBUF);
	i = txt_proc_buf(TEXT_BUF, struct_1, struct_2, len, fn, feof(f), el);

	P(i == NIL, NIL);
	cnt += i;
	PTR = 0;

	if (!feof(f)) 
		goto LOOP;

	set_glb(&WORD_LEN, 0, &SWIPE_NEXT_VALS, 0, &CP, 0);

	T(INFO, "\t[|]\tswipe: %d; beg_ovr: %d; mid_ovr: %d; cutted: %d; clean: %d;", cnt_swipe, cnt_ovr_a, cnt_ovr_b, cnt_between, cnt_clean);
	WORD_LEN = PTR = cnt_swipe = cnt_ovr_a = cnt_ovr_b = cnt_between = cnt_clean = 0;
	T(INFO, "\t[|]\t\t%d got to fn\t", cnt);
	
	R cnt;
}
