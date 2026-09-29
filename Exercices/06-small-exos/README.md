# 06 small exos
Small exercices ; strcmp, strcpy, swap, read, strdup, check_base, fizz_buzz
fizz buzz https://asmtutor.com/#lesson18 mais je crois je vais devoir faire un putnbr


## ex00 - strcmp
On va build strcmp;

int strcmp(const char *s1, const char *s2);

Pour rappel le fonctionnement de strcmp ;

on compare s1 a s2.

```C
char *str = "chat";
char *str2 = "chatons";
printf("%d\n", strcmp(str, str2)); //produit -111 par (\0 - 'o')
```

S'il manque un argument c'est un segmentation fault.

On va jusque la longeur de s1 pour les check ou alors des qu'il y a une difference entre s1 et s2.
