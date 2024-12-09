OUTPUT_FILE = output.txt

all: clean compile_dstring run_dstring compile_matrix run_matrix compile_integral run_integral clean

compile_dstring:
	gcc -std=c17 -o dstring_main ./cmd/dstring_main.c ./adt/dstring/dstring.c

run_dstring:
	./dstring_main

compile_matrix:
	gcc -std=c17 -o matrix_main ./cmd/matrix_main.c ./adt/matrix/matrix.c -lm

run_matrix:
	./matrix_main

compile_integral:
	gcc -std=c17 -o integral_main ./cmd/integral_main.c ./adt/integral/integral.c

run_integral:
	./integral_main

compile_simulate:
	gcc -std=c17 -o main main.c ./simulation/simulation_facade.c ./simulation/simulation.c ./adt/integral/integral.c ./adt/matrix/matrix.c ./adt/dstring/dstring.c -lm

run_simulate:
	./main

generate_outputs:
	./main > $(OUTPUT_FILE)

clean:
	rm -f ./cmd/dstring_main
	rm -f ./cmd/matrix_main
	rm -f ./cmd/integral_main
	