#include <stdio.h>

#include "___.h"

I main()
{
	FILE* f = fopen("../txt/fixme.txt", "r+");
	G c, q = '@';
	I pos;
	W(!feof(f)) {
		fread(&c, SZ(G), 1, f);
		if (c > 126) {
			O("'%d' %d\n", c, ftell(f) - 1);
			fseek(f, -1, SEEK_SET);
			fwrite(&q, SZ(G), 1, f);
			// fseek()
		}
	}
	O("NONE\n");
	fclose(f);
	R 0;
}