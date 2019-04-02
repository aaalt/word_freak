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

/*
//< for ex
UJ str_make_stop_trie(TRIE tri)
{
	S keys[] = {"and", "or", "be", "in", "a", "an", "the", "not"};
	LOG("str_make_stop_trie");
	DO(8, X(!tri_insert(tri, keys[i], scnt(keys[i]), (V*)1), T(FATAL, "can't insert %s", keys[i]), NIL););
	T(TEST,"inserted %lu nodes", tri->cnt);
	R 0;
}
*/
UJ str_tri_ins(V* struct_1, V* struct_2, S word, UJ len)
{
	LOG("str_tri_ins");
	// if (struct_2 != NUL) {
		// T(WARN, "incorrect fn usage");
		// R;
	// }
	X(struct_2 != NULL, T(WARN, "incorrect fn usage"), NIL);


	X(!tri_insert((TRIE)struct_1, word, len, (V*)1), T(FATAL, "can't insert %s", word), NIL);
	// T(TEST,"inserted %lu nodes", t->cnt);

	// if (tri_insert((TRIE)struct_1, word, len, (V*)1) == NIL)
	 	// T(FATAL, "can't insert %s", word);
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
			// O("bkt -->  %d\ns    -->  %s\namn  -->  %d\n\n", b, b->s, (UJ)b->payload);
			O("s     -->  %s\namn   -->  %d\n\n", b->s, b->payload);
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

// UJ str_word_inclusion(S str, HT hsh, TRIE tri)
// {
	// if (!str_tri_in(tri, str))
		// R str_hsh_proc(hsh, str);		//< 0 if already exists; 1 if included
	// R 0;
// }

UJ str_hsh_ins(V* struct_1, V* struct_2, S word, UJ len)
{
	// X(!struct_1 || !struct_2, T(WARN, "invalid pointers"), NIL);
	if(!struct_1 || !struct_2) {
		O("INVALID POINTERS\n");
		R NIL;
	}

	if (!str_tri_in((TRIE)struct_1, word))
		R str_hsh_proc((HT)struct_2, word);
	R 0;

}

