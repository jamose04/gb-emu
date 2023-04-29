all: table
	cc -ggdb jameboy/*.c -o bin/jameboy

table:
	cc jameboy/optable/tablegen.c -o bin/tablegen
	bin/tablegen jameboy/optable/optable.tab instr_lookup > jameboy/tab.h
	bin/tablegen jameboy/optable/prefoptable.tab cb_instr_lookup > jameboy/cbtab.h

clean:
	rm -r bin
	rm jameboy/tab.h
	rm jameboy/cbtab.h
