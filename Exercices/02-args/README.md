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

### ex02 - faire une comparaison

en gros faire un if et jump la ou il faut
on pourrait faire un programme qui regarde juste le nombre d'argument et qui compare


### ex03 - faire une boucle

qui est finalement la meme chose mais pour bien le montrer

### ex04 - faire une loop sur les arguments du main 

on print pas encore mais on fait juste un passage dessus
