all: 
	gcc -Werror -O2 -O0 -g -o bin/main src/bas.c src/adt/bag.c src/utl/trc.c src/alg/chr.c src/alg/txt.c src/alg/str.c src/alg/set.c src/utl/clk.c src/utl/rnd.c src/adt/tri.c src/adt/hsh.c src/alg/ord.c

run:
	./bin/main

clean:
	rm bin/main

test:
	gcc -DRUN_TEST -Werror -O0 -g -o bin/main src/bas.c src/adt/bag.c src/utl/trc.c src/alg/chr.c src/alg/txt.c src/alg/str.c src/alg/set.c src/utl/clk.c src/utl/rnd.c src/adt/tri.c src/adt/hsh.c src/alg/ord.c && ./bin/main