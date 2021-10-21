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
    std::vector<unsigned char> currentPreHash;   //��ǰ��ָ���prehash
    leveldb::DB* mydb;

    BlockIterator(leveldb::DB* db);

    bool Next(Block& block); //����һ���տ飬ÿ�ε���ֻ���տ鸳ֵ�������ٿ��ٿռ��������ʱ����

};

