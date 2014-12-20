LEX = flex
YACC = bison -y
YFLAGS = -d

all: tokenizer.c parser.c

tokenizer.c:
	$(LEX) tokenizer.l

parser.c:
	$(YACC) $(YFLAGS) parser.y

clean:
	rm -f tokenizer.c tokenizer.h parser.c parser-defines.h
	rm -f *.o

.PHONY: all clean
