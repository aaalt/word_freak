//< get set all adt and files

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#include "../___.h"
#include "../cfg.h"

#include "../adt/tri.h"
#include "../adt/hsh.h"
#include "str.h"
#include "txt.h"

#include "../glb.h"

//< TODO
//<	fopen_(FILE* f)
//<	tri_init()
//<	hsh_init()
//<	tri_make_stop_trie()
//<	tri_destroy()
//<	hsh_destroy()
//< TOTAL: tri.c tri.h hsh.c hsh.h

FILE* fopen_(FILE *f, S name)
{
	f = fopen(name, "r+");
	R f;
}


FILE* set_start(FILE* f)
{
	LOG("set_start");
	FILE* g;
	f = fopen_(f, TXT_FILE);
	X(!f, {T(FATAL, "FILE %s does not exist", TXT_FILE);}, (FILE*)NIL);
	
	STOP_TRIE = tri_init();
	X(!STOP_TRIE, {T(FATAL,"cannot init trie"); fclose(f);},(FILE*)NIL);
	
	TEXT_HSH = hsh_init(8, 8);
	X(!TEXT_HSH, {T(FATAL, "cannot init hash"); tri_destroy(STOP_TRIE);fclose(f);}, (FILE*)NIL);
	
	g = fopen_(g, STP_FILE);
	X(!f, {T(FATAL, "FILE %s does not exist", STP_FILE);}, (FILE*)NIL);

	X(txt_process(g, STOP_TRIE, NULL, str_tri_ins) == NIL, {T(FATAL, "cannot make stop words trie");
											tri_destroy(STOP_TRIE); 
											hsh_destroy(TEXT_HSH);fclose(f);}, (FILE*)NIL);


	// X(str_make_stop_trie(STOP_TRIE) == NIL, {T(FATAL, "cannot make stop words trie");
											// tri_destroy(STOP_TRIE); 
											// hsh_destroy(TEXT_HSH);fclose(f);}, (FILE*)NIL);
	fclose(g);
	R f;
}

UJ set_end(FILE* f)
{
	LOG("set_end");
	fclose(f);
	hsh_destroy(TEXT_HSH);
	tri_destroy(STOP_TRIE);
	R 0;
}