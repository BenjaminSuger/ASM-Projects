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
