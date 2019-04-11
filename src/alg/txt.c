//< text processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../___.h"
#include "../glb.h"
#include "../utl/clk.h"

#include "str.h"
#include "txt.h"
#include "chr.h"

G SWIPE_NEXT_VALS = 0;

UJ SZFILE(FILE* ptr)
{
	I a, b;
	a = ftell(ptr);fseek(ptr, 0, SEEK_END);
	b = ftell(ptr);fseek(ptr, a, SEEK_SET);
	R b;
}

UJ txt_get_word(S dir, S source, I max_d, I max_s, I ptr)
{
	LOG("txt_get_word");
	UJ i, j;
	for (j = 0; j < max_d - 1 && valid_key(dir[j]); j++);		//<	j --> next available pos in dir
	for (i = 0; valid_key(source[ptr + i]) && i < max_s - 1 && j + i < max_d - 1; i++)
		dir[j + i] = source[ptr + i];
	dir[j + i] = 0;
	R i;														//<	i 	-->	amount of copied chars
}

V overflow(S w_buf, I sz_w_buf, S t_buf, I sz_t_buf, UJ* i, I pos)
{
	LOG("overflow");
	UJ var;
	T(WARN, "\t[!]\tword '%.10s...' at %d (max. cap. %d)", w_buf, pos, sz_w_buf);
	var = swipe_buf(t_buf, *i, sz_t_buf, 1);
	*i += var;
	if (var == NIL) {
		SWIPE_NEXT_VALS = 1;
		*i = sz_t_buf -1;
	}
}

UJ txt_proc_buf(FILE* f, S buf, V* tri, V* hsh, I len, WORD_ADD fn, I param)
{
	LOG("txt_proc_buf");
	UJ i = 0, j = 0, var = 0, cnt = 0;		

	if (SWIPE_NEXT_VALS) {
		var = swipe_buf(buf, i, len, 1);
		P(var == NIL, 0);
		i += var;
		if (i < len - 1)
			SWIPE_NEXT_VALS = 0;	
	}

	if (valid_key(WORD_BUF[0])) {
		if (valid_key(buf[i])) {
			var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);
			if (!var) {													//<	WORD_BUF was already full ||	i == len - 1 --> non-sence 
				overflow(WORD_BUF, SZ_WBUF, buf, len, &i, ftell(f) - len - SZ_WBUF);
				goto CLEAN;
			}
			i += var;
			if (valid_key(WORD_BUF[SZ_WBUF - 2]) && valid_key(buf[i])) {	//<	WORD_BUF is full and word is longer than SZ_WBUF
				overflow(WORD_BUF, SZ_WBUF, buf, len, &i, ftell(f) - len - SZ_WBUF + var);
				goto CLEAN;	
			}	
		}
		goto INCLUDE;	
	}

	W (i < len - 1) {
		var = swipe_buf(buf, i, len, 0);								//<	swipe unvalids		
		P(var == NIL, cnt);												//<	it's end of buf and all posiible words were processed
		i += var;
		var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);
		P(!var, cnt);													//<	WORD_BUF was already full --> non-sence || i == len - 1 - 
		i += var;
		P(!param && i >= len - 1, cnt);									//<	it could be an incompleted word

		if (var == SZ_WBUF - 1 && i >= len -1) 							//<	if WORD_BUF is full and it's end of buf
				SWIPE_NEXT_VALS = 1;

		if (valid_key(buf[i])) { 										//<	word is too long
			overflow(WORD_BUF, SZ_WBUF, buf, len, &i, ftell(f) - len + i - SZ_WBUF);
			goto CLEAN;	
		}

		P(i >= len - 1 && !param, 0); 									//< it can be an incompleted word 

		INCLUDE:
		cnt++;
		P(fn(tri, hsh, WORD_BUF, sz_buf(WORD_BUF, SZ_WBUF)) == NIL, NIL);
		CLEAN:
		clean_buf(WORD_BUF, SZ_WBUF);
	}
	R cnt;
}

//< FILE* f into TRIE and HSH
//< fn: str_hsh_ins or str_tri_ins
//< parsing main loop
UJ txt_process(FILE* f, V* struct_1, V* struct_2, WORD_ADD fn)
{
	LOG("txt_process");
	UJ len, res, cnt = 0, t, i = 0;

	T(INFO, "\n\n\t\t\t[txt_process]");
	X(SZ_TBUF < SZ_WBUF, T(FATAL, "\t[!]\tSZ_TBUF must be >= SZ_WBUF (now SZ_TBUF = %d; SZ_WBUF = %d)", SZ_TBUF, SZ_WBUF), NIL);

	LOOP:
	len = fread(TEXT_BUF, SZ(C), SZ_TBUF - 1, f);
	TEXT_BUF[len] = 0;
	i = txt_proc_buf(f, TEXT_BUF, struct_1, struct_2, len + 1, fn, feof(f));
	cnt += i;
	P(i == NIL, NIL);
	if (!feof(f)) goto LOOP;

	clean_buf(WORD_BUF, SZ_WBUF);
	clean_buf(TEXT_BUF, SZ_TBUF);
	R 0;
}



