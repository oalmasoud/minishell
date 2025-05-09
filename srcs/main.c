#include "../includes/minishell.h"

int g_code;

static bool validate_data(t_shell_data *data_shell, char **env)
{
    if (!validate_env(data_shell, env))
    {
        simple_error_message("Failed to set up the shell environment properly");
        return (false);
    }
    if (!validate_working_directories(data_shell))
    {
        simple_error_message("Failed to set up the working directories");
        return (false);
    }

    data_shell->tokens = NULL;
    data_shell->input_line = NULL;
    data_shell->cmd = NULL;
    data_shell->pid = -1;
    data_shell->is_active = true;
    g_code = 0;

    return (true);
}

void shell_activation(t_shell_data *data_shell)
{
    while (1)
    {
        configureInteractiveSignals();
        data_shell->input_line = readline(PROMPT);
        ignoreCommandSignals();
        if (process_input_command(data_shell) == true)
        {
            g_code = dispatch_execution_flow(data_shell);
        }
        else
        {
            g_code = 1;
        }
        cleanup_shell_state(data_shell, false);
    }
}

int main(int argc, char **argv, char **env)
{
    t_shell_data data_shell;
    ft_bzero(&data_shell, sizeof(t_shell_data));

    if (argc != 1)
    {
        ft_putendl_fd("Usage: ./minishell", STDERR_FILENO);
        return (EXIT_FAILURE);
    }

    using_history();

    if (!validate_data(&data_shell, env))
        return (EXIT_FAILURE);

    if (data_shell.is_active)
        shell_activation(&data_shell);

    return (EXIT_SUCCESS);
}