#include <stdio.h>

int main() {
	printf("%f\n", ((char) 0x80 >> 4) * 16.0f + 0.5f);
	printf("%f\n", ((unsigned char) 0x80 >> 4) * 16.0f + 0.5f);
	return 0;
}
