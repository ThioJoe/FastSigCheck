// Version: 1.0.0
// Date: 6/23/2024
// Author: ThioJoe

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
    // Valid Signature Case
    case ERROR_SUCCESS:
        return 0;
    // No Signature Case
    case TRUST_E_NOSIGNATURE:
        return 1;
    // Invalid signature cases
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
    case TRUST_E_SUBJECT_NOT_TRUSTED:
        return 2;
    // Unsupported error cases
    case TRUST_E_PROVIDER_UNKNOWN:
    case TRUST_E_ACTION_UNKNOWN:
    case TRUST_E_SUBJECT_FORM_UNKNOWN:
        return 3;
    // Other error cases
    default:
        return -1;
    }
}
