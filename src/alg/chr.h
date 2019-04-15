
ext TXT_TYPE cs(TXT_TYPE c);

ext UJ char_is_ext(TXT_TYPE c);

// ext C in_alphabet(C c);
ext C in_alphabet(TXT_TYPE c);

ext C valid_key(TXT_TYPE c);

ext UJ swipe_buf(TXT_T buf, I ptr, I lim, C par);

ext V clean_buf(TXT_T buf, I len);

ext UJ sz_buf(TXT_T buf, UJ max);

ext TXT_T convert_str(TXT_T key, I len);

ext UJ mfread(FILE* f, TXT_T buf, UJ max);


/*
TXT_TYPE cs(TXT_TYPE c);

UJ char_is_ext(TXT_TYPE c);

TXT_t convert_str(TXT_T key, I len);

C in_alphabet(TXT_TYPE c);

C valid_key(TXT_TYPE c);

//< if par == 0 --> swipe non-alphabetical
//<	if par == 1 --> swipe alphabetical
UJ swipe_buf(TXT_T buf, I ptr, I lim, C par);

V clean_buf(TXT_T buf, I len);

UJ sz_buf(TXT_T buf, UJ max);*/