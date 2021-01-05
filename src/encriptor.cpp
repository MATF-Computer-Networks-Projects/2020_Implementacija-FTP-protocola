#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
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


class RSAEncriptor : public Encriptor {
	public:
		RSAEncriptor(unsigned P, unsigned Q):_P(P), _Q(Q){
			_N = _P * _Q; //modul za generisanje public i private keys
			_lN = (_P - 1) * (_Q - 1); //ovo je secret

			//racunamo e encription key, e je co-prime, to je javni
			//kljuc
			//kao gcd(e, _lN) = 1 takav e da vazi
			getEncriptionKey();
			std::cout << _e; //ovo je public key

		}
	
		
		
		std::string encrypt(const std::string &text)const;
		std::string decrypt(const std::string &text)const;

	private:
		void getEncriptionKey();
		
		unsigned _P, _Q, _N, _lN, _e;
};

inline void RSAEncriptor::getEncriptionKey(){
	
	unsigned key_size = 128;
	unsigned g;
	long long difference = (long long)pow(2,128) - (long long)pow(2,127) - 1;
	srand(time(NULL));

	_e = std::pow(2,127) + (rand() % difference); //u intervalu od 2^127, 2^128 - 1 
	g = std::__gcd(_e, _lN);
	while(g != 1){
		_e = rand() % _lN;
		g = std::__gcd(_e,_lN);
	}
	std::cout << "Enkription key:" << _e << "End:Encription";
}
std::string RSAEncriptor::encrypt(const std::string &text)const{
	
	std::string cipher;
	cipher.resize(text.size());

	std::transform(text.begin(), text.end(), cipher.begin(),
			[=](char c){
			std::cout << (int)c << "]c";
			std::cout << _e << '\n';
			std::cout << _N;
			std::cout << (long long)std::pow(_e, c) << "]]pow";
			std::cout << (char)std::pow((int)c, _e) << '\n';
			return (int)std::pow(_e,c);
			});
	return cipher;
}
std::string RSAEncriptor::decrypt(const std::string &text)const{
	return "";
}

/*
int main(int argc, char *argv[]){

	//pronaci dva velika unsigned prosta broja 
	unsigned P = 91, Q = 107;


	Encriptor *ce = new CesarEncriptor();

	std::string line;
	std::cout << "Enter text:\n";


	getline(std::cin, line);
	
	std::cout << "Original:" << line << "\n";
	

	std::string cipher(ce->encrypt(line));

	std::cout << "Encripted:" << cipher << "\n";

	std::cout << "Decripted:" << ce->decrypt(cipher);

	return 0;
}
*/
