
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../___.h"
#include "tri.h"

/*****************************************************************/
UJ tri_char_idx(C c)
{
	R (IN(0, c - TRI_RANGE_OFFSET, TRI_RANGE - 2)) 	? c - TRI_RANGE_OFFSET
													: (c == EXT_KEY)
																	? TRI_RANGE - 1
																	: NIL;
}

UJ tri_str_check(S key, UJ key_len)
{	
	LOG("tri_str_check");
	DO(key_len, {
				X(tri_char_idx(key[i]) == NIL, 
				T(WARN,"unsupported characters in \e[1;32m(%.*s)\e[0m", 
				key_len, key), NIL);});
	R 1;
}

C tri_is_leaf(NODE n)
{
	R !!n->payload;
}

V tri_destroy_node(NODE n, V*arg, I depth) 
{
	free(n);
}

V tri_dump_node(NODE n, V*arg, I depth) 
{
	DO(depth,O(" "));
	O("%c (%d, %d)\n", n->key, tri_is_leaf(n), n->depth);
}
/*****************************************************************/

Z NODE tri_init_node(TRIE t, C key)
{
	LOG("tri_init_node");
	NODE n = (NODE)calloc(SZ_NODE,1);chk(n,(NODE)NIL);
	n->key = key;
	t->mem += SZ_NODE;
	R n;
}

NODE tri_insert_node(TRIE t, NODE at, C key)
{
	C idx = tri_char_idx(key);
	LOG("tri_insert_node");

	NODE n = at->children[idx];
	if (!n) {										//<	if this node doesn't exist
		n = tri_init_node(t, key);					//<	we will create it
		P((UJ)n == NIL, (NODE)NIL);
		n->parent = at;								//< connect at and n hierarchically
		at->children[idx] = n;
	}
	R n;
}

NODE tri_include(TRIE t, S key, UJ n, V* payload, C param)
{

	LOG("tri_include");
	P(!n, (NODE)NIL)

	P(tri_str_check(key, n) == NIL, (NODE)NIL);
	NODE curr = t->root;
	DO(n,curr = tri_insert_node(t, curr, key[i]))
	if(param||!curr->payload){
		curr->payload = payload;
		t->cnt++;
	}
	curr->depth = n; //< depth <=> keylen
	R curr;
}

/*****************************************************************/

TRIE tri_init()
{
	LOG("tri_init");
	TRIE t = (TRIE)malloc(SZ_TRIE);
	chk(t, (TRIE)NIL);								//<	if malloc is broken
	t->mem = SZ_TRIE;								//< set memory capacity
	t->cnt = 0;										//< amount of nodes is 0
	t->root = tri_init_node(t, 0);					//<	set root node
	R t;
}

NODE tri_insert(TRIE t, S key, UJ n, V* payload)
{
	R tri_include(t, key, n, payload, 0);
}

NODE tri_upsert(TRIE t, S key, UJ n, V* payload)
{
	R tri_include(t, key, n, payload, 1);
}

NODE tri_get(TRIE t, S key)
{
	LOG("tri_get");
	sz l = scnt(key);
	P(!l, (NODE)NIL);
	NODE cur = t->root;
	C c, idx;

	DO(l,  {
		idx = tri_char_idx(key[i]);
		P(idx == NIL, (NODE)NIL);
		P(!cur, NULL);
		cur = cur->children[idx];})

	R cur;
}


ZV tri_each_node(TRIE t, NODE curr, TRIE_EACH fn, V*arg, I depth) {
	fn(curr, arg, depth);
	DO(TRI_RANGE,
		NODE c = curr->children[i];
		if(c)tri_each_node(t,c,fn,arg,depth+1)
	)}

ZV tri_each_node_reverse(TRIE t, NODE curr, TRIE_EACH fn, V*arg, I depth) {
	DO(TRI_RANGE,
		NODE c = curr->children[i];
		if(c)tri_each_node_reverse(t,c,fn,arg,depth+1)
	)
	fn(curr, arg, depth);
}

V tri_each_from(TRIE t, NODE n, TRIE_EACH fn, V*arg) {
	if(!n)
		R;
	tri_each_node(t, n, fn, arg, 0);
}

V tri_dump_from(TRIE t, NODE n)
{
	tri_each_from(t, n, tri_dump_node, NULL);
}

V tri_dump(TRIE t)
{
	// tri_dump_from(t, t->root);
	tri_each(t, tri_dump_node, NULL);
}

V tri_each(TRIE t, TRIE_EACH fn, V* arg)
{
	// tri_each_from(t, t->root, fn, arg);
	tri_each_node(t, t->root, fn, arg, 0);
}

V tri_each_reverse(TRIE t, TRIE_EACH fn, V*arg) {
	tri_each_node_reverse(t, t->root, fn, arg, 0);}


sz tri_destroy(TRIE t){
	tri_each_reverse(t, tri_destroy_node, NULL);
	sz released = t->mem;
	free(t);
	R released;
}









#ifdef RUN_TESTS_TRI

V tri_test_each(NODE n, V*arg, I depth){
	LOG("tri_test_each");
	T(TEST, "key=(%c) depth=%3d arg=%ld", n->key, depth, (J)arg);
}

I main() {

	// S keys[] = {"abbot", "abbey", "abacus", "abolition", "abolitions", "abortion", "abort", "zero"};
	S keys[] = {
	"is","at", "an", "a", "the", "or", "and", "for", "be", "not", "no", "in", "on"};


	LOG("tri_test");
	TRIE t=tri_init();
	X(!t,T(FATAL,"cannot init trie"),1);

	DO(13, X(!tri_insert(t, keys[i], scnt(keys[i]), (V*)1), T(FATAL, "can't insert %s", keys[i]), 1))
	T(TEST,"inserted %lu nodes", t->cnt);

	tri_dump(t);

	tri_each(t, tri_test_each, (V*)42);

	DO(13, X(!tri_get(t, keys[i]), T(FATAL, "can't find %s", keys[i]), 1))

	tri_destroy(t);

	R0;
}

#endif

//:~
