# Exercice 03 - Maths

Objectif; parler des operations de base sur les registres. Les faire et les observer.
Aussi aller plus loin sur le comportement des registres.

## ex00 - basic calc

On peut faire en programmation des operations assez simple; + - *
On peut aussi le faire en ASM du coup ! 

Mais les instructions qui permettent de faire ca sont un peu particulier

L'objectif ici est juste de mettre des valeurs dans des registres et de faire des operations dessus.
Ensuite observe ce qui ce passe avec les registres

Faites un programme qui juste va quitter avec le syscall exit mais que ca soit le syscall ou la valeur 
a laquel vous souhaitez quitter soit obtenu par un add et un sub. Ce qui permettra d'observer apres avec
GDB le comportement specifique de ces operateurs.

## ex01 - div, idiv et modulo 

*rien a faire en pratique a part observer*

Pour la division c'est un peu plus tricky a realiser.

C'est pas une operation super rapide en plus pour le CPU.

Comment ca fonctionne ? 

Quand on veux faire x / y 
On place x dans RAX, et y dans RCX (ca peut etre n'importe quel registre)
On a besoin de bien vider rdx souvent un xor avec lui meme pour vider (voir plus tard)

ensuite on call div avec RCX pour faire l'operation

```asm
mov  rax, 17      ; dividende
xor  edx, edx     ; rdx = 0 (partie haute)
mov  rcx, 5       ; diviseur (jamais un immédiat, toujours reg ou mem)
div  rcx
```

Le resultat est mis dans rax et le reste dasn rdx

Pour le cas des des divisions signe; 

```asm
mov  rax, -17
cqo               ; étend le signe de rax dans rdx (rdx = 0xFFFF...)
mov  rcx, 5
idiv rcx
; rax = -3
; rdx = -2
```

on ajoute une operation avec cqo pour bien gerer les signes.

Pas de reel exercice ici mais vous pouvez essayer de le faire.

## ex02 - registres manipulation

dans le precedent exercice j'ai fais une operations xor.

Operation logique qui est possible comme ; and, or, not 

Mais dans cette operation j'ai utilise edx pour manipuler rdx, comment c'est possible ?

On est sur une machine 64 bits, donc nos registres font 64 bits. Mais on peut acceder a des sous parties de nos registres ;

```
+-------------------------------------------------------------------------------+
|                                      RAX                                      |  (64 bits)
+-------------------------------+-----------------------------------------------+
|            32 bits            |                      EAX                      |  (32 bits)
+-------------------------------+-------------------------------+---------------+
                                |            16 bits            |      AX       |  (16 bits)
                                +-------------------------------+-------+-------+
                                                                |  AH   |  AL   |  (8 bits chacun)
                                                                +-------+-------+
```

Il y a bien sur des raisons historiques sur le fait que nos ordinateurs ont evoluer mais c'est aussi tres pratique car il y a certaines "regles" a connaitre.


```asm
section .text
       global _start
_start:
          mov rax, 0x4545353243
          mov rax, 60
          mov rdi, 0
          syscall
```

dans un code comme ca on peut print les valeurs avec les noms des "sous registre"

```gdb
p/x $al  # return 0x43
p/x $ah  # return 0x32
p/x $eax # return 0x45353243
```

mais c'est surtout qu'on peut les manipuler

```asm
section .text
       global _start
_start:
          mov rax, 0x4545353243
          mov eax, 0x1 ; nouvelle ligne
          mov rax, 60
          mov rdi, 0
          syscall
```

avec cette nouvelle ligne on peut changer la partie ou on a eax mais surtout ca change aussi la partie haute des 32 autres bits.

La regle "Zero-Extension gratuite" => toutes manipulation sur le registre 32 bits met a 0 la partie haute.

C'est une operation qui est souvent fait par le compilateur / NASM meme qu'on essaie de manipuler rax manuellement.

C'est une petite optimisation qui permet de gagne sur la taille de l'instruction

```asm
section .text
       global _start
_start:
          mov rax, 0x4545353243
          mov al, 0x42 ; nouvelle ligne
          mov ah, 0x64 ; nouvelle ligne
          mov rax, 60
          mov rdi, 0
          syscall
```

ici on peut voir que la regle ne se respecte pas car on touche a des plus petits sous registre mais ca permet de faire les modifications comme il faut.

a la ligne de mov al si on print on aura => 0x4545353242
a la ligne de mov ah si on print on aura => 0x4545356442

Mais du coup comment acceder specifiquement au partie qui n'ont pas de nom ? 

Pour ca il faut alors utiliser des operations logique de shifting avec des operations comme 
shl => shift left 
shr => shift right
