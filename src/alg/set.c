//< get set all adt and files
#include <stdio.h>
#include <stdlib.h>

#include "../___.h"
#include "../utl/clk.h"

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
	UJ t;
	f = mfopen(f, TXT_FILE);
	X(!f, 			{T(FATAL, "FILE %s does not exist", 	TXT_FILE);}, (FILE*)NIL);

	g = mfopen(g, STP_FILE);
	X(!g, 			{T(FATAL, "FILE %s does not exist", 	STP_FILE);
															fclose(f);}, (FILE*)NIL);
	clk_start();
	STOP_TRIE = tri_init();
	X(!STOP_TRIE, 	{T(FATAL,"cannot init trie"); 			fclose(f);
															fclose(g);}, (FILE*)NIL);
	t = clk_stop();
	T(INFO, "\t[+]\ttri_init\t\t\t\t\t\t%lums", STOP_TRIE->cnt, STOP_TRIE->mem, t);

	clk_start();
	X(txt_process(g, STOP_TRIE, NULL, str_tri_ins) == NIL, 
					{T(FATAL, "cannot make stop words trie");
															tri_destroy(STOP_TRIE); 
															fclose(g);
															fclose(f);}, (FILE*)NIL);
	t = clk_stop();
	T(INFO, "\t[+]\ttxt_process for trie (%d stop words inserted)\t\t%lums", STOP_TRIE->cnt, t);
	
	TEXT_HSH = hsh_init(256, 3);
	X(!TEXT_HSH, 	{T(FATAL, "cannot init hash"); 			tri_destroy(STOP_TRIE); 
															fclose(f);
															fclose(g);}, (FILE*)NIL);
	fclose(g);
	R f;
}

UJ set_end(FILE* f)
{
	LOG("set_end");
	UJ t;
	sz mem = TEXT_HSH->mem + STOP_TRIE->mem + SZ(FILE*);
	clk_start();
	fclose(f);
	hsh_destroy(TEXT_HSH);
	tri_destroy(STOP_TRIE);
	t = clk_stop();
	T(INFO, "\t[-]\tdeallocate mem of trie, hsh and file (mem %d)\t%lums", mem, t);
	T(INFO, "\t[?]\tSZ_WBUF: %d\tSZ_TBUF: %d\tEXT_KEY_AM: %d (%s)\t\t", SZ_WBUF, SZ_TBUF, EXT_KEY_AM, EXT_KEY);
	R 0;
}