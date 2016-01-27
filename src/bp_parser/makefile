CPP	= g++

#tested with version 2.5.31
FLEX    = flex

# make sure bison is version 2.1
BISON   = bison

TARGETS    = scanner.o parser.o context.o ast.o

DEBUG   = 

.SUFFIXES: .cpp .o

all: $(TARGETS) 

ast.o: ast.cpp ast.h

parser.o: parser.cpp parser.hpp ast.h

scanner.o: scanner.cpp parser.hpp ast.h

scanner.cpp: scanner.l
	$(FLEX) -o $@ $<

parser.hpp: parser.cpp

parser.cpp: parser.y
	$(BISON) -d $< --output-file=$@

.cpp.o:
	$(CPP) $(DEBUG) -Wall -c $(INCLUDE) $< -o $@

clean:
	rm -f $(TARGETS) *~

