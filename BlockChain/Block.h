#pragma once

#include "stdafx.h"
#include <time.h>
#include <sstream>
class Block
{
public:
    Block() {
        PrevBlockHash.resize(64);
        MerKleRoot.resize(64);
        Hash.resize(64);
    }

    std::string Version;
    std::vector<unsigned char>  PrevBlockHash;
    std::vector<unsigned char>  MerKleRoot;
    time_t TimeStamp;
    std::vector<unsigned char>  Hash;
    std::vector<unsigned char>  Data;

    static Block* NewBlock(std::string& data, std::vector<unsigned char>& tmp_PrevBlockHash)
    {



        Block* newbk = new Block();
        newbk->Version = "0";
        newbk->PrevBlockHash = tmp_PrevBlockHash;

        newbk->TimeStamp = time(NULL);        
        newbk->Data.assign(data.begin(), data.end());
 
        Block::SetHash(newbk);
        return newbk;
    }

    static void SetHash(Block* block)
    {
        if (block == nullptr)
            return;
        std::vector<unsigned char> all_data;
        all_data.insert(all_data.end(), block->Version.begin(), block->Version.begin() + block->Version.size());
        all_data.insert(all_data.end(), block->PrevBlockHash.begin(), block->PrevBlockHash.begin() + block->PrevBlockHash.size());
        
        std::stringstream ss;
        std::string str_time;
        long long now = block->TimeStamp;
        ss << now;
        ss >> str_time;
        all_data.insert(all_data.end(), str_time.begin(), str_time.begin() + str_time.size());
        
        all_data.insert(all_data.end(), block->Data.begin(), block->Data.begin() + block->Data.size());
        
              
        std::string tmp;
        tmp.assign(all_data.begin(), all_data.end());

        std::vector<unsigned char> hash = Block::Sha256(tmp);

        block->Hash = hash;

    }

    static std::vector<unsigned char> Sha256(std::string str)
    {
        char buf[3];
        unsigned char hash[32] = { 0 };
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);

        std::string NewString = "";
        for (int i = 0; i < 32; i++)
        {
            int tmp = hash[i];
            sprintf_s(buf, sizeof(buf), "%02x", tmp);

            NewString = NewString.append(buf);
        }

        std::vector<unsigned char> tmp;
        tmp.assign(NewString.begin(), NewString.end());
        return tmp;
    }
};
