//< string algs with adt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../___.h"
#include "../cfg.h"

#include "../adt/tri.h"
#include "../adt/hsh.h"

#include "../glb.h"



// S keys[] = {"abbot", "abbey", "abacus", "abolition", "abolitions", "abortion", "abort", "zero"};
/*
	LOG("tri_test");
	TRIE t=tri_init();
	X(!t,T(FATAL,"cannot init trie"),1);

	DO(8, X(!tri_insert(t, keys[i], scnt(keys[i]), (V*)1), T(FATAL, "can't insert %s", keys[i]), 1))
	T(TEST,"inserted %lu nodes", t->cnt);
*/


//< for ex
UJ str_make_stop_trie(TRIE tri)
{
	S keys[] = {"and", "or", "be", "in", "a", "an", "the", "not"};
	LOG("str_make_stop_trie");
	DO(8, X(!tri_insert(tri, keys[i], scnt(keys[i]), (V*)1), T(FATAL, "can't insert %s", keys[i]), NIL););
	T(TEST,"inserted %lu nodes", tri->cnt);
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
			O("bkt -->  %d\ns    -->  %s\namn  -->  %d\n\n", b, b->s, b->payload);
			res += !!hsh_get(hsh, b->s, b->n);
			b = b->next;
		});
	R res;
}

UJ str_hsh_proc(HT hsh, S str)
{
	LOG("str_hsh_proc");

	BKT B = hsh_get(hsh, str, scnt(str));

	if (!B) {
		X(hsh_ins(hsh, str, scnt(str), (V*)1) == NULL, T(WARN, "null ptr or empty key"), NIL);
		R 1;
	}
	else {
		B->payload++;
		R 0;
	}
}


UJ str_tri_in(TRIE tri, S str)
{
	R (tri_get(tri, str)) ? 1 : 0;
}


UJ str_word_inclusion(S str, HT hsh, TRIE tri)
{

	if (!str_tri_in(tri, str))
		R str_hsh_proc(hsh, str);		//< 0 if already exists; 1 if included
	R 2;

}