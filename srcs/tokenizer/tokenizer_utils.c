#include "../../includes/minishell.h"

int ft_isspace(int c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f')
        return (c);
    return (0);
}
bool is_all_whitespace(char *input)
{
    int i;

    i = 0;
    while (input[i])
    {
        if (!ft_isspace(input[i]))
            return (false);
        i++;
    }
    return (true);
}