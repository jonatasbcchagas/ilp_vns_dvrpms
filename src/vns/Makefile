# macros
CPP= g++
CFLAGS= -O3 -m64 -std=c++17
EXECUTABLE=vns

# targets
all: random_number_generator.o data.o vehicle.o solution.o vns.o main.o
	$(CPP) $(CFLAGS) -o $(EXECUTABLE) random_number_generator.o data.o vehicle.o solution.o vns.o main.o

random_number_generator.o: random_number_generator.cpp random_number_generator.hpp
	$(CPP) $(CFLAGS) -c random_number_generator.cpp

data.o: data.hpp data.cpp random_number_generator.hpp
	$(CPP) $(CFLAGS) -c data.cpp

vehicle.o: vehicle.hpp vehicle.cpp random_number_generator.hpp data.hpp
	$(CPP) $(CFLAGS) -c vehicle.cpp

solution.o: solution.hpp solution.cpp random_number_generator.hpp data.hpp vehicle.hpp
	$(CPP) $(CFLAGS) -c solution.cpp

vns.o: vns.hpp vns.cpp random_number_generator.hpp data.hpp vehicle.hpp solution.hpp
	$(CPP) $(CFLAGS) -c vns.cpp

main.o: main.cpp random_number_generator.hpp data.hpp vehicle.hpp solution.hpp vns.hpp
	$(CPP) $(CFLAGS) -c main.cpp

# remove
clean:
	rm -f ${EXECUTABLE} *.o
