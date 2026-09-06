## ex00

Premiere chose a connaitre c'est les sections ;

  ┌─────────────────┬──────────────────────────────────────────────────────────────────────────┬──────────────────┐
  │     section     │                                 contenu                                  │   permissions    │
  ├─────────────────┼──────────────────────────────────────────────────────────────────────────┼──────────────────┤
  │ .text           │ code                                                                     │ lecture,         │
  │                 │                                                                          │ exécution        │
  ├─────────────────┼──────────────────────────────────────────────────────────────────────────┼──────────────────┤
  │ .rodata         │ constantes, chaînes littérales                                           │ lecture seule    │
  ├─────────────────┼──────────────────────────────────────────────────────────────────────────┼──────────────────┤
  │ .data           │ variables globales initialisées                                          │ lecture,         │
  │                 │                                                                          │ écriture         │
  ├─────────────────┼──────────────────────────────────────────────────────────────────────────┼──────────────────┤
  │ .bss            │ variables globales non initialisées, zéro au chargement, n'occupe pas de │ lecture,         │
  │                 │  place dans le fichier                                                   │ écriture         │
  ├─────────────────┼──────────────────────────────────────────────────────────────────────────┼──────────────────┤
  │ .note.GNU-stack │ vide, sert juste à dire "pile non exécutable"                            │ aucune           │
  └─────────────────┴──────────────────────────────────────────────────────────────────────────┴──────────────────┘

On va ecrire notre code dans la section .text et lui indique ou commence notre programme. Par convention on appelle cela _start

L'autre chose a connaitre c'est les registres pour les arguments (vous pouvez allez lire 'Computer Architecture' dans le dir posts de ce repo)

|                    | Appel de fonction (ABI System V) |
|--------------------|----------------------------------|
| Sélecteur          | l'adresse (`call func`)          |
| 1er argument       | rdi                              |
| 2e / 3e            | rsi / rdx                        |
| 4e                 | rcx                              |
| 5e / 6e            | r8 / r9                          |
| Déclenchement      | `call`                           |
| Retour             | rax                              |
| Écrase en plus     | —                                |
                                                                                               
ce qui va nous donne le resultat suivant ; 

```asm
section .text
    global _start

_start:
        mov rax, 60 # on prepare le syscall 60 pour utiliser exit()
        mov rdi, 42 # on prepare l'argument de exit qui n'en a que un
        syscall     # on fait le syscall qui va declencer le exit()
```

Maintenant il faut pouvoir compiler comme il faut ; 

```
nasm -f elf64 exit.s
ld exit.s -o exit
```

on peut alors tester si tout fonctionne bien et faire appel a strace pour voir le exit()

## ex01 - objserver la difference

il y a 2 syscall different ; exit() et exit_group()
exit ne termine que le thread appelant alors que exit_group termine les threads du processus. 

Depuis au moins linux 2.7 => exit() utilise exit_group()

Dans la pluspart des cours en ligne sur l'ASM de nos jours on continue d'utiliser le syscall de exit (60) au lieu du 231 pour exit_group() c'est sans importance du moment ou on a pas a gerer des threads.

## ex02 - observabilite avec GDB 

on peut lancer gdb meme si on a pas fait le flag de debug avec la commande 

```sh
gdb ./exit
```

vous pouvez faire la commande ;
```sh 
set disassembly-flavor intel 
```
pour bien avoir la syntax intel. Mais si vous voulez pas taper ca a chaque fois dans votre GDB. Vous pouvez mettre en place des settings

```sh
echo "set disassembly-flavor intel" > ~/.gdbinit
```

Ensuite comme on a pas les flag de debug il y a des petites choses a faire de maniere differente; 

```sh
info files   # peut etre utile dans certains CTF la pas obligatoire
starti       # ici on precise la lettre i pour instruction
disas _start # pour voir notre programme
layout asm   # pour voir le split fenetre mais il faut bien retaper disas
ni           # pour next instruction
si           # pour step in instuction
p $eax       # print le registre eax pour un registre il faut toujours metre un $ devant
info registers # pour print tous les registres et flags
```

petit detail ; meme si on a utilise rax, il est possible de voir eax, c'est un concept dans les registres (voir dir posts a ce sujet) => c'est nasm qui fait ca c'est une petite optimisation

## ex03 - compile avec flag de debug

nasm -f elf64 -g -F dwarf exit.s

le DWARF est le moyen plus moderne que GDB accepte facilement pour avoir les informations de debug sinon par defaut nasm fait du STABS

