//< string algs with adt
#include <stdio.h>

#include "../___.h"
#include "../cfg.h"

#include "../adt/tri.h"
#include "../adt/hsh.h"


UJ str_hsh_proc(HT hsh, S str)
{

	/*
	hsh_get(...) returns 0 if not found, 1 if found
	or NIL in case of error 
	
	if (hsh_get(...) == 1) increase payload and R1; 
	if (hsh_get == NIL) panic 
	if (hsh_get == 0) inseart str anr R0

	*/

	/*
	SW(hsh_get(...)) {
	
		case 0:
			insert_str(...);
			R 0;
		case 1:	
			increase payload
			R 1;
		default:
			R NIL;
	}

	*/
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