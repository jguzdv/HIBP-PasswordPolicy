#include "HIBPPolicyEvaluator.h"
#include "Sha1.h"
#include <vector>
#include "Http.h"
#include <string.h>
#include <codecvt>

const std::wstring HIBP_SERVER_NAME = L"hibp.jgu.net";
const std::wstring HIBP_REQUEST_PATH = L"/range";
const int HIBP_PREFIX_LENGTH = 5;

HIBPPolicyEvaluator::HIBPPolicyEvaluator() {

}
HIBPPolicyEvaluator::~HIBPPolicyEvaluator() {

}

bool HIBPPolicyEvaluator::IsAcceptablePassword(const wchar_t* passwordInput) {
    std::string password = ConvertToStr(passwordInput);
    std::wstring passwordHash = CalculateHash(password);
    std::wstring hashBucket = GetHashBucket(passwordHash);

    std::wstring hashSuffix = passwordHash.substr(HIBP_PREFIX_LENGTH);
    if (hashBucket.find(hashSuffix) == std::wstring::npos)
        return true;
    else
        return false;
}

std::string HIBPPolicyEvaluator::ConvertToStr(const wchar_t* input)
{
    std::wstring inputWString = std::wstring(input);
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(inputWString);
}

std::wstring HIBPPolicyEvaluator::CalculateHash(std::string input)
{
    Sha1 sha1 = Sha1();
    return sha1.CalculateHashHexString(input.c_str());
}

std::wstring HIBPPolicyEvaluator::GetHashBucket(std::wstring input)
{
    Http http = Http(HIBP_SERVER_NAME);
    return http.LoadHashBucket(HIBP_REQUEST_PATH, input, HIBP_PREFIX_LENGTH);
}
