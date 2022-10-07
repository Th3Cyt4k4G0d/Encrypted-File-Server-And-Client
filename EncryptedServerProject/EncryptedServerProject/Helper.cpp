#include "Helper.h"

const std::string get_ip(const std::string line) {
	return line.substr(0, line.find(":"));
}

std::string get_port(std::string line) {
	line.erase(0, line.find(":") + 1);
	return line;
}

void handle_tranfer_file(std::string& ip, std::string& port, std::string& username, std::string& path) {
	std::string line;

	std::ifstream fp(TRANSFER_FILE);
	if (!fp) {
		std::cout << "File couldn't be opened" << std::endl;
		exit(0);
	}
	std::getline(fp, line);
	ip = get_ip(line);
	port = get_port(line);
	std::getline(fp, line);
	username = line;
	std::getline(fp, line);
	path = line;
	fp.close();
}

void handle_me_file(std::string& username, std::string& id, std::string& key) {
	std::string line;

	std::ifstream fp(ME_FILE);
	if (!fp) {
		std::cout << "File couldn't be opened" << std::endl;
		exit(0);
	}
	std::getline(fp, line);
	username = line;
	std::getline(fp, line);
	id = line;
	while(std::getline(fp, line))
		key += line;
	fp.close();
}

bool file_exists(const std::string file) {
	std::ifstream infile(file);
	return infile.good();
}

void create_me_file(const std::string username, const std::string& id, const std::string& key) {
	std::ofstream fp(ME_FILE);
	fp << username << std::endl;
	fp << id << std::endl;
	fp << key << std::endl;
}
