# Module 0 — Le modèle mental (aucune ligne d'assembleur)

Objectif : avant d'écrire quoi que ce soit, savoir **où vivent les données** et
**ce que le CPU manipule réellement**. On n'écrit pas d'asm ici — on *observe* un
programme C avec `gdb`.

À la fin, tu dois pouvoir répondre sans hésiter à : « cette valeur, elle est dans un
registre ou en mémoire ? », « dans quel sens grandit la pile ? », « pourquoi
`0x2a` s'affiche octet par octet comme `2a 00 00 00` ? ».

## Les concepts (à lire une fois, puis à vérifier avec gdb)

### Les registres généraux (64 bits)

`rax rbx rcx rdx rsi rdi rbp rsp r8 … r15`. Ce sont les « variables » les plus
rapides du CPU. Chacun a des sous-parties : pour `rax` → `eax` (32 bits),
`ax` (16), `al` (8 bits de poids faible). Écrire dans `eax` **met à zéro** le haut
de `rax` (piège classique).

Rôles particuliers :
- `rsp` = sommet de la pile (stack pointer).
- `rbp` = base de la trame courante (base pointer), quand on l'utilise.
- `rip` = pointeur d'instruction : l'adresse de la **prochaine** instruction.
- `rflags` = drapeaux d'état après une opération (voir plus bas).

### rflags (les drapeaux qui font marcher les `if`)

Après une opération arithmétique/logique, le CPU positionne des bits :
- **ZF** (Zero) : le résultat est nul.
- **SF** (Sign) : le résultat est négatif (bit de poids fort à 1).
- **CF** (Carry) : retenue / emprunt (non signé).
- **OF** (Overflow) : débordement (signé).

Un `if (a == b)` en C devient, en asm, un `cmp` (qui fait une soustraction
« pour voir » et positionne ZF) suivi d'un saut conditionnel qui **lit** ZF.

### Little-endian

x86 stocke les octets du **poids faible en premier**. L'entier `0x2A` (=42) sur
4 octets est en mémoire : `2a 00 00 00`. Ce n'est pas un bug, c'est la convention.

### Complément à deux

Les entiers signés négatifs : `-1` sur 8 bits = `0xFF`, sur 64 bits =
`0xFFFFFFFFFFFFFFFF`. C'est pour ça qu'une même valeur binaire peut être « grande »
en non signé et « -1 » en signé — d'où l'existence de sauts signés (`jl`, `jg`) et
non signés (`jb`, `ja`).

### Le layout mémoire d'un processus

De l'adresse basse vers la haute (schématique) :

```
  adresses basses
  ┌───────────────┐
  │ .text         │  ← ton code (instructions), lecture seule
  ├───────────────┤
  │ .rodata       │  ← constantes ("Hello", nombres immuables)
  ├───────────────┤
  │ .data         │  ← variables globales initialisées
  ├───────────────┤
  │ .bss          │  ← variables globales à zéro
  ├───────────────┤
  │ heap  ↓↓↓     │  ← malloc, grandit vers le HAUT
  │      (libre)  │
  │ stack ↑↑↑     │  ← variables locales, grandit vers le BAS
  └───────────────┘
  adresses hautes
```

Point crucial : **la pile grandit vers les adresses basses**. Un `push` fait
`rsp -= 8` puis écrit. Un `pop` lit puis fait `rsp += 8`. Retiens ça, la moitié des
bugs d'asm viennent d'une pile mal gérée.

## Exercices

Ouvre `EXERCISES.md`. Tu vas compiler `inspect.c` **avec les symboles de debug**
puis répondre aux questions dans `gdb`. Corrigé dans `SOLUTIONS.md`.
