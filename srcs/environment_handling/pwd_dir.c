#include "../../includes/minishell.h"

char **resize_env_array(t_shell_data *shell, int new_size)
{
    char **resized_env;
    int i = 0;

    resized_env = ft_calloc(new_size + 1, sizeof(char *));
    if (!resized_env)
        return NULL;

    while (shell->env[i] && i < new_size)
    {
        resized_env[i] = ft_strdup(shell->env[i]);
        free_safe_2(shell->env[i]);
        i++;
    }

    free_safe_2(shell->env);
    return resized_env;
}

int var_count(char **env)
{
    int i;

    i = 0;
    while (env && env[i])
        i++;
    return (i);
}

char *fetch_environment_var_value(char **env, char *var)
{
    int i;
    char *tmp;

    tmp = ft_strjoin(var, "=");
    if (!tmp)
        return (NULL);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
        {
            free_safe_2(tmp);
            return (ft_strchr(env[i], '=') + 1);
        }
        i++;
    }
    free_safe_2(tmp);
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
            free_safe_2(prefix);
            return (i);
        }
        i++;
    }
    free_safe_2(prefix);
    return (-1);
}