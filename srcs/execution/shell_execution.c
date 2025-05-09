#include "../../includes/minishell.h"

void cleanup_shell_state(t_shell_data *shell, bool full_cleanup)
{
    if (shell && shell->input_line)
    {
        free_safe_2(shell->input_line);
        shell->input_line = NULL;
    }

    if (shell && shell->tokens)
        clear_token_list(&shell->tokens, &free_safe_2);

    if (shell && shell->cmd)
        free_command_list(&shell->cmd, &free_safe_2);

    if (full_cleanup)
    {
        if (shell && shell->current_directory)
            free_safe_2(shell->current_directory);
        if (shell && shell->previous_directory)
            free_safe_2(shell->previous_directory);
        if (shell && shell->env)
            free_string_array(shell->env);

        rl_clear_history();
    }
}

bool initialize_command_pipes(t_shell_data *shell)
{
    t_command *current = shell->cmd;

    while (current)
    {
        bool needs_pipe = current->pipe_output || (current->prev && current->prev->pipe_output);
        if (needs_pipe)
        {
            int *pipe_fd = malloc(sizeof(int) * 2);
            if (!pipe_fd)
            {
                cleanup_shell_state(shell, false);
                return false;
            }

            if (pipe(pipe_fd) == -1)
            {
                free(pipe_fd);
                cleanup_shell_state(shell, false);
                return false;
            }

            current->pipe_fd = pipe_fd;
        }
        current = current->next;
    }
    return true;
}

bool validate_redirection_fds(t_io_redirect *redirects)
{
    if (!redirects || (!redirects->input_file && !redirects->output_file))
        return true;

    if ((redirects->input_file && redirects->input_fd == -1) ||
        (redirects->output_file && redirects->output_fd == -1))
        return false;

    return true;
}

int prepare_command_execution(t_shell_data *shell)
{
    if (!shell || !shell->cmd)
        return EXIT_SUCCESS;

    if (!shell->cmd->command)
    {
        if (shell->cmd->io_fds && !validate_redirection_fds(shell->cmd->io_fds))
            return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }

    if (!initialize_command_pipes(shell))
        return EXIT_FAILURE;

    return CMD_NOT_FOUND;
}

int dispatch_execution_flow(t_shell_data *data_shell)
{
    int ret;

    ret = prepare_command_execution(data_shell);
    if (ret != CMD_NOT_FOUND)
        return (ret);
    if (!data_shell->cmd->pipe_output && !data_shell->cmd->prev && validate_redirection_fds(data_shell->cmd->io_fds))
    {
        apply_io_redirection(data_shell->cmd->io_fds);
        ret = dispatch_builtin(data_shell, data_shell->cmd);
        restore_std_fds(data_shell->cmd->io_fds);
    }
    if (ret != CMD_NOT_FOUND)
        return (ret);
    return (launch_command_processes(data_shell));
}