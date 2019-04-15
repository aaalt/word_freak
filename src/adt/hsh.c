//!\file hsh.c \brief simple hash table with separate chaining

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <wchar.h>
#include <wctype.h>

#include "../___.h"
#include "../utl/rnd.h"
#include "../utl/clk.h"
#include "../alg/chr.h"
#include "hsh.h"

#define HSH_DEFAULT_FN hsh_djb

//<		MAIN ONES
//<	[x]	hsh_init
//<	[x]	hsh_get
//<	[x]	hsh_ins
//<	[x]	hsh_destroy

//<		TODO
//<	[x]	BKT hsh_get_bkt	[for hsh_get]
//<	[x]	hsh_idx

//<		EXTRA
//<	

//! djbhash \see http://www.burtleburtle.net/bob/hash/doobs.html
//! One of the best known hash functions for strings, due to djb.
//! Computes very fast and distributes very well, yields uint32.
Z inline HTYPE hsh_djb(S a,UI n)
{
	HTYPE h = 5381;
	DO(n, 
		h = 33 * (h^a[i]));
	R h;
}

//! copy with seek \param d dest \param s source \param n len
ZS dsn(V* d, V* s, UJ n)
{
	R memcpy(d, s, n) + n;
}

HTYPE hsh_upper_idx(HTYPE hash, UJ level)
{
	R hash & ((level << 1) - 1);
}

Z inline V hsh_idx(HT ht, V* s, UJ n, HTYPE* h, HTYPE* idx)
{
	LOG("hsh_idx");
	HTYPE hash = ht->fn(s, n);					//<	calculate hash
	HTYPE i = hash & (ht->level - 1);			//<	map hash onto the first half of table
	if (i < ht->split) 
		i = hsh_upper_idx(hash, ht->level);

	*h 	 =	hash;
	*idx =	i;
}

BKT hsh_inc_payload(HT ht, BKT B, HTYPE idx, V* payload)
{
	UJ p = (UJ)(payload) + 1;
	B->payload = (V*)p;
	R B;
}

V hsh_print(BKT b) 
{
	LOG("hsh_print");

	T(TEST, "bkt     -> %d",   b);
	T(TEST, "s       -> %s",   b->s);
	T(TEST, "h       -> %d",   b->h);
	T(TEST, "n       -> %d",   b->n);
	T(TEST, "payload -> %lu",  b->packed);	
}

sz hsh_mem(HT ht) 
{
	sz ht_size = SZ(SZ_HT) + SZ(BKT) * hsh_capacity(ht); //< own size
	R ht->mem + ht_size;
}

Z HTYPE hsh_bcnt(HT ht) 
{
	HTYPE r = 0;
	DO(hsh_capacity(ht), r+=!!ht->buckets[i])
	R r;
}

E hsh_factor(HT ht) 
{
	R (E)ht->cnt / hsh_capacity(ht);
}

E hsh_bavg(HT ht) 
{
	R (E)ht->cnt / hsh_bcnt(ht);
}

V hsh_info(HT ht) 
{
	LOG("hsh_info");

	T(TEST, "cap=%8d, cnt=%8d, bcnt=%8d, bavg=%4.2f, lf=%4.2f, spl=%8d, mem=%8lu",
		hsh_capacity(ht), ht->cnt, hsh_bcnt(ht), hsh_bavg(ht), hsh_factor(ht), ht->split, hsh_mem(ht));
}

///////////////////////////////////////////////////////////////

HT hsh_init_custom(I level, H split_rounds, HSH_FN fn)
{
	LOG("hsh_init");

	X(level < 2, 				T(WARN, "level can't be less than 2"), 		(HT)NIL);
	X((level & (level - 1)),	T(WARN, "level must be a power of two"),	(HT)NIL);
	X(split_rounds < 1, 		T(WARN, "split_rounds can't be less than 1"),(HT)NIL);

	HT ht = (HT)malloc(SZ_HT);	chk(ht, (HT)NIL);				//<	allocate mem for ht header
	ht->level 	= level;										//<	set basic ht fields
	ht->rounds 	= split_rounds;
	ht->heap	= NULL;
	ht->fn 		= fn;
	ht->split 	= 0;
	ht->cnt	 	= 0;
	ht->mem 	= 0;

	HTYPE init_size = hsh_capacity(ht);
	ht->buckets = (BKT*)calloc(init_size, SZ_BKT);	chk(ht->buckets, (HT)NIL);

	T(DEBUG,"calloc: level=%d capacity=%d mem=%d",
		ht->level, hsh_capacity(ht), hsh_capacity(ht) * SZ(BKT));
	R ht;
}

BKT hsh_get_bkt(HT ht, V* k, sz n)
{
	LOG("hsh_get");
	HTYPE hash, idx;

	P(!k || !n, (BKT)NIL);						//<	null ptr or empty key
	hsh_idx(ht, k, n, &hash, &idx);			//<	get hash and idx

	BKT b = ht->buckets[idx];

	W(b) {
		if (b->n == n) {
			DO(n, 
				if((G)(b)->s[i] != ((G*)k)[i])
					goto NEXT;)
			R b;
		}
		NEXT:
		b = b->next;
	}
	R NULL;
}
///////////////////////////////////////////////////////////////

V* hsh_get(HT ht, V* k, sz n)
{
	UJ len = n * SZ(CHAR);
	BKT b = hsh_get_bkt(ht, k, len);
	P(!b, NULL);
	P((UJ)b == NIL, (V*)NIL);
	R b->s;
}

