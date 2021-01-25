#pragma once
#include <string>
#include <vector>

class HIBPPolicyEvaluator
{
public:
    HIBPPolicyEvaluator();
    ~HIBPPolicyEvaluator();

    bool IsAcceptablePassword(const std::wstring& password);

private:
    std::wstring CalculateHash(const std::wstring& input);
    std::wstring GetHashBucket(const std::wstring& input);
};

