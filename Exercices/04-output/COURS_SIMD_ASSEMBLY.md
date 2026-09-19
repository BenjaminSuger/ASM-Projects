# SIMD en assembleur x86-64 — de zéro à un `strlen` vectorisé

Syntaxe : NASM / Intel, convention System V (Linux 64 bits). Prérequis : registres généraux, pile, `call`/`ret`, boucles scalaires, notion de page mémoire.

---

## 1. L'idée : une instruction, plusieurs données

Ton `ft_strlen` scalaire traite **un octet par itération** : charger, comparer, sauter. Sur un CPU moderne, une itération coûte au minimum un cycle et demi à cause de la dépendance sur `rax`. Pour une chaîne de 1 000 caractères, c'est ~1 500 cycles.

SIMD (*Single Instruction, Multiple Data*) : une seule instruction applique la même opération à un paquet de valeurs rangées côte à côte dans un registre large. Avec SSE2, un registre fait 128 bits, soit **16 octets**. Une comparaison à zéro traite donc 16 caractères d'un coup. Le même `strlen` de 1 000 caractères passe à ~63 itérations.

Le principe est le même que quand tu traites des `long` de 8 octets au lieu d'octets : tu ne changes pas l'algorithme, tu changes la largeur du "camion". La différence, c'est que les registres SIMD sont découpés en *voies* (*lanes*) indépendantes : une addition 128 bits en mode "16 × 8 bits" ne propage pas la retenue d'une voie à l'autre.

### Vocabulaire

| Terme | Sens |
|---|---|
| Voie (*lane*) | Une des sous-valeurs du registre (octet, mot, dword, qword, float, double) |
| Vecteur | Le contenu complet du registre, vu comme un tableau de voies |
| Largeur | 128 bits (SSE), 256 (AVX), 512 (AVX-512) |
| Extension | Famille d'instructions : SSE2, SSSE3, SSE4.1/4.2, AVX, AVX2, AVX-512… |
| Opération *packed* | Appliquée à toutes les voies en parallèle (suffixe `p` : `paddb`, `pcmpeqb`) |
| Opération *scalar* | Sur la voie basse seulement (suffixe `s` : `addss`, `movsd`) |

---

## 2. Les registres

### XMM (SSE, 128 bits)

`xmm0` à `xmm15`. Seize registres de 128 bits, distincts des registres généraux. Tu ne peux pas faire `mov xmm0, rax` : le transfert passe par des instructions dédiées (`movq`, `movd`) ou par la mémoire.

Un même registre peut être interprété de plusieurs façons, selon l'instruction qu'on lui applique :

```
xmm0 (128 bits)
├── 16 × 8 bits   (b : byte)      → pcmpeqb, paddb, pminub
├──  8 × 16 bits  (w : word)      → pcmpeqw, paddw
├──  4 × 32 bits  (d : dword)     → pcmpeqd, paddd  |  4 × float  → addps
└──  2 × 64 bits  (q : qword)     → pcmpeqq, paddq  |  2 × double → addpd
```

Le registre ne "sait" pas ce qu'il contient. C'est l'instruction qui décide de la découpe. Exactement comme `rax` peut contenir un entier ou un pointeur : les bits sont les bits.

### YMM (AVX, 256 bits) et ZMM (AVX-512, 512 bits)

`ymm0`–`ymm15` sont les mêmes registres physiques que les `xmm`, élargis : `xmm3` est la moitié basse de `ymm3`. De même `zmm` (AVX-512, `zmm0`–`zmm31`) englobe `ymm`.

```
zmm0 [511 ................................................. 0]
     └──────────── ymm0 [255 ................. 0] ───────────┘
                        └──── xmm0 [127 .. 0] ────┘
```

Attention : écrire dans un `xmm` avec une instruction **VEX** (préfixe `v`, AVX) met à zéro la moitié haute du `ymm`, comme `mov eax` met à zéro le haut de `rax`. Écrire avec une instruction SSE *legacy* (sans `v`) **ne touche pas** la moitié haute, ce qui crée des pénalités de transition (voir §9).

### Registre d'état : MXCSR

Contrôle les modes d'arrondi et les exceptions flottantes. Sans importance pour les entiers, donc pour `strlen`.

