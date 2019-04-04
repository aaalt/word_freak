//! \file bag.h \brief bag api

#pragma once

#define BAG_GROW_FACTOR 2

/**************************************************
	a bag is a collection of objects, where 
	you can keep adding objects into the bag,
	but you cannot remove them once added to
	the bag 
**************************************************/
typedef struct bag {
	sz size;			//<	mem capacity of ptr
	sz used;			//<	mem used [len of ptr]
	V* ptr;				//<	bag content
} __attribute__((packed)) pBAG;

#define SZ_BAG SZ(pBAG)
typedef pBAG* BAG;

//<	allocate mem, set fields
ext BAG  bag_init(sz init_sz);

//< add obj into the bag
ext V*   bag_add(BAG h, V*obj, sz obj_sz);

//<	return value is memory used to store the whole bag
ext sz   bag_mem(BAG h);

//<	truncate ptr
ext sz   bag_compact(BAG h);

//<	set used mem 0
ext V    bag_clear(BAG h);

//<	return pointer to ptr
ext V*   bag_data(BAG h);

//<	free mem
ext sz   bag_destroy(BAG h);

//:~