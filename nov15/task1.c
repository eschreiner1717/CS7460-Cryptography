#include <stdio.h>
#include <openssl/bn.h>

void printBN(char *msg, BIGNUM *a) {
    char *number_str = BN_bn2hex(a);
    printf("%s %s\n", msg, number_str);
    OPENSSL_free(number_str);
}

int main() {
    BN_CTX *ctx = BN_CTX_new();

    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BIGNUM *e = BN_new();
    BIGNUM *one = BN_new();
    BIGNUM *p_minus_1 = BN_new();
    BIGNUM *q_minus_1 = BN_new();
    BIGNUM *phi = BN_new();
    BIGNUM *d = BN_new();

    BN_hex2bn(&p, "F7E75FDC469067FFDC4E847C51F452DF");
    BN_hex2bn(&q, "E85CED54AF57E53E092113E62F436F4F");
    BN_hex2bn(&e, "0D88C3");

    // Calculate p-1 and q-1
    BN_dec2bn(&one, "1");
    BN_sub(p_minus_1, p, one);
    BN_sub(q_minus_1, q, one);

    // Calculate phi = (p-1) * (q-1)
    BN_mul(phi, p_minus_1, q_minus_1, ctx);
    printBN("phi = (p-1)*(q-1) = ", phi);

    // Calculate d
    BN_mod_inverse(d, e, phi, ctx);
    printBN("Private key d =     ", d);

    BN_CTX_free(ctx);

    return 0;
}
