//<	qsort for hash table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wchar.h>
#include <wctype.h>

#include "../___.h"
#include "../utl/clk.h"
#include "ord.h"
#include "chr.h"

I cmpf_dec(const V* a, const V* b) {
	R ((PAIR)b)->cnt - ((PAIR)a)->cnt;
}

I cmpf_inc(const V* a, const V* b) {
	R ((PAIR)a)->cnt - ((PAIR)b)->cnt;
}

#if (TXT_DATA_TYPE == 1)
V print_hsh_bkt(BKT b)
{
	O("\t%lu\t\"%s\"     \t%u\n", (UJ)(b->payload), (STR)(b->s), (UI)(b->idx));
}
#else
V print_hsh_bkt(BKT b)
{
	// UJ len = sz_buf((STR)(b->s), SZ_WBUF);
	UJ len = (b->n)/SZ(CHAR);
	I i;
	CHAR c;
	STR s = (STR)(b->s);

	O("\t%lu\t\"", (UJ)(b->payload));
	DO(len,
		O("%lc", s[i]););
	O("\"     \t%u\n", (UI)(b->idx));
	// O("\t%lu\t\"%ls\"     \t%u\n", (UJ)(b->payload), (STR)(b->s), (UI)(b->idx));
	fflush(stdout);
}
#endif

UJ ord_ht(HT ht, C par)
{
	LOG("ord_ht");
	UJ i = 0, j = 0;
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
	if (PRINT_TOP != NIL && j >= PRINT_TOP) 
		j = PRINT_TOP;

	T(INFO, "\n\tq\tstr\t\tidx\n");

	DO(j, 													//<	print vals
		b = vals[i].bucket;
		print_hsh_bkt(b);
	)
#endif	
	R 0;
}

