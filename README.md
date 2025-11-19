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
Next, I found the public key attributes n and e using this for n
```bash
$ openssl s_client -connect www.reddit.com:443 -showcerts
```



See source code in `task6.c`.

```bash
$ ./task6

```


## Task 7: Collaboration Statement
Evan completed tasks 1, 2, and 3, Will completed tasks 4, 5, and 6.

