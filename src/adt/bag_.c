//! \file bag.c \brief bag

#include <stdlib.h>
#include <string.h>
#include "../___.h"
#include "bag_.h"

ZV zero(G*a,sz len){DO(len, a[i]=0)}

/*BAG bag_init(sz init_sz) {
	LOG("bag_init");
	X(init_sz<1,T(WARN, "initial size must be positive"), NULL);
	BAG h = (BAG)calloc(SZ_BAG,1);chk(h,NULL);
	h->ptr = calloc(1,init_sz);chk(h->ptr,NULL);
	h->size = init_sz;
	h->used = 0;
	R h;}
*/

BAG bag_init(sz init_sz)
{
	LOG("bag_init");
	X(init_sz < 1, T(WARN, "initial size must be positive"), 	(BAG)NIL);

	BAG h = (BAG)calloc(SZ_BAG, 1);		chk(h, 		(BAG)NIL);				//<	allocate mem for bag struct
	h->ptr = calloc(1, init_sz);		chk(h->ptr,	(BAG)NIL);				//< allocate mem for bag content
	h->size = init_sz;
	h->used = 0;
	R h;
}


/*V* bag_add(BAG h, V*obj, sz obj_sz) {
	LOG("bag_add");
	// sz offset = 0;

	if((obj_sz+h->used) > h->size){
		// V*old_ptr = h->ptr;
		REALLOC:
		h->ptr = realloc(h->ptr, BAG_GROW_FACTOR * h->size);chk(h->ptr,NULL);
		zero(h->ptr+(h->size), h->size); //< zero out
		h->size *= BAG_GROW_FACTOR;
		if((obj_sz+h->used) > h->size)goto REALLOC;
		// offset = old_ptr-(h->ptr);
		//T(TEST, "realloc bag %lu, diff=%lu", h->size, offset);
	}
	V*obj_addr=h->ptr+(h->used);
	//T(TEST, "mcopy %lu bytes from %p to %p", obj_sz, (V*)obj, obj_addr);
	mcpy(obj_addr, (V*)obj, obj_sz);
	h->used += obj_sz;
	// h->offset = offset;
	R obj_addr;}
*/
//<	works fine!
V* bag_add(BAG h, V* obj, sz obj_sz)
{
	LOG("bag_add");

	if (h->size < obj_sz + h->used) {										//<	if there is not enough mem for new obj
		REALLOC:
		h->ptr = realloc(h->ptr, BAG_GROW_FACTOR * h->size);		chk(h->ptr, (V*)NIL);
		zero(h->ptr + h->size, (BAG_GROW_FACTOR - 1) * h->size);			//<	zero all new unused mem

		h->size *= BAG_GROW_FACTOR;
		if (h->size < obj_sz + h->used)										//<	if there's still not enough mem 
			goto REALLOC;													//< do realloc again
	}
		V* obj_addr = h->ptr + h->used;											//< addr where we will stick the obj

	mcpy(obj_addr, (V*)obj, obj_sz);										//< copy obj into ptr at obj_addr
	h->used += obj_sz;
	R obj_addr;
}

sz bag_compact(BAG h) {
	LOG("bag_compact");
	// sz offset = 0;
	V*old_ptr = h->ptr;
	sz old_sz = h->size;
	h->ptr = realloc(h->ptr, h->used);chk(h->ptr,0);
	h->size = h->used;
	// h->offset = old_ptr-(h->ptr);

	R old_sz - h->size;} // report savings

sz bag_mem(BAG h) {
	R SZ_BAG+h->size;}

E bag_lfactor(BAG h) {
	R (E)h->used/h->size;}

V bag_clear(BAG h) {
	h->used = 0;}

V* bag_data(BAG h) {
	R h->ptr;}

sz bag_destroy(BAG h) {
	sz released = h->size;
	free(h->ptr);
	free(h);
	R SZ_BAG+released;}

#ifdef RUN_TESTS_BAG

I main() {
	LOG("bag_test");

	S keys[] = {"abbot", "abbey", "abacus", "abolition", "abolitions", "abortion", "abort", "zero", "ninth", "abbot", "abbey", "abacus", "abolition", "abolitions", "abortion", "abort", "zero", "ninth"};
	S raw = "abbotabbeyabacusabolitionabolitionsabortionabortzeroninthabbotabbeyabacusabolitionabolitionsabortionabortzeroninth";
	sz sz_raw = scnt(raw);

	BAG h=bag_init(1);

	ASSERT(h != NULL, "should be able to create a bag")

	S obj; sz added = 0;
	DO(18,
		sz l = scnt(keys[i]);
		added += l;
		obj = (S)bag_add(h, keys[i], l))

	ASSERT(added == h->used, "bag_add() should work as expected (#1)")
	ASSERT(128 == h->size, "bag should grow as expected")
	ASSERT(h->used ==sz_raw, "bag should correctly report content size")
	ASSERT(!mcmp(h->ptr, raw, sz_raw), "bag content should match prediction")

	sz bytes = bag_mem(h);
	ASSERT(bytes == (SZ_BAG+128), "bag_mem() should report true size")
	ASSERT(IN(0.7, bag_lfactor(h), 0.9), "bag_lfactor() should report sane values")

	sz expected = 128 - sz_raw;
	ASSERT(bag_compact(h) == expected, "bag_compact() should result in predicted savings")

	ASSERT((sz_raw+SZ_BAG) == bag_destroy(h), "bag_destroy() should return bytes released")

	R0;
}

#endif

//:~
