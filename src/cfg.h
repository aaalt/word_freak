
//< DEFINE TEXT_BUF WORD_BUF SZ_WBUF SZ_TBUF
//< ADT STOP_TRIE TEXT_HSH

#pragma once


#include <stddef.h>
#include "___.h"
#include "adt/hsh.h"
#include "adt/tri.h"

#define TEST_MODE

//! L_FATAL|L_WARN|L_INFO|L_TEST|L_DEBUG|L_TRACE
#ifdef TEST_MODE
#define LOGLEVEL L_TEST
#else
#define LOGLEVEL L_INFO	//< set to WARN in prod builds
#endif





#define SZ_WBUF 100
#define SZ_TBUF 10000

#define TXT_FILE "test.txt"


/*ext TRIE STOP_TRIE;
ext HT TEXT_HSH;

//<	addresses of arrays!
ext S TEXT_BUF;
ext S WORD_BUF;
*/