#include "../../includes/minishell.h"

static bool should_suppress_exit_message(t_shell_data *data_shell)
{
    if (!data_shell->cmd)
        return (false);

    return (data_shell->cmd->next != NULL || data_shell->cmd->prev != NULL);
}

int execute_exit_command(t_shell_data *data_shell, char **args)
{
    int exit_code;
    bool error;
    bool quiet;

    quiet = should_suppress_exit_message(data_shell);
    error = false;
    if (!quiet && data_shell->is_active)
        ft_putendl_fd("exit", 2);
}