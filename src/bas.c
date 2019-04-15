//< main of word_freak
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#include "___.h"

#include "utl/trc.h"
#include "utl/clk.h"

#include "alg/txt.h"
#include "alg/set.h"
#include "alg/str.h"

// TRIE STOP_HSH;
HT STOP_HSH;
HT TEXT_HSH;
/*
C TEXT_BUF_[SZ_TBUF];
C WORD_BUF_[SZ_WBUF];

S TEXT_BUF = &TEXT_BUF_[0];
S WORD_BUF = &WORD_BUF_[0];

C EXT_KEY_[EXT_KEY_AM] = "'";
S EXT_KEY = &EXT_KEY_[0];
*/
CHAR TEXT_BUF_[SZ_TBUF];
CHAR WORD_BUF_[SZ_WBUF];

STR TEXT_BUF = &TEXT_BUF_[0];
STR WORD_BUF = &WORD_BUF_[0];

// CHAR EXT_KEY_[EXT_KEY_AM] = "'";
CHAR EXT_KEY_[EXT_KEY_AM + 1] = L"'_-";
// CHAR EXT_KEY_[EXT_KEY_AM] = {'\'', '_', '-'};

STR EXT_KEY = &EXT_KEY_[0];

FILE* f_ptr;

I main()
{
	LOG("_main_");
	setlocale(LC_ALL, "");
	UJ t;
	clock_t s;
	srand(0);

	f_ptr = set_start(f_ptr);

	P((UJ)f_ptr == NIL, 1);
	s = clk_start();
	X(txt_process(f_ptr, STOP_HSH, TEXT_HSH, str_hsh_ins) == NIL, 
		set_end(f_ptr), 1);		
	t = clk_since(s);
	T(INFO, "\t[~]\ttxt_process for ht (inserted %d str)\t\t\t%lums", TEXT_HSH->cnt, t);
	X(str_hsh_print(TEXT_HSH) == NIL, 		set_end(f_ptr), 1);		
	set_end(f_ptr);			
	R0;
}

