#pragma once
#include <string>

enum response_codes { REGISTRATION_SUCCESS = 2100, PUBLIC_KEY_SUCCESS = 2102, FILE_VALID = 2103, ACCEPTED = 2104 };

class Response {
public:
	unsigned char version;
	short code;
	int payload_size;
	int content_size;
	int cksum;
	std::string payload;
	std::string id;
	std::string AES;
	std::string file_name;
};