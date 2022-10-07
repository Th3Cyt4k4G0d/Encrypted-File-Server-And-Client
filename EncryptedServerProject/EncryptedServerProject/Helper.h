#pragma once

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include "Request.h"
#include "Response.h"
#include <fstream>

const unsigned short CODE_SIZE = 2;
const unsigned short VERSION_SIZE = 1;
const unsigned short ID_SIZE = 16;
const unsigned short SIZE_SIZE = 4;
const unsigned short BYTE_SIZE = 8;
const unsigned short HEADER_SIZE = 7;
const unsigned short KEY_SIZE = 160;
const unsigned short BLOCK_SIZE = 1024;

#define TRANSFER_FILE "C:\\Users\\yoava\\Desktop\\Maman15\\Maman15\\Client\\EncryptedServerProject\\Debug\\transfer.info"
#define ME_FILE "C:\\Users\\yoava\\Desktop\\Maman15\\Maman15\\Client\\EncryptedServerProject\\Debug\\me.info"

const std::string get_ip(const std::string line);
std::string get_port(std::string line);
void handle_tranfer_file(std::string& ip, std::string& port, std::string& username, std::string& path);
void handle_me_file(std::string& username, std::string& id, std::string& key);
bool file_exists(const std::string file);
void create_me_file(const std::string username, const std::string& id, const std::string& key);