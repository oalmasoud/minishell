#include "../../includes/minishell.h"

int handle_env_builtin(t_shell_data *shell, char **arguments)
{
    int index = 0;

    if (arguments && arguments[1])
    {
        display_redirect_error("env", NULL, "too many arguments", false);
        return 2;
    }

    if (!shell->env)
        return EXIT_FAILURE;

    while (shell->env[index])
        ft_putendl_fd(shell->env[index++], STDOUT_FILENO);

    return EXIT_SUCCESS;
}
