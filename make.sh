rm -f *.out
echo "removed old a.out |"
gcc -Wall -Wextra -Werror -g microshell.c -o a.out
echo "----------------- |"
echo "new a.out created |"
