// 0x80.c
#include <stdio.h>

int main() {
    printf("Bug:\t%f\n", ('\x80' >> 4) * 16.0f + 0.5f);
    printf("No bug:\t%f\n", ((unsigned char) '\x80' >> 4) * 16.0f + 0.5f);
    return 0;
}
