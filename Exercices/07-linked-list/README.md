# 07 linked list

## ex00 - ft_list_size

Pour toute cette section on va utiliser un ficher en .c pour tester et pratiquer nos fonctions en asm. Pourquoi ? 

Les structures en asm ca n'existe pas reelement. Il y n'y a pas de mot cle structure ou autres. On est sur du pur data, c'est interessant car on voit vraiment l'idee de ce que c'est en nature.

Donc pour acceder a un champs on doit normalement faire un petit "pointers arithmetics", on a un offset et en fonction du contenu de notre structure on doit faire "+" quelque chose. 

C'est la ou NASM vient a notre rescousse, on va pouvoir ecrire ca avec une syntaxe plus facile a gerer (il y a un equivalent GAS).

En C on doit gerer une structure ; 

```c
typedef struct s_list {
    void *data;
    struct s_list *next;
} t_list
```

Du coup en asm on va avoir la meme chose ; 

```asm
struc t_list
    .data resq 1
    .next resq 1
endstruc
```

Pour nos exercices on va le mettre en haut de tous nos fichiers. On l'aura aussi en declaration en brut dans nos fichiers c. Mais dans la libasm on aura comme dans libft un include ! Pour plus de details vous pouvez voir ma documentation README dans LibASM. 

Vous devez utiliser le fichier exo1.c et pour compiler ; 

```sh
nasm -f elf64 ft_list_size.s -o ft_list_size.o
gcc -Wall -Wextra -Werror exo1.c ft_list_size.o -o exo1
```

Objectif trouver la taille de la list avec ft_list_size


## ex01 - ft_list_push_front

Si on prend la definition de la fonction ; 

"the function ft_list_push_front, which adds a new element of type t_list
to the beginning of the list"

en ayant cette signature de fonction donne ;

```C
void ft_list_push_front(t_list **begin_list, void *data);
```

(a noter que c'est une version legerement differente de ce qu'on avait eu en libft pour l'annee 2025 at least)

Donc ca sous entend qu'on va devoir aussi ici allouer notre memoire nous meme, cree notre node.

Donc comme d'habitude un extern malloc et un call a 

```sh
nasm -f elf64 ft_list_push_front.s -o ft_list_push_front.o
gcc -Wall -Wextra -Werror exo2.c ft_list_push_front.o -o exo2
```

## ex02 - ft_list_remove_if

Donc ici on a pas mal de pointeur de fonction => donc fonction non leaf donc il faut faire attention a nos alignement.

on va passer dans toutes nos nodes pour tester et free s'il faut. 

La signature de la fonction ; 

```C
void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));
```

On nous donne des details sur les fonctions 

```
(*cmp)(list_ptr->data, data_ref);
(*free_fct)(list_ptr->data);
```

Donc ca veut dire qu'on va devoir faire notre free du noeud nous meme, la fonction de free_fct ne fait que free le content / la data.

Ensuite ici il y a un autre soucis des le depart, qu'on a eu dans le precedent exercice mais plus grand ici; 

J'ai 4 arguments ; rdi, rsi, rdx et rcx.
Probleme a chaque call d'une fonction je vais pas etre sur qu'aucun de ces registres a pas bouge.

La solution souvent c'etait de push => et apres on pop => probleme ici c'est que je vais devoir faire ca souvent. Donc une solution c'est d'utiliser des registres qui doivent pas etre modifier (rbx, r12 a r15) => mais moi aussi je dois pas les bouger. donc je vais les push sur la stack et les restore a la fin. tout ca pour les utiliser sans me soucier d'aucun probleme a ce niveau la.


## ex03 - ft_list_sort
