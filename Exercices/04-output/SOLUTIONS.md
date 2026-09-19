## ex00 - Ecrire

Jusqu'a present on avait que des informations dans la section .text qui est la ou on a notre code.

Ici on veut mettre du texte en dur dans le programme et pour ca il faut le mettre dans une autres section.

On a plusieurs choix techniquement ; 
.rodata => read only data
.data   => data => variable en lecture et ecriture

On va le plus souvent mettre une chaine de char * dans .rodata. On le voit en C si vous declarer la chaine "les chats" => vous essayer de la modifier ce n'est pas possible car lecture seul.

comme la section text on la declare ainsi ;

```asm
section .rodata
    msg db "Les chats sont mignons", 10, 0
```

Mais a quoi correspond tout ca ?

**msg** c'est le nom qu'on lui donne, c'est le nom de la variable

**db** Pour "define byte" definit une suite d'octets. Il existe bien sur d'autres solutions en fonction des cas dans le tableau si dessous

┌───────────┬───────────┬────────────────────────────┐
│ Directive │  Taille   │          Exemple           │
├───────────┼───────────┼────────────────────────────┤
│ db        │ 1 octet   │ db "abc", 10               │
├───────────┼───────────┼────────────────────────────┤
│ dw        │ 2 octets  │ dw 0x1234                  │
├───────────┼───────────┼────────────────────────────┤
│ dd        │ 4 octets  │ dd 1000000 ou dd 3.14      │
├───────────┼───────────┼────────────────────────────┤
│ dq        │ 8 octets  │ dq 0xDEADBEEF ou dq 2.5    │
├───────────┼───────────┼────────────────────────────┤
│ dt        │ 10 octets │ dt 3.14 (float étendu x87) │
└───────────┴───────────┴────────────────────────────┘

**"Les chats sont mignons"** jusque la rien de nouveau ce sont des chats apres tout

**10** Cela correspond au code pour le '\n' dans la table ascii 

**0** Precise que la chaine de char est termine par un NULL. Pas forcement necessaire pour ce qu'on va faire car on va utiliser write. Mais si on avait utilise un fonction libc => different.

Les 2 dernieres indications ne sont pas obligatoires on peut avoir plusieurs cas comme ;

db "Hello", 10          ; "Hello\n"
db "Hello", 13, 10      ; "Hello\r\n" (style Windows)
db "Hello", 9, "world"  ; "Hello<tab>world"
db "Hello", 0           ; chaîne C, terminée par null
db "Hello"              ; rien du tout
db "Hello", 33, 10      ; "Hello!\n" (33 = '!')
db "Hello", 'A'         ; les caractères entre quotes marchent aussi

Une fois qu'on a fais ca on peut deja ecrire techniquement notre programme comme il faut

```asm
section .rodata
	msg db "Les chats sont mignons", 10, 0

section .text
	global _start

_start:
	mov rdi, 1
	mov rsi, msg 
	mov rdx, 23
	mov rax, 1
	syscall
	mov rax, 60
	mov rdi, 0
	syscall
```

On peut separer en 2 le programme ; l'appel a write et l'appel a exit

L'appel a write, qui a besoin de 3 arguments ; fd, pointer et length
On charge les arguments dans l'ordre des registres prevu pour (voir /posts => Computer Architecture) => rdi premier argument, rsi second argument et rdx troisieme argument.
Le syscall de write est 1 qu'on met dans rax.

Ici j'ai precise la longueur moi meme en dur dans le troisieme argument. Il y a d'autres moyens de faire.

On peut dans la section precise la longueur de cette maniere ;

```asm
section .rodata
	msg db "Les chats sont mignons", 10, 0
	msgLen equ $ - msg

section .text
	global _start

_start:
	mov rdi, 1
	mov rsi, msg 
	mov rdx, msgLen
	mov rax, 1
	syscall
	mov rax, 60
	mov rdi, 0
	syscall
```

Ici c'est la constante equ qui donne la taille de la chaine.

## ex01 - Fonction

On va du coup reutiliser totalement notre code precedent. Mais quoi utiliser ? 
La preparation des arguments ; fd, message, lenght doit etre faite comme avec le syscall en preparation avant le lancement de la fonction.

