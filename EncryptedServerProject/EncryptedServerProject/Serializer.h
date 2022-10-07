#pragma once
#include "Client.h"

class Serializer {
public:
	static std::vector<unsigned char> SerializeRequest(Request* request);
private:
	static void serializeVersion(Request* request, std::vector<unsigned char>& msg);
	static void serializeCode(Request* request, std::vector<unsigned char>& msg);
	static void serializeId(Request* request, std::vector<unsigned char>& msg);
	static void serializePayload(Request* request, std::vector<unsigned char>& msg);
	static void serializeKey(Request* request, std::vector<unsigned char>& msg);
	static void serializeContentSize(Request* request, std::vector<unsigned char>& msg);
	static void serializeName(Request* request, std::vector<unsigned char>& msg);
	static void serializeFileName(Request* request, std::vector<unsigned char>& msg);
	static void serializeInteger(std::vector<unsigned char>& msg, int num, const unsigned short num_of_bytes);
};