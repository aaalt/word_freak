//< text processing

typedef UJ(*WORD_ADD)(V* struct_1, V* struct_2, S word, UJ len); //< word_add function interface

//< txt file processing 
UJ txt_process(FILE* f, V* struct_1, V* struct_2, WORD_ADD fn);
UJ SZFILE(FILE* ptr);

#ifndef SZ_TBUF
#define SZ_TBUF 5000

C TEXT_BUF_[SZ_TBUF];
S TEXT_BUF = &TEXT_BUF_[0];

#endif
#ifndef SZ_WBUF

#define SZ_WBUF 100
C WORD_BUF_[SZ_WBUF];
S WORD_BUF = &WORD_BUF_[0];
#endif