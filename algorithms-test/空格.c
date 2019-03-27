#include <stdio.h>

int main(void)
{
    printf("%*c%d\n", 5, ' ', 3);
    printf("%*c\n", 5, 'a');
    printf("%c%c%c%c%c\n", 'a', 'a', 'a', 'a', 'a');

    return 0;
}