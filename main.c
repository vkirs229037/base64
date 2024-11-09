#include "stdio.h"
#define BASE64_IMPLEMENTATION
#include "base64.h"

int main(void) {
    char* encoded = base64_encode("Hello World!", 12);
    printf("%s\n", encoded);
    return 0;
}
