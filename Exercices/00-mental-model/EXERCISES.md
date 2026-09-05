# Exercice 0 — Exercices d'observation (gdb)

Dans l'objectif de prendre en main GDB un peu plus et de pouvoir etre plus efficace pour la suite.
Exercice redige par IA (Claude code) puis modifie par moi meme pour ajouter plus de details et d'observations qui m'interessent.
Demande quand meme un minimum de connaissance / aisance en C + GDB de base

Compile avec les symboles de debug, sans optimisation :

```sh
gcc -g -O0 inspect.c -o inspect
gdb ./inspect
```

Aide-mémoire gdb :

| Commande                     | Effet                                             |
|------------------------------|---------------------------------------------------|
| `break add`                  | point d'arrêt à l'entrée de `add`                 |
| `run`                        | lance le programme                                |
| `info registers`             | tous les registres                                |
| `info registers rax rdi rsi` | seulement ceux-là                                 |
| `p/x $rsp`                   | affiche rsp en hexa                               |
| `x/4xb &answer`              | 4 octets (byte) en hexa à l'adresse de `answer`   |
| `x/s text`                   | la mémoire comme une chaîne                       |
| `disas add`                  | désassemble la fonction `add`                     |
| `stepi` / `si`               | avance d'**une instruction** machine              |
| `next` / `step`              | avance d'une **ligne C**                          |
| `continue` / `c`             | reprend l'exécution                               |
| `quit`                       | sortir                                            |

Réponds à chaque question (le corrigé est dans `SOLUTIONS.md`).

**Q1 — Endianness.**
Mets un breakpoint sur `add` (commande gdb => b add), lance, puis `x/4xb &answer` dans la frame de `main`
(remonte avec `up` si besoin). Tu vois `2a 00 00 00`. Pourquoi pas `00 00 00 2a` ?

**Q2 — Registre vs mémoire.**
`disas add`. Repère l'instruction qui calcule `a + b`. Le résultat transite-t-il
par un registre ou est-il écrit directement en mémoire ? Lequel/laquelle ?

**Q3 — Le passage d'arguments.**
Toujours arrêté à l'entrée de `add`, regarde `info registers rdi rsi`. Quelles
valeurs contiennent-ils, et pourquoi ces deux registres précisément ?
(Indice : ABI System V — 1er argument, 2e argument.)

**Q4 — La valeur de retour.**
Fais `next` jusqu'à sortir de `add`, puis regarde `rax`. Que vaut-il ? Quelle
convention cela illustre-t-il ?

**Q5 — Complément à deux.**
`p/x big` puis `x/8xb &big`. Pourquoi `-1` s'affiche-t-il comme une suite de `ff` ?

**Q6 — La pile grandit vers le bas.**
Note `p/x $rsp` à l'entrée de `main`, puis de nouveau à l'entrée de `add` (après le
`call`). La valeur a-t-elle augmenté ou diminué ? Qu'est-ce que ça confirme ?

**Q7 — Une string, c'est des octets.**
`x/7xb text` puis `x/s text`. Retrouve le code ASCII de `'l'` (0x6c). Que vaut le
7e octet, et à quoi sert-il ?

**Q8 - on observe de l'assembleur ???**
relance GDB et fait la commande "layout asm" tu veras de l'assembleur. Mais sous quelle syntax ? comment changer de syntax ?

**Q9 - on observe de l'assembleur intel**
Relance gdb et fait la commande "layout asm" (apres avoir changer pour intel). Tu peux observer de l'assembleur et
regarde les 2 avants dernieres lignes de la fonction add() (en lien avec la Q2),
change le code pour mettre 'return a + b' a la place et observe la difference.
Il s'agit juste d'observation

