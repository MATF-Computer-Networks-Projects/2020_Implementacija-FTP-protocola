#include "./socket.cpp"


class Server {
	
public:
	bool acceptNewClient();
	void sendData();
	void recvData();
	void serveClients();
	void closeServer();
private:
	Socket connection_socket;
	unsigned free_port = 8000;
	unsigned activeClients = 0;
	
};


bool Server::acceptNewClient(){


	connection_socket.acceptNewConnection();
	

		std::string msg = connection_socket.recvData();

		if(msg == "exit"){
			std::cout << "exiting..";
		}

	

	return true;
}

void Server::serveClients(){


}

void Server::closeServer(){
	connection_socket.closeConnection();



}


int main(){
	Server server;
	
	server.acceptNewClient();
	
	server.closeServer();
	return 0;
}

