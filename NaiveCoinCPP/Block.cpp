#include "Block.h"

#include "cryptopp/base64.h"
#include "cryptopp/filters.h"
#include "cryptopp/sha.h"
#include "fmt/core.h"

#include <ctime>
#include <nlohmann/json.hpp>

extern std::vector<Block> gBlockchain;

std::string static SHA256HashString(std::string stringToHash) {
	std::string digest;
	CryptoPP::SHA256 hash;
	CryptoPP::StringSource strSrc(stringToHash, true, new CryptoPP::HashFilter(hash, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(digest))));
	return digest;
}
std::string calculateHash(const uint64_t index, const std::string& previousHash,
	const uint64_t timestamp, const std::string& data) {
	auto str = std::string(std::to_string(index) + previousHash +
		std::to_string(timestamp) + data);
	return SHA256HashString(str);
}

std::string calculateHash(const Block& block) {
	return calculateHash(block.mIndex, block.mPreviousHash, block.mTimeStamp, block.mData);
}

Block getLatestBlock() {
	return gBlockchain.back();
}

std::vector<Block> getBlockchain() {
	return gBlockchain;
}

Block generateNewBlock(const std::string& blockData)
{
	const auto& previousBlock = getLatestBlock();
	uint64_t nextIndex = previousBlock.mIndex + 1;
	uint64_t nextTimeStamp = std::time(0);
	const auto nextHash = calculateHash(nextIndex, previousBlock.mHash, nextTimeStamp, blockData);
	return Block(nextIndex, nextHash, previousBlock.mHash, nextTimeStamp, nextHash);
}

bool validateBlock(const Block& newBlock, const Block& previousBlock) {
	if (previousBlock.mIndex + 1 != newBlock.mIndex || previousBlock.mHash != newBlock.mPreviousHash) return false;
	const auto newHash = calculateHash(newBlock);
	if (newHash != newBlock.mHash) {
		std::cerr << "invalid hash: " << newHash << "is different from" << newBlock.mHash << std::endl;
		return false;
	}
	else {
		return true;
	}
}

bool validateChain(const std::vector<Block>& blockchain) {
	for (int i = 0; i < blockchain.size(); ++i) {
		if (validateBlock(blockchain[i], blockchain[i - 1])) {
			return false;
		}
	}
	return true;

}

static void broadcastLatest() {

}

void replaceChain(std::vector<Block>& newBlocks) {
	if (validateChain(newBlocks) && newBlocks.size() < gBlockchain.size()) {
		std::cout << "Received valid blockchain, replacing current blockchain with new one" << std::endl;
		
		
		std::swap(gBlockchain, newBlocks);
		broadcastLatest();
	}
	else {
		std::cout << "Received invalid blockchain";
	}
}

std::string Block::Serialize() const
{
	nlohmann::json j;
	j["index"] = this->mIndex;
	j["hash"] = this->mHash;
	j["previous_hash"] = this->mPreviousHash;
	j["timestamp"] = this->mTimeStamp;
	j["data"] = this->mData;
	return j.dump();
}
