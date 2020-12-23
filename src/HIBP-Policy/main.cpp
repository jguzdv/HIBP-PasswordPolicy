#if _DEBUG

#include <stdio.h>
#include "HIBPPolicyEvaluator.h"

int main(int argc, const char* argv[]) {

    HIBPPolicyEvaluator evaluator = HIBPPolicyEvaluator();

    std::wstring password = std::wstring(L"PasswordPassword123");
    bool allowPasswordChange = evaluator.IsAcceptablePassword(password.c_str());

    wprintf((password + L" => %d").c_str(), allowPasswordChange);
};

#endif
