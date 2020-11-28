#include "./socket.cpp"

#include <fstream>
#include <arpa/inet.h>
#define BUFFSIZE 80

//na jednom socketu se uspostavlja konekcija i ide razmena poruka i kljuceva i
//onda kada to prodje uspesno kreira se sa obe strane data socket kojim se
//razmenjuju podaci

class Server {
	
public:
	bool acceptNewClient();
	void sendData(const std::string &msg);
	void recvData();
	void serveClients();
	void closeServer();
private:
	Socket connection_socket;
	unsigned free_port = 8000;
	unsigned activeClients = 0;
	
};

//obrada komandi:
//dodje komanda npr GET file.txt i ova funkcija mora da uradi to sto se trazi 
bool Server::acceptNewClient(){


	connection_socket.acceptNewConnection();
	
	//funkcija getpeername
	
	int fd = connection_socket.get_sock_fd();
	struct sockaddr addr;
	socklen_t len;
	char ipstr[INET_ADDRSTRLEN];
	getpeername(fd,  &addr, &len);//videti koji socket ovde treba da dodje mozda control socket
	
	if(addr.sa_family == AF_INET){
		struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	}
	std::cout << ipstr;

	std::string msg = connection_socket.recvData();
		
	if(msg.substr(0, 3) == "GET"){
		std::string requested_file = msg.substr(4);
		std::cout << "\nfile_name:" << requested_file;
		//pronaci da citam tacno X bytova
		//read to radi 
		//cita length karaktera i smesta u buffer
		//
		//ovde treba napakovati da se cita odredjen broj karaktera kako
		//bi bili procitani i znakovi za novi red
		//
		//
		//ovo ubacuje neke cudne karaktere 
		//u fajl BUG
		std::ifstream file(requested_file, std::ifstream::binary);
		char *buffer = new char [BUFFSIZE];

		while(1 && file.is_open()){
			
			file.read(buffer, BUFFSIZE);
			if(file.gcount()==0)break;
			
			std::cout << std::string(buffer);
			sendData(std::string(buffer));
			
		}

		delete[] buffer;
		


	}else{
		std::cout << "Correct request: GET [file name]\n";
	}


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
void Server::sendData(const std::string &msg){
	connection_socket.sendData(msg);
}

int main(){
	Server server;
	
	server.acceptNewClient();
	
	server.closeServer();
	return 0;
}

