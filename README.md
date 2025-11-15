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
