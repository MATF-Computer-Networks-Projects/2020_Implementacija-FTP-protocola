#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string>
#include <memory>
#include <cstdlib>
#include <sys/types.h>
#include <sstream>
#include <cstring>
#include <iostream>


//server postavi port neki i onda client da bi se konektovao mora da pogodi taj
//port
//client mora da unese neki port ako hoce da se konektuje na server

#define BACKLOG 5
#define BUFFSIZE 80

class Socket {
	public:
		bool connect(const std::string &IP, unsigned port);
		unsigned sendData(const std::string &msg)const;
		void closeConnection()const;
		bool bind(std::string port);
		std::string recvData()const;
		char* recvdata()const;
		int get_file_descriptor()const;
		int get_sock_fd()const;
		void listen()const;
		void accept();
		
	private:
		int dataFd, sockFd;
		std::string IP;
		unsigned port;
	
};


int Socket::get_file_descriptor()const { return dataFd;}
int Socket::get_sock_fd()const { return sockFd;}

bool Socket::connect(const std::string &IP, unsigned port){

	struct addrinfo *adinfo, hints, *servinfo;	
	struct addrinfo *p;
	std::stringstream port_str;

	memset(&hints, 0, sizeof hints);
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE;

	getaddrinfo(IP.c_str(), std::__cxx11::to_string(port).c_str(), &hints, &servinfo);

	for(p=servinfo;p!=NULL;p=p->ai_next){

		if ((this->dataFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                        perror("client: socket");
                        continue;
                }

		if(::connect(this->dataFd, p->ai_addr, p->ai_addrlen) == -1){
			perror("client:connect failed");
			close(this->dataFd);
			continue;
		}
		
		break;
               
	}

	return true;
}
unsigned Socket::sendData(const std::string &msg)const {
	const char *buff = new char [msg.length()];
	//prepisati poruku u buffer, pa buffer spakovati
	std::cout << "sock:sendData():" << msg << "..buff\n\n";
	if(send(dataFd, msg.c_str(),msg.length(), 0) < 0){
		perror("send:failed");
		return -1;		
	}

	return 0;
}

void Socket::closeConnection()const{
	close(dataFd);
}
bool Socket::bind(std::string port){
	
	struct addrinfo *adinfo, hints, *servinfo;	
	struct addrinfo *p;
	std::stringstream port_str;

	//int sockfd;
	memset(&hints, 0, sizeof hints);
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	const char * str_port = port.c_str();
	
	int rv = getaddrinfo(NULL, str_port, &hints, &servinfo);
	int yes;
        for(p = servinfo; p != NULL; p = p->ai_next) {

		std::cout << "Obilazak";


                if ((this->sockFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			std::cout << "server: socket";
                        continue;
                }
                if (setsockopt(this->sockFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			std::cout << "server: setsocketoption";
			continue;
                }


		std::cout << p->ai_addr->sa_data << '\n';


               if (::bind(this->sockFd, p->ai_addr, p->ai_addrlen) == -1) {
                        close(this->sockFd);

			std::cout << "server: bind";
                }
		std::cout << "pred break\n";

		break;
        }
		

	//std::cout << sockfd;
        if (p == NULL) {
                fprintf(stderr, "server: failed to bind\n");
                return 2;
        }
	


	std::cout << "Bind:done";
	return true;
}


//da ispravimo da vrati kolicinu primljenih podataka
std::string Socket::recvData() const {
	
	char buff[BUFFSIZE];
	bzero(buff, sizeof(buff));
	unsigned chars_read = read(dataFd, buff, sizeof(buff));
	
	//std::cout << "socket:buff=" << buff << "]\n";
	///std::cout << "Soket procitani karakteri: " << chars_read;
	if(chars_read==0) return "";

	return std::string(buff);
}
char * Socket::recvdata() const {
	
	char *buff = new char [BUFFSIZE];
	bzero(buff, sizeof(buff));
	unsigned chars_read = read(dataFd, buff, sizeof(buff));
	
	std::cout << "socket:buff=[" << buff << "]\n";
	std::cout << "recvdata:Soket procitani karakteri: " << chars_read;
	if(chars_read==0) return (char*)"";

	return buff;
}
//sockfd je soket zaduzen da slusa za povezivanje sa remote socketima
	//
	//
	//ovde je problem ovaj sockFd, mozda moram da nabavim drugi
	//ili da ne pravim uvek novi sockFd
	//
	//znaci napravljen jedan sockFd na portu 8000 i onda listen i accept
	//prihvataju klijente na tom portu u petlji
	

void Socket::listen()const{
	printf("Server listening ");
	if(int listenRvalue = ::listen(this->sockFd, BACKLOG)){
		std::cout << "listen failed:";// << listenRvalue;
		perror("listen failed");
	}
}

void Socket::accept(){
	int connfd;
	sockaddr cli;
	socklen_t len = sizeof(cli);
	connfd = ::accept(this->sockFd, &cli, &len);
	this->dataFd = connfd; //ovo je file descriptor na accept
	
	//ova funkcija se okida kada se neki klijent prijavi postavi se connfd i
	//krece accept
}


