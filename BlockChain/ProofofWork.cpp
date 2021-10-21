#include "stdafx.h"
#include "ProofofWork.h"
#include "Block.h"
#include "UTools.h"
#include <conio.h>
#include <openssl/bn.h>




ProofofWork::ProofofWork(Block& bck):block(bck)
{

	//固定难度值
	//target.insert(target.end(), 64, '0');
	//target[3] = '1';//难度值为0x000100000.....

	//可调节难度值
	char target[64];
	memset(target,'0',sizeof(target));

	BIGNUM* initbignum = BN_new();
	BIGNUM* resultbignum = BN_new();

	int x = BN_hex2bn(&initbignum, "01");

	int i = BN_lshift(resultbignum, initbignum, 248);

	char* p = BN_bn2hex(resultbignum);
	if (p)
	{
		std::string str_target = BN_bn2hex(resultbignum);//返回的结果大小不是64位的

		if (str_target.size() < 64)
		{
			int completion = 64 - str_target.size();
			str_target.insert(str_target.begin(),completion,'0');
		}

		bck.Difficulty.assign(str_target.begin(),str_target.end());
		OPENSSL_free(p);
	}



	BN_free(initbignum);
	BN_free(resultbignum);

}

std::pair<int64_t, std::vector<unsigned char>> ProofofWork::Run()
{
	int64_t nonce = 0;
	while (1)
	{
		std::vector<unsigned char> currentHash = Block::Sha256(this->PrepareData(nonce));
		std::string str_hash;
		str_hash.assign(currentHash.begin(), currentHash.end());
		
		std::cout << str_hash;
		std::cout << "\r";
		if (UTOOLS::CompareByte(currentHash, block.Difficulty) == 0)
		{
			block.Nonce = nonce;
			std::pair<int64_t, std::vector<unsigned char>> retpair = std::make_pair(nonce, currentHash);
			return retpair;
		}
		else
		{
			nonce++;
		}


	}

}

std::string ProofofWork::PrepareData(int64_t Nonce)
{


	std::vector<unsigned char> ver_bytes = UTOOLS::int64to_bytes(block.Version);

	std::vector<unsigned char> all_data;
	all_data.insert(all_data.end(), ver_bytes.begin(), ver_bytes.begin() + ver_bytes.size());

	all_data.insert(all_data.end(), block.PrevBlockHash.begin(), block.PrevBlockHash.begin() + block.PrevBlockHash.size());


	int64_t now = block.TimeStamp;
	std::vector<unsigned char> time_bytes = UTOOLS::int64to_bytes(block.TimeStamp);

	all_data.insert(all_data.end(), time_bytes.begin(), time_bytes.begin() + time_bytes.size());

	all_data.insert(all_data.end(), block.Difficulty.begin(), block.Difficulty.begin() + block.Difficulty.size());

	all_data.insert(all_data.end(), block.Data.begin(), block.Data.begin() + block.Data.size());

	std::vector<unsigned char> nonce_bytes = UTOOLS::int64to_bytes(Nonce);

	all_data.insert(all_data.end(), nonce_bytes.begin(), nonce_bytes.begin() + nonce_bytes.size());

	std::string tmp;
	tmp.assign(all_data.begin(), all_data.end());

	return tmp;

	//std::vector<unsigned char> hash = Block::Sha256(tmp);

	//block->Hash = hash;
}

bool ProofofWork::isValid()
{
	std::vector<unsigned char> isValidHash = Block::Sha256(this->PrepareData(block.Nonce));


	if (UTOOLS::CompareByte(isValidHash, block.Difficulty) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}