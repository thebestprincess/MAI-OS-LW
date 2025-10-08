# Вариант 8
### Условие
В файле записаны команды вида:«число число число<endline>». Дочерний процесс
производит деление первого числа команда, на последующие числа в команде, а результат
выводит в стандартный поток вывода. Если происходит деление на 0, то тогда дочерний и
родительский процесс завершают свою работу. Проверка деления на 0 должна осуществляться на
стороне дочернего процесса. Числа имеют тип int. Количество чисел может быть произвольным.

---
### Compile

```
gcc main.c -o main
gcc child.c -o child
```

*IMPORTANT*: child executable expects the child executable to be named `child` and be located near the parent, otherwise the child will exit with error!

---
### Test files
If you want to get test files, you can create them in same directory using create_test.sh
```
./create_test.sh
```
