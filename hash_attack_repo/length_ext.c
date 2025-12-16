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
	c.h[0] = htole32(0x00000000);
	c.h[1] = htole32(0x00000000);
	// continue...

	// TODO: Update the number of bits processed.
	// Total length of the forged message = (Key + Message + Padding).

	c.Nl = (64 + 13) * 8; // 64 bytes (original block) + 13 bytes ("Extra message") * 8 bits/byte
    	c.Nh = 0; 

    	// Append the extra message
    	// Note: This must match the command you add to the URL (e.g. &download=secret.txt)
    	// The example uses "Extra message" (13 chars). 
    	// TODO: CHANGE THIS to "&download=secret.txt" (20 chars) for the actual attack.
    	SHA256_Update(&c, "Extra message", 13); 

    	SHA256_Final(buffer, &c);

    	// Print the new forged MAC
    	for(i=0; i<32; i++) {
        	printf("%02x", buffer[i]);
    	}
    	printf("\n");
    	return 0;
}
