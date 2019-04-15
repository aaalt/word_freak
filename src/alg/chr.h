
ext CHAR cs(CHAR c);

ext UJ char_is_ext(CHAR c);

// ext C in_alphabet(C c);
ext C in_alphabet(CHAR c);

ext C valid_key(CHAR c);

ext UJ swipe_buf(STR buf, I ptr, I lim, C par);

ext V clean_buf(STR buf, I len);

ext UJ sz_buf(STR buf, UJ max);

ext STR convert_str(STR key, I len);

ext UJ mfread(FILE* f, STR buf, UJ max);


/*
CHAR cs(CHAR c);

UJ char_is_ext(CHAR c);

TXT_t convert_str(STR key, I len);

C in_alphabet(CHAR c);

C valid_key(CHAR c);

//< if par == 0 --> swipe non-alphabetical
//<	if par == 1 --> swipe alphabetical
UJ swipe_buf(STR buf, I ptr, I lim, C par);

V clean_buf(STR buf, I len);

UJ sz_buf(STR buf, UJ max);*/