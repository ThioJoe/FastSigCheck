#include <windows.h>
#include <wintrust.h>
#include <softpub.h>
#include <iostream>

#pragma comment(lib, "wintrust.lib")

int VerifyFileSignature(LPCWSTR filePath)
{
    WINTRUST_FILE_INFO fileData = { 0 };
    fileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    fileData.pcwszFilePath = filePath;
    fileData.hFile = NULL;
    fileData.pgKnownSubject = NULL;

    GUID policyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;

    WINTRUST_DATA winTrustData = { 0 };
    winTrustData.cbStruct = sizeof(WINTRUST_DATA);
    winTrustData.pPolicyCallbackData = NULL;
    winTrustData.pSIPClientData = NULL;
    winTrustData.dwUIChoice = WTD_UI_NONE;
    winTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    winTrustData.dwUnionChoice = WTD_CHOICE_FILE;
    winTrustData.pFile = &fileData;
    winTrustData.dwStateAction = WTD_STATEACTION_IGNORE;
    winTrustData.hWVTStateData = NULL;
    winTrustData.pwszURLReference = NULL;
    winTrustData.dwProvFlags = WTD_CACHE_ONLY_URL_RETRIEVAL;

    LONG status = WinVerifyTrust(NULL, &policyGUID, &winTrustData);

    return status;
}

int wmain(int argc, wchar_t* argv[])
{
    if (argc != 2)
    {
        return 1; // Return an error code for incorrect usage
    }

    LPCWSTR filePath = argv[1];

    int result = VerifyFileSignature(filePath);

    // Map the result to an appropriate exit code
    switch (result)
    {
    case ERROR_SUCCESS:
        return 0; // Signature is valid
    case TRUST_E_NOSIGNATURE:
        return 2; // No signature is present
    case TRUST_E_BAD_DIGEST:
        return 3; // Signature is invalid
    default:
        return 4; // Some other error occurred
    }
}
