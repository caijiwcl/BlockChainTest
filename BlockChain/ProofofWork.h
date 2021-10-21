#pragma once
#include "stdafx.h"

class Block;

class ProofofWork
{
public:
	Block& block;
	std::vector<unsigned char> target; //ÄÑ¶ÈÖµ

	ProofofWork(Block& bck);


	std::pair<int64_t, std::vector<unsigned char>> Run();
	

    std::string PrepareData(int64_t Nonce);
	
	bool isValid();

};