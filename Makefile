CMP = gcc
FL = flex
CFLAGS = -Wall -lm
ENCODE = encode
UTIL = utils
MAIN = bminor
EXEC = bminor
SCANNER = scanner
PARSER = parser
BS = bison
BS_FLAGS = -d
LEX = lex.yy
SCAN = scan
PARSE = parse

all: $(PARSER).tab.c $(LEX).c $(EXEC) 
$(EXEC): $(MAIN).o $(UTIL).o $(ENCODE).o
	$(CMP) $(CFLAGS) -o $(EXEC) $(MAIN).o $(UTIL).o $(ENCODE).o

$(LEX).c: $(SCANNER).l $(PARSER).tab.h
	$(FL) $(SCANNER).l

$(PARSER).tab.c $(PARSER).tab.h: $(PARSER).y
	$(BS) $(BS_FLAGS) $(PARSER).y

$(ENCODE).o: $(ENCODE).c $(ENCODE).h
	$(CMP) $(CFLAGS) -c $(ENCODE).c -o $(ENCODE).o

$(UTIL).o: $(UTIL).c $(UTIL).h
	$(CMP) $(CFLAGS) -c $(UTIL).c -o $(UTIL).o

$(MAIN).o: $(MAIN).c $(ENCODE).h $(SCAN).h
	$(CMP) $(CFLAGS) -c $(MAIN).c -o $(MAIN).o

test: $(EXEC)
	sh runtest.sh

clean:
	rm -f *.o $(EXEC) $(LEX).c $(PARSER).tab.*

