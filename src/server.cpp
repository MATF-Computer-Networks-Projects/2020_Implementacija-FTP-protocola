#include "./socket.cpp"

#include <fstream>
#include <arpa/inet.h>
#define BUFFSIZE 255

//na jednom socketu se uspostavlja konekcija i ide razmena poruka i kljuceva i
//onda kada to prodje uspesno kreira se sa obe strane data socket kojim se
//razmenjuju podaci

class Server {
	
public:
	bool acceptNewClient();
	void sendData(const std::string &data);
	unsigned sendData(const json &data)const;
	void recvData()const;
	void serveClients();
	void closeServer();
	Socket getSocket()const;
	void accept();
	void accept_client();
	bool handshake();
private:
	Socket connection_socket;
	unsigned free_port = 8000;
	unsigned activeClients = 0;
	CesarEncriptor *encriptor;
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
unsigned Server::sendData(const json &data)const{
	connection_socket.sendData(data);
	return 0;
}

void Server::sendData(const std::string &data){
	connection_socket.sendData(data);
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
			
			bzero(buffer, BUFFSIZE);
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

void Server::accept_client(){
	connection_socket.accept();
	

	if(handshake()>0){
		std::cout << "\n\nSuccessfull handshake\n";
	}

}

bool Server::handshake() {
	
	std::string private_key = "server_pr1v4t3_k3y";
	std::string public_key = "server_pubL1c_k3y";

	//phase 1
	std::string hello_client = connection_socket.recvData();

	//std::cout << "Hello from client: " << hello_client;
	log("Hello from client:");
	log(hello_client);
	//ovde se parsira hello_client i dobiju se informacije o tome koji se
	//algoritam koristi za enkripciju i Rand number za generisanje kljuca 
	auto hello_client_struct = json::parse(hello_client);
	

	if(hello_client_struct["hello_msg"]=="hello server"){
		std::cout << hello_client_struct["hello_msg"];
	}else{
		std::cout << "Client hello msg error\n";
		return -1;
	}
	
	if(hello_client_struct["encription"] == "CesarEncription"){
		encriptor = new CesarEncriptor();
	}
	


	unsigned r = rand() % 360;
	
 	json to_client = {
		{"hello_msg", "hello_client"},
		{"protocol", "1"},
		{"random", "r"},
		{"encription", "1"},
		{"ugly", "%!@$#%^&*()''"}
	};
	sendData(to_client);

	//phase 1 end
	
	//phase 2
	json cert_key = {
		{"certificate","server_certificate"},
		{"public_key", public_key},
		{"request", "certificate"}
	};
	sendData(cert_key);

	//sendData("CERTIFICATE:server_certificate;public_key:" + public_key + ";REQUEST:certificate");
	
	
	//phase 2 end 
	//
	
	//phase 3
	std::string client_certificate = connection_socket.recvData();

	log(client_certificate);
	//todo check client certificate


	std::string hashed_data = connection_socket.recvData();


	std::cout <<"\nhashed data:" << hashed_data;

	auto hashed_json = json::parse(hashed_data);

	std::cout << "DATA: " << hashed_json["data"];


	return true;
}

int main(){
	Server server;
	//server.setEncritionMethod();
	server.acceptNewClient();
	server.getSocket().listen();

	while(1){
		server.accept_client();
	}
	server.closeServer();
	return 0;
}

