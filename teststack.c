#include <stdio.h>
#include "stack.h"

int main(){

    stackType stack;
    printf("Initializing stack.\n");
    initializeStack(&stack);

    printf("Pushing: %d\n", 1);
    stackPush(&stack, 1);

    printf("Popping...");
    int x = stackPop(&stack);
    if (x == 1)
        printf("success\n");
    else
        printf("********** ERROR\n");

    printf("Pushing: %d\n", 13);
    stackPush(&stack, 13);
    printf("Pushing: %d\n", 2);
    stackPush(&stack, 2);
    printf("Popping...");
    x = stackPop(&stack);
    if (x == 2)
        printf("success\n");
    else
        printf("********** ERROR\n");

    printf("Pushing: %d\n", 6);
    stackPush(&stack, 6);
    printf("Pushing: %d\n", 9);
    stackPush(&stack, 9);

    printf("Stack contents (should be 9,6,13):\n");
    printStack(&stack);

}

