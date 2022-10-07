#pragma once
#include "Client.h"

class Deserializer {
public:
	static Response* deserializeHeader(std::vector<unsigned char>& msg);
	static void deserializePayloads(Response* response, std::vector<unsigned char>& payload);
private:
	static void deserializeVersion(std::vector<unsigned char>& msg, Response* response);
	static void deserializeCode(std::vector<unsigned char>& msg, Response* response);
	static void deserializeId(std::vector<unsigned char>& msg, Response* response);
	static void deserializeAES(std::vector<unsigned char>& msg, Response* response);
	static void deserializeContentSize(std::vector<unsigned char>& msg, Response* response);
	static void deserializeFileName(std::vector<unsigned char>& msg, Response* response);
	static void deserializePayload(std::vector<unsigned char>& msg, Response* response);
	static void deserializeSize(std::vector<unsigned char>& msg, Response* response);
	static void deserializeChecksum(std::vector<unsigned char>& msg, Response* response);
	static int deserializeInteger(unsigned char num[]);
	static short deserializeShort(unsigned char num[]);
	static std::string deserializeString(std::vector<unsigned char>& msg, int length);
};
