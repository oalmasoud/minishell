#include "../../includes/minishell.h"

void copy_var_value(char *dst, char *src, int *j)
{
    int k = 0;
    while (src[k])
        dst[(*j)++] = src[k++];
}
