#pragma once

#include "Block.h"

class BlockChain
{
private:
    std::vector<Block*> blockchain;

public:

    BlockChain()
    {
        //构建创世区块
        std::string str = "hello world";
        std::vector<unsigned char> prevHash(64, '0');
        Block* genesis = Block::NewBlock(str, prevHash);
        blockchain.push_back(genesis);
    }

    ~BlockChain()
    {
        for (auto& oneblock : blockchain)
        {
            delete oneblock;
        }
        blockchain.clear();
    }

    void AddBlock(std::string& data, std::vector<unsigned char>& tmp_PrevBlockHash)
    {
        Block* newbck = Block::NewBlock(data, tmp_PrevBlockHash);
        blockchain.push_back(newbck);
    }

    void PrintBlockChain()
    {
        int i = 0;
        for (auto& oneblock : blockchain)
        {

            std::cout << "BlockNum:" << i << std::endl;

            std::string prehash;
            prehash.assign(oneblock->PrevBlockHash.begin(), oneblock->PrevBlockHash.end());
            std::cout << "PreHash:" << prehash << std::endl;

            std::string hash;
            hash.assign(oneblock->Hash.begin(), oneblock->Hash.end());
            std::cout << "Hash:" << hash << std::endl;

            std::cout << "TimeStamp:" << oneblock->TimeStamp<<std::endl;

            i++;
        }
    }

};
