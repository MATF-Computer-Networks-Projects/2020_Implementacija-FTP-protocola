#include "./socket.cpp"



class Client{
public:
	bool connect(const std::string& IP, const unsigned port);
	unsigned sendData(const std::string &msg)const;
	bool closeConnection() const;
	bool requestFile(const std::string &file_name)const;
private:
	Socket connection_socket;
};


bool Client::connect(const std::string& IP, const unsigned port){
	connection_socket.connect(IP, port);
	return true;
}


unsigned Client::sendData(const std::string &msg)const{
	connection_socket.sendData(msg);
	return 0;

}


bool Client::closeConnection() const{
	connection_socket.closeConnection();
	return 0;

}

bool Client::requestFile(const std::string &file_name) const{
	connection_socket.sendData("GET " + file_name);
	return 0;

}



int main(int argc, char *argv[]){
	
	/*
	if(argc!=3){
		std::cout << "Program pokrenuti sa ./client <IP> <PORT>\n";
		exit(1);
	}
	*/
	Client c;

	c.connect("127.0.0.1", 8182);

	c.requestFile("input.txt");

	c.closeConnection();
	return 0;
}