Maintenant la fonction en elle meme comment on la declare ? 
Dans la section .text on va preciser comme _start qu'on a une fonction que j'appelle ici write.

```asm
section .rodata
	msg db "Les chats sont mignons", 10, 0
	msgLen equ $ - msg - 1

section .text
	global _start
	global write ; nouveaute

write:           ; nouveaute
	mov rax, 1
	syscall
	ret
_start:
	mov rdi, 1
	mov rsi, msg 
	mov rdx, msgLen
	call write
	mov rax, 60
	mov rdi, 0
	syscall
```

Mais je n'etais pas oblige de faire ca ! je pouvais ne pas l'ecrire ca aurait fonctionne aussi. Pourquoi ? global est important pour _start car sinon ld emet un warning et arrive a s'en sortir avec le debut de la section .text => probleme on commence par la fonction write au lieu de _start......pas top car va planter.

Mais la pour cette fonction "helper" il y a pas besoin car il est capable de trouver seul dans le fichier. 

Cet ajout permet d'utiliser la fonction dans un autre fichier si on en avait eu besoin. On en reparlera dans une autre section d'exos

Ca un impact sur l'ELF que vous pouvez observer avec la commande nm ; 
0000000000403000 R __bss_start
0000000000403000 R _edata
0000000000403000 R _end
0000000000401008 T _start
0000000000402000 r msg
0000000000000018 a msgLen
0000000000401000 T write

on voit le symbole de write avec un T majuscule et sans global avec un t minuscule. Cette difference c'est justement le scope

Le reste de la fonction fait juste un appel a rax pour preparer le write, faire le syscall et ensuite faire un return (ret).

Si on avait pas ce "ret" qu'est ce qu'il ce passe ? 
On aurait une boucle infini de write !
Les instructions sont lu ligne par ligne => apres le syscall ca continue de lire et d'appliquer jusqu'au "call write". Ce qui le fait venir dans la fonction en boucle. En realite ca sera pas vraiment une boucle infini mais plutot un segfault / stack overflow un jour.

Autre point interessant, c'est si vous faites tourner ca dans GDB et que vous observer le registre rax, apres le syscall il a change de valeur. C'est le retour du syscall qui prend la valeur de la longueur de la chaine de msg.


## ex02 - Longueur

```asm
section .text
        global _start
        global ft_strlen

ft_strlen:
        or rax, -1 ; plus lent qu'un mov rax, -1 il semblerait mais rigolo
.loop:
        inc rax
        cmp byte [rdi + rax], 0
        jne .loop
        ret

_start:
        mov rdi, QWORD [rsp + 16] ; je charge le 1er argument
        call ft_strlen
        mov rdi, rax
        mov rax, 60
        syscall
```

Encore une fois le reflexe pas simple a avoir sur la fonction ft_strlen c'est de faire une boucle avec jne et pas avoir 2 jump (dont un qui arrive quoi qu'il arrive).

Du coup pour avoir moins d'instruction au tout debut on met a -1 avant d'increase (attention a bien le faire sur rax et pas eax et voir ce que ca implique).

La premiere ligne vaut une explication car elle a plusieurs implications;

Un ou logique permet de mettre a la valeur -1 comme on le souhaite.

Elle presente un avantage, l'instruction represente moins d'opcodes, moins de bytes utlise pour l'instruction qu'un "mov rax, -1".

Cependant ca ne veut pas forcement dire que c'est mieux et plus efficace. Quand un mov va simplement ecrire dans le registre peut importe la valeur presente. Le or logique, doit lui regarder a l'interieur de rax (ce qui est un probleme de dependance).

On parle pas d'une grande optimisation ou d'une perte majeur mais ca compte et surtout c'est interessant de comprendre tout ca je pense.

En parlant d'optimisation, il existe une partie du CPU dont on a jamais parle; le SIMD.

Dans ce directory vous trouverez un minicours a ce sujet (genere par IA) qui parle de comment ecrire une ft_strlen avec SIMD, ce qui ne vas pas etre explore ici plus en detail par manque de temps. Cependant j'encourage fortement cette lecture pour comprendre l'architecture moderne du CPU et s'eloigner de certains cours qui reste sur des anciennes notions.
