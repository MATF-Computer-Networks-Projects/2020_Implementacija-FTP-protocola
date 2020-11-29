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
	Socket getSocket()const;
	void accept();
private:
	Socket connection_socket;
	unsigned free_port = 8000;
	unsigned activeClients = 0;
	
};

//obrada komandi:
//dodje komanda npr GET file.txt i ova funkcija mora da uradi to sto se trazi 
bool Server::acceptNewClient(){


	connection_socket.bind(std::__cxx11::to_string(free_port));
	
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

Socket Server::getSocket()const {
	return connection_socket;
}

void Server::accept(){
	connection_socket.accept();
	//neka logika jeste da listen blokirajuci osluskivac koji kada dodje
	//klijent na nekom portu i prodje povezivanje tada se okida funkcija
	//accept i onda zapocinje obrada klijentskog zahteva na serveru
	
	//ovo parce kod-a ide u poseban proces 
	//
	pid_t pid = fork();
	
	if(pid == 0){
		std::cout << "\nAccepted client\n";
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
	//std::cout << ipstr;

	std::string msg = connection_socket.recvData();
		
	if(msg.substr(0, 3) == "GET"){
		std::string requested_file = msg.substr(4);
		std::cout << "\nfile_name:" << requested_file << '\n';
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

		while(!file.eof()){
			
			bzero(buffer, sizeof buffer);
			file.read(buffer, BUFFSIZE);
			//if(file.gcount()==0)break;
			
			//std::cout << "broj karaktera" << file.gcount();
			std::cout << std::string(buffer);

			sendData(std::string(buffer));

		}

		delete[] buffer;
		
		sendData("EOF");//kako ovo ne stigne 


	}else{
		std::cout << "Correct request: GET [file name]\n";
	}


	
	connection_socket.closeConnection();
	std::cout << "posle citanja iz fajla";
	
	//kada se zatvori konekcija tada se zavrsava klijentski proces
	exit(0);

	}else if(pid < 0){
		perror("forking error");
	}else{//parent process
		
	}


}

int main(){
	Server server;
	
	server.acceptNewClient();
	server.getSocket().listen();

	while(1){
		server.accept();
	}
	server.closeServer();
	return 0;
}

