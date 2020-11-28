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
		Socket();
		bool connect(const std::string &IP, unsigned port);
		unsigned sendData(const std::string &msg)const;
		void closeConnection()const;
		bool acceptNewConnection();
		std::string recvData()const;
		int get_file_descriptor()const;
		int get_sock_fd()const;
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
	
	if(send(dataFd, msg.c_str(), msg.length(), 0) < 0){
		perror("send:failed");
		return -1;		
	}
	return 0;
}

void Socket::closeConnection()const{
	close(dataFd);
}
bool Socket::acceptNewConnection(){
	
	struct addrinfo *adinfo, hints, *servinfo;	
	struct addrinfo *p;
	std::stringstream port_str;

	//int sockfd;
	memset(&hints, 0, sizeof hints);
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;


	int rv = getaddrinfo(NULL, "8182", &hints, &servinfo);
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
	//sockfd je soket zaduzen da slusa za povezivanje sa remote socketima
	
	std::cout << "Server:listening\n";
	if(int listenRvalue = listen(this->sockFd, BACKLOG)){
		std::cout << "listen failed:";// << listenRvalue;
		perror("listen failed");
	}

	int connfd;
	sockaddr cli;
	socklen_t len = sizeof(cli);
	connfd = accept(this->sockFd, &cli, &len);
	this->dataFd = connfd; //ovo je file descriptor na accept

	return true;
	}


//da ispravimo da vrati kolicinu primljenih podataka
std::string Socket::recvData() const {
	
	char buff[BUFFSIZE];
	bzero(buff, sizeof(buff));
	read(dataFd, buff, sizeof(buff));
	
	//std::cout << "ispis" << std::string(buff);
	return std::string(buff);
}

Socket::Socket() {

}
