## ex00 un code en C

Donc pour rappel le code de ft_putstr ; 

```asm
section .text
	global ft_write
	global ft_strlen
	global ft_putstr
	extern __errno_location

ft_strlen:
        mov rax, -1
.loop:
        inc rax
        cmp byte [rdi + rax], 0
        jne .loop
        ret

ft_write:
	mov rax, 1 
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

ft_putstr:
	push rbx
	mov rbx, rdi
	call ft_strlen
	mov rsi, rbx
	mov rdi, 1
	mov rdx, rax
	call ft_write
	pop rbx
	ret
```

Il y a un ft_write mais aussi un ft_strlen inclut. Ceux de la libasm.

A noter qu'il y a pas de _start, puisque la le main de la fonction ca va etre justement notre code en C fournit dans le README.

Du coup si on connait bien comment la compilation fonctionne en C on sait qu'on a besoin d'object files. 

Donc on pourrait compiler avec notre C file transforme en .o ensuite on link tout.
Mais ! gcc peut nous faire le travail plus simplement. 

On compile avec nasm comme d'habitude notre ft_putstr.s pour faire un .o et on peut tout de suite faire ca ;

```sh
cc test.c ft_pustr.o 
```

c'est tout ? 

non il manque bien sur la forward declaration de ft_putstr().
En sachant qu'ici mon ft_pustr() a une particularite un peu precise, il renvoie un int par defaut.

donc on peut mettre

```c
int ft_putstr(char *str);

int main(void) {
	ft_putstr("les chats sont mignons\n");
	return 0;
}
```
et la ca va fonctionner et etre ok.

Ca fonctionne mais on a un warning pour l'eviter il faut preciser la chose suivante dans notre code en asm;
"section .note.GNU-stack noalloc noexec nowrite progbits"

Ce que ca implique ; 
- section .note.GNU-stack : le nom exact que le linker cherche. Un autre nom ne serait pas reconnu.
- noalloc : ne pas charger la section en mémoire à l'exécution. Elle existe seulement dans le fichier .o.
- noexec et nowrite : les flags de la section elle-même. Ce sont eux que le linker lit pour deviner tes besoins. Si tu mettais exec ici, tu demanderais au contraire une pile exécutable.
- progbits : le type de section, ici « données du programme ». Elle fait 0 octet, comme le montre readelf -SW sur le .o.


Mais une question interessante ce pose assez vite ! est ce que je renvoie vraiment un int ? Par definition oui mon ft_pustr finit avec un RAX qui a une value par sont ft_write. Mais est ce que c'etait volontaire ? non.

Du coup si maintenant on recompile.....ca marche aussi ! 

En fait le compilateur ne sait rien du .s de base. Avec une declaration en int ou en void le comportement de GCC est different. Il peut etre amenner a utiliser RAX tout de suite apres. 

On peut donc mentir au compilateur. Ce qui est a eviter car ca pourrait cree des soucis plus tard.

Est ce qu'on pouvait pas aussi donner directement notre .s a gcc ? si mais dans notre cas avec la syntaxe Intel ca n'aurait pas fonctionner.

