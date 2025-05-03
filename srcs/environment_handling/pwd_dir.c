#include "../../includes/minishell.h"

char *fetch_environment_var_value(char **env, char *var)
{
    char *prefix;
    char *value_start;
    size_t prefix_length;
    int i;

    prefix = ft_strjoin(var, "=");
    if (!prefix)
    {
        return (NULL);
    }
    prefix_length = ft_strlen(prefix);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(prefix, env[i], prefix_length) == 0)
        {
            value_start = ft_strchr(env[i], '=') + 1;
            if (value_start)
            {
                free(prefix);
                return (value_start);
            }
        }
        i++;
    }
    free(prefix);
    return (NULL);
}

int find_environment_var_index(char **env, char *var)
{
    int i;
    char *prefix;
    size_t prefix_length;

    prefix = ft_strjoin(var, "=");
    if (!prefix)
        return (-1);

    i = 0;
    prefix_length = ft_strlen(prefix);
    while (env[i])
    {
        if (ft_strncmp(prefix, env[i], prefix_length) == 0)
        {
            free(prefix);
            return (i);
        }
        i++;
    }
    free(prefix);
    return (-1);
}