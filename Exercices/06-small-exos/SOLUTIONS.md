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
