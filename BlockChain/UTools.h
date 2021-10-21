#pragma once

#include "stdafx.h"
#include <cctype>

namespace UTOOLS
{
    std::vector<unsigned char>  int64to_bytes(int64_t num)
    {
        std::vector<unsigned char> bytes;

        unsigned char* cc = (unsigned char*)&num;


        for (int i = 0; i < 8; i++)
        {
            bytes.push_back(*(cc + i));
        }

        return bytes;
    }

    int64_t bytesto_int64(std::vector<unsigned char> bytes /*size must be 8*/)
    {
        if (bytes.size() != 8)
            return 0;

        //turn bytes array to integer
        int64_t num = bytes[0] & 0xFF;
        num |= (((int64_t)bytes[1] << 8) & 0xFF00);
        num |= (((int64_t)bytes[2] << 16) & 0xFF0000);
        num |= (((int64_t)bytes[3] << 24) & 0xFF000000);
        num |= (((int64_t)bytes[4] << 32) & 0xFF00000000);
        num |= (((int64_t)bytes[5] << 40) & 0xFF0000000000);
        num |= (((int64_t)bytes[6] << 48) & 0xFF000000000000);
        num |= (((int64_t)bytes[7] << 56) & 0xFF00000000000000);
        return num;
    }

    int CompareByte(std::vector<unsigned char> Hash1, std::vector<unsigned char> Hash2)
    {
        if (Hash1.size() != 64 || Hash2.size() != 64)
            return -1;  //不满足Hash大小
        
        for (int i = 0; i <Hash1.size(); i++)
        {
            Hash1[i] = tolower(Hash1[i]);
            Hash2[i] = tolower(Hash2[i]);

            if (Hash1[i] < Hash2[i])
            {
                return 0;  //Hash1小于Hash2
            }
            else if (Hash1[i] > Hash2[i])
            {
                return 1; //Hash1大于Hash2
            }
            else
            {
                continue;
            }

        }
        return 3;    //Hash1等于Hash2

    }
}
