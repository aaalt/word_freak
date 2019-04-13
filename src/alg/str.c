//< string algs with adt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../___.h"
#include "../utl/clk.h"

#include "../adt/tri.h"
#include "../adt/hsh.h"
#include "ord.h"

#include "../glb.h"


UJ str_tri_ins(V* struct_1, V* struct_2, S word, UJ len)
{
	LOG("str_tri_ins");

	X(struct_2 != NULL, 	T(WARN, "incorrect fn usage"), 		NIL);

	X(!tri_insert((TRIE)struct_1, word, len, (V*)1), 
							T(FATAL, "can't insert %s", word), 	NIL);
	R 0;
}

UJ str_hsh_print(HT hsh)
{
	LOG("str_hsh_print");
	UJ res, t;
	res = ord_ht(hsh, PRINT_PAR);
	R res;
}

//<	hsh_ins
UJ str_hsh_proc(HT hsh, S str)
{
	LOG("str_hsh_proc");
	X((UJ)hsh_ins(hsh, str, scnt(str), (V*)1) == NIL, T(WARN, "null ptr or empty key"), NIL);
	R 0;
}

UJ str_tri_in(TRIE tri, S str)
{
	R (tri_get(tri, str)) ? 1 : 0;
}

UJ str_hsh_ins(V* struct_1, V* struct_2, S word, UJ len)
{
	LOG("str_hsh_ins");
	X(!struct_1 || !struct_2, T(WARN, "invalid pointers"), NIL);

	if (!str_tri_in((TRIE)struct_1, word))
		R str_hsh_proc((HT)struct_2, word);
	R 0;

}

