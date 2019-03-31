all: 
	gcc -o bin/main src/bas.c src/adt/bag.c src/utl/trc.c src/alg/txt.c src/alg/str.c src/alg/set.c src/utl/clk.c src/adt/vec.c src/utl/rnd.c src/adt/tri.c src/adt/hsh.c

run:
	./bin/main