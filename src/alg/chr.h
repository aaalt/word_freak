
ext CHAR cs(CHAR c);

ext UJ char_is_ext(CHAR c);

ext C in_alphabet(CHAR c);

ext C valid_key(CHAR c);

ext UJ swipe_buf(STR buf, I ptr, I lim, C par);

ext V clean_buf(STR buf, I len);

ext UJ sz_buf(STR buf, UJ max);

ext STR convert_str(STR key, I len, I max,C par);

ext UJ mfread(FILE* f, STR buf, UJ max);
