#include "../../includes/minishell.h"

bool apply_io_redirection(t_io_redirect *redirects)
{
    bool success = true;

    if (!redirects)
        return true;

    redirects->stdin_backup = dup(STDIN_FILENO);
    if (redirects->stdin_backup == -1)
    {
        display_redirect_error("dup", "stdin backup", strerror(errno), false);
        success = false;
    }

    redirects->stdout_backup = dup(STDOUT_FILENO);
    if (redirects->stdout_backup == -1)
    {
        display_redirect_error("dup", "stdout backup", strerror(errno), false);
        success = false;
    }

    if (redirects->input_fd != -1)
    {
        if (dup2(redirects->input_fd, STDIN_FILENO) == -1)
        {
            display_redirect_error("dup2", redirects->input_file, strerror(errno), false);
            success = false;
        }
    }

    if (redirects->output_fd != -1)
    {
        if (dup2(redirects->output_fd, STDOUT_FILENO) == -1)
        {
            display_redirect_error("dup2", redirects->output_file, strerror(errno), false);
            success = false;
        }
    }

    return success;
}

bool restore_std_fds(t_io_redirect *redirect)
{
    bool success = true;

    if (!redirect)
        return success;

    if (redirect->stdin_backup != -1)
    {
        if (dup2(redirect->stdin_backup, STDIN_FILENO) == -1)
            success = false;
        close(redirect->stdin_backup);
        redirect->stdin_backup = -1;
    }

    if (redirect->stdout_backup != -1)
    {
        if (dup2(redirect->stdout_backup, STDOUT_FILENO) == -1)
            success = false;
        close(redirect->stdout_backup);
        redirect->stdout_backup = -1;
    }

    return success;
}

void release_redirect_resources(t_io_redirect *redirect)
{
    if (!redirect)
        return;

    restore_std_fds(redirect);

    if (redirect->heredoc_tag)
    {
        unlink(redirect->input_file);
        free_safe_2(redirect->heredoc_tag);
    }

    if (redirect->input_file)
        free_safe_2(redirect->input_file);

    if (redirect->output_file)
        free_safe_2(redirect->output_file);

    free_safe_2(redirect);
}

void free_command_node(t_command *node, void (*del)(void *))
{
    if (!node)
        return;

    if (node->command)
        del(node->command);

    if (node->args)
        free_string_array(node->args);

    if (node->pipe_fd)
        del(node->pipe_fd);

    if (node->io_fds)
        release_redirect_resources(node->io_fds);

    del(node);
}

void free_command_list(t_command **head, void (*del)(void *))
{
    t_command *next;

    if (!head || !*head)
        return;

    while (*head)
    {
        next = (*head)->next;
        free_command_node(*head, del);
        *head = next;
    }
}