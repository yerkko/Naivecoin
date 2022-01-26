// NaiveCoinCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>

#include "Block.h"
#include <vector>
#include "HTTPServer.h"

std::vector<Block> gBlockchain;

void initializeHTTPServer(const unsigned int port) {
	auto const& server = HTTPServer(port);
}

int main()
{
	
  std::cout << "Hello World!\n";
  auto const &block = Block(
        0, "816534932c2b7154836da6afc367695e6337db8a921823784c14378abed4f7d7",
        "", 1465154705, "whatever dude");
  gBlockchain.push_back(block);

  std::thread t1(initializeHTTPServer, 80);

  //auto const& server = HTTPServer(80);

  
  std::cout << block.mData;
  std::cout << getLatestBlock().mData;

  t1.join();




}