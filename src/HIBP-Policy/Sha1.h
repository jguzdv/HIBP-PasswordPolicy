#pragma once

#include <Windows.h>
#include <bcrypt.h>
#include <string>

class Sha1
{
private:
    BCRYPT_HASH_HANDLE _hashHandle = NULL;

    unsigned long cbData = 0;
    long _hashLength = 0;
    long _hashObjectLength = 0;
    
    unsigned char* _hashObject = NULL;
    unsigned char* _hashResult = NULL;

    std::wstring ResultToHexString();

public:
    Sha1();
    ~Sha1();

    std::wstring CalculateHashHexString(std::string inputText);
};

