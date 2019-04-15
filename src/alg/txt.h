//< text processing

typedef UJ(*WORD_ADD)(V* struct_1, V* struct_2, STR word, UJ len); //< word_add function interface

//< txt file processing 
UJ txt_process(FILE* f, V* struct_1, V* struct_2, WORD_ADD fn);
UJ SZFILE(FILE* ptr);

#ifndef SZ_TBUF

#define SZ_TBUF 131072
CHAR TEXT_BUF_[SZ_TBUF];
STR TEXT_BUF = &TEXT_BUF_[0];
#endif
#ifndef SZ_WBUF

#define SZ_WBUF 1024
CHAR WORD_BUF_[SZ_WBUF];
STR WORD_BUF = &WORD_BUF_[0];
#endif