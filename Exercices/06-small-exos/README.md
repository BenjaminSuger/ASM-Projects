# 06 small exos

## ex00 - strcmp
On va build strcmp;

int strcmp(const char *s1, const char *s2);

Pour rappel le fonctionnement de strcmp ;

on compare s1 a s2.

```C
char *str = "chat";
char *str2 = "chatons";
printf("%d\n", strcmp(str, str2)); //produit -111 par (\0 - 'o')
```

Si un argument est a NULL => segfault

On va jusque la longeur de s1 pour les check ou alors des qu'il y a une difference entre s1 et s2.

Attention le -111 dont je parle est pas assure par le standard c'est juste le cote negatif / positif. On peut si on le souhaite renvoyer -1 uniquement (mais honnetement c'est moins de boulot de renvoyer -111).

Petits indices; 
il existe une instruction pour mov et bien mettre tout a 0 direct ce qui est pas necessaire.

il existe une instruction test

## ex01 - strcpy

Ici c'est un peu plus dur a mettre en place, pas vraiment pour la fonction mais plutot pour le travail avant la fonction. 

Il faut qu'on prepare une destination, et on va pas le faire en C mais bien en ASM. Il y a 2 solutions ; 

*utiliser BSS
*utiliser la stack

BSS c'est les donnees non init donc ca nous donne quelque chose d'assez facile a utiliser comme ca ;

```asm
BUF_SIZE equ 64

section .bss
    dest:   resb BUF_SIZE        ; 64 octets réservés

section .data
    src:    db "hello", 0        ; chaîne source terminée par \0

section .text
    ; ...
    lea  rdi, [rel dest]         ; 1er argument : dest
    lea  rsi, [rel src]          ; 2e argument : src
    call ft_strcpy
```

sinon pour le stack, on reserve la place necessaire ; 

```asm
my_test:
    push rbp
    mov  rbp, rsp
    sub  rsp, 64                 ; 64 octets pour le buffer

    mov  rdi, rsp                ; dest = début du buffer
    lea  rsi, [rel src]
    call ft_strcpy

    leave                        ; restaure rsp et rbp
    ret
```
A noter que leave est une instruction qui fait la remise en place du prelude que nous avons realise avec push rbp / mov rbp, rsp.
Ca fait 2 operations ; 
mov  rsp, rbp
pop rbp

peut importe la maniere que vous souhaiter utilise ca fonctionnement pour faire le strcpy.


## ex02 - strdup

## ex03 - read

## ex04 - check_base

## ex05 - swap

## ex06 - fizz_buzz
