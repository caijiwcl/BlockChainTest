#pragma once
#include "stdafx.h"
class Block;
namespace leveldb
{
    class DB;
}


class BlockIterator
{
public:
    std::vector<unsigned char> currentPreHash;   //当前所指向的prehash
    leveldb::DB* mydb;

    BlockIterator(leveldb::DB* db);

    bool Next(Block& block); //传入一个空块，每次迭代只给空块赋值，避免再开辟空间或生成临时变量

};

