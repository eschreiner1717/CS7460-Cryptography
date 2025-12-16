#include <stdio.h>
#include <openssl/bn.h>
#include <string.h>

void printBN(char *msg, BIGNUM *a) {
    char *number_str = BN_bn2hex(a);
    printf("%s %s\n", msg, number_str);
    OPENSSL_free(number_str);
}

int main() {
    BN_CTX *ctx = BN_CTX_new();

    BIGNUM *n = BN_new();
    BIGNUM *e = BN_new();
    BIGNUM *S = BN_new();
    BIGNUM *S2 = BN_new();
    BIGNUM *M = BN_new();
    BIGNUM *MOUT = BN_new();

    BN_hex2bn(&n, "AE1CD4DC432798D933779FBD46C6E1247F0CF1233595113AA51B450F18116115");
    BN_hex2bn(&e, "010001");
    BN_hex2bn(&S, "643D6F34902D9C7EC90CB0B2BCA36C47FA37165C0005CAB026C0542CBDB6802F");
    BN_hex2bn(&S2, "643D6F34902D9C7EC90CB0B2BCA36C47FA37165C0005CAB026C0542CBDB6803F");
    char msg[] = "Launch a missile.";
    BN_bin2bn((unsigned char *)msg, strlen(msg), M);
    //BN_hex2bn(&M, "");   // "Launch a missile."
    printBN("Original Message =", M);

    BN_mod_exp(MOUT, S, e, n, ctx);

    printBN("Output Message with good signature =", MOUT);

    //Check if they match
    if(BN_cmp(M, MOUT) == 0){
	printf("Valid Signature\n");
    } else{
	printf("Invalid Signature\n");
    }

    BN_mod_exp(MOUT, S2, e, n, ctx);
    printBN("Output Message with corrupted signature =", MOUT);
 
   //Check if they match
    if(BN_cmp(M, MOUT) == 0){
        printf("Valid Signiture\n");
    } else{
        printf("Invalid Signiture\n");
    }


    BN_CTX_free(ctx);
    return 0;
}
