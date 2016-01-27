CPP	= g++
all: coverability

full: parsers coverability

cleanmain:
	rm -f main.o coverability

clean:
	rm -f *.o coverability
	$(MAKE) -C bp_parser clean

coverability: parsers ttd.o bp.o main.o
	$(CPP) -o $@ *.o bp_parser/*.o

main.o: main.cc

vass.o: vass.cc

ttd.o: ttd.cc

bp.o: bp.cc

parsers:
	$(MAKE) -C bp_parser

