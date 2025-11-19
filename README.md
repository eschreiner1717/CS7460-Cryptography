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

<img width="1117" height="86" alt="image" src="https://github.com/user-attachments/assets/b0198ddb-c1a8-4578-9482-9d4cbd7dc651" />


## Task 2: Encrypting a Message

See source code in `task2.c`.

```bash
$ ./task2
Encrypted ciphertext C = 6FB078DA550B2650832661E14F4F8D2CFAEF475A0DF3A75CACDC5DE5CFC5FADC
```

<img width="1164" height="60" alt="image" src="https://github.com/user-attachments/assets/027e5ee5-ea9e-47db-b8d0-b1944342ddcd" />

Then veryifying via decyption with `task3.c` (:
```bash
$ ./task3
Decrypted M = 4120746F702073656372657421

$ python3 -c 'print(bytes.fromhex("4120746F702073656372657421").decode("utf-8"))'
A top secret!
```

<img width="1186" height="133" alt="image" src="https://github.com/user-attachments/assets/e700700f-7b3d-40ec-a1d1-7afc6a6d597d" />


## Task 3: Decrypting a Message

See source code in `task3.c`.

```bash
$ ./task3
Decrypted M = 50617373776F72642069732064656573

$ python3 -c 'print(bytes.fromhex("50617373776F72642069732064656573").decode("utf-8"))'
Password is dees
```

<img width="1188" height="133" alt="image" src="https://github.com/user-attachments/assets/905f6ddc-84c1-426a-a28b-68c9d155fa61" />


## Task 4: Signing a Message

See source code in `task4.c`.

<img width="487" height="57" alt="Screenshot 2025-11-18 at 9 40 02 PM" src="https://github.com/user-attachments/assets/686367bf-4e97-4370-b6f2-2191d7cf29b0" />

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

<img width="549" height="67" alt="Screenshot 2025-11-18 at 9 40 38 PM" src="https://github.com/user-attachments/assets/5a36accf-531a-4e49-af0b-5ad56d7fa91b" />


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

<img width="963" height="793" alt="Screenshot 2025-11-18 at 9 25 52 PM" src="https://github.com/user-attachments/assets/a1f28c8c-9379-452c-8601-a6b5d91430c0" />

Next, I found the public key attributes n and e taking the Modulus value in the first command as n, and the exponent value in the second command as e. 

<img width="952" height="71" alt="Screenshot 2025-11-18 at 9 26 26 PM" src="https://github.com/user-attachments/assets/4fdac712-48bd-4e03-926d-616ba6bb962a" />

<img width="496" height="420" alt="Screenshot 2025-11-18 at 9 26 52 PM" src="https://github.com/user-attachments/assets/6bb08550-574e-4315-ba94-27fa7f646004" />



Then I extracted the signature using a command. I then removed the colons and spaces by copying and pasting the signature into a file and running atruncate function.

<img width="499" height="479" alt="Screenshot 2025-11-18 at 9 27 26 PM" src="https://github.com/user-attachments/assets/42aeb755-0363-41ee-aaad-1d927816866e" />
<img width="948" height="272" alt="Screenshot 2025-11-18 at 9 27 55 PM" src="https://github.com/user-attachments/assets/94aa1951-7c79-481a-ab0b-2b2e942ec830" />


Next I did commands to get the body of the certificate and calculate the hash:
<img width="586" height="472" alt="Screenshot 2025-11-18 at 9 34 54 PM" src="https://github.com/user-attachments/assets/c7dc763c-d4aa-4347-a6a4-0f46bc09c32e" />
<img width="951" height="666" alt="Screenshot 2025-11-18 at 9 35 39 PM" src="https://github.com/user-attachments/assets/8d7fe640-c392-4bb1-86e1-2a6ae327bf30" />


With the hash I can now try to verify the signature with task6.c.

See source code in `task6.c`.

<img width="950" height="144" alt="Screenshot 2025-11-18 at 9 41 15 PM" src="https://github.com/user-attachments/assets/8e2b4359-9298-4fd6-80d2-e97e31c53f95" />

```bash
$ ./task6
Decrypted Signature = 01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF003031300D060960864801650304020105000420B4757CE7679543C2A51EF74D56F7A94025004E9BFA391112818E34440876277A
The original signature hash = b4757ce7679543c2a51ef74d56f7a94025004e9bfa391112818e34440876277a
```
In this case the signature was valid as the last bits of the decrypted signature matched the original signature hash.

## Task 7: Collaboration Statement
Evan completed tasks 1, 2, and 3. Will completed tasks 4, 5, and 6.

