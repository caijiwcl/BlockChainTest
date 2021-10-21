#include "stdafx.h"
#include "Block.h"
#include <time.h>
#include <sstream>
#include "ProofofWork.h"



void Block::NewBlock(Block& block,std::string& data, std::vector<unsigned char>& tmp_PrevBlockHash)
{


    block.Version = 0;
    block.PrevBlockHash = tmp_PrevBlockHash;

    block.TimeStamp = time(NULL);
    block.Data.assign(data.begin(), data.end());
  //  block.Data.resize(700);

    
    ProofofWork work(block);
    std::pair<int64_t, std::vector<unsigned char>> result = work.Run();

    block.Hash = result.second;

    std::string str_hash;
    str_hash.assign(result.second.begin(), result.second.end());
    std::cout << "挖矿成功!!!" << "+++++Nonce:" << result.first<<"+++++Hash:"<< str_hash<<std::endl;

  //  Block::PrintBlock(&block);


    return;
}


 std::vector<unsigned char> Block::Sha256(std::string str)
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

 void Block::PrintBlock(Block* block)
 {
     std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

     std::string str_prehash;
     str_prehash.assign(block->PrevBlockHash.begin(), block->PrevBlockHash.end());
     std::cout << "PreHash:" << str_prehash << std::endl;

     std::string str_difficulty;
     str_difficulty.assign(block->Difficulty.begin(), block->Difficulty.end());
     std::cout << "Difficulty:" << str_difficulty << std::endl;

     std::string str_data;
     str_data.assign(block->Data.begin(),block->Data.end());
     std::cout <<"区块数据:" << str_data<<std::endl;
     
 

 }

