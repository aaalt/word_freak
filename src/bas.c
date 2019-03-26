//< main loop

#include <stdio.h>
#include <stdlib.h>

#include "___.h"
#include "cfg.h"

#include "alg/txt.h"
#include "alg/set.h"

//< TODO
//<	hsh_print()


I main()
{
	FILE *f;
	P(set_start(f) == NIL, 1);					//< make adt and open files
	X(txt_process(f) == NIL, set_end(f), 1);	//< process the text 
	X(hsh_print() == NIL, set_end(f), 1);		//< print results
	set_end(f);									//<	clean
	R0;
}