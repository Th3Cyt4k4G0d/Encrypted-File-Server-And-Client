#include "Deserializer.h"

Response* Deserializer::deserializeHeader(std::vector<unsigned char>& msg)
{
	Response* response = new Response();
	deserializeVersion(msg, response);
	deserializeCode(msg, response);
	deserializeSize(msg, response);
	return response;
}

void Deserializer::deserializePayloads(Response* response, std::vector<unsigned char>& payload) {
	short code = response->code;

	deserializeId(payload, response);
	switch (code) {
	case REGISTRATION_SUCCESS:
		break;
	case PUBLIC_KEY_SUCCESS:
		deserializeAES(payload, response);
		break;
	case FILE_VALID:
		deserializeContentSize(payload, response);
		deserializeFileName(payload, response);
		deserializeChecksum(payload, response);
		break;
	case ACCEPTED:
		std::cout << "Message Received Successfully." << std::endl;
		break;
	}
	return;
}

void Deserializer::deserializeVersion(std::vector<unsigned char>& msg, Response* response)
{
	response->version = msg.at(0);
	msg.erase(msg.begin());
}

void Deserializer::deserializeCode(std::vector<unsigned char>& msg, Response* response)
{
	unsigned char num[2];
	for (int i = 0; i < CODE_SIZE; i++) {
		num[i] = msg.at(0);
		msg.erase(msg.begin());
	}

	response->code = deserializeShort(num);
}

void Deserializer::deserializeSize(std::vector<unsigned char>& msg, Response* response)
{
	unsigned char num[4];
	for (int i = 0; i < SIZE_SIZE; i++) {
		num[i] = msg.at(0);
		msg.erase(msg.begin());
	}

	response->payload_size = deserializeInteger(num);
}

void Deserializer::deserializeChecksum(std::vector<unsigned char>& msg, Response* response)
{
	unsigned char num[4];
	for (int i = 0; i < SIZE_SIZE; i++) {
		num[i] = msg.at(0);
		msg.erase(msg.begin());
	}

	response->cksum = deserializeInteger(num);
}

void Deserializer::deserializeContentSize(std::vector<unsigned char>& msg, Response* response)
{
	unsigned char num[4];
	for (int i = 0; i < SIZE_SIZE; i++) {
		num[i] = msg.at(0);
		msg.erase(msg.begin());
	}

	response->content_size = deserializeInteger(num);
}

void Deserializer::deserializeId(std::vector<unsigned char>& msg, Response* response)
{
	response->id = deserializeString(msg, ID_SIZE);
}

void Deserializer::deserializeFileName(std::vector<unsigned char>& msg, Response* response)
{
	response->file_name = deserializeString(msg, response->payload_size - ID_SIZE - 2 * SIZE_SIZE);
}

void Deserializer::deserializeAES(std::vector<unsigned char>& msg, Response* response)
{
	response->AES = deserializeString(msg, response->payload_size - ID_SIZE);
}

void Deserializer::deserializePayload(std::vector<unsigned char>& msg, Response* response)
{
	response->payload = deserializeString(msg, response->payload_size);
}

int Deserializer::deserializeInteger(unsigned char num[])
{
	int res = 0;
	for (int i = 0; i < SIZE_SIZE; i++) {
		res |= num[i] << (BYTE_SIZE * i);
	}

	return res;
}

short Deserializer::deserializeShort(unsigned char num[])
{
	int res = 0;
	for (int i = 0; i < CODE_SIZE; i++) {
		res |= num[i] << (BYTE_SIZE * i);
	}

	return res;
}

std::string Deserializer::deserializeString(std::vector<unsigned char>& msg, int length)
{
	std::string str = "";

	for (int i = 0; i < length && msg.size() > 0; i++) {
		str += msg.at(0);
		msg.erase(msg.begin());
	}

	return str;
}
