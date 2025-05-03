#include "../../includes/minishell.h"

static int env_counter(char **env)
{
    int i;

    i = 0;
    while (env && env[i])
        i++;

    return (i);
}

// char *testing_strdup(const char *str)
// {
//     static int count = 0;
//     count++;
//     if (count == 3)
//     { // Simulate failure on the third call
//         return NULL;
//     }
//     return strdup(str);
// }

bool validate_env(t_shell_data *data_shell, char **env)
{
    int i;

    data_shell->env = ft_calloc(env_counter(env) + 1, sizeof(*data_shell->env));
    if (!data_shell->env)
        return (false);
    for (i = 0; env[i] != NULL; i++)
    {
        data_shell->env[i] = ft_strdup(env[i]); // testing_strdup(env[i]);
        if (!data_shell->env[i])
        {
            while (--i >= 0)
            {
                free(data_shell->env[i]);
            }
            free(data_shell->env);
            return (false);
        }
    }

    data_shell->env[i] = NULL;
    return (true);
}

bool validate_working_directories(t_shell_data *data_shell, char **env)
{
    int oldPwdIndex;
    char *wd;
    const char *oldPwd;

    wd = getcwd(NULL, 0);

    if (!wd)
        return (false);
    data_shell->current_directory = wd;

    oldPwdIndex = find_environment_var_index(data_shell->env, "OLDPWD");
    if (oldPwdIndex != -1)
    {
        oldPwd = fetch_environment_var_value(data_shell->env, "OLDPWD");
        data_shell->previous_directory = ft_strdup(oldPwd);
        if (!data_shell->previous_directory)
        {
            free(wd);
            return (false);
        }
    }
    else
    {
        data_shell->previous_directory = ft_strdup(wd);
        if (!data_shell->previous_directory)
        {
            free(wd);
            return (false);
        }
    }
    free(wd);
    return (true);
}