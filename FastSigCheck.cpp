#include <windows.h>
#include <wintrust.h>
#include <softpub.h>
#include <iostream>

#pragma comment(lib, "wintrust.lib")

int VerifyFileSignature(LPCWSTR filePath, bool debug)
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

    if (debug)
    {
        std::wcout << L"Running WinVerifyTrust on file: " << filePath << std::endl;
    }

    LONG status = WinVerifyTrust(NULL, &policyGUID, &winTrustData);

    if (debug)
    {
        std::wcout << L"WinVerifyTrust returned: " << std::hex << status << std::endl;
    }

    return status;
}

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 2 || argc > 3)
    {
        std::wcerr << L"Usage: VerifySignature.exe <file-path> [--debug]" << std::endl;
        return -1;
    }

    LPCWSTR filePath = argv[1];
    bool debug = (argc == 3 && wcscmp(argv[2], L"--debug") == 0);

    if (debug)
    {
        std::wcout << L"Received file path: " << filePath << std::endl;
    }

    int result = VerifyFileSignature(filePath, debug);

    if (debug)
    {
        std::wcout << L"VerifyFileSignature result: " << result << std::endl;
    }

    switch (result)
    {
    case ERROR_SUCCESS:
        std::wcout << L"The file is signed and the signature is valid." << std::endl;
        return 0;
    case TRUST_E_NOSIGNATURE:
        std::wcout << L"The file is not signed." << std::endl;
        return 1;
    case TRUST_E_BAD_DIGEST:
    case CERT_E_CHAINING:
        std::wcout << L"The file's signature is invalid." << std::endl;
        return 2;
    default:
        std::wcout << L"An error occurred: " << std::hex << result << std::endl;
        return -1;
    }
}
