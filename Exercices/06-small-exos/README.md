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

Il etait important de faire strcpy avant strdup car on peut ici faire de l'assemblage (sans mauvais jeu de mot) => c'est a dire qu'on peut utiliser des fonctions qu'on a fait auparavant ; strcpy et strlen.

Pour les utiliser on va devoir

```asm
section .text
    global ft_strdup
    extern ft_strlen
    extern ft_strcpy
    extern malloc
```

A la fin quand on va vouloir le compiler on va pouvoir utiliser nos autres fichier (en .o) pour bien compiler comme il faut ou alors un libasm.a (ce que je vais lui donner dans mon cas).

On va arriver aussi sur quelques soucis ! Ici on doit utiliser malloc. On nous precise bien que l'output de strdup est bien d'origine du malloc. 

```asm
    extern malloc
```

Donc la jusqu'a present on avait pu faire notre propre _start => sauf que la on va devoir utiliser le dynamic linker de libc => donc ca veut dire qu'il va y avoir un _start deja existant qui initialise la libc.

Comment faire ? 

2 solutions ; 
* on compile un main.c 
* on fait en asm pas un _start mais un main => solution plus simple et qui nous fait pratiquer un peu, meme si c'est simple, un peu plus d'asm

(en partant du principe qu'on a deja un libasm.a)

```sh
nasm -f elf64 -g -F dwarf main.s -o main.o
gcc main.o libasm.a -o exe
```

GCC va se charger de faire la liaison avec la libc de lui meme.
il faudrat quand meme ajoute ca a la fin de notre main pour faire partir un message de warning (je crois que j'en ai deja parle dans une autre section avant => sinon allez voir mon readme de libasm)

```asm
section .note.GNU-stack noalloc noexec nowrite progbits
```

Ici dans cette fonction, non leaf, il faut faire attention a l'alignement a 16. Puisque les call qu'on va faire, nous meme etant deja decale de 8 provenant d'un call (decalage du a l'adresse de retour de la fonction).

franchement vu la complexite deja, le free c'est optional pour moi ici, l'objectif c'est de vraiment construire ft_strdup


## ex03 - read

On continue la logiqu de suivre la libasm ici avec un syscall a gerer sur read.

Fonction tres proche du fonctionnement de ft_write, notre interet a nous est d'encapsuler la logique de la gestion de l'errno.

Donc la aussi attention a bien utiliser __errno_location et donc comme d'habitude pour une fonction non leaf l'alignement a 16.

Read prend en argument un buffer. Pareil que pour ft_strcpy => on peut soit passer par BSS soit passer par la stack.

Pour gagner du temps, je vais passer par le BSS.


## ex04 - check_base

Cet exercice est la pour nous aider a construire atoi_base. Comme pour l'ensemble de ce document je recommande de faire pwn college - computing 101 la partie "strings as number". Aussi dans un autre module avant, il y a un exercice qui implique un genre de set qui peut etre pas mal.

Mais ici c'est un helper qui va gerer que 2 choses; 
*pas de doublon
*pas de contenu interdit ; on doit avoir que des valeurs "a-zA-Z0-9" et caractere speciaux => c'est a dire pas de isspace() et de + ou -

Pour ca on va cette fois utiliser un main.c 

```C
#include <stdio.h>

int check_base(char *str);

int main(int argc, char **argv) {
	printf("is it good ? %d\n", check_base("01234567"));
	printf("is it good ? %d\n", check_base("01234567 "));
	printf("is it good ? %d\n", check_base("abcdefgATF"));
	printf("is it good ? %d\n", check_base("abc-defgATF"));
	printf("is it good ? %d\n", check_base("abcdefga"));
}
```

et ensuite pour compile

```sh
nasm -f elf64 -g -F dwarf check_base.s check_base.o
gcc ex04.c check_base.o
```

## ex05 - atoi_base a la 42

il faut faire atoi base a la 42 piscine, il y a aura pas plus d'explication que ca.
Mais le fichier test.c permet de faire les tests complet de plein de cas.
La majeur difference avec celui de libasm c'est que dans libasm on demande un comportement comme ft_atoi => qui replique atoi => qui replique strtol sur le comportement des signes => "+-+-42" => renvoie donc 0 alors que nous on peut rendre 42.
C'est un petit detail mais a prendre en compte.
