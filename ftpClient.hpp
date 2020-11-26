/*
 * =====================================================================================
 *
 *       Filename:  ftpClient.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  22.11.2020. 12:29:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string>
#include <memory>
#include <iostream>


class ftpClient {
	private:
		socket control_socket;
	

	public:
		int connect(std::string IP, int port);	
};



int main(){
	
	struct addrinfo *adinfo, *hints;	

	int rv = getaddrinfo("127.0.0.1", );

	return 0;
}
