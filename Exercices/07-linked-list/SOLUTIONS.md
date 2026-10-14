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
