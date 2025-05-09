#include "../../includes/minishell.h"

static int wait_for_all_children(t_shell_data *shell_ctx)
{
    pid_t child_pid;
    int status;
    int last_status = 0;

    close_fds(shell_ctx->cmd, false);
    errno = 0;
    child_pid = 0;

    while ((child_pid = waitpid(-1, &status, 0)) > 0 || (child_pid == -1 && errno != ECHILD))
    {
        if (child_pid == shell_ctx->pid)
            last_status = status;
    }

    if (WIFSIGNALED(last_status))
        return 128 + WTERMSIG(last_status);
    else if (WIFEXITED(last_status))
        return WEXITSTATUS(last_status);
    else
        return last_status;
}

int launch_command_processes(t_shell_data *shell)
{
    t_command *cmd = shell->cmd;

    while (shell->pid != 0 && cmd)
    {
        shell->pid = fork();
        if (shell->pid == -1)
        {
            display_redirect_error("fork", NULL, strerror(errno), false);
            return EXIT_FAILURE;
        }
        else if (shell->pid == 0)
        {
            exec_child_command(shell, cmd);
        }
        cmd = cmd->next;
    }
    return wait_for_all_children(shell);
}

int dispatch_builtin(t_shell_data *shell, t_command *cmd)
{
    if (!cmd || !cmd->command)
        return CMD_NOT_FOUND;

    if (ft_strncmp(cmd->command, "cd", 3) == 0)
        return handle_cd_builtin(shell, cmd->args);
    else if (ft_strncmp(cmd->command, "echo", 5) == 0)
        return handle_echo_builtin(shell, cmd->args);
    else if (ft_strncmp(cmd->command, "env", 4) == 0)
        return handle_env_builtin(shell, cmd->args);
    else if (ft_strncmp(cmd->command, "export", 7) == 0)
        return handle_export_builtin(shell, cmd->args);
    else if (ft_strncmp(cmd->command, "pwd", 4) == 0)
        return handle_pwd_builtin(shell, cmd->args);
    else if (ft_strncmp(cmd->command, "unset", 6) == 0)
        return handle_unset_builtin(shell, cmd->args);
    else if (ft_strncmp(cmd->command, "exit", 5) == 0)
        return handle_exit_builtin(shell, cmd->args);

    return CMD_NOT_FOUND;
}
