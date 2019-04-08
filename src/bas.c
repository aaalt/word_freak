//< main of word_freak

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>


#include "___.h"
#include "cfg.h"
#include "utl/trc.h"

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

I main()
{
	f_ptr = set_start(f_ptr);
	P((UJ)f_ptr == NIL, 1);

	X(txt_process(f_ptr, STOP_TRIE, TEXT_HSH, str_hsh_ins) == NIL, 
											set_end(f_ptr), 1);
	// O("FINE!\n");
	X(str_hsh_print(TEXT_HSH) == NIL, 		set_end(f_ptr), 1);		

	// tri_dump(STOP_TRIE);


	set_end(f_ptr);												
	R0;
}