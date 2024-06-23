#include <windows.h>
#include <wintrust.h>
#include <softpub.h>
#include <iostream>
#include <unordered_map>

#pragma comment(lib, "wintrust.lib")

std::unordered_map<LONG, std::wstring> errorMessages = {
    {TRUST_E_NOSIGNATURE, L"The file is not signed."},
    {TRUST_E_BAD_DIGEST, L"The digital signature of the object did not verify."},
    {CERT_E_CHAINING, L"A chain of certificates could not be created."},
    {CERT_E_CRITICAL, L"A certificate contains an unknown extension that is marked 'critical.'"},
    {CERT_E_INVALID_NAME, L"The certificate has a name that is not valid. The name is either not included in the permitted list or is explicitly excluded."},
    {CERT_E_INVALID_POLICY, L"The certificate has a policy that is not valid."},
    {CERT_E_ISSUERCHAINING, L"A parent of a given certificate in fact did not issue that child certificate."},
    {CERT_E_MALFORMED, L"A certificate is missing or has an empty value for an important field, such as a subject or issuer name."},
    {CERT_E_PATHLENCONST, L"A path length constraint in the certification chain has been violated."},
    {CERT_E_UNTRUSTEDCA, L"A certification chain processed correctly, but one of the CA certificates is not trusted by the policy provider."},
    {CRYPT_E_NO_REVOCATION_CHECK, L"The revocation function was unable to check revocation for the certificate."},
    {TRUST_E_BASIC_CONSTRAINTS, L"The basic constraint extension of a certificate has not been observed."},
    {TRUST_E_CERT_SIGNATURE, L"The signature of the certificate cannot be verified."},
    {TRUST_E_COUNTER_SIGNER, L"One of the counter signatures was not valid."},
    {TRUST_E_EXPLICIT_DISTRUST, L"The certificate was explicitly marked as untrusted by the user."},
    {TRUST_E_FINANCIAL_CRITERIA, L"The certificate does not meet or contain the Authenticode financial extensions."},
    {TRUST_E_NO_SIGNER_CERT, L"The certificate for the signer of the message is not valid or not found."},
    {TRUST_E_SYSTEM_ERROR, L"A system-level error occurred while verifying trust."},
    {TRUST_E_TIME_STAMP, L"The time stamp signature or certificate could not be verified or is malformed."}
};

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
        std::wcout << L"WinVerifyTrust returned: " << std::hex << status;
        auto it = errorMessages.find(status);
        if (it != errorMessages.end())
        {
            std::wcout << L" (" << it->second << L")";
        }
        std::wcout << std::endl;
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
    case CERT_E_CRITICAL:
    case CERT_E_INVALID_NAME:
    case CERT_E_INVALID_POLICY:
    case CERT_E_ISSUERCHAINING:
    case CERT_E_MALFORMED:
    case CERT_E_PATHLENCONST:
    case CERT_E_UNTRUSTEDCA:
    case CRYPT_E_NO_REVOCATION_CHECK:
    case TRUST_E_BASIC_CONSTRAINTS:
    case TRUST_E_CERT_SIGNATURE:
    case TRUST_E_COUNTER_SIGNER:
    case TRUST_E_EXPLICIT_DISTRUST:
    case TRUST_E_FINANCIAL_CRITERIA:
    case TRUST_E_NO_SIGNER_CERT:
    case TRUST_E_SYSTEM_ERROR:
    case TRUST_E_TIME_STAMP:
        std::wcout << errorMessages[result] << std::endl;
        return 2;
    default:
        std::wcout << L"An error occurred: " << std::hex << result << std::endl;
        return -1;
    }
}
