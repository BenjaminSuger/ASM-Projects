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

### ex01 - faire une comparaison

en gros faire un if et jump la ou il faut


### ex02 - faire une boucle

qui est finalement la meme chose mais pour bien le montrer

### ex03 - faire une loop sur les arguments du main 

on print pas encore mais on fait juste un passage dessus
