
#include "stdafx.h"
#include "BlockIterator.h"
#include "Block.h"
#include "leveldb/db.h"

#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

BlockIterator::BlockIterator(leveldb::DB* db)
{
    mydb = db;

    std::string str_lasthashvalue;
    leveldb::Status status = mydb->Get(leveldb::ReadOptions(), "LastHash", &str_lasthashvalue);

    if (status.ok())
    {

        currentPreHash.assign(str_lasthashvalue.begin(), str_lasthashvalue.end());
    }
}

bool BlockIterator::Next(Block& block)
{
    std::vector<unsigned char> GensisPreHash(64, '0');  //创世区块的PreHash



    std::string str_prehash;
    str_prehash.assign(currentPreHash.begin(), currentPreHash.end());

    std::string blockvalue;

    leveldb::Status status = mydb->Get(leveldb::ReadOptions(), leveldb::Slice(str_prehash), &blockvalue);

    std::stringstream binary_sstream(blockvalue);

    boost::archive::binary_iarchive binary_ia(binary_sstream);


    binary_ia >> block;

    currentPreHash = block.PrevBlockHash;

    if (currentPreHash == GensisPreHash)
        return false;

    return true;
 
    


}