section .text
	global ft_atoi_base

check_base:
	mov rcx, -1
.fill:
	inc rcx
	cmp BYTE [rdi + rcx], 0
	je .end
	movzx r8, BYTE [rdi + rcx]
	cmp BYTE [rsi + r8], -1
	jne .wrong
	mov BYTE [rsi + r8], cl
	cmp r8, 32
	je .wrong
	cmp r8, 43
	je .wrong
	cmp r8, 45
	je .wrong
	sub r8, 9
	cmp r8, 4
	jbe .wrong
	jmp .fill
.end:
	cmp rcx, 2
    	jb .wrong
	mov rax, rcx
	ret
.wrong:
	xor eax, eax
	ret


ft_atoi_base:

.setup: 
	push r12
	push r13
	mov r12, rdi
	mov r13, rsi
	mov rcx, -1
	sub rsp, 256
	xor eax, eax
	mov r9, 1
.zeroed:
	inc rcx
	mov BYTE [rsp + rcx], -1
	cmp rcx, 255
	jne .zeroed
	mov rcx, -1
.check_base:
	mov rdi, r13
	mov rsi, rsp
	call check_base
	test rax, rax
	je .end_atoi_base
	mov r11, rax
	mov rcx, -1
	xor eax, eax
.pass_whitespace:
	inc rcx
	movzx r8, BYTE [r12 + rcx]
	test r8, r8
	je .end_atoi_base
	cmp r8, 32
	je .pass_whitespace
	sub r8, 9
	cmp r8, 4
	jbe .pass_whitespace
.pass_sign:
	movzx r8, BYTE [r12 + rcx]
	test r8, r8
	je .end_atoi_base
	cmp r8, 43
	je .change_sign
	cmp r8, 45
	je .change_sign
	jmp .loop
.change_sign: 
	inc rcx
	cmp r8, 45
	jne .pass_sign
	neg r9
	jmp .pass_sign
.loop:
	movzx r8, BYTE [r12 + rcx]
	cmp r8, 0
	je .end_atoi_base
	movsx r10, BYTE [rsp + r8]
	cmp r10, -1
	je .end_atoi_base
	imul rax, r11
	add rax, r10
	inc rcx
	jmp .loop
.end_atoi_base:
	add rsp, 256
	pop r13
	pop r12
	imul rax, r9
	ret
