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

UJ SWIPE_NEXT_VALS = 0;

I cnt_swipe, cnt_ovr_a, cnt_ovr_b, cnt_between, cnt_clean;

UJ WORD_LEN = 0;
UJ WORD_PTR = 0;
UJ PTR = 0;
UJ CP;

UJ SZFILE(FILE* ptr)
{
	I a, b;
	a = ftell(ptr);fseek(ptr, 0, SEEK_END);
	b = ftell(ptr);fseek(ptr, a, SEEK_SET);
	R b;
}

Z inline V set_glb(UJ* var_1, UJ par_1, UJ* var_2, UJ par_2, UJ* var_3, UJ par_3)
{	
	if (var_1 != NULL)
		*var_1 = par_1;
	if (var_2 != NULL)
		*var_2 = par_2;
	if (var_3 != NULL)
		*var_3 = par_3;
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

UJ cp_buf(STR dir, STR source, I max_d, I max_s, I ptr_d, I ptr_s, I am)
{
	I i;
	for (i = 0; i < am && i + ptr_s < max_s && i + ptr_d < max_d; i++) 
		dir[i + ptr_d] = source[ptr_s + i];
	dir[i + ptr_d] = 0;
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

UJ txt_proc_buf(STR buf, V* hsh, V* stop, I len, WORD_ADD fn, I param, sz el)
{
	LOG("txt_proc_buf");
	UJ i = 0, var = 0, cnt = 0;
	P(!buf[0], 0);
	set_glb(&PTR, 0, &CP, 0, NULL, 0);
	T(INFO, "\n");

	if (SWIPE_NEXT_VALS) {														//<	if previous word was too long and all next valids should be swiped (till first unvalid)
		cnt_swipe++;
		var = swipe_buf(buf, PTR, len, 1);
		if (var == NIL)
			T(INFO, "\tall valids were swiped");
		else 
			T(INFO, "\tswipe %d valids", var);
		P(var == NIL, 0);														//<	if the whole buf consits of unvalids
		set_glb(&PTR, PTR + var, &SWIPE_NEXT_VALS, 0, NULL, 0);
	}

	if (WORD_LEN) {																//<	if the beginning of word was in previous buf
		T(INFO, "\tword was at the end of buf [%d %ls]", WORD_LEN, WORD_BUF);
		if (valid_key(buf[PTR])) {												//<	if continuation of word exists in current buf
			cnt_between++;
			var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, PTR, 1);
			set_glb(&WORD_LEN, WORD_LEN + var, &PTR, PTR + var, NULL, 0);
			T(INFO, "\ttotal word |%ls| [%d]", WORD_BUF, WORD_LEN);
			if (!var || (WORD_LEN >= SZ_WBUF - 1 && valid_key(buf[PTR]))) {		//<	if it's end of buf or word has a max len (WORD_LEN == SZ_WBUF - 1) and there is a continuation of word in buf
				cnt_ovr_a++;
				if (!var) {											//<	if it's end of buf or word has a max len (WORD_LEN == SZ_WBUF - 1) and there is a continuation of word in buf
					SWIPE_NEXT_VALS = 1;
					T(INFO,"\tit's end of buf or word already had max_len");
				}
				else
					T(INFO, "\tword has max len and next kay is valid. OVERFLOW");
				txt_overflow(WORD_BUF, SZ_WBUF, buf, len, &PTR, 0);
				goto CLEAN;
			}			
		}
		T(INFO, "\tand now looks like |%ls| [%d]", WORD_BUF, WORD_LEN);
		CP = 1;
		goto INCLUDE;
	}

	W (PTR < len - 1) {
		var = swipe_buf(buf, PTR, len, 0);
		if (var == NIL) 
			T(INFO, "\tend of buf was swiped (unvalids) [last 2: |%lc|%lc|]", buf[len - 3], buf[len -2]);
		else
			T(INFO, "\tswiped %d unvalids", var);

		P(var == NIL, cnt);														//<	if it's end of  buf

		set_glb(&PTR, PTR + var, &WORD_PTR, PTR + var, NULL, 0);

		var = txt_get_word((STR)NIL, buf, NIL, len, PTR, 0);

		if (!var) {
			if (!param)
				R cnt;
			else {
				if (WORD_LEN) {
					cp_buf(WORD_BUF, buf, SZ_WBUF -1, len -1, 0, WORD_PTR, WORD_LEN);
					CP = 1;
					goto INCLUDE;
				}
			}

		}
		set_glb(&WORD_LEN, WORD_LEN + var, &PTR, PTR + var, NULL, 0);

		if (!param && PTR >= len - 1) {											//<	if it's end of buf and not end of file
			if (WORD_LEN < SZ_WBUF && !CP) {
				CP = 1;
				cp_buf(WORD_BUF, buf, SZ_WBUF -1, len -1, 0, WORD_PTR, WORD_LEN);
				T(INFO, "\tend of buf, but not end of file. will be checked comparing with next buf |%ls| [%d]", WORD_BUF, WORD_LEN);
			}
			else {
				if (WORD_LEN >= SZ_WBUF) {
					T(INFO, "\tend of buf, but not end of file. word is too long, no need in check |%ls| [%d]", WORD_BUF, WORD_LEN);	
					set_glb(&SWIPE_NEXT_VALS, 1, &WORD_LEN, 0, NULL, 0);
				}
				else
					T(INFO, "\tend of buf, but not end of file. will be checked comparing with next buf |%ls| [%d]", WORD_BUF, WORD_LEN);
			}
			R cnt;
		}

		if (PTR >= len - 1 && WORD_LEN >= SZ_WBUF - 1) {							//<	if it's end of buf and word_len is max
			T(INFO, "\tword is too long and it's end of file");
			if (!CP) { 
				CP = 1;
				cp_buf(WORD_BUF, buf, SZ_WBUF - 1, len - 1, 0, WORD_PTR, var);
			}
			set_glb(&SWIPE_NEXT_VALS, 1, &WORD_LEN, 0, NULL, 0);
		}

		INCLUDE:
		cnt++;
		//<	WORD_BUF gets here perfectly safe and last bytes are 0
		
		if (CP) {																//<	if word is stored in WORD_BUF
			convert_str(WORD_BUF, WORD_LEN, 1);
			T(INFO, "\t1.|%ls|%d", WORD_BUF, WORD_LEN);
			P(fn(hsh, stop, WORD_BUF, WORD_LEN + 1, el) == NIL, NIL);
		}
		else {																	//<	if word is stored as WORD_PTR in buf
			convert_str((STR)&buf[WORD_PTR], WORD_LEN, 1);
			P(fn(hsh, stop, (STR)&buf[WORD_PTR], WORD_LEN + 1, el) == NIL, NIL);
			T(INFO, "\t0.|%ls|%d [next 2: |%lc|%lc|]", &buf[WORD_PTR], WORD_LEN, buf[WORD_PTR + WORD_LEN], buf[WORD_PTR + WORD_LEN + 1]);
		}
		CLEAN:
		cnt_clean++;			
		set_glb(&WORD_LEN, 0, &WORD_PTR, 0, &CP, 0);
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

	WORD_LEN = cnt_swipe = cnt_ovr_a = cnt_ovr_b = cnt_between = cnt_clean = 0;

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
	T(INFO, "\t[|]\t\t%d got to fn\t", cnt);
	
	R cnt;
}
