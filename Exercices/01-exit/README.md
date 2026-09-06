# Exercice 01 - Exit and more

Objectif de cet exercice ; ecrire un premier programme avec NASM, comprendre et observer

## Exercices

### ex00 - premier programme

Ecrire un premier programme avec pour objectif de juste quitter avec la valeur 42. On doit quitter en utilisant exit().

```sh
./exit
echo $?       # doit afficher 42
strace ./exit # doit voir exit(42)
```
Outil a utiliser ; nasm, ld, strace
Fichiers ; exit.s => produit un .o => produit l'executable qu'on nomme exit

### ex01 - objserver la difference

Si vous faites un programme en C qui ne fait que return 42; et ensuite vous utilisez strace dessus, vous verrez une difference. 

Pourquoi il y a cette difference ?

### ex02 - observabilite avec GDB 

Sans flag de debug essayer d'observer le code en assembly depuis l'executable exit pour le parcourir et reussir a print les valeurs des registres

### ex03 - compile avec flag de debug

Objectif ; trouver comment compiler avec nasm pour avoir le debug comme il faut (j'insiste sur le comme il faut)

### ex04 - observabilite avec objdump

Ici rien de special mais on peut observer avec d'autres CLI tools.

objdump -d -M intel exit
-d pour disassemble les sections executables
-D possible a la place va tout montrer (a tester)
-M intel pour que ca soit la bonne flavor



