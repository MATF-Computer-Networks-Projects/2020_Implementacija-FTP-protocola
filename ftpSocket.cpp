#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sstream>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include <cstring>

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


	int rv = getaddrinfo(NULL, port_str.str().c_str(), &hints, &servinfo);

	std::cout << rv << '\n';

	for(p=servinfo;p!=NULL;p=p->ai_next){
		std::cout << "for:";
		std::cout << (*p).ai_canonname;
		
		if(sockfd
	}

	return 0;
}