---

## 3. Le sous-ensemble d'instructions dont tu as besoin

Il existe des centaines d'instructions SIMD. Pour `strlen` et la plupart des fonctions de libasm, cinq suffisent. Apprends celles-là, tu déduiras les autres par analogie de nom.

### 3.1 Charger et ranger : `movdqu` / `movdqa`

```nasm
movdqu xmm0, [rdi]      ; charge 16 octets, adresse quelconque
movdqa xmm0, [rdi]      ; charge 16 octets, adresse ALIGNÉE sur 16 — sinon #GP (SIGSEGV)
movdqu [rsi], xmm0      ; range 16 octets
```

`dq` = *double quadword* (128 bits), `u` = *unaligned*, `a` = *aligned*. Historiquement `movdqa` était nettement plus rapide ; sur les CPU depuis ~2010, `movdqu` sur une adresse alignée coûte pareil. On utilise `movdqa` quand on **veut** que ça plante si l'alignement est faux : c'est une assertion gratuite.

Transferts avec les registres généraux :

```nasm
movq  xmm0, rax         ; 64 bits bas de xmm0 ← rax, reste mis à 0
movq  rax, xmm0         ; rax ← 64 bits bas
movd  xmm0, eax         ; 32 bits
```

### 3.2 Mettre à zéro : `pxor`

```nasm
pxor xmm1, xmm1         ; xmm1 = 0 (idiome, reconnu par le CPU comme xor eax,eax)
```

Même astuce que `xor eax, eax` : pas de dépendance, encodage court.

### 3.3 Comparer octet par octet : `pcmpeqb`

```nasm
pcmpeqb xmm0, xmm1      ; pour chaque voie i : xmm0[i] = (xmm0[i] == xmm1[i]) ? 0xFF : 0x00
```

*Packed Compare Equal Bytes.* Il n'y a **pas de flags** : le résultat est un masque d'octets écrit dans le registre destination. Voie égale → tous les bits à 1 (`0xFF`), sinon `0x00`. Le second opérande peut être en mémoire : `pcmpeqb xmm0, [rdi]`.

Exemple avec `xmm1 = 0` et 16 octets de `"Hello\0......."` :

```
[rdi]   :  48 65 6C 6C 6F 00 xx xx xx xx xx xx xx xx xx xx
pcmpeqb :  00 00 00 00 00 FF ?? ?? ?? ?? ?? ?? ?? ?? ?? ??
                          ↑ voie 5 : premier zéro
```

(les `??` valent `FF` si l'octet après la chaîne est aussi 0, `00` sinon — on s'en fiche, seul le **premier** `FF` compte.)

Variantes : `pcmpeqw`, `pcmpeqd`, `pcmpeqq` (SSE4.1), et `pcmpgtb` (*greater than*, signé).

### 3.4 Compresser le masque : `pmovmskb`

```nasm
pmovmskb edx, xmm0      ; edx[i] = bit de poids fort de xmm0[i], pour i = 0..15 ; bits 16-31 à 0
```

*Packed Move Mask Byte.* C'est le pont entre le monde SIMD et le monde scalaire. Il prend le bit 7 de chaque voie et les assemble en un entier de 16 bits dans un registre général. Après un `pcmpeqb`, chaque voie vaut `0xFF` ou `0x00`, donc le bit 7 dit simplement "égal ou pas".

Avec l'exemple ci-dessus : `edx = 0b0000_0000_0010_0000 = 0x0020` (bit 5 à 1).

Maintenant tu peux `test edx, edx` : zéro → aucun octet nul dans le bloc ; non nul → il y en a au moins un, et la position du premier bit à 1 est la position du premier `\0`.

### 3.5 Trouver le premier bit : `bsf` / `tzcnt`

```nasm
bsf   eax, edx          ; eax = index du bit de poids faible à 1 dans edx ; INDÉFINI si edx == 0
tzcnt eax, edx          ; idem, mais renvoie 32 si edx == 0 (BMI1, ~2013+)
```

`bsf` existe depuis le 386, `tzcnt` est plus rapide sur AMD et sûr sur zéro. Comme on ne l'appelle qu'après avoir vérifié `edx != 0`, les deux marchent. Note : `bsf eax, edx` écrit `eax`, donc met à zéro le haut de `rax` — pratique pour la valeur de retour.

