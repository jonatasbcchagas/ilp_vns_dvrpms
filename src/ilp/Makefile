# macros
CPP= g++
CFLAGS= -O3 -m64 -w -std=c++17
CPLEXPATH=/opt/ibm/ILOG/CPLEX_Studio125/cplex
CONCERTPATH=/opt/ibm/ILOG/CPLEX_Studio125/concert
EXECUTABLE=ilp

# targets
all: data.o main.o
	$(CPP) $(CFLAGS) -L$(CPLEXPATH)/lib/x86-64_sles10_4.1/static_pic -L$(CONCERTPATH)/lib/x86-64_sles10_4.1/static_pic -o $(EXECUTABLE) data.o main.o -lilocplex -lcplex -lconcert -lm -lpthread

data.o: ../vns/data.hpp ../vns/data.cpp
	$(CPP) $(CFLAGS) -c ../vns/data.cpp

main.o: main.cpp ../vns/data.hpp ilp.hpp
	$(CPP) $(CFLAGS) -DIL_STD -g -I$(CPLEXPATH)/include -I$(CONCERTPATH)/include -L$(CPLEXPATH)/lib -c main.cpp

# remove
clean:
	rm -f ${EXECUTABLE} *.o
