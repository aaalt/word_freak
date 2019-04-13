//<	qsort for hash table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../___.h"
#include "../utl/clk.h"
#include "ord.h"

I cmpf_dec(const V* a, const V* b) {
	// const PAIR x = (PAIR)a;
	// const PAIR y = (PAIR)b;

	// I cmp = x->cnt - y->cnt;
	R ((PAIR)b)->cnt - ((PAIR)a)->cnt;
	// R 	 (cmp < 0)	? 	 1	
		// :(cmp > 0)	? 	-1	: 0;
	// R -cmp;
}

I cmpf_inc(const V* a, const V* b) {
	// const PAIR x = (PAIR)a;
	// const PAIR y = (PAIR)b;

	R ((PAIR)a)->cnt - ((PAIR)b)->cnt;
}

UJ ord_ht(HT ht, C par)
{
	LOG("ord_ht");
	UJ i, j = 0;
	UJ t;
	pPAIR vals[ht->cnt];
	BKT b;

	clk_start();
	DO(hsh_capacity(ht), 									//<	create vals 
		b = ht->buckets[i];
		W(b) {
			vals[j].cnt = (UJ)(b->payload);
			vals[j++].bucket = b;
			b = b->next;
		}
	)
	t = clk_stop();
	T(INFO, "\t[~]\tht->buckets into vals (%lu buckets)\t\t\t%lums", ht->cnt, t);

	clk_start();
	qsort(vals, j, SZ_PAIR, (par) ? cmpf_dec : cmpf_inc);	//<	sort vals
	t = clk_stop();
	T(INFO, "\t[~]\tqsort %d buckets\t\t\t\t\t%lums", j, t);

#ifndef RUN_TEST
	if (PRINT_TOP != NIL && j >= PRINT_TOP) {
		j = PRINT_TOP;
	}
	t = clk_start();
	DO(j, 													//<	print vals
		b = vals[i].bucket;
		O("\t%lu\t\"%s\"\n", (UJ)(b->payload), (S)(b->s));
	)
	t = clk_stop();
	T(INFO, "\t[~]\tprint %d strings with payload\t\t\t\t%lums", j, t);

#endif	
	R 0;
}

