CMP = gcc
CFLAGS = -Wall
ENCODE = encode
UTIL = utils
MAIN = bminor
EXEC = bminor
FLEX = scanner
LEX = lex.yy

all: $(LEX).c $(EXEC) 

$(EXEC): $(MAIN).o $(UTIL).o $(ENCODE).o
	$(CMP) $(CFLAGS) -o $(EXEC) $(MAIN).o $(UTIL).o $(ENCODE).o

$(LEX).c: $(FLEX).l
	flex $(FLEX).l

$(ENCODE).o: $(ENCODE).c $(ENCODE).h
	$(CMP) $(CFLAGS) -c $(ENCODE).c -o $(ENCODE).o

$(UTIL).o: $(UTIL).c $(UTIL).h
	$(CMP) $(CFLAGS) -c $(UTIL).c -o $(UTIL).o

$(MAIN).o: $(MAIN).c $(ENCODE).h
	$(CMP) $(CFLAGS) -c $(MAIN).c -o $(MAIN).o -lfl

test: $(EXEC)
	sh runtest.sh

clean:
	rm -f *.o $(EXEC) $(LEX).c

