#include "../../includes/minishell.h"

bool is_directory_command(char *cmd_name)
{
    struct stat path_info;
    ft_memset(&path_info, 0, sizeof(path_info));
    stat(cmd_name, &path_info);
    if (stat(cmd_name, &path_info) == -1)
        return false;
    return S_ISDIR(path_info.st_mode);
}

static int execute_system_command(t_shell_data *shell, t_command *cmd)
{
    if (!cmd->command || cmd->command[0] == '\0')
        return CMD_NOT_FOUND;

    if (is_directory_command(cmd->command))
        return CMD_NOT_FOUND;

    cmd->path = resolve_command_path(shell, cmd->command);
    if (!cmd->path)
        return CMD_NOT_FOUND;

    if (execve(cmd->path, cmd->args, shell->env) == -1)
        display_redirect_error("execve", NULL, strerror(errno), false);

    return EXIT_FAILURE;
}

int exec_child_command(t_shell_data *shell, t_command *cmd)
{
    int ret;

    if (!cmd || !cmd->command)
    {
        display_redirect_error("child", NULL, "parsing error: no command to execute!", false);
        terminate_shell(shell, EXIT_FAILURE);
    }

    if (!validate_redirection_fds(cmd->io_fds))
        terminate_shell(shell, EXIT_FAILURE);

    configure_pipe_redirection(shell->cmd, cmd);
    apply_io_redirection(cmd->io_fds);
    close_fds(shell->cmd, false);

    if (!ft_strchr(cmd->command, '/'))
    {
        ret = dispatch_builtin(shell, cmd);
        if (ret != CMD_NOT_FOUND)
            terminate_shell(shell, ret);

        ret = execute_system_command(shell, cmd);
        if (ret != CMD_NOT_FOUND)
            terminate_shell(shell, ret);
    }

    ret = run_local_executable(shell, cmd);
    terminate_shell(shell, ret);
    return ret;
}
