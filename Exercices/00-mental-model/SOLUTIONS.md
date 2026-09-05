# Module 0 — Corrigé

**Q1 — Endianness.**
x86 est *little-endian* : l'octet de poids faible est stocké à l'adresse la plus
basse. `42 = 0x0000002A`, donc en mémoire on lit `2a 00 00 00`. C'est purement une
convention de rangement ; la valeur logique reste 42.

**Q2 — Registre vs mémoire.**
Dans le désassemblage de `add` (compilé en `-O0`), tu vois typiquement les
arguments recopiés depuis les registres vers la pile, rechargés dans `eax`, un
`add`, puis le résultat stocké dans la variable locale `result` sur la pile. Le
calcul lui-même se fait **dans un registre** (`eax`) — l'ALU ne travaille jamais
« directement en mémoire » pour une addition d'entiers : les opérandes passent par
des registres. (En `-O2`, tout ça se réduirait à un `lea`/`add` sans aller-retour
mémoire.)

**Q3 — Passage d'arguments.**
`rdi` contient le 1er argument (`answer`, soit 42) et `rsi` le 2e (`0`). L'ABI
System V AMD64 impose l'ordre : `rdi, rsi, rdx, rcx, r8, r9` pour les six premiers
arguments entiers/pointeurs. C'est **la** règle que tu appliqueras dans toutes tes
fonctions libasm.

**Q4 — Valeur de retour.**
Après le retour de `add`, `rax` (ou sa partie `eax`) vaut 42. Convention : la
valeur de retour d'une fonction transite par `rax`. D'où le `ret` de tes fonctions
qui laisse le résultat dans `rax`.

**Q5 — Complément à deux.**
`-1` en complément à deux, c'est « tous les bits à 1 ». Sur 8 octets :
`ff ff ff ff ff ff ff ff`. C'est ce codage qui permet à l'addition matérielle de
traiter signés et non signés avec le même circuit.

**Q6 — La pile grandit vers le bas.**
`rsp` à l'entrée de `add` est **plus petit** qu'à l'entrée de `main`. Le `call` a
empilé l'adresse de retour (`rsp -= 8`) et le prologue de `add` a réservé de la
place (`rsp` diminue encore). Confirmation : la pile progresse vers les adresses
**basses**.

**Q7 — String = octets.**
`text` contient `6c 69 62 61 73 6d 00` → `l i b a s m \0`. Le 7e octet, `0x00`, est
le terminateur nul : c'est lui (et pas une longueur stockée) qui marque la fin
d'une chaîne C. Retiens-le : `ft_strlen` consistera exactement à compter les octets
jusqu'à ce `0x00`.

**Q8 - flavor **
On peut changer la syntaxe de base par la commande 'set disassembly-flavor intel'

---
