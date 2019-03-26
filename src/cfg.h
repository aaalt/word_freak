
//< DEFINE TEXT_BUF WORD_BUF SZ_WBUF SZ_TBUF
//< ADT STOP_TRIE TEXT_HSH

#pragma once

#include "___.h"



#define SZ_WBUF 100
#define SZ_TBUF 10000




ext TRIE STOP_TRIE;
ext HT TEXT_HSH;
ext C TEXT_BUF[SZ_TBUF];
ext C WORD_BUF[SZ_WBUF];
