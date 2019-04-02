//< text processing


typedef UJ(*WORD_ADD)(V* struct_1, V* struct_2, S word, UJ len); //< word_add function interface

//< txt file processing 
UJ txt_process(FILE* f, V* struct_1, V* struct_2, WORD_ADD fn);
