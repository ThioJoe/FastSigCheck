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

    #ifdef _DEBUG
    std::wcout << L"Running WinVerifyTrust on file: " << filePath << std::endl;
    #endif

    LONG status = WinVerifyTrust(NULL, &policyGUID, &winTrustData);

    #ifdef _DEBUG
    std::wcout << L"WinVerifyTrust returned: " << std::hex << status << std::endl;
    #endif

    return status;
}

int wmain(int argc, wchar_t* argv[])
{
    if (argc != 2)
    {
        std::wcerr << L"Usage: VerifySignature.exe <file-path>" << std::endl;
        return -1;
    }

    LPCWSTR filePath = argv[1];

    #ifdef _DEBUG
    std::wcout << L"Received file path: " << filePath << std::endl;
    #endif

    int result = VerifyFileSignature(filePath);

    #ifdef _DEBUG
    std::wcout << L"VerifyFileSignature result: " << result << std::endl;
    #endif

    switch (result)
    {
    case ERROR_SUCCESS:
        std::wcout << L"The file is signed and the signature is valid." << std::endl;
        return 0;
    case TRUST_E_NOSIGNATURE:
        std::wcout << L"The file is not signed." << std::endl;
        return 1;
    case TRUST_E_BAD_DIGEST:
        std::wcout << L"The file's signature is invalid." << std::endl;
        return 2;
    default:
        std::wcout << L"An error occurred: " << std::hex << result << std::endl;
        return -1;
    }
}
