#include "../../includes/minishell.h"

void close_fds(t_command *cmds, bool close_backups)
{
    if (cmds->io_fds)
    {
        if (cmds->io_fds->input_fd != -1)
            close(cmds->io_fds->input_fd);
        if (cmds->io_fds->output_fd != -1)
            close(cmds->io_fds->output_fd);
        if (close_backups)
            restore_std_fds(cmds->io_fds);
    }
    close_unused_pipes(cmds, NULL);
}

void terminate_shell(t_shell_data *shell_context, int exit_code)
{

    if (shell_context)
    {
        if (shell_context->cmd && shell_context->cmd->io_fds)
            close_fds(shell_context->cmd, true);
        cleanup_shell_state(shell_context, true);
    }
    exit(exit_code);
}
