flex: test.l
	flex test.l && g++ ./lex.yy.cc -O3 -o lexTest
lang: main.cc main.hh macros.hh classes.hh funcs.hh
	g++ -O3 main.cc
main:
	make flex
	make lang