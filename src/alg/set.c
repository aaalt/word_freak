//< get set all adt and files
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#include "../___.h"

#include "../adt/tri.h"
#include "../adt/hsh.h"
#include "str.h"
#include "txt.h"

#include "../glb.h"


FILE* mfopen(FILE *f, S name)
{
	f = fopen(name, "r+");
	R f;
}


FILE* set_start(FILE* f)
{
	LOG("set_start");
	FILE* g;
	f = mfopen(f, TXT_FILE);
	X(!f, 			{T(FATAL, "FILE %s does not exist", 	TXT_FILE);}, (FILE*)NIL);

	g = mfopen(g, STP_FILE);
	X(!g, 			{T(FATAL, "FILE %s does not exist", 	STP_FILE);
															fclose(f);}, (FILE*)NIL);
	
	STOP_TRIE = tri_init();
	X(!STOP_TRIE, 	{T(FATAL,"cannot init trie"); 			fclose(f);
															fclose(g);}, (FILE*)NIL);

	X(txt_process(g, STOP_TRIE, NULL, str_tri_ins) == NIL, 
					{T(FATAL, "cannot make stop words trie");
															tri_destroy(STOP_TRIE); 
															fclose(g);
															fclose(f);}, (FILE*)NIL);
	
	TEXT_HSH = hsh_init(8, 8);
	X(!TEXT_HSH, 	{T(FATAL, "cannot init hash"); 			tri_destroy(STOP_TRIE); 
															fclose(f);
															fclose(g);}, (FILE*)NIL);

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