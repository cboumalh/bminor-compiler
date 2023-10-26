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
BS_FLAGS = -d -v
LEX = lex.yy
SCAN = scan
PARSE = parse
EXPR = expr
TYPE = type
PARAM_LIST = param_list
STMT = stmt
DECL = decl


all: $(PARSER).tab.c $(LEX).c $(EXEC) 

$(EXEC): $(MAIN).o $(UTIL).o $(ENCODE).o $(EXPR).o $(TYPE).o $(PARAM_LIST).o $(STMT).o $(DECL).o
	$(CMP) $(CFLAGS) -o $(EXEC) $(MAIN).o $(UTIL).o $(ENCODE).o $(EXPR).o $(TYPE).o $(PARAM_LIST).o $(STMT).o $(DECL).o

$(LEX).c: $(SCANNER).l $(PARSER).tab.h
	$(FL) $(SCANNER).l

$(PARSER).tab.c $(PARSER).tab.h: $(PARSER).y $(DECL).h
	$(BS) $(BS_FLAGS) $(PARSER).y

$(ENCODE).o: $(ENCODE).c $(ENCODE).h
	$(CMP) $(CFLAGS) -c $(ENCODE).c -o $(ENCODE).o

$(UTIL).o: $(UTIL).c $(UTIL).h
	$(CMP) $(CFLAGS) -c $(UTIL).c -o $(UTIL).o

$(EXPR).o: $(EXPR).c $(EXPR).h
	$(CMP) $(CFLAGS) -c $(EXPR).c -o $(EXPR).o

$(TYPE).o: $(TYPE).c $(TYPE).h
	$(CMP) $(CFLAGS) -c $(TYPE).c -o $(TYPE).o

$(PARAM_LIST).o: $(PARAM_LIST).c $(PARAM_LIST).h
	$(CMP) $(CFLAGS) -c $(PARAM_LIST).c -o $(PARAM_LIST).o

$(STMT).o: $(STMT).c $(STMT).h
	$(CMP) $(CFLAGS) -c $(STMT).c -o $(STMT).o	

$(DECL).o: $(DECL).c $(DECL).h
	$(CMP) $(CFLAGS) -c $(DECL).c -o $(DECL).o	

$(MAIN).o: $(MAIN).c $(ENCODE).h $(SCAN).h $(PARSE).h
	$(CMP) $(CFLAGS) -c $(MAIN).c -o $(MAIN).o

test: $(EXEC)
	sh runtest.sh

clean:
	rm -f *.o $(EXEC) $(LEX).c $(PARSER).tab.* $(PARSER).output ./test/encode/*.out ./test/scanner/*.out ./test/parser/*.out ./test/printer/*.out

