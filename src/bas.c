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

#define LEN_ 5000
#define _LEN_ 50

C TEXT_BUF_[LEN_];
C WORD_BUF_[_LEN_];

S TEXT_BUF = TEXT_BUF_;
S WORD_BUF = WORD_BUF_;

I main()
{
	FILE *f;
	P(set_start(f) == NIL, 1);					//< make adt and open files
	X(txt_process(f) == NIL, set_end(f), 1);	//< process the text 
	X(str_hsh_print(TEXT_HSH) == NIL, set_end(f), 1);		//< print results
	set_end(f);									//<	clean
	R0;
}