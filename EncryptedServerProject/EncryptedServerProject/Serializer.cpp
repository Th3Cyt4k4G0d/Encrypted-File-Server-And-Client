#include "Serializer.h"

std::vector<unsigned char> Serializer::SerializeRequest(Request* request)
{
	std::vector<unsigned char> msg;
	short code = request->code;

	serializeId(request, msg);
	serializeVersion(request, msg);
	serializeCode(request, msg);
	

	switch (code)
	{
	case REGISTER:
		// inserting size 
		serializeInteger(msg, request->payload_size, SIZE_SIZE);
		serializeName(request, msg);
		break;
	case PUBLIC_KEY:
		serializeInteger(msg, request->payload_size, SIZE_SIZE);
		serializeName(request, msg);
		serializeKey(request, msg);
		break;
	case SEND_FILE:
		serializeInteger(msg, request->payload_size, SIZE_SIZE);
		serializeId(request, msg);
		serializeContentSize(request, msg);
		serializeFileName(request, msg);
		//serializeContent(request, msg);
		break;
	}
	
	return msg;
}


void Serializer::serializeVersion(Request* request, std::vector<unsigned char>& msg) {
	// insert version.
	serializeInteger(msg, request->version, VERSION_SIZE);
}

void Serializer::serializeId(Request* request, std::vector<unsigned char>& msg) {
	int id_len = request->client_id.length();

	// insert id.
	for (int i = 0; i < id_len; i++) {
		msg.push_back(unsigned char(request->client_id[i]));
	}
}

void Serializer::serializeCode(Request* request, std::vector<unsigned char>& msg) {
	// insert code.
	serializeInteger(msg, request->code, CODE_SIZE);
}

void Serializer::serializeContentSize(Request* request, std::vector<unsigned char>& msg) {
	// insert content size.
	serializeInteger(msg, request->content_size, SIZE_SIZE);
}

void Serializer::serializePayload(Request* request, std::vector<unsigned char>& msg) {
	// insert size. 
	int size = request->payload_size;
	serializeInteger(msg, size, SIZE_SIZE);

	// insert content.
	for (int i = 0; i < size; i++) {
		msg.push_back(unsigned char(request->payload[i]));
	}
}

void Serializer::serializeKey(Request* request, std::vector<unsigned char>& msg) {
	int size = KEY_SIZE;

	// insert key.
	for (int i = 0; i < size; i++) {
		msg.push_back(unsigned char(request->public_key[i]));
	}
}

void Serializer::serializeName(Request* request, std::vector<unsigned char>& msg)
{
	// insert name.
	for (int i = 0; i < request->name_size; i++) {
		msg.push_back(unsigned char(request->name[i]));
	}
}

void Serializer::serializeFileName(Request* request, std::vector<unsigned char>& msg)
{
	// insert name.
	for (int i = 0; i < request->file_name_size; i++) {
		msg.push_back(unsigned char(request->file_name[i]));
	}
}

void Serializer::serializeInteger(std::vector<unsigned char>& msg, int num, const unsigned short num_of_bytes) {
	for (int i = 0; i < num_of_bytes; i++) {
		msg.push_back(unsigned char(num >> i * BYTE_SIZE)); // check whether it is indeed little endian. :)
	}
}

