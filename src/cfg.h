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

//!	size of EXT_KEY (amount of extra keys for parsing, trie and hash)
#define EXT_KEY_AM 1

#if 1
//! live run
#define SZ_WBUF 1024
#define SZ_TBUF 131072
//!	path to file which will be processed into hash table
#define TXT_FILE "txt/_tr_.txt"
//!	path to file which will be processed into trie (stop words)
#define STP_FILE "txt/stop.txt"

#else
//! minimal test
#define SZ_WBUF 4
#define SZ_TBUF 8
#define TXT_FILE "txt/test_body.txt"
#define STP_FILE "txt/test_stop.txt"

#endif

#include "adt/hsh.h"
#include "adt/tri.h"