CPLEXFLAGS=-O3 -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -I/opt/cplex/cplex/include -I/opt/cplex/concert/include  -L/opt/cplex/cplex/lib/x86-64_osx/static_pic -lilocplex -lcplex -L/opt/cplex/concert/lib/x86-64_osx/static_pic -lconcert -lm -pthread -std=c++0x

CFLAGS=-std=c++11 -static-libstdc++ -Wall

all:feed.o
	g++  feed.o -o feed.run $(CPLEXFLAGS) $(CFLAGS)


%.o: %.cpp %.hpp
	g++ -c $< -o $@ $(CFLAGS)

feed.o: feed.cpp
	g++ -c -o feed.o feed.cpp $(CPLEXFLAGS) $(CFLAGS)

clean:
	rm -f *.o
	rm -f *.run
	rm -f *.log

