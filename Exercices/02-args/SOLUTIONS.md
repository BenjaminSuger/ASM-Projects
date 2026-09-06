## ex00 - Observer les arguments avec GDB 

votre programme va commencer avec quelque chose de cet ordre; 

    114d:       55                      push   rbp
    114e:       48 89 e5                mov    rbp,rsp
    1151:       48 83 ec 10             sub    rsp,0x10
    1155:       89 7d fc                mov    DWORD PTR [rbp-0x4],edi
    1158:       48 89 75 f0             mov    QWORD PTR [rbp-0x10],rsi

Si on regarde ce qu'il se passe 

push rbp => sauvegarde la valeur de rbp sur la stack
mov rbp, rsp => maintenant rbp pointe vers cette sauvegarde. Ce qui fait que rbp est un point d'encrage utilise pour la suite.
sub rsp, 0x10 => reserver de l'espace memoire pour les variables locales
les deux dernieres lignes c'est la ou on retrouve argc, dans edi et argv dans rsi.

Pour les examine dans GDB ; 
```sh
gdb --args ./a.out chats
```

ensuite pour avoir la valeur de argc;
```sh
p $edi
```

pour voir les argv, il faut le plus souvent faire un petit cast
il existe differentes maniere de faire plus ou moins sympa
```
p ((char **)$rsi)[0]
p {char *}$rsi
p {char *}($rsi + 8) # pour avoir le second argument oblige de faire l'addittion
```

est ce que si on ecrit nous meme notre code en assembly on les retrouve au meme endroit ?
Non, car la le compilateur suit l'ABI du C mais nous par defaut on a rien a suivre. Donc notre rsp va avoir argc, suivit de argv et meme de envp par la suite. C'est a nous de le gerer tout seul.
