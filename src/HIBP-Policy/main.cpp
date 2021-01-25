#if _DEBUG

#include <stdio.h>
#include "HIBPPolicyEvaluator.h"

int main(int argc, const char* argv[]) {

    HIBPPolicyEvaluator evaluator = HIBPPolicyEvaluator();

    try {
        std::wstring password = std::wstring(L"Password123.");
        bool allowPasswordChange = evaluator.IsAcceptablePassword(password);

        wprintf((password + L" => %d").c_str(), allowPasswordChange);
    }
    catch (std::exception e) {
        wprintf(L"Exception occured:\n");
        printf(e.what());
    }
};

#endif
