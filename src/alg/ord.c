//<	qsort for hash table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../___.h"
#include "ord.h"

I cmpf (const V* a, const V* b) {
	const PAIR x = (PAIR)a;
	const PAIR y = (PAIR)b;

	I cmp = x->cnt - y->cnt;

	R 	 (cmp < 0)	? 	 1	
		:(cmp > 0)	? 	-1	: 0;
}

UJ ord_ht(HT ht)
{
	I i, j = 0;
	pPAIR vals[ht->cnt];
	BKT b;

	DO(hsh_capacity(ht), 						//<	create vals 
		b = ht->buckets[i];
		W(b) {
			vals[j].cnt = (UJ)(b->payload);
			vals[j++].bucket = b;
			b = b->next;
		}
	)
	
	qsort(vals, j, SZ_PAIR, cmpf);				//<	sort vals

	DO(j , 										//<	print vals
		b = vals[i].bucket;
		O("%lu\t\"%s\"\n", (UJ)(b->payload), (S)(b->s));
	)

	R 0;
}

