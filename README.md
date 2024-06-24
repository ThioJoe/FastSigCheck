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


### Speed Comparison

FastSigCheck demonstrates significant performance improvements over both Signtool and Sigcheck in all tested scenarios. This makes it a more efficient choice for signature verification tasks, particularly for unsigned files.

Below are some benchmarking results using the "Hyperfine" command line benchmarking tool.

<table border="1" cellpadding="5" cellspacing="0">
    <thead>
        <tr>
            <th>Signature Type</th>
            <th>FastSigCheck</th>
            <th>Signtool <sub>(Windows SDK)</sub></th>
            <th>Sigcheck64 <sub>(Sysinternals)</sub></th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>Signed</td>
            <td>${\textsf{\color{green}48.1 ms}}$ <sub>± 3.7 ms</sub></td>
            <td>65.1 ms <sub>± 5.2 ms</sub> ${\textsf{\color{red}(+ 35.3\%)}}$ </td>
            <td>67.4 ms <sub>± 4.4 ms</sub> ${\textsf{\color{red}(+ 40.1\%)}}$ </td>
        </tr>
        <tr>
            <td>Unsigned</td>
            <td>${\textsf{\color{green}25.1 ms}}$ <sub>± 2.8 ms</sub></td>
            <td>38.4 ms <sub>± 5.5 ms</sub> ${\textsf{\color{red}(+ 53\%)}}$ </td>
            <td>57.7 ms <sub>± 4.8 ms</sub> ${\textsf{\color{red}(+ 129.9\%)}}$ </td>
        </tr>
        <tr>
            <td>Untrusted Signature</td>
            <td>${\textsf{\color{green}50.7 ms}}$ <sub>± 3.8 ms</sub></td>
            <td>67.7 ms <sub>± 4.4 ms</sub> ${\textsf{\color{red}(+ 33.5\%)}}$ </td>
            <td>75.0 ms <sub>± 3.8 ms</sub> ${\textsf{\color{red}(+ 47.9\%)}}$ </td>
        </tr>
    </tbody>
</table>


<details>
<summary>Click for raw data from Hyperfine benchmarks, including commands:</summary>

```plaintext
hyperfine "FastSigCheck.exe Signed.exe" --warmup 10
    Time (mean ± σ):      48.1 ms ±   3.7 ms    [User: 17.5 ms, System: 25.2 ms]
    Range (min … max):    41.5 ms …  57.7 ms    41 runs

hyperfine "FastSigCheck.exe Unsigned.exe" -i --warmup 10
    Time (mean ± σ):      25.1 ms ±   2.8 ms    [User: 10.8 ms, System: 11.6 ms]
    Range (min … max):    18.5 ms …  32.3 ms    65 runs

hyperfine "FastSigCheck.exe BadSignature.cat" -i --warmup 10
    Time (mean ± σ):      50.7 ms ±   3.8 ms    [User: 16.8 ms, System: 26.7 ms]
    Range (min … max):    43.2 ms …  58.4 ms    38 runs

---------------------------------------------------------------------------------

hyperfine "signtool verify /pa /q Signed.exe" --warmup 10
    Time (mean ± σ):      65.1 ms ±   5.2 ms    [User: 24.2 ms, System: 34.5 ms]
    Range (min … max):    54.3 ms …  77.7 ms    36 runs

hyperfine "signtool verify /pa /q Unsigned.exe" -i --warmup 10
    Time (mean ± σ):      38.4 ms ±   5.5 ms    [User: 14.6 ms, System: 17.3 ms]
    Range (min … max):    30.2 ms …  65.3 ms    50 runs

hyperfine "signtool verify /pa /q BadSignature.cat" -i --warmup 10
    Time (mean ± σ):      67.7 ms ±   4.4 ms    [User: 24.8 ms, System: 34.6 ms]
    Range (min … max):    60.6 ms …  83.0 ms    35 runs

---------------------------------------------------------------------------------

hyperfine "sigcheck64.exe -nobanner Signed.exe" --warmup 10
    Time (mean ± σ):      67.4 ms ±   4.4 ms    [User: 16.6 ms, System: 34.8 ms]
    Range (min … max):    60.0 ms …  77.2 ms    34 runs

hyperfine "sigcheck64.exe -nobanner Unsigned.exe" -i --warmup 10
    Time (mean ± σ):      57.7 ms ±   4.8 ms    [User: 14.0 ms, System: 35.9 ms]
    Range (min … max):    49.6 ms …  78.2 ms    38 runs

hyperfine "sigcheck64.exe -nobanner BadSignature.cat" -i --warmup 10
    Time (mean ± σ):      75.0 ms ±   3.8 ms    [User: 23.4 ms, System: 39.6 ms]
    Range (min … max):    68.3 ms …  86.7 ms    31 runs
```
</details>


## Limitations:
- Does not work with catalog signatures

# Other Notes:
- The "Headless" Version: This version returns results only as an exit code with no option for string outputs of any kind, in case maximum efficiency is required. Though I haven't found it to really be any faster, but have included it anyway just in case.