### Résumé du pipeline `strlen`

```
[mémoire] --movdqa--> xmm0 --pcmpeqb 0--> masque d'octets --pmovmskb--> masque 16 bits --test/bsf--> index
```

---

## 4. Le problème de la lecture hors limites

C'est le point qui rend un `strlen` SIMD réellement difficile, et c'est celui qui t'a intrigué.

### 4.1 Pourquoi on lit forcément trop loin

Un `strlen` scalaire lit octet par octet et **s'arrête au `\0`** : il ne lit jamais un octet après le terminateur. Un `strlen` SIMD charge 16 octets d'un coup. Si le `\0` est en position 3, il a lu les octets 4 à 15 qui sont *après* la chaîne. Il n'a aucun moyen de faire autrement : `movdqu` ne s'arrête pas en cours de route.

Est-ce grave ? Lire des octets de garbage n'est pas un problème en soi : on ignore ce qui suit le premier `\0`. Le problème est **si ces octets ne sont pas mappés**.

### 4.2 Les pages

La mémoire virtuelle est gérée par **pages** de 4 096 octets (`0x1000`), alignées sur 4 096. Le noyau mappe ou non chaque page entière : soit tu as le droit de lire tous les octets de la page, soit aucun. Il n'y a pas de demi-page.

Imagine une chaîne `"abc\0"` dont le `\0` est le **dernier octet d'une page mappée**, et la page suivante n'est pas mappée (fin du tas, fin d'un `mmap`, garde entre deux zones) :

```
adresse   0x7f...0FFC  0FFD  0FFE  0FFF | 0x7f...1000 ...
contenu       'a'       'b'   'c'   \0  |   NON MAPPÉ
                                        ↑ frontière de page
```

`movdqu xmm0, [0x...0FFC]` demande les octets `0FFC` à `100B`. Les quatre premiers sont dans la page mappée, les douze suivants dans la page non mappée. Le CPU lève une faute de page → **SIGSEGV**. Alors que la chaîne est parfaitement valide et qu'un `strlen` scalaire l'aurait traitée sans problème.

Ce n'est pas un cas théorique. Un tester un peu sérieux, ou simplement `malloc` d'un gros buffer, te mettra tôt ou tard une chaîne en fin de page.

### 4.3 La propriété qui sauve : une lecture alignée ne traverse jamais une page

Voilà la clé, et c'est de l'arithmétique pure :

- Une page commence à une adresse multiple de 4 096.
- 4 096 est un multiple de 16.
- Donc **toute adresse multiple de 16 est à distance multiple de 16 du début de sa page**, et un bloc de 16 octets qui commence à un multiple de 16 se termine **avant** le prochain multiple de 16, donc avant la prochaine frontière de page.

Autrement dit : les blocs alignés sur 16 *pavent* chaque page exactement, 256 blocs par page, sans qu'aucun ne chevauche deux pages. Si le premier octet du bloc est dans une page mappée, **les 16 le sont**.

```
page N                                                   page N+1
|----|----|----|----| ... |----|----|----|----|          |----|----
0    16   32   48         4032 4048 4064 4080 4096
                                                ↑ un bloc aligné s'arrête pile ici, jamais après
```

### 4.4 Ce que fait `and rdi, -16`

`-16` en binaire, c'est `...1111_0000`. `and rdi, -16` met les 4 bits de poids faible à zéro : l'adresse est **arrondie vers le bas** au multiple de 16 le plus proche.

```
rdi = 0x...0FFC  (= ...1111_1111_1100)
and rdi, -16
rdi = 0x...0FF0  (= ...1111_1111_0000)
```

Ta question : « mais du coup on lit avant le début de la chaîne, au mauvais endroit ? »

**Oui, on lit avant.** Entre 0 et 15 octets avant le premier caractère. Et c'est légal : ces octets sont dans **la même page** que le premier caractère de la chaîne (puisque le bloc aligné ne traverse pas de frontière, et que le premier caractère est dedans). La page est mappée, la lecture est autorisée. Ce qu'il y a dedans — un bout de chaîne précédente, un en-tête de `malloc`, n'importe quoi — on s'en fiche, on va **ignorer** ces voies.

Ce qu'on a gagné : le bloc aligné qui contient le premier caractère est garanti lisible en entier. Et tous les blocs suivants, tant qu'on n'a pas vu de `\0`, sont garantis lisibles aussi : si un bloc aligné ne contient pas de `\0`, c'est que la chaîne continue dans le bloc suivant, donc ce bloc suivant contient au moins un octet de la chaîne, donc il est dans une page mappée, donc il est lisible en entier. Par récurrence, on ne segfaulte jamais.

Reprenons l'exemple qui plantait :

```
0x...0FF0  : ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 'a' 'b' 'c' \0 | page N+1 non mappée
             └────── 12 octets "avant" ─────────┘ └── chaîne ──┘
movdqa xmm0, [0x...0FF0]   → lit exactement 0FF0..0FFF, tout dans la page N. OK.
```

### 4.5 Ignorer les octets d'avant : décaler le masque

Après `pcmpeqb` + `pmovmskb`, le bit *i* du masque correspond à l'octet *i* du bloc aligné. Les octets 0 à `k-1` (où `k` = désalignement = `adresse_originale & 15`) sont avant la chaîne. S'il y a un `\0` parmi eux, il ne doit pas compter.

Solution : `shr edx, k`. Ça jette les *k* bits de poids faible. Le bit 0 du nouveau masque correspond alors au **premier caractère de la chaîne**, et `bsf` donne directement la longueur.

```
bloc aligné :  ?? ?? 00 ?? ?? ?? ?? ?? ?? ?? ?? ?? 'a' 'b' 'c' 00
index        :  0  1  2  3  4  5  6  7  8  9 10 11 12  13  14  15
masque       :  bit 2 = 1, bit 15 = 1                → edx = 0x8004
désalignement : k = 12
shr edx, 12  →  edx = 0x0008  → bit 3 = 1 → bsf = 3 = strlen("abc"). ✓
```

Le faux `\0` en position 2 a été éjecté par le décalage.

### 4.6 Note pour la conscience tranquille

Du point de vue de C, lire avant le début d'un objet est un comportement indéfini. En assembleur, il n'y a pas de C et pas d'objet : il y a des adresses mappées ou non. La glibc fait exactement ça dans son `strlen`, et Valgrind embarque des suppressions spécifiques pour ne pas râler dessus. Si tu testes ton `ft_strlen` SIMD sous Valgrind ou AddressSanitizer, attends-toi à des faux positifs — c'est attendu, pas un bug.

---

## 5. `ft_strlen` en SSE2 — implémentation complète

```nasm
section .text
    global ft_strlen_sse2

; size_t ft_strlen_sse2(const char *s)
ft_strlen_sse2:
    mov      rax, rdi            ; rax = adresse d'origine (on en aura besoin à la fin)
    and      rdi, -16            ; rdi = début du bloc aligné contenant s[0]
    mov      ecx, eax
    and      ecx, 15             ; ecx = désalignement k (0..15)

    pxor     xmm1, xmm1          ; xmm1 = 16 × 0x00

    ; --- premier bloc, avec les k octets parasites en tête ---
    movdqa   xmm0, [rdi]         ; lecture alignée, sûre (cf. §4)
    pcmpeqb  xmm0, xmm1          ; 0xFF là où il y a un \0
    pmovmskb edx, xmm0           ; masque 16 bits
    shr      edx, cl             ; éjecte les k bits des octets d'avant la chaîne
    jnz      .in_first_block     ; (shr met ZF selon le résultat)

    ; --- blocs suivants, tous alignés, aucun parasite ---
.loop:
    add      rdi, 16
    movdqa   xmm0, [rdi]
    pcmpeqb  xmm0, xmm1
    pmovmskb edx, xmm0
    test     edx, edx
    jz       .loop

    ; \0 trouvé dans le bloc [rdi, rdi+16)
    sub      rdi, rax            ; rdi = offset du bloc par rapport au début de la chaîne
    bsf      edx, edx            ; edx = position du \0 dans le bloc
    lea      rax, [rdi + rdx]    ; longueur = offset du bloc + position dans le bloc
    ret

.in_first_block:
    bsf      eax, edx            ; après le shr, bit i ↔ s[i] : la position EST la longueur
    ret
```

### Lecture guidée

- **Lignes 1–4** : on garde le pointeur original dans `rax` (il servira à calculer l'offset final) et on en déduit le bloc aligné et le désalignement. `mov ecx, eax` copie les 32 bits bas, suffisant puisqu'on ne garde que 4 bits.
- **`shr edx, cl`** : le compte de décalage variable doit être dans `cl`, c'est une contrainte de l'ISA. C'est pour ça que `k` est dans `ecx` et pas ailleurs.
- **`jnz` après `shr`** : `shr` positionne ZF sur le résultat (sauf quand le compte est 0, où les flags sont préservés — mais `test`/`pmovmskb` ne les met pas… donc pour être rigoureux, ajoute `test edx, edx` avant le `jnz` ; je l'omets ici pour rester lisible, mais **dans ta version, mets-le**).
- **Boucle** : trois instructions SIMD + un test + un saut pour 16 octets. C'est le cœur qui fait 90 % du travail sur une longue chaîne.
- **Épilogue du chemin long** : `rdi - rax` est positif (on a fait au moins un `add rdi, 16`), `bsf edx, edx` écrit `edx` donc met à zéro le haut de `rdx`, et `lea` additionne sans toucher aux flags.
- **Épilogue du chemin court** : le `shr` a déjà aligné le masque sur le début de la chaîne, `bsf` donne directement la réponse.

### Pourquoi deux sorties et pas une ?

Après le `shr`, le masque du premier bloc est exprimé relativement à `s[0]`. Ceux de la boucle sont relatifs au début du bloc. Fusionner les deux chemins demanderait soit de re-décaler à l'envers, soit de compenser `k` dans le calcul final (`lea rax, [rdi + rdx - k]` avec un `rdi` qui part du bloc aligné). C'est faisable en une sortie unique ; l'exercice t'est laissé.

---

## 6. Version AVX2 — mêmes idées, deux fois plus large

AVX2 (2013, Haswell) double la largeur : 32 octets par bloc. Les instructions prennent un préfixe `v`, un opérande de plus (destination séparée des sources, ce qui évite les copies), et travaillent sur `ymm`.

```nasm
; size_t ft_strlen_avx2(const char *s)
ft_strlen_avx2:
    mov       rax, rdi
    and       rdi, -32
    mov       ecx, eax
    and       ecx, 31

    vpxor     ymm1, ymm1, ymm1        ; ymm1 = 0 (3 opérandes : dst, src1, src2)

    vpcmpeqb  ymm0, ymm1, [rdi]       ; ymm0 = (ymm1 == mem) par octet
    vpmovmskb edx, ymm0               ; masque 32 bits cette fois
    shr       edx, cl
    test      edx, edx
    jnz       .in_first_block

.loop:
    add       rdi, 32
    vpcmpeqb  ymm0, ymm1, [rdi]
    vpmovmskb edx, ymm0
    test      edx, edx
    jz        .loop

    sub       rdi, rax
    tzcnt     edx, edx
    lea       rax, [rdi + rdx]
    vzeroupper                        ; cf. §9
    ret

.in_first_block:
    tzcnt     eax, edx
    vzeroupper
    ret
```

Points nouveaux :

- **Forme à 3 opérandes** : `vpcmpeqb dst, src1, src2` ne détruit pas `src1`. Plus besoin de `movdqa` séparé, la comparaison lit directement la mémoire.
- **32 est aussi un diviseur de 4 096**, donc le raisonnement du §4 tient tel quel avec des blocs de 32.
- **`vzeroupper`** avant de rendre la main : obligatoire pour éviter les pénalités de transition AVX → SSE dans le code appelant (§9).
- **Alignement** : `vpcmpeqb` avec opérande mémoire **n'exige pas** l'alignement (contrairement au `movdqa` SSE). On aligne quand même, pour la sécurité des pages.

---

## 7. Tester correctement

Un `strlen` SIMD qui passe `"hello"` ne prouve rien. Les trois cas qui cassent les implémentations naïves :

1. **Chaîne vide** (`\0` en position 0), avec tous les désalignements de 0 à 15.
2. **`\0` exactement à la fin d'un bloc** (longueur 15, 16, 17, 31, 32, 33…).
3. **Chaîne collée à une frontière de page non mappée.**

Harnais C pour le cas 3, à brancher dans ton tester différentiel :

```c
#include <sys/mman.h>
#include <string.h>
#include <assert.h>

size_t ft_strlen_sse2(const char *s);

static void test_page_boundary(void)
{
    // deux pages contiguës, la seconde inaccessible
    char *base = mmap(NULL, 8192, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(base != MAP_FAILED);
    assert(mprotect(base + 4096, 4096, PROT_NONE) == 0);

    // pour chaque longueur, place la chaîne pour que le \0 soit le dernier octet de la page 1
    for (size_t len = 0; len < 100; len++) {
        char *s = base + 4096 - 1 - len;
        memset(s, 'x', len);
        s[len] = '\0';
        assert(ft_strlen_sse2(s) == len);   // segfault ici = lecture hors page
        assert(ft_strlen_sse2(s) == strlen(s));
    }
    munmap(base, 8192);
}
```

Le `for` fait varier à la fois la longueur **et** l'alignement du départ, donc il couvre aussi les cas 1 et 2 pour les petites longueurs. Ton tester à base de `fork` isolera le segfault proprement.

Et le test différentiel classique : chaînes aléatoires de longueur 0 à 5 000, à tous les offsets d'alignement, comparées à `strlen` de la libc.

---

## 8. Convention d'appel et registres SIMD

Dans System V x86-64 :

- **Tous les `xmm0`–`xmm15` sont *caller-saved*** (volatiles). Une fonction peut les écraser librement ; l'appelant qui y tient doit les sauvegarder. Ton `ft_strlen` peut donc utiliser `xmm0`/`xmm1` sans les restaurer.
- Les arguments **flottants** passent dans `xmm0`–`xmm7` ; les entiers dans `rdi`, `rsi`… Pour `strlen`, rien ne change.
- Pour une fonction *variadique* (`printf`), `al` doit contenir le nombre de registres `xmm` utilisés pour les arguments. Sans rapport ici, mais ça explique le `xor eax, eax` que tu as peut-être vu avant un `call printf`.
- **Alignement de la pile à 16** : c'est *précisément* pour que les fonctions puissent faire `movdqa` sur leurs variables locales et sauvegarder des `xmm` avec `movaps [rsp], xmm0`. Si tu appelles une fonction libc avec `rsp` désaligné, c'est un `movaps` dans son prologue qui plante. Tu as maintenant la raison du piège mentionné au tout début.

Sauvegarder un `xmm` sur la pile :

```nasm
sub    rsp, 16
movdqu [rsp], xmm6      ; movdqu par sécurité, movdqa si tu es sûr de l'alignement
; ...
movdqu xmm6, [rsp]
add    rsp, 16
```

Il n'y a pas de `push xmm0`.

---

## 9. Pièges de performance spécifiques au SIMD

### Transitions SSE ↔ AVX et `vzeroupper`

Si une instruction AVX a écrit dans un `ymm` (donc "sali" la moitié haute) et qu'ensuite une instruction SSE legacy écrit dans le `xmm` correspondant, le CPU doit préserver la moitié haute qu'il ne connaît pas → sur beaucoup de microarchitectures Intel, pénalité de plusieurs dizaines de cycles, voire état "lent" persistant. `vzeroupper` met à zéro les moitiés hautes de tous les `ymm` et remet l'état propre. Règle : **toute fonction qui touche un `ymm` termine par `vzeroupper`** avant `ret`, sauf si elle rend une valeur dans un `ymm`.

### Ne pas mélanger les domaines

Les CPU ont des unités "entier vectoriel" et "flottant vectoriel". `movaps` (float) et `movdqa` (entier) font la même chose, mais utiliser `movaps` pour déplacer un résultat de `pcmpeqb` peut coûter un cycle de *bypass delay*. Règle simple : préfixe `p` pour tout ce qui est entier, `ps`/`pd` pour les flottants.

### Latence vs débit

`pcmpeqb` : latence 1 cycle, débit 3 par cycle sur un cœur récent. `pmovmskb` : latence 2–3 cycles. La boucle du §5 est limitée par la dépendance `pcmpeqb → pmovmskb → test → jz`, pas par les unités d'exécution. Les implémentations très optimisées (glibc) déroulent 4 blocs par itération et combinent les masques avec `pminub`/`por` avant un seul `pmovmskb`, pour n'avoir qu'une branche toutes les 64 octets.

### Détection à l'exécution

Rien ne garantit qu'un CPU supporte AVX2. Le code de production lit `cpuid` au premier appel et choisit l'implémentation (c'est ce que fait la glibc avec ses *ifuncs*). Pour libasm, compile et exécute sur la même machine, donc pas nécessaire ; mais ne rends pas une version AVX-512 à un correcteur sur un laptop qui ne l'a pas.

```nasm
mov eax, 7
xor ecx, ecx
cpuid
bt  ebx, 5          ; bit 5 de EBX = AVX2
jc  .has_avx2
```

---

## 10. Au-delà de `strlen` : le vocabulaire complet, par famille

Une fois `strlen` compris, le reste de libasm se vectorise avec quelques instructions supplémentaires. Voici la carte, sans détail :

| Besoin | Instructions | Fonction libasm |
|---|---|---|
| Chercher un octet précis | `pcmpeqb` contre un registre "broadcast" de cet octet (`movd` + `punpcklbw` + `pshufd`, ou `vpbroadcastb` en AVX2) | `ft_strchr` |
| Comparer deux blocs | `pcmpeqb` entre deux chargements, `pmovmskb`, `xor edx, 0xFFFF` pour trouver la première **différence** | `ft_strcmp`, `ft_memcmp` |
| Copier | `movdqu` chargé / `movdqu` rangé, 16 octets par tour, prologue/épilogue scalaires pour le reste | `ft_memcpy`, `ft_strcpy` |
| Combiner plusieurs masques | `por`, `pand`, `pminub` (min par octet : si l'un des blocs contient 0, le min contient 0) | déroulage de boucle |
| Réarranger des octets | `pshufb` (SSSE3) : table de permutation arbitraire | conversions, `ft_strrev` |
| Comparaison de chaînes en une instruction | `pcmpistri` (SSE4.2) : compare, cherche, renvoie l'index et les flags | `strlen`/`strchr` "en une ligne", mais lent en pratique |

Le pattern est toujours le même : **charger large → comparer en parallèle → réduire à un masque scalaire → `bsf`/`tzcnt` → gérer les bords avec de l'alignement ou un prologue scalaire.**

---

## 11. Fiche récapitulative

```
Registres      xmm0-15 (128) ⊂ ymm0-15 (256) ⊂ zmm0-31 (512) ; tous caller-saved
Charger        movdqu [mem]  (quelconque) / movdqa [mem]  (aligné 16, sinon SIGSEGV)
Zéro           pxor xmm1, xmm1
Comparer       pcmpeqb xmm0, xmm1      → 0xFF/0x00 par octet, pas de flags
Réduire        pmovmskb edx, xmm0      → 16 bits, bit i = voie i
Premier bit    bsf / tzcnt eax, edx
Alignement     and rdi, -16 (SSE) / -32 (AVX2) : un bloc aligné ne traverse jamais une page
Bords          shr edx, cl pour éjecter les k = (adr & 15) octets avant la chaîne
AVX            préfixe v, 3 opérandes, vzeroupper avant ret
Test           chaîne collée à une page PROT_NONE + toutes les longueurs 0..64 + tous les offsets
```

## 12. Pour aller plus loin

- *Intel® 64 and IA-32 Architectures Software Developer's Manual*, vol. 2 : la référence de chaque instruction (recherche `PCMPEQB`, `PMOVMSKB`).
- *Agner Fog — Optimizing subroutines in assembly language* et ses tables d'instructions : latences et débits par microarchitecture.
- uops.info : mêmes données, mesurées, consultables en ligne.
- Le source de `sysdeps/x86_64/multiarch/strlen-sse2.S` et `strlen-avx2.S` dans la glibc : ce que tu viens d'écrire, en version industrielle.
- Intel Intrinsics Guide : chaque instruction avec son équivalent C (`_mm_cmpeq_epi8`, `_mm_movemask_epi8`) — utile quand tu passeras au C++ et voudras la même chose sans assembleur inline.
