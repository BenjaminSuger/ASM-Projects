# Exercice 02 - Arguments

Objectif ; manipuler les arguments du programme pour introduire d'autres notions par la suite

## Exercices

### ex00 - Observer les arguments avec GDB 

On commence par une premiere observation d'un petit code en C

```C
#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	printf("%s\n", argv[1]);	
}
```

Objectif; avec GDB (ou objdump) voir comment les arguments sont charges et ce qui ce passe au debut.
Ou est argc ?
Ou est argv ?
est ce que si on ecrit nous meme notre code en assembly on les retrouve au meme endroit ?

### ex01 - Suite logique notre propre stack a observer

On peut reprendre l'exercice 01 du exit, creer un petit programme qui pour le moment ne fait rien d'autres que quitter pur ensuite observer nous meme notre propre stack.

C'est la suite logique de l'exercice precedent et de sa derniere question. C'est juste sa mise en pratique 

### ex02 - if

Ici on va faire un programme en asm. L'objectif de check la valeur de notre argc;
Si il est egale a 3 (donc ./a.out <value> <value>) on quitte avec la valeur 42. Sinon avec la valeur 1.

```sh
./exo
echo $? #doit produire 1
./exo les chats
echo $? #doit produire 42
```

Pour ca il y a plusieurs themes ; cmp, jmp, je, les labels

### ex03 - faire une boucle

qui est finalement la meme chose mais pour bien le montrer

### ex04 - faire une loop sur les arguments du main 

on print pas encore mais on fait juste un passage dessus
on precise bien que c'est un building block pour la suite
