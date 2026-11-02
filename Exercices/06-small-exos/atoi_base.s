section .text
	global ft_atoi_base
	global check_base ; peut etre a mettre en equivalent static

check_base:
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
	cmp rcx, 2
    	jb .wrong
	add rsp, 256
	mov rax, 1
	ret
.wrong:
	mov rax, 0
	ret


atoi_base:

.setup: 
	push r12
	push r13
	mov r12, rdi
	mov r13, rsi
	mov rcx, -1
	sub rsp, 256

.zeroed:
	inc rcx
	mov BYTE [rsp + rcx], -1
	cmp rcx, 255
	jne .zeroed
	mov rcx, -1

.check_base: ;
	mov rdi, r12
	call check_base
	cmp rax, 0
	je .end_wrong
	mov rcx, -1
.pass_whitespace:; surement utiliser r8 pour les manipulations
		


.loop:


.end:

.end_wrong:
	add rsp, 256
	pop r13
	pop r12
	mov rax, 0
	ret
