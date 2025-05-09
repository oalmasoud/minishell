#include "../../includes/minishell.h"

static char **split_env_assignment(const char *arg)
{
    char **pair = malloc(sizeof(char *) * 3);
    char *eq_pos;

    if (!pair)
        return NULL;

    eq_pos = ft_strchr(arg, '=');
    if (!eq_pos)
        return NULL;

    pair[0] = ft_substr(arg, 0, eq_pos - arg);
    pair[1] = ft_substr(eq_pos + 1, 0, ft_strlen(eq_pos + 1));
    pair[2] = NULL;

    if (!pair[0] || !pair[1])
    {
        free_string_array(pair);
        return NULL;
    }

    return pair;
}

bool is_valid_env_var_key(char *var)
{
    int i;

    i = 0;
    if (ft_isalpha(var[i]) == 0 && var[i] != '_')
        return (false);
    i++;
    while (var[i] && var[i] != '=')
    {
        if (ft_isalnum(var[i]) == 0 && var[i] != '_')
            return (false);
        i++;
    }
    return (true);
}

int handle_export_builtin(t_shell_data *shell, char **args)
{
    int i;
    char **tmp;
    int ret;

    ret = EXIT_SUCCESS;
    i = 1;
    if (!args[i])
        return (handle_env_builtin(shell, NULL));
    while (args[i])
    {
        if (!is_valid_env_var_key(args[i]))
        {
            display_redirect_error("export", args[i], "not a valid identifier", false);
            ret = EXIT_FAILURE;
        }
        else if (ft_strchr(args[i], '=') != NULL)
        {
            tmp = split_env_assignment(args[i]);
            update_env_variable(shell, tmp[0], tmp[1]);
            free_string_array(tmp);
        }
        i++;
    }
    return (ret);
}
