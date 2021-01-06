#include "./socket.cpp"
#include <fstream>
#include <map>



class Client{
public:
	bool connect(const std::string& IP, const unsigned port);
	bool closeConnection() const;
	unsigned sendData(const std::string & data)const;
	unsigned sendData(const json &data)const;
	bool requestFile(const std::string &file_name)const;
	bool handshake()const;
private:
	Socket connection_socket;
};


bool Client::connect(const std::string& IP, const unsigned port){
	connection_socket.connect(IP, port);
	return true;
}


unsigned Client::sendData(const json & data)const{
	connection_socket.sendData(data);
	return 0;
}


unsigned Client::sendData(const std::string &data)const{
	connection_socket.sendData(data);
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

//ovo ce biti implementirano po koracima sa openssl 

bool Client::handshake() const {

	std::string private_key = "client_pr1v4t3_k3y";
	std::string public_key = "client_publ1c_k3y";
	//phase 1
	unsigned r = rand() % 360;
	//std::map<std::string, std::string> struct_message;
	json struct_message;
	struct_message["hello_msg"] = "hello server";
	struct_message["random"] = std::__cxx11::to_string(r);
	struct_message["protocol"] = "1";
	struct_message["encription"] = "1";
	
	std::cout << struct_message.dump();
	sendData(struct_message.dump());
	//sendData("HELLO:HELLO SERVER;PROTOCOL:1; RANDOM:" + std::__cxx11::to_string(r) + ";ENCRIPTION:1");
	
	std::string recvd = connection_socket.recvData();
	std::cout << "recvd:[" << recvd;
	
	
	json s = json::parse(recvd);
	
	//auto hello_server_struct = json::parse(recvd);
	std::cout << s;

	//ovde ide obrada hello_server
	if(s["hello_msg"]=="hello_client"){	
		std::cout << s["hello_msg"] << '\n';
	}else{
		std::cout << "Server hello msg error\n";
		return -1;
	}

	//phase 1 end
	
	//phase 2
	//
	
	std::string certificate_msg = connection_socket.recvData();

	std::cout << certificate_msg;
	//todo check certificate
	//ako je sve ok saljem nazad svoj sertifikat
	

	//phase 2  end 
	

	//phase 3 
	json cert_key = {
		{"certificate","client_certificate"},
		{"public_key", public_key}
	};
	sendData(cert_key);
	
	// hash

	json hashed_data = {
		{"data", "HASHEDdAATA WITH PRIVATE KEY"}
		
	};
	std::cout << hashed_data["data"];
	
	sendData(hashed_data);





	return true;
}
int main(){
	
	/*
	if(argc!=3){
		std::cout << "Program pokrenuti sa ./client <IP> <PORT>\n";
		exit(1);
	}
	*/
	Client c;

	c.connect("127.0.0.1", 8000);
	
	//posle uspesne konekcije ide protokol uspostavljanja data socketa sa
	//serverom
	if(c.handshake()){std::cout << "\n\nSuccessfull handshake\n";}


	//c.requestFile("input.txt");
	

	c.closeConnection();
	return 0;
}

