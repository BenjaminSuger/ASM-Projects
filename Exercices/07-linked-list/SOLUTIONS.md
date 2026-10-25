## ex00 - ft_list_size

```asm
struc t_list
    .data resq 1
    .next resq 1
endstruc

section .text
	global ft_list_size

ft_list_size:
	mov rcx, 0 ; servir de compteur
	cmp rdi, 0 ; on check deja si rdi est pas nul
	je .end
.loop:
	inc rcx
	mov rdi, [rdi + t_list.next]
	cmp rdi, 0
	jnz .loop
.end:
	mov rax, rcx
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
```

Alors cette version clean m'a pris un peu de temps. 

On commence avant la boucle a bien verifier si ce n'est pas null.
Si ce n'est pas le cas on peut tout de suite compte 1 noeud. 
Puis on va changer le pointeur pour qu'il continue vers next. 
On fait un test si notre next est null => pas d'autre noeud on peut renvoyer la valeur. sinon c'est bon on continue notre boucle.

Toujours cette gymnastique mental sur les jump pour eviter d'en avoir plusieur ce qui etait mon cas au debut a ma premiere ecriture.


## ex01 - ft_list_push_front

```asm
struc t_list
    .data resq 1
    .next resq 1
endstruc

section .text
	global ft_list_push_front
	extern malloc

ft_list_push_front:
	test rdi, rdi
	jz .end
	push rdi
	push rsi
	sub rsp, 8
	mov rdi, t_list_size
	call malloc wrt ..plt
	add rsp, 8
	pop rsi
	pop rdi
	test rax, rax
	jz .end
	mov [rax + t_list.data], rsi
	mov rdx, [rdi]
	mov [rax + t_list.next], rdx
	mov [rdi], rax
.end:
	ret

section .note.GNU-stack noalloc noexec nowrite progbits

```

On reprend toujours notre petite aide pour "struc t_list" de debut de fichier.

J'essaie de plus souvent utiliser test qui semble etre plus idiomatique car economise par rapport a un cmp.

On va utiliser malloc, donc il faut penser a tout ce que ca implique => c'est a dire l'alignement avant le call + les valeurs des registres qu'on veut peut etre garder.
Probleme ici c'est qu'on a plein de chose a push et on finit quand meme desaligne donc je fais mon ajout a moi de rsp de 8.


