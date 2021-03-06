#include <Windows.h>
#include <SubAuth.h>
#include "HIBPPolicyEvaluator.h"

extern "C" __declspec(dllexport) unsigned char __stdcall PasswordFilter(
        PUNICODE_STRING accountName,
        PUNICODE_STRING fullName,
        PUNICODE_STRING password,
        unsigned char operation) {

    bool allowPasswordChange = true;

    try {
        HIBPPolicyEvaluator evaluator = HIBPPolicyEvaluator();

        allowPasswordChange = evaluator.IsAcceptablePassword(std::wstring(password->Buffer, password->Length / sizeof(wchar_t)));
    }
    catch(...) {
        // Catch any errors.
    }
    
    return allowPasswordChange;
}
