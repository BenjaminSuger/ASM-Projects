## ex00 - Observer les arguments avec GDB 

votre programme va commencer avec quelque chose de cet ordre; 

    114d:       55                      push   rbp
    114e:       48 89 e5                mov    rbp,rsp
    1151:       48 83 ec 10             sub    rsp,0x10
    1155:       89 7d fc                mov    DWORD PTR [rbp-0x4],edi
    1158:       48 89 75 f0             mov    QWORD PTR [rbp-0x10],rsi

Si on regarde ce qu'il se passe 

push rbp => sauvegarde la valeur de rbp sur la stack
mov rbp, rsp => maintenant rbp pointe vers cette sauvegarde. Ce qui fait que rbp est un point d'encrage utilise pour la suite.
sub rsp, 0x10 => reserver de l'espace memoire pour les variables locales
les deux dernieres lignes c'est la ou on retrouve argc, dans edi et argv dans rsi.

Pour les examine dans GDB ; 
```sh
gdb --args ./a.out chats
```

ensuite pour avoir la valeur de argc;
```sh
p $edi
```

pour voir les argv, il faut le plus souvent faire un petit cast
il existe differentes maniere de faire plus ou moins sympa
```
p ((char **)$rsi)[0]
p {char *}$rsi
p {char *}($rsi + 8) # pour avoir le second argument oblige de faire l'addittion
```

est ce que si on ecrit nous meme notre code en assembly on les retrouve au meme endroit ?
Non, car la le compilateur suit la libc et sa maniere de gerer mais nous par defaut on a rien a suivre. Donc notre rsp va avoir argc, suivit de argv et meme de envp par la suite. C'est a nous de le gerer tout seul.

## ex01 - Suite logique notre propre stack a observer

On peut reprendre le code de la derniere fois ;

```asm
section .text
	global _start

_start:
	mov rax, 60
	mov rdi, 42
	syscall
```

Pour la compilation on peut le faire avec les flags de debug comme il faut; 

```
nasm -f elf64 -g -F dwarf 
```

ensuite maintenant on peut observer avec les commandes print qu'on a vu precedement (je pars du principe que vous avez un .gdbinit ou alors que vous savez faire 'set disassembly-flavor intel' meme si la c'est pas le plus important)

```sh
gdb ./exo 
starti les chats sont mignons #c'est pour bien demarrer avec des arguments du main
```

Ici l'interet c'est de vous parler / pratiquer la commande inspect (x) pour print les valeurs.

Comment ca marche ?
x/NFU <adresse>
N = combien d'unite a afficher => par defaut 1
F = format d'affiche et on a le choix entre x (hexadecimal), d (decimal), u (unisgned int), s (string), c (caractere), i (instruction), t (binaire) et a (adresse avec symbole)
U = la taille d'une unite b = 1 octet, h = 2 octet, w = 4 octets, g = 8 octet

Tout est optionnels

```asm
p $rsp      #on voir l'adresse top of the stack
x $rsp      #on demande juste d'inspect la valeur de rsp
x/d $rsp    #on voit la valeur en decimal
```

La on prend le top de notre stack mais nous on veut aller voir les arguments du main maintenant.

Donc on va faire un calcul de "pointer arithmetique" en ajoutant + 8

Pourquoi + 8 ?

C'est la ou le terme WORD vient pas mal nous embrouiller on a souvent cette notion sur Intel et NASM (voir /posts); 

┌──────────┬──────────┬──────────┬─────────────┐
│ Nom NASM │  Taille  │ Registre │ Suffixe gdb │
├──────────┼──────────┼──────────┼─────────────┤
│ BYTE     │ 1 octet  │ al       │ b           │
├──────────┼──────────┼──────────┼─────────────┤
│ WORD     │ 2 octets │ ax       │ h           │
├──────────┼──────────┼──────────┼─────────────┤
│ DWORD    │ 4 octets │ eax      │ w           │
├──────────┼──────────┼──────────┼─────────────┤
│ QWORD    │ 8 octets │ rax      │ g           │
└──────────┴──────────┴──────────┴─────────────┘

mais ici le +8 on parle du word dans le sens que c'est la taille d'un pointeur et d'un registre en 64 bits mais c'est un QWORD pour NASM.

Donc meme si le premier arguments est int argc, on a une case qui est plus grande. D'ailleurs c'est pour ca que vous avez pu dans le premier exercice voir des choses comme edi manipuler pour charger argc.

donc pour print le premier argument ; 

```sh
x/s {char*} ($rsp+8)
```

ensuite on peut faire +16 +24 +32 etc......

On peut aussi print comme il faut les valeurs d'un coup avec 

```sh
x/5s {char*} ($rsp+8)
```

si on continue on va continuer sur envp 
