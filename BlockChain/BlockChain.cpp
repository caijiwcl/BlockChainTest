#include "stdafx.h"
#include "BlockChain.h"
#include "leveldb/db.h"

#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include "BlockIterator.h"

void BlockChain::testDB()
{
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "testdb", &db);
    assert(status.ok());

    std::string key = "id";
    std::string value = "object";
    if (status.ok())
    {
        db->Put(leveldb::WriteOptions(), key, value);
    }


    delete db;
}

BlockChain::BlockChain()
{

}

std::pair<BlockChain*, std::vector<unsigned char>>BlockChain::NewBlockChain()
{
    BlockChain* blockchain = new BlockChain();  //创建一个区块链对象

    //功能分析：
//1.打开数据库，写入数据

    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "testdb", &(blockchain->db));
    assert(status.ok());

    //2.写入创世块

    Block block;  //空块，使用局部变量，存储完就销毁，不进行new

    std::string str_lasthashvalue;
    status = blockchain->db->Get(leveldb::ReadOptions(), "LastHash", &str_lasthashvalue);


    if (!status.ok())  //没查询到就添加数据
    {

        std::string str = "hello world";
        std::vector<unsigned char> prevHash(64, '0');
        Block::NewBlock(block, str, prevHash);  //创世块初始化

        std::string str_hash;
        str_hash.assign(block.Hash.begin(), block.Hash.end());
        leveldb::Slice Hash = str_hash;

        std::stringstream binary_sstream;
        boost::archive::binary_oarchive binary_oa(binary_sstream);

        binary_oa << block;
        
        std::string bin_str = binary_sstream.str();

        leveldb::Slice slice_block = bin_str;

        status = blockchain->db->Put(leveldb::WriteOptions(), Hash, slice_block);

        //3.写入lasthash
        status = blockchain->db->Put(leveldb::WriteOptions(), "LastHash", Hash);
        
    }



    //没查询到就写入区块，不管查没查到都返回最后一个区块的hash

    return std::make_pair(blockchain,block.Hash);



   


}

BlockChain::~BlockChain()
{
    delete db;
}


void BlockChain::AddBlock( std::string data)
{
    std::string str_lasthashvalue;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), "LastHash", &str_lasthashvalue);
        
    std::vector<unsigned char> PreHash;
    PreHash.assign(str_lasthashvalue.begin(), str_lasthashvalue.end());

    Block block; //空block

    //新建区块
    Block::NewBlock(block,data, PreHash);
    
    std::stringstream binary_sstream;

    boost::archive::binary_oarchive binary_oa(binary_sstream);

    binary_oa << block;

    std::string str_hash;
    str_hash.assign(block.Hash.begin(), block.Hash.end());
    leveldb::Slice Hash = str_hash;

    std::string bin_block = binary_sstream.str();
    leveldb::Slice slice_block = bin_block;

    status = db->Put(leveldb::WriteOptions(), Hash, slice_block);

    //3.更新lasthash

    status = db->Put(leveldb::WriteOptions(), "LastHash", Hash);

    //blockchain.push_back(&block);
}

void BlockChain::PrintBlockChain()
{
    std::cout << "遍历区块:------------------" << std::endl;

    //倒着遍历

    //std::string str_lasthashvalue;
    //leveldb::Status status = db->Get(leveldb::ReadOptions(), "LastHash", &str_lasthashvalue);

    //if (status.ok())
    //{

    //    std::vector<unsigned char> GensisPreHash(64, '0');  //创世区块的PreHash

    //    std::vector<unsigned char> PreHash;
    //    //tail区块的value就是倒数第二个区块的hash，可以假设他就是最后一个区块的prehash

    //    PreHash.assign(str_lasthashvalue.begin(), str_lasthashvalue.end());

    //    while (PreHash != GensisPreHash)  //prehash只要不是创世区块的prehash就一直遍历
    //    {
    //        std::string str_prehash;
    //        str_prehash.assign(PreHash.begin(),PreHash.end());

    //        std::string blockvalue;

    //        status = db->Get(leveldb::ReadOptions(), leveldb::Slice(str_prehash), &blockvalue);

    //        std::stringstream binary_sstream(blockvalue);

    //        boost::archive::binary_iarchive binary_ia(binary_sstream);

    //        Block block;

    //        binary_ia >> block;

    //        PreHash = block.PrevBlockHash;

    //        Block::PrintBlock(&block);
    //    }
    //  

    //}

    BlockIterator iterblk(db);
    Block block;//空块，避免开辟空间，每次迭代都只是给块中数据赋值
    while (iterblk.Next(block) != false)
    {
        Block::PrintBlock(&block);
    }
    Block::PrintBlock(&block);
}