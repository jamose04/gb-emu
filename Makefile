all: jameboy

test: jameboy
	cc -ggdb -Ijameboy tests/alu_test.c tests/test_util.c jameboy/*.c -o bin/alu_test
	cc -ggdb -Ijameboy tests/op_test.c tests/test_util.c jameboy/*.c -o bin/op_test
	cc -ggdb -Ijameboy tests/mem_test.c tests/test_util.c jameboy/*.c -o bin/mem_test
	bin/alu_test
	bin/op_test
	bin/mem_test

jameboy: table
	cc -ggdb -Ijameboy jameboy/driver/main.c jameboy/*.c -o bin/jameboy

table:
	cc jameboy/optable/tablegen.c -o bin/tablegen
	bin/tablegen jameboy/optable/optable.tab instr_lookup > jameboy/tab.h
	bin/tablegen jameboy/optable/prefoptable.tab cb_instr_lookup > jameboy/cbtab.h

clean:
	rm -r bin
	rm jameboy/tab.h
	rm jameboy/cbtab.h