/*UJ txt_get_word(FILE* f, S dir, S source, I max_d, I max_s, I ptr)
{
	LOG("txt_get_word");

	UJ i, j;

	for (j = 0; j < max_d - 1 && valid_key(dir[j]); j++);		//<	j --> next available pos in dir
	P(j == max_d - 1, NIL);

	for (i = 0; valid_key(source[ptr + i]) && i < max_s - 1 && j + i < max_d - 1; i++)
		dir[j + i] = source[ptr + i];
	dir[j + i] = 0;

	R (i == max_s - 1 || j + i == max_d - 1) ? NIL : i;			//<	i --> amount of copied chars

}*/

//<	txt_get_word(FILE* f, S dir, S source, I max_d, I max_s, I ptr)
//<	
//<		get next_pos at dir 
//<		if next_pos == max_d - 1 				//<	R NIL next check (valid(dir[max_d - 2]))
//<			tell that it's close to dir overflow 
//<			swipe all next valids at source
//<			update source ptr
//<			goto CLEAN 
//<		copy from source into dir (iterating)
//<		if (j + i == max_d - 1)					//<	end of dir | R NIL next check (valid(dir[max_d - 2]))
//<			do the same as before
//<	
//<		if (i == max_s - 1)						//<	end of source | R NIL next check param
//<			if (end of file)
//<				goto INCLUDE 
//<			else go out of txt_process_buf	
//<		return i


/*
UJ txt_get_word(FILE* f, S dir, S source, I max_d, I max_s, I ptr)
{
	LOG("txt_get_word");
	UJ j, i;

	for (j = 0; valid_key(dir[j]) && j < max_d; j++);				//< j is the next available dir pos or sz(dir)

	// X(j == max_d, T(WARN, "WORD_BUF is overflowed (max. capcity %d); file ptr at %d", max_d, ftell(f)), NIL);

	for (i = 0; valid_key(source[ptr + i]) && i < max_s && j + i < max_d; i++) 
		dir[j + i] = source[ptr + i];
	dir[j + i] = 0;

	R (i == max_s || j + i == max_d) ? NIL : i;						//<	j + i == max_d 	-->	dir overflow
} */

//<	process buf and break it into words 
//<	use fn for each word found
/*UJ txt_process_buf(FILE* f, S buf, V* tri, V* hsh, I len, WORD_ADD fn, I param)
{
	LOG("txt_process_buf");
	I i = 0, var;
	if (valid_key(WORD_BUF[0])) {
		if (valid_key(buf[0])) {
			var = txt_get_word(f, WORD_BUF, buf, SZ_WBUF, len, i);		
			P (var == NIL, NIL);									//< WORD_BUF or buf is overfloweds 1
			i += var;
		}
		goto INCLUDE;
	}

	W(i < len - 1) {
		var = swipe_buf(buf, i, len, 0);							//< how many chars swiped or NIL
		P(var == NIL, 0);											//< end of buf
		i += var;													//< move pointer

		var = txt_get_word(f, WORD_BUF, buf, SZ_WBUF, len, i);		//< length of word or NIL

		if (var == NIL && valid_key(WORD_BUF[SZ_WBUF - 2]))  {		//<	WORD_BUF[len - 1] should ALWAYS be zero 

			T(WARN, "WORD_BUF is overflowed (max. capcity %d); file ptr at %d [%s]", SZ_WBUF, ftell(f) - SZ_WBUF, WORD_BUF);
		}
		

		// P(var == NIL, NIL);											//< problems with buf's capacity
		i += var;

		P(i >= len - 1 && !param, 0); 								//< it could be an incompleted word 

		INCLUDE:
		P(fn(tri, hsh, WORD_BUF, sz_buf(WORD_BUF, SZ_WBUF)) == NIL, NIL);
		CLEAN:
		clean_buf(WORD_BUF, SZ_WBUF);
	}

	fflush(stdout);
	R0;
}*/

//< FILE* f into TRIE and HSH
//< fn: str_hsh_ins or str_tri_ins
//< parsing main loop
/*UJ txt_process(FILE* f, V* struct_1, V* struct_2, WORD_ADD fn)
{
	LOG("txt_process");
	UJ len, res;

	LOOP:
	len = fread(TEXT_BUF, SZ(C), SZ_TBUF - 1, f);
	TEXT_BUF[len] = 0;
	P(txt_process_buf(f, TEXT_BUF, struct_1, struct_2, len + 1, fn, feof(f)) == NIL, NIL);
	if (!feof(f)) goto LOOP;

	clean_buf(WORD_BUF, SZ_WBUF);
	clean_buf(TEXT_BUF, SZ_TBUF);
	R 0;
}
*/






