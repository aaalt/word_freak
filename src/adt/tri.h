//! \file tri.h \brief trie api

/**************************************************
	so trie consists of pTRIE (it's like main root 
	node which has not any parents node) and pNODEs 

	pNODE contains:	key (one ascii char)
					parent node pointer
					children nodes array
					void pointer payload
	pTRIE contains:	cnt conter for nodes
					mem is size of memort
					root is pointer for root node 
	nothing to add :)
**************************************************/
	

#pragma once

// #define TRI_RANGE 26
#define TRI_RANGE 27 						//< because I need '\''

//! 97 ascii lowercase, 65 ascii uppercase
#define TRI_RANGE_OFFSET 97

#define EXT_KEY '\''

typedef struct node {
	C key;
	V* payload;
	struct node* parent;
	I depth;
	struct node* children[TRI_RANGE];
} pNODE;

#define SZ_NODE SZ(pNODE)
typedef pNODE* NODE;

typedef struct trie {
	UJ cnt;
	sz mem;
	NODE root;
} pTRIE;

#define SZ_TRIE SZ(pTRIE)
typedef pTRIE* TRIE;


//< this is list of functions that I will write by myself

//< allocate memory for TRIE and set base TRIE structure
ext TRIE tri_init();

//< insert key into TRIE t; n is length of key 
ext NODE tri_insert(TRIE t, S key, UJ n, V*payload);

//< overwrite payload for key in TRIE t
ext NODE tri_upsert(TRIE t, S key, UJ n, V*payload);

//< find key in TRIE t; if key exists return leaf pointer, else NULL
ext NODE tri_get(TRIE t, S key);

//< is this node is leaf?
ext C    tri_is_leaf(NODE n);

//< ~ tri_each(t, trie_dump_node, NULL) ~ tri_each_node(t, t->root, trie_dunp_node, NULL, 0) 
//<	dump the whole trie (print it)
ext V    tri_dump(TRIE t);

//< dump childs from n
ext V    tri_dump_from(TRIE t, NODE n);

typedef V(*TRIE_EACH)(NODE n, V*arg, I depth); //< tri_each function interface

//< do fn with each node from root
ext V    tri_each(TRIE t, TRIE_EACH fn, V*arg);

//<	do fn with each node from n, (fn; recursive)
ext V    tri_each_from(TRIE t, NODE n, TRIE_EACH fn, V*arg);

//<	(recursive; fn)
ext V    tri_each_reverse(TRIE t, TRIE_EACH fn, V*arg);

//< free allocated memory
ext sz   tri_destroy(TRIE t);


//:~
