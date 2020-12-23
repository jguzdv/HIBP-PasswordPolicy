#pragma once
#include <string>
#include <vector>

class HIBPPolicyEvaluator
{
public:
    HIBPPolicyEvaluator();
    ~HIBPPolicyEvaluator();

    bool IsAcceptablePassword(const wchar_t* password);

private:
    std::string ConvertToStr(const wchar_t* input);
    std::wstring CalculateHash(std::string input);
    std::wstring GetHashBucket(std::wstring input);
};

