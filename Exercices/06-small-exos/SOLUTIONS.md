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
