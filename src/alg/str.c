//< string algs with adt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>

#include "../___.h"
#include "../cfg.h"

#include "../adt/tri.h"
#include "../adt/hsh.h"

#include "../glb.h"


UJ str_tri_ins(V* struct_1, V* struct_2, S word, UJ len)
{
	LOG("str_tri_ins");

	X(struct_2 != NULL, 	T(WARN, "incorrect fn usage"), 		NIL);

	X(!tri_insert((TRIE)struct_1, word, len, (V*)1), 
							T(FATAL, "can't insert %s", word), 	NIL);
	// O("[str_tri_ins] '%s'\t (%d)\n", word, len);
	R 0;
}



UJ str_hsh_print(HT hsh)
{
	LOG("str_hsh_print");
	UJ res = 0;
	BKT b;
	DO(hsh_capacity(hsh), 
		b = hsh->buckets[i];
		W(b) {
			O("%lu\t'%s'\t%d\n", b, b->s, b->payload);
			res += !!hsh_get(hsh, b->s, b->n);
			b = b->next;
		});
	R res;
}

UJ str_hsh_proc(HT hsh, S str)
{
	LOG("str_hsh_proc");
	X(hsh_ins(hsh, str, scnt(str), (V*)1) == NULL, T(WARN, "null ptr or empty key"), NIL);
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

