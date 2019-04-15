//< string algs with adt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "../___.h"
#include "../utl/clk.h"

#include "../adt/tri.h"
#include "../adt/hsh.h"
#include "ord.h"

#include "../glb.h"

UJ str_tri_ins(V* struct_1, V* struct_2, TXT_T word, UJ len)
{
	LOG("str_tri_ins");
	
	X(struct_2 != NULL, 	T(WARN, "incorrect fn usage"), 		NIL);
	X(!tri_insert((TRIE)struct_1, (S)word, len, (V*)1), 
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
UJ str_hsh_proc(HT hsh, TXT_T str, UJ len)
{
	LOG("str_hsh_proc");
	X((UJ)hsh_ins(hsh, str, len, (V*)1) == NIL, T(WARN, "null ptr or empty key"), NIL);
	R 0;
}

UJ str_tri_in(HT tri, TXT_T str, UJ max)
{
	R (hsh_get((HT)tri, str, max) != NULL) ? 1 : 0; 
}

UJ str_hsh_ins_(V* struct_1, V* struct_2, TXT_T word, UJ len)
{
	LOG("str_hsh_ins_");
	X(!struct_1 || struct_2, T(WARN, "invalid pointer"), NIL);

	R str_hsh_proc((HT)struct_1, word, len);
}

UJ str_hsh_ins(V* struct_1, V* struct_2, TXT_T word, UJ len)
{
	LOG("str_hsh_ins");
	X(!struct_1 || !struct_2, T(WARN, "invalid pointers"), NIL);
	// convert_str(word, len);
	if (!str_tri_in((HT)struct_1, word, len))
		R str_hsh_proc((HT)struct_2, word, len);
	R 0;

}



