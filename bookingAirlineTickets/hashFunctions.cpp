#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream>
#include "Header.h"

using namespace std;

std::string secr::hashfunc(std::string& input) {
	unsigned int hash = 5381;
	for (char c : input)
		hash = ((hash << 5) + hash) + static_cast<unsigned int>(c);
	std::stringstream ss;
	ss << std::hex << hash;
	return ss.str();
}

void secr::check_func() {
	std::string correctHash = "17f6dc38";
	std::string input;
	interface::setstars('*');
	std::cout << "Enter a password: "; //password
	std::cin >> input;
	std::string inputHash = secr::hashfunc(input);
	//std::cout << inputHash;
	if (inputHash == correctHash)
	{
		interface::setstars('*');
		std::cout << "Access Allowed!\n";
		interface::write_function();
	}
	else
	{
		interface::setstars('*');
		std::cout << "Access Denied!\n";
		datafile::exit_function();
	}

}