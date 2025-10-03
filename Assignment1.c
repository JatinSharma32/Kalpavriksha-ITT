#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Operator Priority
int priority(char operatorChar)
{
    if (operatorChar == '+' || operatorChar == '-')
        return 1;
    else if (operatorChar == '/' || operatorChar == '*')
        return 2;
    printf("\nERROR : Invalid Operator.");
    exit(1);
}

// Operand Stack
int valueStackTop = -1;
int valueStack[100];
void pushValue(int value)
{
    if (valueStackTop >= 99)
    {
        printf("Stack Overflow");
        exit(1);
    }
    valueStack[++valueStackTop] = value;
    return;
}
int popValue()
{
    if (valueStackTop <= -1)
    {
        printf("\nERROR : Invalid Expression, Value Stack Empty");
        exit(1);
    }
    int value = valueStack[valueStackTop];
    valueStackTop--;
    return value;
}
int peekValue()
{
    if (valueStackTop <= -1)
    {
        printf("\nERROR : Invalid Expression, Value Stack Empty");
        exit(1);
    }
    return valueStack[valueStackTop];
}

// Operator Stack
int operatorStackTop = -1;
char operatorStack[100];
void pushOperator(char character)
{
    if (operatorStackTop >= 99)
    {
        printf("Stack Overflow");
        exit(1);
    }
    operatorStack[++operatorStackTop] = character;
    return;
}
char popOperator()
{
    if (operatorStackTop <= -1)
    {
        printf("\nERROR : Invalid Expression, Operator Stack Empty");
        exit(1);
    }
    char character = operatorStack[operatorStackTop];
    operatorStackTop--;
    return character;
}
char peekOperator()
{
    if (operatorStackTop <= -1)
    {
        printf("\nERROR : Invalid Expression, Operator Stack Empty");
        exit(1);
    }
    return operatorStack[operatorStackTop];
}

// Extraction function
int extractNumber(char *expression, int *indexPtr)
{
    int number = 0;
    while (expression[*indexPtr] >= '0' && expression[*indexPtr] <= '9')
    {
        number = (number * 10) + (expression[*indexPtr] - '0');
        (*indexPtr)++;
    }
    (*indexPtr)--;
    return number;
}

// Airthmatic Operations logic
void performOperation()
{
    char operatorChar = popOperator();
    int firstOperand = popValue();
    int secondOperand = popValue();
    if (operatorChar == '+')
    {
        pushValue(firstOperand + secondOperand);
    }
    else if (operatorChar == '-')
    {
        pushValue(secondOperand - firstOperand);
    }
    else if (operatorChar == '*')
    {
        pushValue(firstOperand * secondOperand);
    }
    else if (operatorChar == '/')
    {
        if (firstOperand == 0)
        {
            printf("\n ERROR : Division by Zero.\n");
            exit(1);
        }
        pushValue(secondOperand / firstOperand);
    }
}

// Main logic
int evaluateExpression(char *expression)
{
    for (int index = 0; expression[index] != 0; index++)
    {
        if (expression[index] >= '0' && expression[index] <= '9')
        {
            pushValue(extractNumber(expression, &index));
        }
        else if (expression[index] == '+' || expression[index] == '*' || expression[index] == '-' || expression[index] == '/')
        {
            while (operatorStackTop != -1 && priority(expression[index]) <= priority(peekOperator()))
            {
                performOperation();
            }
            pushOperator(expression[index]);
        }
        else if (expression[index] == ' ' || expression[index] == '\n')
        {
            continue;
        }
        else
        {
            printf("\nERROR : Invalid Expression");
            exit(1);
        }
    }
    while (operatorStackTop != -1)
    {
        performOperation();
    }
    return popValue();
}

int main(int argc, char **argv)
{
    printf("Enter a valid expression: ");
    char expression[100];
    fgets(expression, sizeof(expression), stdin);
    bool hasContent = false;
    for (int i = 0; expression[i] != '\0'; i++)
    {
        if (expression[i] != ' ' && expression[i] != '\n' && expression[i] != '\t')
        {
            hasContent = true;
            break;
        }
    }
    if (!hasContent)
    {
        printf("\nERROR : Invalid Expression");
        exit(1);
    }
    printf("\nANS: %d\n", evaluateExpression(expression));
    return 0;
}