
//< TODO
//<	tri_in(TRIE tri, S word)
//<	hsh_proc(HT hsh, S word)
//< DEFINE TEXT_BUF WORD_BUF SZ_WBUF SZ_TBUF
//< ADT STOP_TRIE TEXT_HSH

C in_alphabet(C c)
{
	R ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'b') || c == '\'') ? 1 : 0;
}


UJ txt_swipe(S buf, I ptr, I lim)
{
	for (I i = 0; i + ptr < lim && !in_alphabet(buf[i+ptr]); i++);
	R (i + ptr <= lim) ? NIL : i + ptr;
}

V txt_clean_buf(S buf, I len)
{
	for (i = 0; i < len; i++)
		buf[i] = 0
}

//< returns last i+1 if success
//< else NIL
UJ txt_get_word(S dir, S source, I i, I max_d, I max_s)
{
	LOG("txt_get_word");
	UJ j, ptr;

	for (j = 0; in_alphabat(dir[j]) && j < max_d; j++);

	if (j == max_d) {
		T(FATAL, "WORD_BUF is overflowed (max. capacity %d)", max_d);
		R NIL;
	}

	for (ptr = 0; in_alphabet(source[i]) && i < max_s && j < max_d;) 
		dir[j++ + ptr++] = source[i++];

	dir[j + ptr] = 0;
	
	R i;
}

//<	return 0 if success
//< else NIL
UJ txt_process_buf(S buf, TRIE tri, HT hsh, I len)
{
	LOG("txt_process_buf");
	UJ i = 0, ptr;

	if (in_alphabet(WORD_BUF[0])) 									//< if previous word_buf contains a word
		if (!in_alphabet(buf[0])) 									//< and now we see that it's a completed
			goto INCLUDE;											//< word then just go to INCLUDE

	while (i < len - 1) {
		i = txt_swipe(buf, i, len - 1);								//< swipe non-alphabetical symbols
		if (i == NIL) R0;											//< if empty end of buffer
		ptr = i;
		i = txt_get_word(WORD_BUF, buf, i, SZ_WBUF - 1, len -1);	//<	get word form buf into WORD_BUF from buf[i]

		P(i == NIL, NIL);											//<	word is longer than WORD_BUF
		if (i >= len - 1 || ptr == i) R0;							//< if tail may be not empty or there are no
																	//<	words left in a buf
		INCLUDE:
		if (!tri_in(tri, WORD_BUF))									//< if it's not a stop word
			hsh_proc(hsh, WORD_BUF);								//< then decide what to do with hash

		txt_clean_buf(WORD_BUF, SZ_WBUF);							//< clean WORD_BUF
	}	
	R0;
}

//< FILE* f into TRIE and HSH
UJ txt_process(FILE* f)
{
	LOG("txt_process");
	UJ len;

	LOOP:
	
	len = fread(TEXT_BUF, SZ(C), SZ_TBUF - 1, f);
	TEXT_BUF[len] = 0;
	P(txt_process_buf(TEXT_BUF, STOP_TRIE, TEXT_HSH, len + 1) == NIL, NIL);

	if (!feof(f)) goto LOOP;
	R 0;
}