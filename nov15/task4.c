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
    //BIGNUM *e = BN_new();
    BIGNUM *d = BN_new();
    BIGNUM *M = BN_new();
    BIGNUM *SM = BN_new();
    BIGNUM *M2 = BN_new();
    BIGNUM *SM2 = BN_new();
    //BIGNUM *C = BN_new();

    BN_hex2bn(&n, "DCBFFE3E51F62E09CE7032E2677A78946A849DC4CDDE3A4D0CB81629242FB1A5");
    //BN_hex2bn(&e, "010001");
    BN_hex2bn(&d, "74D806F9F3A62BAE331FFE3F0A68AFE35B3D2E4794148AACBC26AA381CD7D30D");   // "A top secret!"
    char msg[] = "I owe you $2000";
    BN_bin2bn((unsigned char *)msg, strlen(msg), M);
    
    printBN("Original Message =", M); // "I owe you $2000"

    char msg2[] = "I owe you $3000";
    BN_bin2bn((unsigned char *)msg2, strlen(msg2), M2);

    printBN("Modified Message =", M2); // "I owe you $3000"

    BN_mod_exp(SM, M, d, n, ctx);
    BN_mod_exp(SM2, M2, d, n, ctx);

    printBN("Signature for original message:", SM);// "I owe you $2000"
    printBN("Signature for modified message:", SM2);// "I owe you $3000"

    BN_CTX_free(ctx);
    return 0;
}
