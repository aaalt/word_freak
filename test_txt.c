//< text processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/___.h"
#include "src/cfg.h"

#include "src/adt/tri.h"
#include "src/alg/str.h"

#include "src/glb.h"

TRIE STOP_TRIE;
HT TEXT_HSH;

// #define LEN_ 5000
// #define _LEN_ 50

C TEXT_BUF_[SZ_TBUF];
C WORD_BUF_[SZ_WBUF];

S TEXT_BUF = &TEXT_BUF_[0];
S WORD_BUF = &WORD_BUF_[0];
/*


typedef UI HTYPE; //< hash width
typedef HTYPE(*HSH_FN)(S val, UI len); //< hsh_each function interface

typedef struct bucket{
	HTYPE h;				//< value hash
	UH n;					//< value len
	struct bucket* next;	//< pointer to next 
//	HTYPE idx;				//< table index
	C packed;				//< in heap
	V* payload;				//< pointer to payload
	G s[];					//< value bytes
} __attribute__((packed))  pBKT;

const Z sz SZ_BKT = SZ(pBKT); //< bucket header size
typedef pBKT* BKT;

typedef struct hash_table {
	HTYPE	split;		//< split position
	HTYPE	level;		//< capacity is 2^level
	H		rounds;		//< split rounds	
	HTYPE	cnt; 		//< total values
	HTYPE	bcnt;		//< occupied buckets
	HSH_FN  fn;			//< hash function
	sz 		mem;		//< total byte size
	V*		heap;		//< bucket heap pointer
	BKT* 	buckets;	//< pointer to array of bucket pointers
} pHT;

const Z sz SZ_HT = SZ(pHT); //< hash table header size
typedef pHT* HT;
*/

UJ cnt_upd(UJ* cnt_0, UJ* cnt_1, UJ* cnt_2, UJ res)
{
	SW(res) {
		CS(0, {*cnt_0++;});
		CS(1, {*cnt_1++;});
		CS(2, {*cnt_2++;});
	}	
	R res;
}


C in_alphabet(C c)
{
	R (IN('A', c, 'Z') || IN('a', c, 'z') || c == '\'') ? 1 : 0;
}


UJ txt_swipe(S buf, I ptr, I lim)
{
	I i;
	for (i = 0; i + ptr < lim && !in_alphabet(buf[i+ptr]); i++);

	R (i + ptr >= lim - 1) ? NIL : i;
}

V txt_clean_buf(S buf, I len)
{
	for (I i = 0; i < len; i++)
		buf[i] = 0;
}

V txt_include_word(S str)
{
	O("'%s' included\n", str);
}

UJ txt_get_word(S dir, S source, I max_d, I max_s, I ptr)
{
	// LOG("txt_get_word");
	UJ j, i;

	for (j = 0; in_alphabet(dir[j]) && j < max_d; j++);

	if (j == max_d) {
		// T(FATAL, "WORD_BUF is overflowed (max. capacity %d)", max_d);
		R NIL;
	}

	for (i = 0; in_alphabet(source[ptr + i]) && i < max_s && j + i < max_d; i++) 
		dir[j + i] = source[ptr + i];
	dir[j + i] = 0;

	R (i == max_s || j + i == max_d) ? NIL : i;
	
} 


UJ txt_process_buf(S buf, TRIE tri, HT hsh, I len)
{
	I i = 0, var;
	if (in_alphabet(WORD_BUF[0])) {
		if (in_alphabet(buf[0])) {
			var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);
			P (var == NIL, NIL);
			i += var;
		}
		goto INCLUDE;
	}

	W(i < len - 1) {
		var = txt_swipe(buf, i, len);							//< how many chars swiped or NIL
		P(var == NIL, 0);										//< end of buf
		i += var;												//< move pointer

		var = txt_get_word(WORD_BUF, buf, SZ_WBUF, len, i);

		P(var == NIL, NIL);										//< problems with buf's capacity
		i += var;

		P(i >= len - 1, 0); 

		INCLUDE:
											//< it could be incompleted words
		txt_include_word(WORD_BUF);
		txt_clean_buf(WORD_BUF, SZ_WBUF);

	}
	R 0;
}

//< FILE* f into TRIE and HSH
UJ txt_process(FILE* f)
{
	LOG("txt_process");
	UJ len;

	LOOP:

	len = fread(TEXT_BUF, SZ(C), SZ_TBUF - 1, f);
	TEXT_BUF[len] = 0;
	X(txt_process_buf(TEXT_BUF, STOP_TRIE, TEXT_HSH, len + 1) == NIL, O("IT WAS NIL!\n"), NIL);

	if (!feof(f)) goto LOOP;

	R 0;
}


V bits(V* p)
{
	UJ i, res = (UJ)p, k;
	I szbits = SZ(V*)*8;

	O("%d!\n", szbits);

	for (i = szbits; i > 0; i--) {
		k = res >> (i-1);
		if (k & 1)
			O("1");
		else 
			O("0");
	}
	O("\n");
	fflush(stdout);
}


I main()
{
	// FILE *f = fopen("test.txt", "r+");

	// V* a = (V*)100;
	// O("test bits SZ(V*) == %d\n", SZ(V*));
	// R 0;                         
	// bits(a);

	// O("*V is %d\n*C is %d\n*I is %d\n*H is %d\n*E is %d\n*F is %d\n*UJ is %d\nJ J is %d\n\n", SZ(V*), SZ(S), SZ(I*), SZ(H*), SZ(E*), SZ(F*), SZ(UJ*), SZ(long long));
	O("SZ_HTptr == %d\nSZ_TRIptr == %d\nV* == %d\n\n", SZ(HT), SZ(TRIE), SZ(V*));

	// X(txt_process(f) == NIL, T(FATAL, "oh no"), 1);
	// txt_process(f);
	// fclose(f);
	R 0;


}