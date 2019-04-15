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

UJ str_hsh_print(HT hsh)
{
	LOG("str_hsh_print");
	UJ res, t;
	res = ord_ht(hsh, PRINT_PAR);
	R res;
}

//<	hsh_ins
UJ str_hsh_proc(HT hsh, STR str, UJ len)
{
	LOG("str_hsh_proc");
	X((UJ)hsh_ins(hsh, str, len, (V*)1) == NIL, T(WARN, "null ptr or empty key"), NIL);
	R 0;
}

/*UJ str_in_stop(HT ht, STR str, UJ len)
{
	R (hsh_get((HT)ht, str, len) == NULL) ? 0 : 1;
}*/

UJ str_hsh_ins(V* struct_1, V* struct_2, STR word, UJ len, sz el)
{
	LOG("str_hsh_ins");
	UJ l = len * el;

	X(!struct_1 || (!struct_1 && !struct_2), T(FATAL, "invalid pointers"), NIL);

	if (!struct_2 || hsh_get((HT)struct_2, word, l) == NULL)
		 R str_hsh_proc((HT)struct_1, word, l);

	R 0;
}



