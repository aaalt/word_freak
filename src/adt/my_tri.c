
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../___.h"
#include "../tri.h"

//<		TODO
//<	tri_init_node(parent node, C key)
UJ tri_char_idx(C c)
{
	R (IN(0, c, TRI_RANGE_OFFSET, TRI_RANGE - 2)) 	? c - TRI_RANGE_OFFSET
													: (c == EXT_KEY)
																	? TRI_RANGE - 1;
																	: NIL;
}

UJ tri_str_check(S key, UJ key_len)
{	
	for (i = 0; i < key_len; i++)
		if (tri_char_idx(key[i]) == NIL) {
			T(WARN,"unsupported characters in \e[1;32m(%.*s)\e[0m", n, key);
			R NIL;
		}
	R 1;
}


NODE tri_insert_node(TRIE t, NODE at, C key)
{
	// C idx 	= (IN(0, key - TRI_RANGE_OFFSET, TRI_RANGE - 2)) 
			// ? key - TRI_RANGE_OFFSET : TRI_RANGE - 1;
	C idx = tri_char_idx(key);
	LOG("tri_insert_node");

	NODE n = at->children[idx];
	if (!n) {										//<	if this node doesn't exist
		n = tri_init_node(t, key);					//<	we will create it
		P(n == NIL, NIL);
		n->parent = at;								//< connect at and n hierarchically
		at->children[idx] = n;
	}
	R n;
}

NODE tri_include(TRIE t, S key, UJ n, V* payload, C param)
{
	LOG("tri_include");
	I i;
	NODE cur;

	P(tri_str_check(key, len) == NIL, NIL);
	
	cur = t->root; 									//<	start from root

	for (i = 0; i < n; i++) 
		cur = tri_insert_node(t, cur, key[i]);		//< insert node with key[i] in trie t a
	
	if (!cur->payload || param)						//< if there is no payload at last node
		cur->payload = payload;						//<	it means that this key didn't exist
	cur->depth = n;									//<	depth == key len
	R cur;
}

/*****************************************************************/

TRIE tri_init()
{
	LOG("tri_init");
	TRIE t = (TRIE)malloc(SZ_TRIE);
	chk(t, NIL);									//<	if malloc is broken
	t->mem = SZ_TRIE;								//< set memory capacity
	t->cnt = 0;										//< amount of nodes is 0
	t->root = tri_init_node(t, 0);					//<	set root node
	R t;
}

NODE tri_insert(TRIE t, S key, UJ n, V* payload)
{
	R tri_include(t, key, n, payload, 0);
}

NODE tri_upsert(TRIE t, S key, UJ n, V* payload);
{
	R tri_include(t, key, n, payload, 1);
}

NODE tri_get(TRIE t, S key)
{
	LOG("tri_get");
	UJ len = scnt(key);
	NODE cur = t->root;
	X(!len, T(WARN, "key len 0"), NIL);
	P(tri_str_check(key, len))

	for (i = 0; i < len; i++) {
		if (!(cur->children[tri_char_idx(key[i])]))
			R NULL;
		cur = children[tri_char_idx(key[i])];
	}

	R cur;
}

C tri_is_leaf(NODE n)
{
	R !!n->payload;
}

V tri_dump_from(TRIE t, NODE n)
{
	tri_each_from(t, n, tri_dump_node, NULL);
}

V tri_dump(TRIE t)
{
	tri_dump_from(t, t->root);
}

V tri_each(TRIE t, TRIE_EACH fn, V* arg)
{
	tri_each_from(t, t->root, fn, arg);
}

