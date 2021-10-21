#pragma once

#include "Block.h"

namespace leveldb 
{
    class DB;
}

class BlockChain
{

public:

    BlockChain();


    ~BlockChain();



    void PrintBlockChain();
    
    static void testDB();

    static std::pair<BlockChain*,std::vector<unsigned char>> NewBlockChain();
    void AddBlock(std::string data);

    leveldb::DB* db;

};
