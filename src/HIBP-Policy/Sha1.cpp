#include "Sha1.h"
#include <bcrypt.h>
#include <codecvt>

Sha1::Sha1()
{
    long bCryptStatus;

    bCryptStatus = BCryptGetProperty(
        BCRYPT_SHA1_ALG_HANDLE, BCRYPT_OBJECT_LENGTH,
        (unsigned char*)&_hashObjectLength, sizeof(unsigned long),
        &cbData, 0);

    if (bCryptStatus < 0)
        throw std::exception("BCrypt: NTStatus of GetProperty(Object_Length) was -lt 0");

    bCryptStatus = BCryptGetProperty(
        BCRYPT_SHA1_ALG_HANDLE, BCRYPT_HASH_LENGTH,
        (unsigned char*)&_hashLength, sizeof(unsigned long),
        &cbData, 0);

    if (bCryptStatus < 0)
        throw std::exception("BCrypt: NTStatus of GetProperty(BCRYPT_HASH_LENGTH) was -lt 0");

    _hashObject = (unsigned char*)HeapAlloc(GetProcessHeap(), 0, _hashObjectLength);
    _hashResult = (unsigned char*)HeapAlloc(GetProcessHeap(), 0, _hashLength);

    bCryptStatus = BCryptCreateHash(
        BCRYPT_SHA1_ALG_HANDLE, &_hashHandle,
        _hashObject, _hashObjectLength,
        NULL, 0, 0);

    if (bCryptStatus < 0)
        throw std::exception("BCrypt: NTStatus of CreateHash was -lt 0");
}

Sha1::~Sha1()
{
    if (_hashHandle)
        BCryptDestroyHash(_hashHandle);

    if (_hashObject)
        HeapFree(GetProcessHeap(), 0, _hashObject);
    if (_hashResult)
        HeapFree(GetProcessHeap(), 0, _hashResult);
}

constexpr char hexmap[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

std::wstring Sha1::ResultToHexString() {
    std::string resultString = std::string(_hashLength * 2, ' ');
    for (int i = 0; i < _hashLength; ++i) {
        resultString[2 * i] = hexmap[(_hashResult[i] & 0xF0) >> 4];
        resultString[2 * i + 1] = hexmap[(_hashResult[i] & 0x0F)];
    }

    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(resultString);
}

std::wstring Sha1::CalculateHashHexString(const std::wstring& inputText)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8Converter;
    std::string utf8String = utf8Converter.to_bytes(inputText);

    long bCryptStatus;
    bCryptStatus = BCryptHashData(
        _hashHandle,
        (unsigned char*)utf8String.c_str(),
        utf8String.length(),
        0);

    if (bCryptStatus < 0)
        throw std::exception("BCrypt: NTStatus of HashData was -lt 0");

    bCryptStatus = BCryptFinishHash(
        _hashHandle,
        _hashResult,
        _hashLength,
        0);

    if (bCryptStatus < 0)
        throw std::exception("BCrypt: NTStatus of FinishHash was -lt 0");

    return ResultToHexString();
}
