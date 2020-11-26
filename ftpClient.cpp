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

class Socket {
	public:
		Socket();
	private:



};
int converse(int sockfd);
int acceptFile(int sockfd);

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
	//hints.ai_flags = AI_PASSIVE;


	int rv = getaddrinfo("127.0.0.1", "23456", &hints, &servinfo);

	std::cout << rv << '\n';
	std::cout << sizeof(servinfo);

	for(p=servinfo;p!=NULL;p=p->ai_next){
		std::cout << (*p).ai_canonname;
		
		std::cout << "Client:obilazak";

		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                        perror("client: socket");
                        continue;
                }

		if(::connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			perror("client:connect failed");
			close(sockfd);
			continue;
		}
		
		break;
               
	}

	const char *msg = "input";
	send(sockfd, msg, strlen(msg), 0);
	
	acceptFile(sockfd);

	return 0;
		}


int converse(int sockfd){
	
	while(1){
		std::cout << "Enter a message for server:\n";
		
		std::string line;

		getline(std::cin, line);
	        const char * buff = line.c_str();

		if(send(sockfd, buff, strlen(buff), 0)>0){ //size_t 3 argument treba da dodje broj karaktera koji se salje ,
			// a povratna vrednost je broj karaktera koji je stigao
			
			if(strncmp(buff, "exit", 4) == 0){
				std::cout << "exiting..."; return 0;
			}
		}

		
		

	}

	
	

}

/* ovako izgleda kada prihvatamo l
 *
 * */
int acceptFile(int sockfd){
	std::cout << "acceptFile()";
	char buff[10];
	printf("%li", sizeof buff);
	bzero(buff, sizeof(buff));

	FILE *fd;
	fd = fopen("transfered.txt", "w");
	


	while(recv(sockfd, buff, sizeof(buff),0)){
		printf("%s", buff);
		fputs(buff, fd);
		bzero(buff, sizeof(buff)); //potrebno je pocistiti prostor, da tu legnu novi karakteri
		memset((char*)buff, 0, sizeof(buff));
	}

	fclose(fd);
	return 0;
}



