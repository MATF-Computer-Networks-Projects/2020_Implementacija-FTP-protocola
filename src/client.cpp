#include "./socket.cpp"

#include <fstream>

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
//bez ikakve provere da li fajl moze da se posalje mozda uvesti neka ogranicenja
bool Client::requestFile(const std::string &file_name) const{
	connection_socket.sendData("GET " + file_name);

	std::ofstream file;
	file.open(file_name + "_1");
	unsigned steps = 0;
	while(1){
		
		std::string line(connection_socket.recvdata());//nije da ovo blokira proces

		//fora je sto se ovo nikad nece desiti 
		//zato sto ako je buffer 80 karaktera a ovo procita poslednjih
		//25
		//sledece citanje nece da vrati 0 karaktera
		//
		//zato je bolje 
		//ono sto blokira je ovaj recvData() koji blokira proces da
		//saceka server da napuni buffer
		//zato moramo da znamo da li smo stigli do kraja fajla i da
		//posaljem poruku da je pisanje iz fajla u buffer stiglo do
		//kraja 
	/*	if(line.length()==0){
			std::cout << "Line.length() == 0";
			file.close();
			return true;
		}
			*/
		if(line.find("EOF") != std::string::npos){
			break;
		}
		file << line;
		std::cout << "xxx[" <<line << "]xxx";
	}
	//ne razumem zasto ne iskoci iz ove petlje 
	//RAZLOG: zato sto recvData ceka da mu se posalje neki string i onda ako
	//server zavrsi slanje podataka client i dalje ceka
	//tako da je potrebno da client zna da server nema vise sta da salje i
	//dobija poruku EOF ili tako nesto da je kraj fajla
	//
	//e sad slanje poruka iz nekih razloga ide traljavo 
	//pa se mora raaspodeliti u posebne sokete i dataFd-ove

	file.close();
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

	c.connect("127.0.0.1", 8000);

	c.requestFile("input.txt");
	

	std::cout << "posle request file";
	c.closeConnection();
	return 0;
}

