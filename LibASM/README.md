# LIBASM

*This project has been created as part of the 42 curriculum by bsuger*

Summary: The aim of this project is to become familiar with assembly language.

## THE PROJECT

We are building a library (libasm.a) written in assembly with the following functions:

◦ ft_strlen (man 3 strlen)
◦ ft_strcpy (man 3 strcpy)
◦ ft_strcmp (man 3 strcmp)
◦ ft_write (man 2 write)
◦ ft_read (man 2 read)
◦ ft_strdup (man 3 strdup)
◦ ft_atoi_base 
◦ ft_list_push_front
◦ ft_list_size 
◦ ft_list_sort 
◦ ft_list_remove_if

We are allowed to call the extern errno_location.
We should check errors during syscalls and handle them properly when needed.

## REQUIREMENTS

To launch the project you'll need:
-A Linux distribution (x86_64)
-Make
-NASM
-LD

## EXPLANATIONS

This part will explain more about each functions and some concepts.
You'll find more details in my repo on github with the section "Posts"

### 'It is forbidden to use the compilation flag: -no-pie'

*This sentence comes from the subject*

While the idea of doing it is kind of crazy! We need to understand what we are talking about.
PIE stands for Position-Independent Executable, it means that each time we launch our ELF it is launched from a random place in
virtual memory and does not impact the program. It's not PIE doing it, it's the ASLR from the kernel doing it while PIE makes it possible. So we don't have hard-coded memory addresses inside our code and 
we avoid easy ROP (Return-Oriented Programming) exploits (it's when you know where you have to offset the code to take control).

So this mention in the subject is to make sure we do it the right way.

### What is x86_64

Computers come with different CPU, the CPU can have different architecture. x86 is one of them!
The "_64" is here to precise it's 64 bits.

This whole project is built for this architecture.

### What is the system V ABI ?

System V ABI is a standard specification that defines low-level binary compatibility for compiled application programs running on Unix and Unix-like operating systems.

It defines also what we call the "Calling Conventions". Which is important for our project for a specific detail ; 

-registers callee-saved ; the one we need to restore if modify during our function (rbp, rsp, rbx, r12-r15)
-registers caller-saved ; the called function can modify them without any issue (all other registers)

The arguments of functions are (in orders) put in those registers ; rdi, rsi, rdx, rcx, r8, r9.
The return value of functions or syscalls is in rax.
To use a syscall, you use rax to put the syscall number.

### Man

In the man 2 syscall you can find valuable information if you forget the order of arguments and return values.
Also I found interesting information in man 2 intro about the syscalls and wrapper libc.

### Intel and AT&T syntax

We write with Intel syntax as God intend to. That's it !

### leaf and non leaf function

Leaf function => does not call any other function
Non leaf function => call another function

With a Non leaf function you have to guarantee rsp align on 16 for each call.

### section .note.GNU-stack noalloc noexec nowrite progbits

You'll find this indication at each of the function. It's to avoid warnings during the use with C code.
Without this indication the linker is assuming that it needs executable stack (an executable stack can be used for nested functions in C for example).

### ft_strlen

Where we met our first loop and it's where you learn tips to start the counter with -1. Which allow you to write a better loop.
There is also another tips interesting to set the register to -1 with the or instruction. While or is an 4 bytes and mov 7 bytes size, or is dependent from the previous value inside the register.

### ft_write

First function where you encounter errno. 
You do your syscall => the value inside rax can be negative if there is an error (a value between -1 and -4095).
This value will help us for __errno_location => we need to call this extern function with wrt (for NASM, stands for With Respect To) and ..plt (Procedure Linkage Table => Global Offset Table).
The errno_location function gives us an address in rax where we can put the value found previously. 
In my solution I decided to push rax, to align value (ABI System V rules of alignment 16 bytes) and not put it into a specific register and also needed to save the value).

### ft_strcmp

Interesting function where you see 2 new instructions ; 
*test*
=> a usual way to test if a register is 0 or not (using it on himself) its an and operation which throw its result.
=> xor write in a register the result while test only change the flags
=> test size is smaller than cmp (2 vs 3 bytes)

