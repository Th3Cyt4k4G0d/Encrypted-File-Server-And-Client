#include "Client.h"
#include "Deserializer.h"

using boost::asio::ip::tcp;

void client(const std::string ip, const std::string port, std::string username, const std::string filename) {
	Request* req = new Request();
	Response* resp = new Response();
	std::vector<unsigned char> request;
	boost::asio::io_context io_context;
	tcp::socket s(io_context);
	tcp::resolver resolver(io_context);

	boost::asio::connect(s, resolver.resolve(ip, port)); // connect to server.

	std::string pubkey, id, privkey;
	if (file_exists(ME_FILE)) {
		handle_me_file(username, id, privkey);
		privkey = Base64Wrapper::decode(privkey);
		RSAPrivateWrapper keys(privkey);
		pubkey = keys.getPublicKey();

		privkey = Base64Wrapper::encode(privkey);
	}
	else {
		handle_register(s, req, resp);
		RSAPrivateWrapper keys;
		privkey = keys.getPrivateKey();
		pubkey = keys.getPublicKey();

		privkey = Base64Wrapper::encode(privkey);
		create_me_file(username, resp->id, privkey); // create a "me file" with the new id
	}
	

	send_pub_key(s, req, id, pubkey);

	get_aes(s, resp, privkey);
	
	int file_length_after_encryption = 2;
	std::string file_name = "";
	prepare_third_request(req, file_length_after_encryption, file_name);

	int blocks = file_length_after_encryption / BLOCK_SIZE;
	int reminder = file_length_after_encryption % BLOCK_SIZE;


	std::ifstream fin(file_name, std::ifstream::binary);
	char* buffer = new char[BLOCK_SIZE];
	while (!fin.eof())
	{
		fin.read(buffer, BLOCK_SIZE);
		std::streamsize size = fin.gcount();
		std::cout << buffer << std::endl;
		//serialize_third_payload(req, size, buffer);
		request = Serializer::SerializeRequest(req); // send public key
		boost::asio::write(s, boost::asio::buffer(request, request.size()));
	}
}

void handle_register(tcp::socket &s, Request* req, Response* resp) {
	std::string id1 = "AAAAAAAAAAAAAAAB";
	std::string name = "1111111112111111111222";
	req->client_id = id1;
	req->name = name;
	req->name_size = name.size();
	req->code = 1100;
	req->version = '3';
	req->payload_size = name.size();

	/* Register and get response */
	std::vector<unsigned char> request = Serializer::SerializeRequest(req); // register
	boost::asio::write(s, boost::asio::buffer(request, request.size()));

	std::vector<unsigned char> responseHeader(HEADER_SIZE);
	boost::asio::read(s, boost::asio::buffer(responseHeader, HEADER_SIZE));

	/* Get ID from response */
	resp = Deserializer::deserializeHeader(responseHeader);
	std::vector<unsigned char> responsePayload(resp->payload_size);
	boost::asio::read(s, boost::asio::buffer(responsePayload, resp->payload_size));
	Deserializer::deserializePayloads(resp, responsePayload);

	std::cout << "\n" << resp->id << std::endl;
}

void send_pub_key(tcp::socket& s, Request* req, std::string id, std::string pubkey) {
	req->client_id = id; // update the id
	req->payload_size += pubkey.size();
	req->public_key = pubkey;
	req->code = PUBLIC_KEY;

	std::vector<unsigned char> request = Serializer::SerializeRequest(req); // send public key
	boost::asio::write(s, boost::asio::buffer(request, request.size()));
}

void get_aes(tcp::socket& s, Response* resp, std::string privkey) {
	std::vector<unsigned char> responseHeader(HEADER_SIZE);

	boost::asio::read(s, boost::asio::buffer(responseHeader, HEADER_SIZE)); // get answer.
	resp = Deserializer::deserializeHeader(responseHeader); // deserialize answer header.

	std::vector<unsigned char> responsePayload(resp->payload_size);
	boost::asio::read(s, boost::asio::buffer(responsePayload, resp->payload_size)); // read payload.
	Deserializer::deserializePayloads(resp, responsePayload); // get AES key.

	RSAPrivateWrapper rsapriv_other(Base64Wrapper::decode(privkey));
	resp->AES = rsapriv_other.decrypt(resp->AES); // decrypt AES key with privkey.
}

void serialize_third_payload(Request* req, int size, std::string buffer) {
	req->content_size = size;

	// req->message_conten
}

void prepare_third_request(Request* req, int file_length_after_encryption, std::string file_name)
{
	req->payload_size = ID_SIZE + SIZE_SIZE + file_name.size() + file_length_after_encryption;

	req->file_name = file_name;
}

void read_handler(const boost::system::error_code& error, std::size_t b)
{
	std::cout << "b" << std::endl;
	return;
}

void register_user(const std::string username, std::string& id, std::string& key) {
	return;
}

int main(int argc, char* argv[]) {
	std::string port, ip, username, file_name;

	handle_tranfer_file(ip, port, username, file_name);
	client(ip, port, username, file_name);
}

