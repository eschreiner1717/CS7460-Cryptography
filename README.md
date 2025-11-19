# CS7460 Lab for Module 6

## Important commands

```bash
gcc <file> -o <name> -lcrypto
```

```bash
python3 -c ’print("A top secret!".encode("utf-8").hex())’
python3 -c ’print(bytes.fromhex("4120746f702073656372657421").decode("utf-8"))’
```

## Task 1: Deriving the Private Key

See source code in `task1.c`.

```bash
$ ./task1
phi = (p-1)*(q-1) =  E103ABD94892E3E74AFD724BF28E78348D52298BD687C44DEB3A81065A7981A4
Private key d =      3587A24598E5F2A21DB007D89D18CC50ABA5075BA19A33890FE7C28A9B496AEB
```

## Task 2: Encrypting a Message

See source code in `task2.c`.

```bash
$ ./task2
Encrypted ciphertext C = 6FB078DA550B2650832661E14F4F8D2CFAEF475A0DF3A75CACDC5DE5CFC5FADC
```

Then veryifying via decyption with `task3.c` (:
```bash
$ ./task3
Decrypted M = 4120746F702073656372657421

$ python3 -c 'print(bytes.fromhex("4120746F702073656372657421").decode("utf-8"))'
A top secret!
```


## Task 3: Decrypting a Message

See source code in `task3.c`.

```bash
$ ./task3
Decrypted M = 50617373776F72642069732064656573

$ python3 -c 'print(bytes.fromhex("50617373776F72642069732064656573").decode("utf-8"))'
Password is dees
```


## Task 4: Signing a Message

See source code in `task4.c`.

```bash
$ ./task4
Original Message = 49206F776520796F75202432303030
Modified Message = 49206F776520796F75202433303030
Signature for original message: 80A55421D72345AC199836F60D51DC9594E2BDB4AE20C804823FB71660DE7B82
Signature for modified message: 04FC9C53ED7BBE4ED4BE2C24B0BDF7184B96290B4ED4E3959F58E94B1ECEA2EB
```

Comparing the two signatures, we can tell that they are not similar to each other. We changed one digit of the message to make $2000 become $3000, but that resulted in a completely unrelated signature. 

## Task 5: Verifying a Signature

See source code in `task5.c`.

```bash
$ ./task5
Original Message = 4C61756E63682061206D697373696C652E
Output Message with good signature = 4C61756E63682061206D697373696C652E
Valid Signature
Output Message with corrupted signature = 91471927C80DF1E42C154FB4638CE8BC726D3D66C83A4EB6B7BE0203B41AC294
Invalid Signiture
```

After changing the last byte of the signature from 2F to 3F, the resulting output message becomes something completely unrelated to the original message, causing it to fail signature verification.

## Task 6: Manually Verifying an X.509 Certificate

In my case I retreived the RSA certificates from www.reddit.com with this command. It came with 2 certificates, and I saved the first one to c0.pem and the second to c1.pem. 

```bash
$ openssl s_client -connect www.reddit.com:443 -showcerts
```
Next, I found the public key attributes n and e taking the Modulus value in the first command as n, and the exponent value in the second command as e. 

```bash
$ openssl x509 -in c1.pem -noout -modulus
Modulus=CCF710624FA6BB636FED905256C56D277B7A12568AF1F4F9D6E7E18FBD95ABF260411570DB1200FA270AB557385B7DB2519371950E6A41945B351BFA7BFABBC5BE2430FE56EFC4F37D97E314F5144DCBA710F216EAAB22F031221161699026BA78D9971FE37D66AB75449573C8ACFFEF5D0A8A5943E1ACB23A0FF348FCD76B37C163DCDE46D6DB45FE7D23FD90E851071E51A35FED4946547F2C88C5F4139C97153C03E8A139DC690C32C1AF16574C9447427CA2C89C7DE6D44D54AF4299A8C104C2779CD648E4CE11E02A8099F04370CF3F766BD14C49AB245EC20D82FD46A8AB6C93CC6252427592F89AFA5E5EB2B061E51F1FB97F0998E83DFA837F4769A1

$ openssl x509 -in c1.pem -text -noout
...
Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                Public-Key: (2048 bit)
                Modulus:
                    00:cc:f7:10:62:4f:a6:bb:63:6f:ed:90:52:56:c5:
                    6d:27:7b:7a:12:56:8a:f1:f4:f9:d6:e7:e1:8f:bd:
                    95:ab:f2:60:41:15:70:db:12:00:fa:27:0a:b5:57:
                    38:5b:7d:b2:51:93:71:95:0e:6a:41:94:5b:35:1b:
                    fa:7b:fa:bb:c5:be:24:30:fe:56:ef:c4:f3:7d:97:
                    e3:14:f5:14:4d:cb:a7:10:f2:16:ea:ab:22:f0:31:
                    22:11:61:69:90:26:ba:78:d9:97:1f:e3:7d:66:ab:
                    75:44:95:73:c8:ac:ff:ef:5d:0a:8a:59:43:e1:ac:
                    b2:3a:0f:f3:48:fc:d7:6b:37:c1:63:dc:de:46:d6:
                    db:45:fe:7d:23:fd:90:e8:51:07:1e:51:a3:5f:ed:
                    49:46:54:7f:2c:88:c5:f4:13:9c:97:15:3c:03:e8:
                    a1:39:dc:69:0c:32:c1:af:16:57:4c:94:47:42:7c:
                    a2:c8:9c:7d:e6:d4:4d:54:af:42:99:a8:c1:04:c2:
                    77:9c:d6:48:e4:ce:11:e0:2a:80:99:f0:43:70:cf:
                    3f:76:6b:d1:4c:49:ab:24:5e:c2:0d:82:fd:46:a8:
                    ab:6c:93:cc:62:52:42:75:92:f8:9a:fa:5e:5e:b2:
                    b0:61:e5:1f:1f:b9:7f:09:98:e8:3d:fa:83:7f:47:
                    69:a1
                Exponent: 65537 (0x10001)
...
```

Then I extracted the signature using this command:

```bash
$ openssl x509 -in c0.pem -text -noout
...
Signature Algorithm: sha256WithRSAEncryption
    Signature Value:
        01:8c:95:77:c9:b9:2d:6c:3d:3c:e4:bf:be:c1:ff:27:66:5e:
        d1:e0:29:fa:43:eb:1b:85:97:35:a8:0e:ed:98:53:51:07:54:
        7a:fb:fd:30:a1:99:c5:42:c1:cb:82:8b:2c:0d:6f:48:e8:b7:
        a6:bc:11:6e:96:5e:fd:b1:18:66:75:ec:78:f7:38:00:b1:2a:
        d9:57:18:45:4a:56:01:1a:d0:1e:15:30:3c:9e:15:fb:15:6c:
        86:04:82:be:60:de:05:33:66:3d:15:49:e3:e3:e3:57:a8:ac:
        65:4f:79:6b:33:a8:e6:57:ee:a6:bc:27:a8:12:f6:82:8a:ed:
        e3:6a:a7:bf:dc:7f:af:06:28:b7:82:4b:30:17:29:90:03:61:
        d0:8a:49:40:2d:12:6f:af:7d:e4:5c:9f:36:ab:b3:af:92:d5:
        e4:42:6c:9d:58:99:29:f5:38:d8:d8:61:87:be:a4:c9:16:01:
        50:68:a8:9c:7b:16:2f:7b:41:25:41:cd:04:7d:36:d2:20:f1:
        1c:36:71:06:38:61:54:1b:72:34:93:51:2d:ae:88:5f:45:4f:
        69:7b:60:7b:4b:65:ff:31:3c:74:3a:e1:81:10:82:c4:45:5f:
        9a:36:52:12:50:34:58:a6:65:39:a4:2c:ef:12:d5:f7:dc:79:
        1a:1a:01:24
```
To remove the colons and spaces I copied and pasted the signature into a file and ran this:
```bash
$ cat signature | tr -d ’[:space:]:’
018c9577c9b92d6c3d3ce4bfbec1ff27665ed1e029fa43eb1b859735a80eed98535107547afbfd30a199c542c1cb828b2c0d6f48e8b7a6bc116e965efdb1186675ec78f73800b12ad95718454a56011ad01e15303c9e15fb156c860482be60de0533663d1549e3e3e357a8ac654f796b33a8e657eea6bc27a812f6828aede36aa7bfdc7faf0628b7824b301729900361d08a49402d126faf7de45c9f36abb3af92d5e4426c9d589929f538d8d86187bea4c916015068a89c7b162f7b412541cd047d36d220f11c3671063861541b723493512dae885f454f697b607b4b65ff313c743ae1811082c4455f9a365212503458a66539a42cef12d5f7dc791a1a0124
```
Next I did commands to get the body of the certificate and calculate the hash:

```bash
$ openssl asn1parse -i -in c0.pem
    0:d=0  hl=4 l=1760 cons: SEQUENCE          
    4:d=1  hl=4 l=1480 cons:  SEQUENCE          
    8:d=2  hl=2 l=   3 cons:   cont [ 0 ]        
    10:d=3  hl=2 l=   1 prim:    INTEGER           :02
...
$ openssl asn1parse -i -in c0.pem -strparse 4 -out c0_body.bin -noout

$ sha256sum c0_body.bin
b4757ce7679543c2a51ef74d56f7a94025004e9bfa391112818e34440876277a  c0_body.bin

```

With the hash I can now try to verify the signature with task6.c.

See source code in `task6.c`.

```bash
$ ./task6
Decrypted Signature = 01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF003031300D060960864801650304020105000420B4757CE7679543C2A51EF74D56F7A94025004E9BFA391112818E34440876277A
The original signature hash = b4757ce7679543c2a51ef74d56f7a94025004e9bfa391112818e34440876277a
```
In this case the signature was valid as the last bits of the decrypted signature matched the original signature hash.

## Task 7: Collaboration Statement
Evan completed tasks 1, 2, and 3. Will completed tasks 4, 5, and 6.

