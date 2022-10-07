#pragma once
#include <string>

enum request_type { REGISTER = 1100, PUBLIC_KEY = 1101, SEND_FILE = 1103, CRC_VALID = 1104, CRC_INVALID = 1105, CRC_INVALID_2 = 1106 };

class Request {
public:
	std::string client_id;
	char version;
	short code;
	int content_size;
	int payload_size;
	int name_size;
	int file_name_size;
	std::string message_content;
	std::string payload;
	std::string public_key;
	std::string file_name;
	std::string name;
};