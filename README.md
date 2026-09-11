# ASM-Projects

This repository will group all my projects in Assembly code.

You'll find here differents directories with projects listed in the below sections but also some Markdown files with explanations of different concepts.

## Projects

### LIBASM
42 project to rewrite some functions in assembly x86

### Posts
The series of articles I published through diverse media ; medium and dev.to
This series of posts follow my learning of assembly programming

### Piscine ASM (on going)
A lot of exercices from my initial piscine in C done in ASM

### Exercices
A serie of exercices (in french), each directories have multiple steps

#### 00 Mental Model
Compiling in C and observe the code with different tools

#### 01 Exit
First program in ASM compiled with NASM

#### 02 Args
Create a program and using arguments (argc, argv)
Introduction to if and loops

#### 03 maths
Doing some very basic mathematical operations
Registers manipulations

#### 04 Writing
Write and strlen


-les additions https://asmtutor.com/#lesson12 , les soustraction https://asmtutor.com/#lesson13, les multiplications https://asmtutor.com/#lesson14 et les division https://asmtutor.com/#lesson15 
-puis de print une string en dur pour montrer le rodata => du coup faire ft_write comme il faut a la fin (asm tutor https://asmtutor.com/#lesson4 et aussi https://asmtutor.com/#lesson5 pour montrer comment le sortir, peut etre j'aurais besoni de ca https://asmtutor.com/#lesson6 et de ca https://asmtutor.com/#lesson7)
-puis de calculer la longeur (ft_strlen done)
-ca serait bien de montrer un overflow je pense en asm parce que ma question de base sur les registres vs les variable (observable avec un code en c d'abord)
-comment faire son code en C et ajouter son code de asm qui vient de l'object file (utiliser le strlen ou le write) => voir faire aussi un putstr
-peut etre un exo qui fait en sorte que ca retombe sur un truc utile de atoi ou on verifie qu'il y a pas de doublon ou autres 
-fizz buzz https://asmtutor.com/#lesson18 mais je crois je vais devoir faire un putnbr
-un truc sur pointeur comme 42 piscine => du type ft_ft pour voir
-faire un petit swap
-puis de comparer (ft_strcmp done)
-faire l'exo piscine ou on compare les arguments du main pour les mettres dans l'ordre (ft_sort_param C06 de la piscine)
-puis de faire ft_strcpy 
-malloc avec ft_strdup 
-ft_atoi_base
-le ft_read (https://asmtutor.com/#lesson9 et/ou https://asmtutor.com/#lesson25)
-les listes chaine
-ft_for_each avec le pointer de fonction
-namespace https://asmtutor.com/#lesson17

## Ressources

A list of all ressources used ; 

[PWN COLLEGE](https://pwn.college/) ; pwn college is program from Arizona State University. everything is free! Labs with videos about cybersecurity. I absolutly love it!

[Algorithms for Modern Hardware](https://en.algorithmica.org/hpc/)

[x86 playlist video form OlivStem](https://www.youtube.com/watch?v=yBO-EJoVDo0&list=PL2EF13wm-hWCoj6tUBGUmrkJmH1972dBB)

[CPU Land](https://cpu.land/)

[Where the top of the stack is on x86](https://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86)

[NASM Tutorial](https://cs.lmu.edu/~ray/notes/nasmtutorial/)

[ASM Tutor](https://asmtutor.com/)

[Learn x86-64 assembly by writing a GUI from scratch](https://gaultier.github.io/blog/x11_x64.html)

[Some Assembly required](https://github.com/hackclub/some-assembly-required/)

[System Inside; ASM](https://0xax.dev/books/asm/)

[(French) Conventions d'appel](https://beta.hackndo.com/conventions-d-appel/)

[(French) Stack introduction](https://beta.hackndo.com/stack-introduction/)

[(French) Assembly Basics](https://beta.hackndo.com/assembly-basics/)

[(French) Introduction a GDB](https://beta.hackndo.com/introduction-a-gdb/)
