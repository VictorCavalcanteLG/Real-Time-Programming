OUTPUT_FILE = output.txt

all: clean compile_dstring run_dstring compile_matrix run_matrix compile_integral run_integral clean

compile_dstring:
	gcc -std=c17 -o dstring_main dstring_main.c ./dstring/dstring.c

run_dstring:
	./dstring_main

compile_matrix:
	gcc -std=c17 -o matrix_main matrix_main.c ./matrix/matrix.c -lm

run_matrix:
	./matrix_main

compile_integral:
	gcc -std=c17 -o integral_main integral_main.c ./integral/integral.c

run_integral:
	./integral_main

compile_simulate:
	gcc -o main main.c ./simulate/simulate.c ./integral/integral.c ./matrix/matrix.c ./dstring/dstring.c -lm

run_simulate:
	./main

generate_outputs:
	./main > $(OUTPUT_FILE)

clean:
	rm -f dstring_main
	rm -f matrix_main
	rm -f integral_main
	