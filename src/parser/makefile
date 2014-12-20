LEX = flex
YACC = bison -y
YFLAGS = -d

all: lex.yy.c y.tab.c

lex.yy.c:
	$(LEX) tokenizer.l

y.tab.c:
	$(YACC) $(YFLAGS) parser.y

clean:
	rm -f lex.yy.c y.tab.c y.tab.h
	rm -f *.o

.PHONY: all clean
