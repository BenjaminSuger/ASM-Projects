## ex00 - strcmp

```asm
section .rodata
	str1 db "chat", 0;"a", 0
	str2 db "chatons", 0;"b", 0

section .text
	global ft_strcmp
	global _start

ft_strcmp:
	mov rdx, -1
.start:
	inc rdx
	movzx eax, byte [rdi + rdx]
	movzx ecx, byte [rsi + rdx]
	cmp al, cl
	jne .end
	test al, al
	jnz .start
.end:
	sub eax, ecx
	ret

_start:
	mov rdi, str1
	mov rsi, str2
	call ft_strcmp
	mov rdi, rax
	mov rax, 60
	syscall
```

Sur le _start => rien de special => tout fonctionne comme d'habitude.

Le strcmp commence par utilise la meme technique que le strlen avec une valeur a -1 pour bien commencer a 0.

Ensuite on va utiliser l'instruction movzx.
Cette instruction permet de dire je charge dans destination ce que je souhaite de la source, et je met a 0 tout le reste.
Ici on copie donc 1 byte puisqu'on est sur un char. Donc movzx le fait sur 32 bits, il remplit le byte qu'on veut, met le reste de eax a 0. On manipule eax => donc le reste de rax est mis a 0.

On utilise pas movsx car on compare en unsigned int chaque char.

Ensuite on compare les 2 chaines a l'index ou on est.
S'il y a un ecart => on va a la fin.

Si ca marche on va faire l'instruction test.
Test est une instruction souvent utilise d'apres mes lectures pour tester si un registre est a 0. Si on est pas a 0 => on continue la boucle.

Attention a un detail => inc rdx comme beaucoup de calcul modifie les flags.

Il existe aussi une autre technique sans faire de SIMD qui s'appelle le SWAR qui essaie effectivement de faire un masque mais sur un registre. Un peu plus complexe et pas sur que ca vaille le coup ici.

## ex01 - strcpy

```asm
BUF_SIZE equ 64

section .bss
	dest resb BUF_SIZE

section .data
	src db "hello",0

section .text
	global _start
	global ft_strcpy

ft_strcpy:
	mov rcx, -1
.loop:
	inc rcx
	movzx eax, byte [rsi + rcx]
	mov [rdi + rcx], al
	test al, al	
	jne .loop
	mov rax, rdi
	ret

_start:
	lea rdi, [rel dest]
	lea rsi, [rel src]
	call ft_strcpy
	mov rdi, 0
	mov rax, 60 
	syscall
```

Donc ici j'ai decide de faire l'approche de BSS pour envoyer a la fonction
la taille est superieur a ce que j'ai besoin

on reutilise test ici pour savoir si c'est bien 0.
On doit faire attention a la valeur de retour qui est la destination.
sinon c'est un peu la meme technique fondamentalement que l'exo precedent.


## ex02 - strdup

```asm
section .rodata
	str: db "chatons", 0

section .text
	global ft_strdup
	global main
	extern ft_strlen
	extern ft_strcpy
	extern ft_write
	extern malloc
	extern free

ft_strdup:
	push rdi
	call ft_strlen
	inc rax
	mov rdi, rax
	call malloc wrt ..plt
	cmp rax, 0
	je .end
	mov rdi, rax
	mov rsi, QWORD [rsp]
	call ft_strcpy 
.end:
	pop rdi
	ret

main:
	lea rdi, [rel str] ; charge la string
	push rdi
	call ft_strdup
	cmp rax, 0
	je .end_bad
	mov QWORD [rsp], rax
	mov rdi, rax
	call ft_strlen 
	mov rsi, QWORD [rsp]
	mov rdi, 1
	mov rdx, rax
	call ft_write
	mov rdi, QWORD [rsp]
	call free wrt ..plt
	pop rdi
	mov rax, 0
	ret

.end_bad:
	pop rdi
	mov rax, 1
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
```

