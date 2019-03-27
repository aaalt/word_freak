//< main loop

#include <stdio.h>
#include <stdlib.h>

#include "___.h"
#include "cfg.h"

#include "alg/txt.h"
#include "alg/set.h"
#include "alg/str.h"

//< TODO
//<	fopen_()

TRIE STOP_TRIE;
HT TEXT_HSH;

// #define LEN_ 5000
// #define _LEN_ 50

C TEXT_BUF_[SZ_TBUF];
C WORD_BUF_[SZ_WBUF];

S TEXT_BUF = &TEXT_BUF_[0];
S WORD_BUF = &WORD_BUF_[0];

FILE* f_ptr;

I main()
{
	// FILE *f;
	P(set_start(f_ptr) == NIL, 1);					//< make adt and open files
	O("%d!\n", f_ptr);
	X(txt_process(f_ptr) == NIL, set_end(f_ptr), 1);	//< process the text 
	X(str_hsh_print(TEXT_HSH) == NIL, set_end(f_ptr), 1);		//< print results
	set_end(f_ptr);									//<	clean
	R0;
}