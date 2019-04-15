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
	UJ t, ptr;

	X(TXT_DATA_TYPE != 1 && TXT_DATA_TYPE != 4, T(FATAL, "\t[!]\tinvalid data type size\t[TXT_DATA_TYPE = %d]", TXT_DATA_TYPE), (FILE*)NIL);

	f = mfopen(f, TXT_FILE);
	X(!f, 			{T(FATAL, "FILE %s does not exist", 	TXT_FILE);}, (FILE*)NIL);

	g = mfopen(g, STP_FILE);
	X(!g, 			{T(FATAL, "FILE %s does not exist", 	STP_FILE);
															fclose(f);}, (FILE*)NIL);
	clk_start();
	
	STOP_HSH = hsh_init(16, 2);
	X(!STOP_HSH, 	{T(FATAL,"cannot init stop_hash"); 			fclose(f);
															fclose(g);}, (FILE*)NIL);

	t = clk_stop();
	T(INFO, "\t[+]\tstop_hsh_init\t\t\t\t\t\t%lums", STOP_HSH->cnt, STOP_HSH->mem, t);

	clk_start();

	ptr = txt_process(g, STOP_HSH, NULL, (WORD_ADD)str_hsh_ins, SZ(CHAR));
	if (ptr == NIL){
		T(FATAL, "cannot make stop_hash");
		hsh_destroy(STOP_HSH); 
		fclose(g);
		fclose(f); 
		R (FILE*)NIL;
	}

	t = clk_stop();
	T(INFO, "\t[+]\ttxt_process for stop_hash (%d stop words inserted)\t%lums", STOP_HSH->cnt, t);
	
	TEXT_HSH = hsh_init(2048, 2);
	X(!TEXT_HSH, 	{T(FATAL, "cannot init txt_hash"); 			hsh_destroy(STOP_HSH); 
															fclose(f);
															fclose(g);}, (FILE*)NIL);
	fclose(g);
	R f;
}

UJ set_end(FILE* f)
{
	LOG("set_end");
	UJ t;
	sz mem = TEXT_HSH->mem + STOP_HSH->mem + SZ(FILE*);
	clk_start();
	fclose(f);
	hsh_destroy(TEXT_HSH);
	hsh_destroy(STOP_HSH);
	t = clk_stop();
	T(INFO, "\t[-]\tdeallocate mem of trie, hsh and file (mem %d)\t%lums", mem, t);
	T(INFO, "\t[?]\tSZ_WBUF: %d\tSZ_TBUF: %d\tEXT_KEY_AM: %d (%ls)\t\t", SZ_WBUF, SZ_TBUF, EXT_KEY_AM, EXT_KEY);
	R 0;
}