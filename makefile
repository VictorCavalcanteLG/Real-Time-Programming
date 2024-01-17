all: clean compile_dstring run_dstring compile_matrix run_matrix compile_integral run_integral clean

compile_dstring:
	gcc -std=c17 -o dstring_main dstring_main.c ./dstring/dstring.c

run_dstring:
	./dstring_main

compile_matrix:
	gcc -std=c17 -o matrix_main matrix_main.c ./matrix/matrix.c

run_matrix:
	./matrix_main

compile_integral:
	gcc -std=c17 -o integral_main integral_main.c ./integral/integral.c

run_integral:
	./integral_main

clean:
	rm -f dstring_main
	rm -f matrix_main
	rm -f integral_main
	