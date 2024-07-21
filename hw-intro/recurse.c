#include <stdio.h>
#include <stdlib.h>

int recur(int i) {
    printf("i is %i. Address of i is %p\n", i, &i);
    char* p = malloc(2);
    printf("Address of dynamic memory is %p\n", (void*)p);

    if (i > 0) {
        return recur(i - 1);
    }

    free(p);
    return 0;
}