HT hsh_init(I level, H split_rounds)
{
	R hsh_init_custom(level, split_rounds, HSH_DEFAULT_FN);
}

sz hsh_destroy(HT ht) 
{
	LOG("hsh_destroy");
	BKT cur, next;
	HTYPE c = 0;
	sz released = ht->mem;

	DO(hsh_capacity(ht), 
			cur = ht->buckets[i];
			W(cur) {
				next = cur->next;
				if (!cur->packed)
					free(cur);
				cur = next;
				c++;

			});
	if (ht->heap)
		free(ht->heap);
	free(ht->buckets);
	free(ht);
	T(DEBUG, "released %lu values, hash table destroyed", c);
	R released;
}

BKT hsh_ins(HT ht, V* k, sz n, V* payload)
{
	LOG("hsh_ins");

	P(!k || !n, (BKT)NIL);										//<	null ptr or empty key

	// O("HSH_INS '%ls'\t%lu\t last %u, %u %u\n", (UTF)k, n, ((UTF)k)[n-2], ((UTF)k)[n-1], ((UTF)k)[n]);

	UJ str_len = n * SZ(CHAR);
	UJ rec_len = SZ_BKT + str_len + 1;
	// BKT B = hsh_get_bkt(ht, convert_str(k, n), str_len);
	BKT B = hsh_get_bkt(ht, k, str_len);
	HTYPE hash, idx;

	BKT* bp;

	hsh_idx(ht, k, str_len, &hash, &idx);

	P(B, hsh_inc_payload(ht, B, idx, B->payload));				//< if B with k already exists, just increase payload

	T(TRACE, "cnt=%lu bucket=%d, split=%d, level=%d",
		ht->cnt, idx, ht->split, ht->level);

	//<	insert the value
	B = malloc(rec_len);		chk(B, (BKT)NIL);					//<	init new bucket with ext len
	ht->mem  	 +=	rec_len;									//<	increment odometers
	ht->cnt++;
	B->h 	  		  =	hash;									//<	set hash
	// B->n 			  =	n;										//<	val length
	B->n 			  =	str_len;										//<	val length

	B->packed 		  =	0;										//<	not in heap
	B->next 		  =	ht->buckets[idx];						//<	link existing list if any
	B->payload 		  =	payload;								//<	set payload
	// *dsn(B->s, k, n)  =	0;
	((STR)(k))[n] = 0;
	*dsn(B->s, k, str_len)  =	0;

	B->idx 			  =	idx;
	ht->buckets[idx]  =	B;

	//<	if new bucket has a tail
	if (B->next) {
		// T(INFO, "\tht clash for idx %u \t hash %lu \t (str. \"%ls\")", idx,hash,  B->s);
		DO(ht->rounds, 
			BKT* bp = &ht->buckets[ht->split];					//<	work with bucket starting from idx split
			HTYPE new_idx = ht->split + ht->level;				//<	nex available upper index

			W(*bp) {												//<	while list continues
				//! if item's upper hash lands on target index, move it there: 
				if (hsh_upper_idx((*bp)->h, ht->level) == new_idx) {
					BKT moved = *bp;								//<	item to move
					(*bp) = (*bp)->next;								//<	shift head of the list 
					moved->next = ht->buckets[new_idx];			//<	link existing items if any
					ht->buckets[new_idx] = moved;			
				}
				else 
					bp = &(*bp)->next;
			}

			if (++ht->split == ht->level) {
				ht->level <<= 1;
				ht-> split = 0;
				sz cap = hsh_capacity(ht);
				ht->buckets = (BKT*)realloc((G*)ht->buckets, cap * SZ(BKT));
				chk(ht->buckets, (BKT)NIL);
				DO(ht->level, 
						ht->buckets[ht->level + i] = NULL;);
			}
		)
	}
	R B;
}

///////////////////////////////////////////////////////////////

V hsh_dump(HT ht)
{
	LOG("hsh_dump");

	BKT b;

	DO(hsh_capacity(ht), 
		b = ht->buckets[i];
		if (!b)
			continue;
		TSTART();
		T(TEST, "%5lu ", i);
		W(b){
			T(TEST,"(%s)=%d,%c %s ", b->s, b->n, "NY"[b->packed], STR_ARROW_RIGHT);
			b = b->next;
		}
		T(TEST, STR_EMPTY_SET);
		TEND();)
}


/*	??????????????????
hsh_pack(HT ht) {
	LOG("hsh_pack");
	BKT prev, curr, next;
	V*tmpheap = malloc(ht->mem); chk(tmpheap, 0);
	UJ hptr = 0; //< heap pointer
	V*mc;
	sz buckets_packed = 0;
	DO(hsh_capacity(ht),
		prev = NULL;
		curr = ht->buckets[i];
		W(curr){	
			//hsh_print(curr);
			sz bsize = SZ_BKT + curr->n + 1;
			BKT old_addr = curr;
			BKT new_addr = tmpheap + hptr;
			mc = mcpy(new_addr, curr, bsize);
			if(!prev)
				ht->buckets[i] = new_addr;
			else
				prev->next = new_addr;
			prev = new_addr;
			next = curr->next;
			if (!new_addr->packed) {
				free(old_addr);
				new_addr->packed = 1;
			}
			curr = next;
			hptr += bsize;
			buckets_packed++;
			//T(TEST, "packed bkt=%lu (size=%lu)", i, bsize);
		}
	)
	if(ht->heap)free(ht->heap);
	ht->heap = tmpheap;
	T(DEBUG, "packing complete, bkts=%lu", buckets_packed);
	R1;
}

*/

