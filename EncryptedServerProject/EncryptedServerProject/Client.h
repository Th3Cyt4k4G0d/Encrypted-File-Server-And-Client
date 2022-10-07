#pragma once
#include "Helper.h"
#include <boost/asio.hpp>
#include "RSAWrapper.h"
#include "Serializer.h"
#include "Base64Wrapper.h"

using boost::asio::ip::tcp;

void client(const std::string ip, const std::string port, std::string username, const std::string file_name);
void read_handler(const boost::system::error_code& error, std::size_t b);
void handle_register(tcp::socket& s, Request* req, Response* resp);
void send_pub_key(tcp::socket& s, Request* req, std::string id, std::string pub_key);
void get_aes(tcp::socket& s, Response* resp, std::string privkey);
void prepare_third_request(Request* req, int file_length_after_encryption, std::string file_name);