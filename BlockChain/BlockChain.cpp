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
    BlockChain* blockchain = new BlockChain();  //����һ������������

    //���ܷ�����
//1.�����ݿ⣬д������

    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "testdb", &(blockchain->db));
    assert(status.ok());

    //2.д�봴����

    Block block;  //�տ飬ʹ�þֲ��������洢������٣�������new

    std::string str_lasthashvalue;
    status = blockchain->db->Get(leveldb::ReadOptions(), "LastHash", &str_lasthashvalue);


    if (!status.ok())  //û��ѯ�����������
    {

        std::string str = "hello world";
        std::vector<unsigned char> prevHash(64, '0');
        Block::NewBlock(block, str, prevHash);  //�������ʼ��

        std::string str_hash;
        str_hash.assign(block.Hash.begin(), block.Hash.end());
        leveldb::Slice Hash = str_hash;

        std::stringstream binary_sstream;
        boost::archive::binary_oarchive binary_oa(binary_sstream);

        binary_oa << block;
        
        std::string bin_str = binary_sstream.str();

        leveldb::Slice slice_block = bin_str;

        status = blockchain->db->Put(leveldb::WriteOptions(), Hash, slice_block);

        //3.д��lasthash
        status = blockchain->db->Put(leveldb::WriteOptions(), "LastHash", Hash);
        
    }



    //û��ѯ����д�����飬���ܲ�û�鵽���������һ�������hash

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

    Block block; //��block

    //�½�����
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

    //3.����lasthash

    status = db->Put(leveldb::WriteOptions(), "LastHash", Hash);

    //blockchain.push_back(&block);
}

void BlockChain::PrintBlockChain()
{
    std::cout << "��������:------------------" << std::endl;

    //���ű���

    //std::string str_lasthashvalue;
    //leveldb::Status status = db->Get(leveldb::ReadOptions(), "LastHash", &str_lasthashvalue);

    //if (status.ok())
    //{

    //    std::vector<unsigned char> GensisPreHash(64, '0');  //���������PreHash

    //    std::vector<unsigned char> PreHash;
    //    //tail�����value���ǵ����ڶ��������hash�����Լ������������һ�������prehash

    //    PreHash.assign(str_lasthashvalue.begin(), str_lasthashvalue.end());

    //    while (PreHash != GensisPreHash)  //prehashֻҪ���Ǵ��������prehash��һֱ����
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
    Block block;//�տ飬���⿪�ٿռ䣬ÿ�ε�����ֻ�Ǹ��������ݸ�ֵ
    while (iterblk.Next(block) != false)
    {
        Block::PrintBlock(&block);
    }
    Block::PrintBlock(&block);
}