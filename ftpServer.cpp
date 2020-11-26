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
#include <unistd.h>

#define BACKLOG 10



int converse(int sockfd);
int sendFile(int sockfd);


int main(){

	struct addrinfo *adinfo, hints, *servinfo;	
	struct addrinfo *p;
	std::stringstream port_str;

	//funkcija koja dohvata povezanu listu struktura adresa na koje moze da
	//se primeni bind, connect
	//
	//getaddrinfo() - ima nekoliko karakteristika kako se zove
	
	int sockfd;
	memset(&hints, 0, sizeof hints);
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;


	int rv = getaddrinfo(NULL, "23456", &hints, &servinfo);
	int yes = 1;
	std::cout << "Return value:getaddrinfo: " << rv << '\n';


 // loop through all the results and bind to the first we can



        for(p = servinfo; p != NULL; p = p->ai_next) {

		std::cout << "Obilazak";


                if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			std::cout << "server: socket";
                        continue;
                }
                if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			std::cout << "server: setsocketoption";
			continue;
                }


		std::cout << p->ai_addr->sa_data << '\n';


		//kada se navede :: scope operator i onda ime
		//funkcije/promenljive koji se dohvata, on tada dohvata globalnu
		//funkciju a ne lokalnu
		//
		//on ovde moze da mesa izmedju klasnog bind i globalne bind
		//funkcije u zaglavlju ...
		//
		//
		//
		//povezivanje adrese sa socketom 
                if (::bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                        close(sockfd);

			std::cout << "server: bind";
                }
		std::cout << "pred break\n";

		break;
        }
		

	std::cout << sockfd;
        if (p == NULL) {
                fprintf(stderr, "server: failed to bind\n");
                return 2;
        }
	//sockfd je soket zaduzen da slusa za povezivanje sa remote socketima
	
	std::cout << "Server:listening\n";
	if(int listenRvalue = listen(sockfd, BACKLOG)){
		std::cout << "listen failed:";// << listenRvalue;
	}

	int connfd;
	sockaddr cli;
	socklen_t len = sizeof(cli);
	connfd = accept(sockfd, &cli, &len);

	//ovo cli bi trebalo da sadrzi informacije o clientu
	
	//postoji funkcija getpeername() koja po sockfd dohvata informacije o
	//konekciji i to smesta u addr
	//
	
	char hostname[40];
	gethostname(hostname, strlen(hostname));

	std::cout << "Hostname ime trenutnog kompjutera: " << hostname;
	
	if(connfd < 0){
		perror("accept() error");
	}
	else std::cout << "accept done";
	

	//converse(connfd);
	char buff[1024];
	bzero(buff, sizeof(buff));

	
	recv(connfd, buff, sizeof(buff), 0);

	std::cout << "Msg from client: " <<buff;
	if(strncmp("input", buff, strlen(buff)) == 0)
		sendFile(connfd);



	return 0;
}



int converse(int sockfd){
	
	char buff[1024];
	while(1){
		std::cout << "Waiting for message in recv:\n";
		if(recv(sockfd, buff, sizeof(buff), 0)>0){ //ekvivalentni su  recv sa flags 0 i read
			
			if(strncmp(buff, "exit", 4) == 0){
				std::cout << "exiting..."; return 0;
			}
			std::cout << "" << buff << '\n';

		}
		bzero(buff, sizeof(buff));

	}
	

}


int sendFile(int sockfd){
	
	FILE *fp;
	fp = fopen("input.txt", "r");

	char buff[10];
	if(!fp){
		perror("fopen error");
		return -1;
	}

	while(fgets(buff, 10, (FILE*)fp)){
			std::cout << buff;

			if(send(sockfd, buff, strlen(buff), 0) < 0){
				perror("Server error");return -1;
			}

	}


	fclose(fp);

	return 0;

}
