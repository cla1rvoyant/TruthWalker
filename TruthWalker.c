#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char* getExpression()
{
    char* expression = NULL;
    int length = 0;
    char ch;

    printf("f = ");
    while ((ch = getchar()) != '\n')
    {
        length++;
        char* temp = (char*)realloc(expression, (length + 1) * sizeof(char));
        if (!temp) exit(EXIT_FAILURE);
        expression = temp;
        expression[length - 1] = ch;
        expression[length] = '\0';
    }

    return expression;
}

char* getNames(char* expression)
{
    char* names = NULL;
    int variables = 0;
    int length = strlen(expression);
    _Bool same_variable = 0;
    char ch;

    for (int i = 0; i < length; i++)
    {
        ch = expression[i];
        if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
        {
            if (variables > 0)
            {
                same_variable = 0;
                for (int i = 0; i < variables; i++)
                {
                    if (names[i] == ch)
                    {
                        same_variable = 1;
                        break;
                    }
                }
            }

            if (!same_variable)
            {
                variables++;
                char* temp = (char*)realloc(names, (variables + 1) * sizeof(char));
                if (!temp) exit(EXIT_FAILURE);
                names = temp;
                names[variables - 1] = ch;
                names[variables] = '\0';
            }
        }
    }

    return names;
}

void unifyingAlphabetSort(char* names, int variables)
{
    _Bool bigger_letter = 1;
    char swap_ch;
    while (bigger_letter)
    {
        bigger_letter = 0;
        for (int i = 0; i < variables - 1; i++)
        {
            if (names[i] > names[i + 1])
            {
                bigger_letter = 1;
                swap_ch = names[i];
                names[i] = names[i + 1];
                names[i + 1] = swap_ch;
            }
        }
    }
}

void printVariableValues(int* variableValues, int variables)
{
    for (int i = 0; i < variables; i++)
        printf("%d ", variableValues[i]);
}

_Bool fullUnityCheck(int* variableValues, int variables)
{
    _Bool full = 1;
    for (int i = 0; i < variables; i++)
    {
        if (!variableValues[i])
            full = 0;
    }
    return full;
}

char* replace(char* expression, char* string, int beginIndex, int replaceLength)
{
    const size_t expressionLength = strlen(expression);
    const size_t stringLength = strlen(string);

    char* buffer = (char*)malloc((expressionLength - replaceLength + stringLength + 1) * sizeof(char));
    if (!buffer) exit(EXIT_FAILURE);
    buffer[expressionLength - replaceLength + stringLength] = '\0';

    for (int i = 0; i < beginIndex; i++)
        buffer[i] = expression[i];
    for (int i = beginIndex; i < beginIndex + stringLength; i++)
        buffer[i] = string[i - beginIndex];
    for (int i = beginIndex + replaceLength; i < expressionLength; i++)
        buffer[i - replaceLength + stringLength] = expression[i];

    return buffer;
}

char* getNumericExpression(char* expression, char* names, int* variablesValues)
{
    size_t const expressionLength = strlen(expression);
    size_t const variablesAmount = strlen(names);

    char* numericExpression = (char*)malloc((expressionLength + 1) * sizeof(char));
    if (!numericExpression) exit(EXIT_FAILURE);
    numericExpression[expressionLength] = '\0';

    for (int i = 0; i < expressionLength; i++)
    {
        if (expression[i] >= 'a' && expression[i] <= 'z' || expression[i] >= 'A' && expression[i] <= 'Z')
        {
            for (int j = 0; j < variablesAmount; j++)
            {
                if (expression[i] == names[j])
                    numericExpression[i] = variablesValues[j] + 48;
            }
        }
        else
            numericExpression[i] = expression[i];
    }

    return numericExpression;
}

char* parseExpression(char* expression)
{
    size_t expressionLength = strlen(expression);

    for (int i = 0; expression[i] != '\0'; i++)
    {
        if (expression[i] == '(')
        {
            int interExpressionLength = 0;
            int interIndex = i + 1;
            char* interExpression = NULL;
            while (expression[interIndex] != ')')
            {
                interExpressionLength++;
                char* temp = (char*)realloc(interExpression, (interExpressionLength + 1) * sizeof(char));
                if (!temp) exit(EXIT_FAILURE);
                interExpression = temp;
                interExpression[interExpressionLength - 1] = expression[interIndex];
                interIndex++;
            }
            interExpression[interExpressionLength] = '\0';
            interIndex = i;
            expression = replace(expression, parseExpression(interExpression), interIndex, interExpressionLength + 2);
        }
    }

    for (int i = 0; i < expressionLength; i++)
    {
        if (expression[i] == '!')
        {
            if (atoi(&expression[i + 1]))
                expression = replace(expression, "0", i, 2);
            else
                expression = replace(expression, "1", i, 2);
        }
    }

    for (int i = 0; i < expressionLength; i++)
    {
        if (expression[i] == '&')
        {
            if (atoi(&expression[i - 2]) && atoi(&expression[i + 2]))
                expression = replace(expression, "1", i - 2, 5);
            else
                expression = replace(expression, "0", i - 2, 5);
        }
    }

    for (int i = 0; i < expressionLength; i++)
    {
        if (expression[i] == '|')
        {
            if (atoi(&expression[i - 2]) || atoi(&expression[i + 2]))
                expression = replace(expression, "1", i - 2, 5);
            else
                expression = replace(expression, "0", i - 2, 5);
        }
    }

    for (int i = 0; i < expressionLength; i++)
    {
        if (expression[i] == '=' && expression[i + 1] == '>')
        {
            if (!atoi(&expression[i - 2]) || atoi(&expression[i + 3]))
                expression = replace(expression, "1", i - 2, 6);
            else
                expression = replace(expression, "0", i - 2, 6);
        }
    }

    for (int i = 0; i < expressionLength; i++)
    {
        if (expression[i] == '=' && expression[i + 1] == ' ')
        {
            if (atoi(&expression[i - 2]) == atoi(&expression[i + 2]))
                expression = replace(expression, "1", i - 2, 5);
            else
                expression = replace(expression, "0", i - 2, 5);
        }
    }

    return expression;
}

void TableCreation(char* expression, char* names, int length, int variables)
{
    int* variableValues = (int*)calloc(variables, sizeof(int));
    if (!variableValues) exit(EXIT_FAILURE);
    _Bool interUnityFull = 1;

    for (int i = 0; i < strlen(names); i++)
        printf("%c ", names[i]);
    printf("f");
    printf("\n");

    while (1)
    {
        printVariableValues(variableValues, variables);
        printf("%s", parseExpression(getNumericExpression(expression, names, variableValues)));
        printf("\n");

        if (fullUnityCheck(variableValues, variables))
            break;
        for (int i = variables - 1; i >= 0; i--)
        {
            if (variableValues[i] && !interUnityFull)
            {
                variableValues[variables - 1] = 1;
                interUnityFull = 1;
                break;
            }
            if (!variableValues[i] && interUnityFull)
            {
                variableValues[i] = 1;
                if (i < variables - 1)
                {
                    for (int j = variables - 1; j > i; j--)
                        variableValues[j] = 0;
                    interUnityFull = 0;
                }
                break;
            }
        }
    }
}

int main()
{
    char* expression = getExpression();
    int length = strlen(expression);

    char* names = getNames(expression);
    int variables = strlen(names);
    unifyingAlphabetSort(names, variables);

    TableCreation(expression, names, length, variables);
}
