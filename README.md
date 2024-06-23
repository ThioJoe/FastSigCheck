# FastSigCheck

FastSigCheck is a console application that verifies the embedded digital signature of a file in Windows as efficiently as possible. It returns basic result types via exit codes so no parsing is required, making it suitable for integration with other programs that can't handle return data.

## Results / Exit Codes
- `0`: The file is signed and the signature is valid.
- `1`: The file is not signed.
- `2`: The file's signature is invalid or other signature-related errors occurred.
- `3`: An unsupported error occurred during the verification process.
- `-1`: Usage error or unexpected system-level error.

## Usage

```bash
FastSigCheck.exe <file-path> [--debug]
```

## Limitations:
- Does not work with catalog signatures

# Other Notes:
- The "Headless" Version: This version returns results only as an exit code with no option for string outputs of any kind, in case maximum efficiency is required. Though I haven't found it to really be any faster, but have included it anyway just in case.
