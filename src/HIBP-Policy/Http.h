#pragma once

#include <Windows.h>
#include <winhttp.h>
#include <string>
#include <vector>

class Http
{
public:
    Http(const std::wstring& serverName);
    ~Http();

    std::wstring LoadHashBucket(const std::wstring& requestPath, const std::wstring& hexHash, int prefixLength);

private:
    HINTERNET _hSession;
    HINTERNET _hConnect;

    std::string ReadResult(HINTERNET hRequest);
};

