#include "../../includes/minishell.h"

int check_local_command_validity(t_shell_data *shell, t_command *cmd)
{
    if (ft_strchr(cmd->command, '/') == NULL &&
        find_environment_var_index(shell->env, "PATH") != -1)
    {
        display_redirect_error(cmd->command, NULL, "command not found", false);
        return CMD_NOT_FOUND;
    }

    if (access(cmd->command, F_OK) != 0)
    {
        display_redirect_error(cmd->command, NULL, strerror(errno), false);
        return CMD_NOT_FOUND;
    }

    if (is_directory_command(cmd->command))
    {
        display_redirect_error(cmd->command, NULL, "Is a directory", false);
        return CMD_NOT_EXECUTABLE;
    }

    if (access(cmd->command, F_OK | X_OK) != 0)
    {
        display_redirect_error(cmd->command, NULL, strerror(errno), false);
        return CMD_NOT_EXECUTABLE;
    }

    return EXIT_SUCCESS;
}

int run_local_executable(t_shell_data *shell, t_command *cmd)
{
    int result;

    result = check_local_command_validity(shell, cmd);
    if (result != EXIT_SUCCESS)
        return result;

    if (execve(cmd->command, cmd->args, shell->env) == -1)
    {
        display_redirect_error("execve", NULL, strerror(errno), false);
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
