#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int digitSum(int n)
{
    int sum = 0;
    while (n > 0)
    {
        sum += (n % 10);
        n /= 10;
    }
    return sum;
}

int main()
{

    int n;
    scanf("%d", &n);
    printf("%d", digitSum(n));
    return 0;
}