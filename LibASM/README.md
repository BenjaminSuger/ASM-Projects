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

We are allowed to call the extern ___error or errno_location.
We should check errors during syscalls and handle them properly when needed.

## REQUIREMENTS

To launch the project you'll need:
-A Linux distribution
-Make
-NASM
-LD

## RESOURCES

[PWN COLLEGE](https://pwn.college/): pwn.college is a program from Arizona State University. Everything is free! Labs with videos about cybersecurity. I absolutely love it!

[Algorithms for Modern Hardware](https://en.algorithmica.org/hpc/)

[x86 video playlist from OliveStem](https://www.youtube.com/watch?v=yBO-EJoVDo0&list=PL2EF13wm-hWCoj6tUBGUmrkJmH1972dBB)

[CPU Land](https://cpu.land/)

[Where the top of the stack is on x86](https://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86)

[NASM Tutorial](https://cs.lmu.edu/~ray/notes/nasmtutorial/)

[ASM Tutor](https://asmtutor.com/)

[Learn x86-64 assembly by writing a GUI from scratch](https://gaultier.github.io/blog/x11_x64.html)

[Some Assembly Required](https://github.com/hackclub/some-assembly-required/)

[System Inside; ASM](https://0xax.dev/books/asm/)

[(French) Conventions d'appel](https://beta.hackndo.com/conventions-d-appel/)

[(French) Stack introduction](https://beta.hackndo.com/stack-introduction/)

[(French) Assembly Basics](https://beta.hackndo.com/assembly-basics/)

[(French) Introduction a GDB](https://beta.hackndo.com/introduction-a-gdb/)

## The use of AI

For this project, AI was primarily used to correct the English in this document.
It was also used to help me create tests for my functions.

## Why did I take this subject? What did I learn?

After completing the Core Curriculum at 42 school, you can choose from a lot of subjects without any obligation.
So why did I take this subject?

I wanted to learn more about computer architecture. Even if this subject doesn't require it, it's the lowest level you can get to while programming to see how computers work.
So I took this opportunity to look at how computers work: registers, L1/L2/L3 caches, ALU, FPU, RAM, disk, etc.

So here is a summary of everything I learned:
-Computer architecture
-ISA (CISC vs RISC)
-How the CPU works
-Translation of code to CPU instructions
-Intel and AT&T syntax basics (but stick to Intel syntax as God intended)
-Compiler output with different optimisation levels
-NASM
-Foundations for reverse engineering CTF challenges
-Better use of GDB to inspect ELF files without debug flags
-Better use of objdump
