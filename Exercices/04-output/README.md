# 04 output

Objectif ; commencer a voir comment faire pour sortir un output autre que la simple valeur de retour
Second objectif ; produire une fonction ft_write (qui respecte man 2 write) pour le projet libft.

## ex00 - Ecrire

Ici l'objectif est d'ecrire un output dans le terminal ; "Les chats sont mignons"

Pour ca quelques precisions => on va utiliser le syscall vers write (man 2 write)
La string est pour le moment totalement en dur dans le programme (c'est un des challenge, de savoir ou le mettre dans notre code en assembleur)

## ex01 - Fonction

Jusqu'a present on a fait tout dans le _start, qu'on imagine un peu comme notre fonction main() (meme si c'est pas tout a fait le cas mais c'est une approximation pour simplifier).

Mais c'est embetant de faire ca, donc on va separer ca dans une fonction qu'on appelle aussi subroutine.

On refait la meme chose que dans l'ex00 on a une chaine de char dans notre programme qu'on print.

## ex02 - Longueur

On a appris les boucles et les additions. Maintenant si on lie le tout ensemble ?

On va donc calculer la longueur de notre chaine de caractere et le faire aussi dans une subroutine.
La l'objectif c'est juste de compter la longueur de la chaine de char qui vient d'un 

Le programme retourne la longueur trouve; 

```sh
./len "chats"
echo $? # doit print 5
./len ""
echo $? # doit print 0
```

On va donc produire ft_strlen demander par la libasm

## ex03 - ft_putstr

On a pas mal de chose qu'on peut assembler ensemble 

## ex04 - ft_write

Ici objectif de bien suivre la consigne de la libasm en produisant ft_write qui suit man 2 write.
A premiere vue, aucun interet car on ne met pas ft_strlen dedans et on a fait plus complexe avant. 
Mais la c'est surtout les valeurs de retour qui vont nous interesser et la gestion des erreurs pas vu encore.
