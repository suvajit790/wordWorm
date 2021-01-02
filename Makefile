# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BASE = wordWorm
CXX = g++
LIBFLAGS=-lm

main: $(BASE)

%.o: %.cc
	$(CXX) -c -o$@ $<

$(BASE): $(BASE).o classes.o
	$(CXX) -o $@ $^ $(LIBFLAGS)

run: $(BASE)
	@echo "Running wordWorm"
	./$<

clean:
	@echo "Cleaning up..."
	rm -f $(BASE) *.o
