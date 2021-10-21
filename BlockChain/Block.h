#pragma once

#include "stdafx.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>


class Block
{
public:
    friend class boost::serialization::access;

    Block() {
        PrevBlockHash.resize(64);
        MerKleRoot.resize(64);
        Hash.resize(64);
        Difficulty.resize(64);
    }



    int64_t Version = 0;
    std::vector<unsigned char>  PrevBlockHash;
    std::vector<unsigned char>  MerKleRoot;
    int64_t TimeStamp = 0;
    std::vector<unsigned char>  Hash;
    std::vector<unsigned char>  Data;
    std::vector<unsigned char>  Difficulty;

    int64_t Nonce = 0;

    static void NewBlock(Block& block,std::string& data, std::vector<unsigned char>& tmp_PrevBlockHash);
    static std::vector<unsigned char> Sha256(std::string str);
    static void PrintBlock(Block* block);



    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& Version;
        ar& PrevBlockHash;
        ar& MerKleRoot;
        ar& TimeStamp;
        ar& Data;
        ar& Difficulty;
        ar& Nonce;
    }
 

};
