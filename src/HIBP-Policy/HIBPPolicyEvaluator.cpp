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

bool HIBPPolicyEvaluator::IsAcceptablePassword(const std::wstring& passwordInput) {
    std::wstring passwordHash = CalculateHash(passwordInput);
    std::wstring hashBucket = GetHashBucket(passwordHash);

    std::wstring hashSuffix = passwordHash.substr(HIBP_PREFIX_LENGTH);
    if (hashBucket.find(hashSuffix) == std::wstring::npos)
        return true;
    else
        return false;
}

std::wstring HIBPPolicyEvaluator::CalculateHash(const std::wstring& input)
{
    Sha1 sha1 = Sha1();
    return sha1.CalculateHashHexString(input.c_str());
}

std::wstring HIBPPolicyEvaluator::GetHashBucket(const std::wstring& input)
{
    Http http = Http(HIBP_SERVER_NAME);
    return http.LoadHashBucket(HIBP_REQUEST_PATH, input, HIBP_PREFIX_LENGTH);
}
