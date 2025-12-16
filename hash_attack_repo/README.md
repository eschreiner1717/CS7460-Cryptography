# Hash Length Extension Attack Lab

**Team:** Evan Schreiner & Will Curry
**Fall 2025**


## Project Overview
This repo contains our work for the SEED Labs Hash Length Extension Attack.
- **Evan:** Tasks 1 & 2 (Setup, MAC Analysis, Padding Calculation)
- **Will:** Tasks 3 & 4 (Attack Execution, HMAC Remediation)


## Task 1 & 2

### 1. Target Information

We are attacking the server at `www.seedlab-hashlen.com` (10.9.0.80).
* **Student Name used:** `EvanSchreiner`
* **UID:** `1001`
* **Key:** `123456` (Found in `key.txt`)

### 2. The Baseline Request

This is the valid request we generated. We will extend this message.
* **Original Command String (R):** `myname=EvanSchreiner&uid=1001&lstcmd=1`
* **Original MAC Hash:** `53f5e9d89774bf0f1050fb4cadc83ca04465e56afd17a3cde64c64c989a9faa1`
    * *Note to Will: You must use this hash to initialize the registers in `length_ext.c`.*

### 3. The Calculated Padding
I calculated the SHA-256 padding for the 64-byte block.
* **Padding Hex:** `80000000000000000000000000000000000168`
* **Padding URL Encoded:** `%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%68`
* **Length Field (Hex):** `0x168` (360 bits)


## Task 3 (*Will*)
**Objective:** Forge a signature for `&download=secret.txt` without knowing the key.

1.  Open `length_ext.c`.
2.  Input the **Original MAC Hash** (from Section 2 above) into the `c.h[]` registers.
    * *Note: Use `htole32` because our VM is Little Endian.*
3.  Update the length field in the code to `64 + len("Extra message")`.
    * *Note: The C code needs the total length of the forged message to finalize the hash correctly.*
4.  Compile and Run:
    ```bash
    gcc length_ext.c -o length_ext -lcrypto
    ./length_ext
    ```
5.  Construct the Attack URL:
    `http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1` + `[Padding URL Encoded]` + `&download=secret.txt` + `&mac=[New_Forged_MAC]`


## Task 4 (Remediation)
1.  Open `lab.py` (the server code).
2.  Replace the insecure SHA-256 concatenation with `hmac.new`.
3.  Rebuild the docker container (`dcbuild && dcup`) and verify the attack above no longer works.
