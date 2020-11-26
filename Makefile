CXX = g++

client server: ftpClient.cpp ftpServer.cpp
	$(CXX) ftpServer.cpp -o server
	$(CXX) ftpClient.cpp -o client
	



.PHONY:
	clean

clean:
	rm -rf client server
