#include "stdio.h"
#define BASE64_IMPLEMENTATION
#include "base64.h"

int main(int argc, char** argv) {
    char* input = argv[1];
    int size = atoi(argv[2]);
    char* encoded = base64_encode(input, size);
    printf("encoded %s\n", encoded);
    char* decoded = base64_decode(encoded);
    printf("decoded %s\n", decoded);
    return 0;
}
