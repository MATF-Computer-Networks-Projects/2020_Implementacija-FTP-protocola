CXX = g++

client server: ftpClient.cpp ftpServer.cpp
	$(CXX) server.cpp -o server
	$(CXX) client.cpp -o client
	



.PHONY:
	clean run

clean:
	rm -rf client server

run:
	make clean && make
