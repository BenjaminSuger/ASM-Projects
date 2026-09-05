/*
 * inspect.c — le cobaye du Module 0.
 * On ne le modifie PAS. On le compile avec -g et on l'explore dans gdb.
 *
 *   gcc -g -O0 inspect.c -o inspect
 *   gdb ./inspect
 *
 * -O0 est important : sans optimisation, chaque variable a une existence
 * concrète et observable. Avec -O2, le compilateur fait disparaître la moitié
 * du code, ce qui est déroutant quand on débute.
 */
#include <stdio.h>

int add(int a, int b)
{
    int result = a + b;   // point d'observation n°2
    return result;
}

int main(void)
{
    int answer = 0x2a;         // 42 en hexadécimal
    long big = -1L;            // pour voir le complément à deux
    char text[] = "libasm";    // un tableau sur la pile

    answer = add(answer, 0);   // point d'observation n°1 (mets un breakpoint sur add)

    printf("answer = %d\n", answer);
    printf("big    = %ld\n", big);
    printf("text   = %s\n", text);
    return 0;
}
