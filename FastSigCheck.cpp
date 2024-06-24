// Version: 1.0.1
// Date: 6/23/2024
// Author: ThioJoe

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
    {TRUST_E_TIME_STAMP, L"The time stamp signature or certificate could not be verified or is malformed."},
    {TRUST_E_SUBJECT_NOT_TRUSTED, L"The subject failed the specified verification action. Check the EnableCertPaddingCheck registry key for additional verification."},
    {TRUST_E_PROVIDER_UNKNOWN, L"The trust provider is not recognized on this system."},
    {TRUST_E_ACTION_UNKNOWN, L"The trust provider does not support the specified action."},
    {TRUST_E_SUBJECT_FORM_UNKNOWN, L"The trust provider does not support the form specified for the subject."},
    {CRYPT_E_REVOKED, L"The certificate or signature has been revoked."},
    {CERT_E_UNTRUSTEDROOT, L"A certification chain processed correctly but terminated in a root certificate that is not trusted by the trust provider."},
    {CERT_E_UNTRUSTEDTESTROOT, L"The root certificate is a testing certificate and policy settings disallow test certificates."},
    {CERT_E_WRONG_USAGE, L"The certificate is not valid for the requested usage."},
    {CERT_E_EXPIRED, L"A required certificate is not within its validity period."},
    {CRYPT_E_REVOCATION_OFFLINE, L"The revocation function was unable to check revocation because the revocation server was offline."},
    {CERT_E_VALIDITYPERIODNESTING, L"The validity periods of the certification chain do not nest correctly."},
    {CERT_E_PURPOSE, L"The certificate is being used for a purpose other than one specified by the issuing CA."},
    {CERT_E_REVOCATION_FAILURE, L"The revocation process could not continue and the certificate could not be checked."},
    {CERT_E_CN_NO_MATCH, L"The certificate's CN name does not match the passed value."},
    {CERT_E_ROLE, L"A certificate that can only be used as an end-entity is being used as a CA or vice versa."}
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
        std::wcerr << L"Usage: VerifySignature.exe <file-path> [--debug]\n"
            << L"Return codes:\n"
            << L"  0: The file is signed and the signature is valid.\n"
            << L"  1: The file is not signed.\n"
            << L"  2: The file's signature is invalid or other signature-related errors occurred.\n"
            << L"  3: An unsupported error occurred during the verification process.\n"
            << L" -1: Usage error or unexpected system-level error." << std::endl;

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
    // Valid Signature Case
    case ERROR_SUCCESS:
        std::wcout << L"The file is signed and the signature is valid." << std::endl;
        return 0;
    // No Signature Case
    case TRUST_E_NOSIGNATURE:
        std::wcout << L"The file is not signed." << std::endl;
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
    case CRYPT_E_REVOKED:
    case CERT_E_UNTRUSTEDROOT:
    case CERT_E_UNTRUSTEDTESTROOT:
    case CERT_E_WRONG_USAGE:
    case CERT_E_EXPIRED:
    case CRYPT_E_REVOCATION_OFFLINE:
    case CERT_E_VALIDITYPERIODNESTING:
    case CERT_E_PURPOSE:
    case CERT_E_REVOCATION_FAILURE:
    case CERT_E_CN_NO_MATCH:
    case CERT_E_ROLE:
        std::wcout << errorMessages[result] << std::endl;
        return 2;
    // Unsupported error cases
    case TRUST_E_PROVIDER_UNKNOWN:
    case TRUST_E_ACTION_UNKNOWN:
    case TRUST_E_SUBJECT_FORM_UNKNOWN:
        std::wcout << L"An error occurred: " << errorMessages[result] << std::endl;
        return 3;
    default:
        std::wcout << L"An error occurred: " << std::hex << result << std::endl;
        return -1;
    }
}
