//< main of word_freak

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>


#include "___.h"
#include "cfg.h"
#include "utl/trc.h"
#include "utl/clk.h"

#include "alg/txt.h"
#include "alg/set.h"
#include "alg/str.h"

// #include "adt/tri.h"

TRIE STOP_TRIE;
HT TEXT_HSH;

C TEXT_BUF_[SZ_TBUF];
C WORD_BUF_[SZ_WBUF];

S TEXT_BUF = &TEXT_BUF_[0];
S WORD_BUF = &WORD_BUF_[0];

C EXT_KEY_[EXT_KEY_AM] = "'";
S EXT_KEY = &EXT_KEY_[0];

FILE* f_ptr;

UJ SZFILE(FILE* ptr)
{
	I a, b;
	a = ftell(ptr);
	fseek(ptr, 0, SEEK_END);
	b = ftell(ptr);
	fseek(ptr, a, SEEK_SET);
	R b;

}


I main()
{
	UJ t1, t2, t3, t4, szfile;
	clk_start();
	f_ptr = set_start(f_ptr);
	t1 = clk_stop();
	P((UJ)f_ptr == NIL, 1);

	// clk_start();
	X(txt_process(f_ptr, STOP_TRIE, TEXT_HSH, str_hsh_ins) == NIL, 
											set_end(f_ptr), 1);
	t2 = clk_stop();

	// clk_start();
	X(str_hsh_print(TEXT_HSH) == NIL, 		set_end(f_ptr), 1);		
	t3 = clk_stop();

	szfile = SZFILE(f_ptr);
	O("set_start\t%lums\n", t1);
	O("txt_process\t%lums\tszfile %lu\tinserted %lu\n", t2, szfile, TEXT_HSH->cnt);
	O("str_hsh_print\t%lums\n", t3);
		set_end(f_ptr);			

	R0;
}