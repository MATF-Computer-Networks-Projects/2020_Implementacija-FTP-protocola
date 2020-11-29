CXX = g++
CXXFLAGS = -Wall -Wextra


client server: client.o server.o
	$(CXX) $(CXXFLAGS) client.o -o client
	$(CXX) $(CXXFLAGS) server.o -o server


client.o: src/client.cpp headers/client.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

server.o: src/server.cpp headers/server.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

encriptor.o: src/encriptor.cpp headers/encriptor.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@




.PHONY:
	clean run

clean:
	rm -rf client server encriptor client.o server.o

run:
	make clean && make
