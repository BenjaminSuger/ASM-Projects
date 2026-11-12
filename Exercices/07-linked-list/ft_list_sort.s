struc t_list
    .data resq 1
    .next resq 1
endstruc

section .text
	global ft_list_sort
	extern ft_list_size

ft_list_sort:
	cmp rdi, 0
	je .finish
	cmp rsi, 0
	je .finish

	push r12 ; t_list **begin_list
	push r13 ; int (*cmp)()
	push rbx
	push r14
	mov rbx, -1
	mov r12, rdi
	mov r13, rsi
	mov rdi, [rdi]
	sub rsp, 8 ; sale mais bon
	call ft_list_size
	add rsp, 8 ; sale mais bon 
	cmp rax, 1
	jle .end
	mov rcx, rax
	dec rbx; comme je trouve 2 ma boucle doit faire 0 et 1 mais c'est surement rbx plutot a changer
.loop:
	inc rbx
	cmp rcx, rbx
	je .end 
	; jump dans un autre label qui lui aussi revient ici
	mov r14, [r12] 
	jmp .pass

.pass: ; ma boucle qui parcours la liste chaine fait le compare et avance 
	cmp [r14 + t_list.next], 0
	je .loop
	mov rdi, [r14 + t_list.data]
	mov rsi, [r14 + t_list.next]
	mov rsi, [rsi + t_list.data]
	push rcx
	call r13
	pop rcx
	test eax, eax
	jg .switch ; change pour jg
	mov r14, [r14 + t_list.next]
	jmp .pass
.switch: 
	mov rdi, [r14 + t_list.data] ; courant
	mov rsi, [r14 + t_list.next]
	mov rsi, [rsi + t_list.data] ; suivant
	mov [r14 + t_list.data], rsi ; donc la j'ai change le node courant
	mov rax, [r14 + t_list.next] ; je pourrais opti plus tard quand ca va marche
	mov [rax + t_list.data], rdi ; donc la j'ai change le node suivant

	mov r14, [r14 + t_list.next]
	jmp .pass

.end:
	pop r14
	pop rbx
	pop r13
	pop r12
.finish:
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