*movzx*
=> used to mov + zeroed the part of the registers 
=> done here because we have to compare unsigned char (so we don't use movsx)
=> We do it on eax like this the other part is zeroed also.

### ft_strdup

Function where you have to reuse other functions ; ft_strlen and ft_strcpy.
To do that you just have to declare them as extern.
As ft_write you have to be careful avout the alignment of 16 bytes.

### struct and list.inc

A very interesting part of the bonus is the use of struct. We know them from C programming language. In assembly programming it's very interesting to see them "crude". We knew from the begining it was a memory layout (with padding possible) but as we don't have any struct keyword (in some sort yes but keep reading this please) we see that it's mainly pionters arithmetic.

Hopefully there is a way of writing the offset in a more beautiful way with NASM. 

```asm
struc t_list
	.data:	resq 1
	.next:	resq 1
endstruc
```

The indication "resq 1" allow you to allocate 8 bytes the size of a pointer.

So now if we want to use it we can simply do;

```asm
mov rax, [rdi + t_list.next]
```

Like this we have rax getting the content of next.

We have 2 options to use this struc. First we can put it at the top of each of our files touching the stuctures. Second option like in Libft programming, we can use an include. It is very similar to do libft.h but here it's a list.inc.

We will also include it at the beginning of the file where needed;

```asm
%include "list.inc"
```

### to do later

-image registres en haut de la pyramide pour le CPU et peut etre image CPU un peu
-NASM et LD
-les registres
-program section
-pourquoi l'alignement de 16 bytes => a cause des registres SSE (xmm0 a xmm15) qui font 128 bits => 16 bytes => du coup en correction attention a ca, on peut verifier par gdb

## RESOURCES

[PWN COLLEGE](https://pwn.college/): pwn.college is a program from Arizona State University. Everything is free! Labs with videos about cybersecurity. I absolutely love it!
For preparing Libasm you should use the "Computing 101" path but also less known to people in the archives you'll find useful ressources in "Assembly crash course" (which is
the first course done in asm by PWN College).

[Linux Syscall table](https://filippo.io/linux-syscall-table/)

[Tips for golfing in x86_64 machine code](https://codegolf.stackexchange.com/questions/132981/tips-for-golfing-in-x86-x64-machine-code) Where I learned about size instruction and tips for ft_strlen to put -1 at a minimum cost (or operator but not used in the strlen I did for other consideration)

[Set all bits in CPU register to 1 efficiently](https://stackoverflow.com/questions/45105164/set-all-bits-in-cpu-register-to-1-efficiently)

[x86 video playlist from OliveStem](https://www.youtube.com/watch?v=yBO-EJoVDo0&list=PL2EF13wm-hWCoj6tUBGUmrkJmH1972dBB)

[How does glibc make a system call in Linux?](https://youtu.be/NVXJZUa0kKo?si=XLUzi-4USffP0Btq)

[CPU Land](https://cpu.land/)

[Calling Convention](https://en.wikipedia.org/wiki/Calling_convention)

[Where the top of the stack is on x86](https://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86)

[NASM Tutorial](https://cs.lmu.edu/~ray/notes/nasmtutorial/)

[ASM Tutor](https://asmtutor.com/) : Great exercices but the solution is given too fast. Not sure what was the plan about that initialy. But helped me to create my own practice.

[Learn x86-64 assembly by writing a GUI from scratch](https://gaultier.github.io/blog/x11_x64.html)

[Some Assembly Required](https://github.com/hackclub/some-assembly-required/)

[System Inside; ASM](https://0xax.dev/books/asm/)

[(French) Conventions d'appel](https://beta.hackndo.com/conventions-d-appel/)

[(French) Stack introduction](https://beta.hackndo.com/stack-introduction/)

[(French) Assembly Basics](https://beta.hackndo.com/assembly-basics/)

[(French) Introduction a GDB](https://beta.hackndo.com/introduction-a-gdb/)

[Algorithms for Modern Hardware](https://en.algorithmica.org/hpc/) : Not totally related to the subject. Goes deeper than needed for the project, but helped me learn how computers
are working with modern hardware.

## The use of AI

For this project, AI was primarily used to correct the English in this document.
It was also used to help me create tests for my functions.
AI helps me also with practice exercises before doing libasm then I extend the practice side alone with various resources you can find here [LINK]
Helps me to generate a cleaner version of my Makefile for debug option and includes in bonus part.

## Why did I take this subject? What did I learn?

After completing the Core Curriculum at 42 school, you can choose from a lot of subjects without any obligation.
So why did I take this subject?

I wanted to learn more about computer architecture. Even if this subject doesn't require it, it's the lowest level you can get to while programming to see how computers work.
So I took this opportunity to look at how computers work: registers, L1/L2/L3 caches, ALU, FPU, RAM, disk, etc.

So here is a summary of everything I learned:
-Computer architecture
-ISA (CISC vs RISC)
-How the CPU works
-SIMD
-Translation of code to CPU instructions
-Compiler's work behind the scene and his optimization
-Intel and AT&T syntax basics (but stick to Intel syntax as God intended)
-Compiler output with different optimisation levels
-NASM
-Foundations for reverse engineering CTF challenges
-Better use of GDB to inspect ELF files without debug flags
-Better use of objdump
-PLT / GOT with glibc

## SIMD

About the SIMD, while I was in the idea to use it for at least ft_strlen() and few others functions. I have decided to not write it and let it scalar way.
The main reason is the time constraint I had, but I explored SSE2 and AVX concepts while doing my research about Assembly.

## FUN FACTS

While exploring godbolt and different compiler stuff, I discovered a way of "cheating" on some functions. 
It's not really a cheat but a very helpful way of comparing your asm to the compiler's work....

While using godbolt I did this ; [godbolt output for a ft_strlen](https://godbolt.org/z/oYT4f6hWv)
The GCC compiler is using the real strlen it knows => because GCC knows the idiom of strlen (looking for '\0'). The strlen in libc is using SIMD which makes the substitution interesting.

So in order to not have any optimization like that, you need to add some flags; [godbolt output for a ft_strlen with -ffreestanding](https://godbolt.org/z/czejWqrsa)
What do we see ? a very clean version of ft_strlen, very close to mine (but mine is better and yes it is the ego talking right here)

So maybe.....you can pass your C code directly to godbolt with this flag and everything will work out; [godbolt on ft_strcmp with --ffreestanding](https://godbolt.org/z/344G7jcv3)
*You have to know that 42 students have already done most of those functions in C at the beginning of their cursus*

It works !
Is it good ? No but it's not THAT bad
Is it useful ? A little bit for small functions, after that you need to do it by yourself
Is it funny ? YES !
