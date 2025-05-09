#include "../../includes/minishell.h"

bool unset_env_variable(t_shell_data *shell, int index)
{
    int i;

    if (!shell || !shell->env || index < 0 || index >= var_count(shell->env))
        return false;

    free_safe_2(shell->env[index]);

    i = index;
    while (shell->env[i + 1])
    {
        shell->env[i] = shell->env[i + 1];
        i++;
    }

    shell->env[i] = NULL;

    shell->env = resize_env_array(shell, i);
    if (!shell->env)
        return false;

    return true;
}

int handle_unset_builtin(t_shell_data *shell, char **arguments)
{
    int index = 1;
    int found_index;
    int status = EXIT_SUCCESS;

    while (arguments[index])
    {
        if (!is_valid_env_var_key(arguments[index]) || ft_strchr(arguments[index], '='))
        {
            display_redirect_error("unset", arguments[index], "not a valid identifier", false);
            status = EXIT_FAILURE;
        }
        else
        {
            found_index = find_environment_var_index(shell->env, arguments[index]);
            if (found_index != -1)
                unset_env_variable(shell, found_index);
        }
        index++;
    }
    return status;
}
