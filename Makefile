CMP = gcc
CFLAGS = -Wall
ENCODE = encode
UTIL = utils
MAIN = bminor
EXEC = bminor

all: $(EXEC)

$(EXEC): $(MAIN).o $(UTIL).o $(ENCODE).o
	$(CMP) $(CFLAGS) $(MAIN).o $(UTIL).o $(ENCODE).o -o $(EXEC)

$(ENCODE).o: $(ENCODE).c $(ENCODE).h
	$(CMP) $(CFLAGS) -c $(ENCODE).c -o $(ENCODE).o

$(UTIL).o: $(UTIL).c $(UTIL).h
	$(CMP) $(CFLAGS) -c $(UTIL).c -o $(UTIL).o

$(MAIN).o: $(MAIN).c $(ENCODE).h
	$(CMP) $(CFLAGS) -c $(MAIN).c -o $(MAIN).o

test: $(EXEC)
	sh runtest.sh

clean:
	rm -f *.o $(EXEC)

