## ex00 - basic calc

```asm
section .text
	global _start

_start:
	mov rax, 10
	mov rbx, 50
	add rax, rbx
	mov rdi, 100
	mov rcx, 99
	sub rdi, rcx
	syscall
```

Dans cette maniere d'illustrer, ce qui est interessant d'illustrer c'est de comprendre que le premier registre apres le add est modifie.

Tandis que le second lui reste inchange.

L'autre point interessant a observer est sur la presence des flags.
A la premiere addition "eflags" est modifie.

En examinant avec GDB vous verrez, que eflags a une valeur de [ PF IF] 

IF => Interrupt Flag => indique que les processeur accepte els interruptions materielles
PF => PARITY FLAG =>  Set to 1 if the lowest byte of the result contains an even number of set bits (1-bits); otherwise set to 0 (odd parity)

Il existe d'autres flags ; 

CF => CARRY FLAG => sur les unsigned permet de signaler un overflow

OF => OVERFLOW FLAG => perment comme CF de signaler un overflow mais sur les signed

ZF => ZERO FLAG => s'active si le resultat de l'operation est 0

SF => SIGN FLAG => permet de savoir si le resultat est negatif