## ex03 - read

```asm
BUF_SIZE equ 6

section .bss
	dest: resb BUF_SIZE

section .text
	global _start
	global ft_read
	extern __errno_location

ft_read:
	mov rax, 0
	syscall
	cmp rax, 0
	jge .fin
	neg rax
	push rax
	call  __errno_location wrt ..plt
	pop rdi
	mov [rax], edi
	mov rax, -1
.fin:
	ret

_start:
	mov rdi, 0
	lea rsi, [rel dest]
	mov rdx, BUF_SIZE
	call ft_read
	cmp rax, 0
	jl .error
	mov rax, 60
	mov rdi, 0
	syscall

.error:
	mov rax, 60
	mov rdi, 1
	syscall
```

C'est presque un copie colle de ft_write et pour etre honnete en le faisant j'ai regarder mon ft_write.

Dans cette solution j'ai uniquement fait la demarche de pas ecrire avec write ou autres, pour verifier j'ai directement checker avec GDB l'output.


## ex04 check base

Dans l'enonce j'ai indique de regarder pour un exercice avec un "set". 

Si on veut regarder s'il y a un doublon, on pourrait faire une solution un peu simple O(n^2) => je parcours pour chaque char la string pour voir si je rencontre un autre.

C'est une solution possible et acceptable pour ce qu'on fait.

Mais on connait aussi une autre solution avec un set => pas de doublon dans un set. C'est un truc un peu leetcode style en plus donc c'est bien ca nous fait travailler ca. Dans un exercice de pwn college on a vu comment le mettre en place.
On connait les valeurs que peuvent prendre notre char => 256.

Du coup ce qu'on va faire c'est reserver de la stack => pour mettre une indication qu'on a croise cette valeur. Comme ca on aura;
debut de notre stack + <valeur qu'on rencontre> 
c'est des pointers arithmetics classique. Si la casse est vide => all good
si la casse est pas vide => doublon.

Pour ca il faut reserver l'espace "sub rsp, 256" et ensuite la mettre a -1 (donc quand meme un passage a O(n)) => mais ensuite on va pas mettre n'importe quoi dedans => on va mettre l'index ou on a croiser la valeur 

Pourquoi mettre a -1 ? parce que 0 est une valeur possible. La meilleur solution serait surement de prendre une plus grande quantite qu'un BYTE pour mettre une valeur mais normalement par les interdictions qu'on a, on aura jamais de probleme.

```asm
section .text
	global check_base

check_base:
	sub rsp, 256
	mov rcx, -1
.zero:
	inc rcx
	mov BYTE [rsp + rcx], -1
	cmp rcx, 255
	jne .zero
	mov rcx, -1
.fill:
	inc rcx
	cmp BYTE [rdi + rcx], 0
	je .end
	movzx r8, BYTE [rdi + rcx]
	cmp BYTE [rsp + r8], -1
	jne .wrong
	mov BYTE [rsp + r8], cl ; attention a la taille ici 
	cmp r8, 32 ; si c'est un espace
	je .wrong
	cmp r8, 43 ; si c'est un +
	je .wrong
	cmp r8, 45 ; si c'est un -
	je .wrong
	sub r8, 9
	cmp r8, 4 ; si c'est dans le range des spaces
	jbe .wrong
	jmp .fill
.end:
	add rsp, 256
	mov rax, 1
	ret
.wrong:
	add rsp, 256
	mov rax, 0
	ret

```

Pour le test des isspaces() on utilise une comparaison en regardant le range (astuce sur pwn college aussi)

Concretement c'est ce qu'on va utiliser pour le prochain exercice......atoi_base !!!
Le probleme qu'on va avoir ca va surtout etre lie a cette stack qu'on va essayer de faire vivre avant le call a check base. Comme c'est une fonction leaf ca va etre ok mais il va falloir bien gerer tout ca


## ex05 atoi_base


