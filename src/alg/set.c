//< get set all adt and files

#include <stdio.h>
#include <stdlib.h>

#include "../___.h"
#include "../cfg.h"

#include "../adt/tri.h"
#include "../adt/hsh.h"
#include "str.h"

//< TODO
//<	fopen_(FILE* f)
//<	tri_init()
//<	hsh_init()
//<	tri_make_stop_trie()
//<	tri_destroy()
//<	hsh_destroy()
//< TOTAL: tri.c tri.h hsh.c hsh.h

FILE* fopen_(S name)
{
	FILE*f = fopen(name, "r");
	R f;
}


UJ set_start(FILE* f)
{
	LOG("set_start");
	f  = fopen_(TXT_FILE);
	X(!f, {T(FATAL, "FILE %s does not exist", TXT_FILE);}, NIL);
	STOP_TRIE = tri_init();
	X(!STOP_TRIE, {T(FATAL,"cannot init trie"); fclose(f);},NIL);
	TEXT_HSH = hsh_init(1, 2);
	X(!TEXT_HSH, {T(FATAL, "cannot init hash"); tri_destroy(STOP_TRIE);}, NIL);
	X(str_make_stop_trie(STOP_TRIE) == NIL, {T(FATAL, "cannot make stop words trie");
											tri_destroy(STOP_TRIE); 
											hsh_destroy(TEXT_HSH);}, NIL);
	R 0;
}

UJ set_end(FILE* f)
{
	LOG("set_end");
	fclose(f);
	hsh_destroy(TEXT_HSH);
	tri_destroy(STOP_TRIE);
	R 0;
}