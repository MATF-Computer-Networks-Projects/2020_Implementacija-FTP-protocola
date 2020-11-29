#include <fstream>
#include <iostream>
#include <algorithm>
// apstraktna klasa Encriptor ce da implementira osnovne metode za enkripciju i dekripciju


class Encriptor{
	public: 
		virtual std::string encrypt(const std::string &text)const = 0;
		virtual std::string decrypt(const std::string &text)const = 0;
	

};

//<> je ukupan broj karaktera 
//cesar encriptor radi sledece: (c + s) % <> --> cipher char
//cesar decriptor (cipher - s) % <> --> original char


class CesarEncriptor : public Encriptor {
	public:
		CesarEncriptor(unsigned shift=5, unsigned total=128):_shift(shift), _total(total)
	{}
		
		
		std::string encrypt(const std::string &text)const;
		std::string decrypt(const std::string &text)const;

	private:
		unsigned _shift, _total;
};


std::string CesarEncriptor::encrypt(const std::string &text)const {

	std::string encripted_msg;

	std::transform(text.cbegin(), text.cend(), back_inserter(encripted_msg), [this](char c){
		return (c + _shift) % _total;
	});
	

	return encripted_msg;
}

std::string CesarEncriptor::decrypt(const std::string &text)const {

	std::string decripted_msg;

	std::transform(text.cbegin(), text.cend(), back_inserter(decripted_msg), [this](char c){
		return (c - _shift) % _total;
	});
	

	return decripted_msg;
}



int main(int argc, char *argv[]){

	
	Encriptor *ce = new CesarEncriptor(5, 128);

	std::string line;
	std::cout << "Enter text:\n";
	getline(std::cin, line);
	
	std::cout << "Original:" << line << "\n";
	

	std::string cipher(ce->encrypt(line));

	std::cout << "Encripted:" << cipher << "\n";

	std::cout << "Decripted:" << ce->decrypt(cipher);

	return 0;
}

