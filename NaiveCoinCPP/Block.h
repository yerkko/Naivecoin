#pragma once

#include <cinttypes>
#include <string>
#include <vector>

struct Block
{
	uint64_t mIndex;
	const std::string mHash;
	const std::string mPreviousHash;
	uint64_t mTimeStamp;
	std::string mData;

	Block(uint64_t index, const std::string &hash,
		  const std::string &previousHash, uint64_t timestamp,
		  const std::string &data)
		: mIndex{index}, mHash{hash}, mPreviousHash{previousHash},
		  mTimeStamp{timestamp}, mData{data} {}

	// copy constructor
	Block(const Block &other)
		: mIndex{other.mIndex}, mHash{other.mHash},
		  mPreviousHash{other.mPreviousHash}, mTimeStamp{other.mTimeStamp},
		  mData{other.mData} {}

	std::string Serialize() const;
};

std::string calculateHash(uint64_t index, std::string &previousHash,
						  uint64_t timestamp, std::string &data);

std::string calculateHash(const Block &block);

Block getLatestBlock();

Block generateNewBlock(const std::string &blockData);

bool validateBlock(const Block &newBlock, const Block &previousBlock);

void replaceChain(std::vector<Block> &newBlocks);

std::vector<Block> getBlockchain();