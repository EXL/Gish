// txt.c
#include <stdio.h>
#define TXT_GISHUSES "Gish uses"

int main() {
    printf("Bug:\t%d\n", sizeof(TXT_GISHUSES+1)*12+66);
    printf("No Bug:\t%d\n", sizeof(TXT_GISHUSES)*12+66);
    return 0;
}
