#include <stdio.h>
#include <stdlib.h>

// Operator Priority
int priority(char operator)
{
    if (operator == '+' || operator == '-')
        return 1;
    else if (operator == '/' || operator == '*')
        return 2;
    printf("\nERROR : Invalid Operator.");
    exit(1);
}

// Operand Stack
int topVal = -1;
int value[100];
void pushVal(int n)
{
    if (topVal >= 99)
    {
        printf("Stack Overflow");
        exit(1);
    }
    value[++topVal] = n;
    return;
}
int popVal()
{
    if (topVal <= -1)
    {
        printf("\nERROR : Invalid Expression, Value Stack Empty");
        exit(1);
    }
    int n = value[topVal];
    topVal--;
    return n;
}
int peekVal()
{
    if (topVal <= -1)
    {
        printf("\nERROR : Invalid Expression, Value Stack Empty");
        exit(1);
    }
    return value[topVal];
}

// Operator Stack
int topOpr = -1;
char opr[100];
void pushOpr(char ch)
{
    if (topOpr >= 99)
    {
        printf("Stack Overflow");
        exit(1);
    }
    opr[++topOpr] = ch;
    return;
}
char popOpr()
{
    if (topOpr <= -1)
    {
        printf("\nERROR : Invalid Expression, Operator Stack Empty");
        exit(1);
    }
    char ch = opr[topOpr];
    topOpr--;
    return ch;
}
char peekOpr()
{
    if (topOpr <= -1)
    {
        printf("\nERROR : Invalid Expression, Operator Stack Empty");
        exit(1);
    }
    return opr[topOpr];
}

// Extraction function
int extract(char *exp, int *i)
{
    int val = 0;
    while (exp[*i] >= '0' && exp[*i] <= '9')
    {
        val = (val * 10) + (exp[*i] - '0');
        (*i)++;
    }
    (*i)--;
    return val;
}

// Airthmatic Operations logic
void operations()
{
    char operator = popOpr();
    int a = popVal();
    int b = popVal();
    if (operator == '+')
    {
        pushVal(a + b);
    }
    else if (operator == '-')
    {
        pushVal(b - a);
    }
    else if (operator == '*')
    {
        pushVal(a * b);
    }
    else if (operator == '/')
    {
        if (a == 0)
        {
            printf("\n ERROR : Division by Zero.\n");
            exit(1);
        }
        pushVal(b / a);
    }
}

// Main logic
int solve(char *exp)
{
    for (int i = 0; exp[i] != 0; i++)
    {
        if (exp[i] >= '0' && exp[i] <= '9')
        {
            pushVal(extract(exp, &i));
        }
        else if (exp[i] == '+' || exp[i] == '*' || exp[i] == '-' || exp[i] == '/')
        {
            while (topOpr != -1 && priority(exp[i]) <= priority(peekOpr()))
            {
                operations();
            }
            pushOpr(exp[i]);
        }
        else if (exp[i] == ' ' || exp[i] == '\n')
        {
            continue;
        }
        else
        {
            printf("\nERROR : Invalid Expression");
            exit(1);
        }
    }
    while (topOpr != -1)
    {
        operations();
    }
    return popVal();
}

int main(int argc, char **argv)
{
    printf("Enter a valid expression: ");
    char exp[100];
    fgets(exp, sizeof(exp), stdin);
    printf("\nANS: %d\n", solve(exp));
    return 0;
}