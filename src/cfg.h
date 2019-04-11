//! \file cfg.c \brief parsing settings

#pragma once

#include <stddef.h>

#define TEST_MODE

//! L_FATAL|L_WARN|L_INFO|L_TEST|L_DEBUG|L_TRACE
#ifdef TEST_MODE
#define LOGLEVEL L_TEST
#else
#define LOGLEVEL L_INFO	//< set to WARN in prod builds
#endif

#define PRINT_PAR 1			//<	set 0 if increasing sort is needed
#define PRINT_TOP 10		//<	set NIL if you want to print all	

//<	size of WORD_BUF
// #define SZ_WBUF 120
#define SZ_WBUF 128			//<	larger SZ_WBUF 	-->	SLOWER program	(*n^2?)
//<	size of TEXT_BUF
#define SZ_TBUF 32768*2*2		//<	larger SZ_TBUF 	--> FASTER program (*1/logn?)
// #define SZ_TBUF 60

//<	size of EXT_KEY (amount of extra keys for parsing, trie and hash)
#define EXT_KEY_AM 1

//<	path to file which will be processed into hash table
#define TXT_FILE "txt/_tr_.txt"
// #define TXT_FILE "txt/test.txt"

// #define TXT_FILE "txt/overflow.txt"
//<	path to file which will be processed into trie (stop words)
// #define STP_FILE "txt/stop.txt"
#define STP_FILE "txt/plain.txt"

#include "adt/hsh.h"
#include "adt/tri.h"