#include "../../includes/minishell.h"

bool clear_previous_file_ref(t_io_redirect *redirect, bool is_input)
{
    if (is_input && redirect->input_file)
    {
        if (redirect->input_fd == -1 || (redirect->output_file && redirect->output_fd == -1))
            return false;

        if (redirect->heredoc_tag)
        {
            free_safe_2(redirect->heredoc_tag);
            redirect->heredoc_tag = NULL;
            unlink(redirect->input_file);
        }
        free_safe_2(redirect->input_file);
        close(redirect->input_fd);
    }
    else if (!is_input && redirect->output_file)
    {
        if (redirect->output_fd == -1 || (redirect->input_file && redirect->input_fd == -1))
            return false;

        free_safe_2(redirect->output_file);
        close(redirect->output_fd);
    }
    return true;
}

static void configure_input_file(t_io_redirect *redirect, char *filepath, char *original_name)
{
    if (!clear_previous_file_ref(redirect, true))
        return;

    redirect->input_file = ft_strdup(filepath);
    if (redirect->input_file && redirect->input_file[0] == '\0')
    {
        display_redirect_error(original_name, NULL, "ambiguous redirect", false);
        return;
    }

    redirect->input_fd = open(redirect->input_file, O_RDONLY);
    if (redirect->input_fd == -1)
        display_redirect_error(redirect->input_file, NULL, strerror(errno), false);
}

void initialize_redirect_struct(t_command *cmd_node)
{
    if (!cmd_node->io_fds)
    {
        cmd_node->io_fds = malloc(sizeof(t_io_redirect));
        if (!cmd_node->io_fds)
            return;

        cmd_node->io_fds->input_file = NULL;
        cmd_node->io_fds->output_file = NULL;
        cmd_node->io_fds->heredoc_tag = NULL;
        cmd_node->io_fds->heredoc_is_quoted = false;
        cmd_node->io_fds->input_fd = -1;
        cmd_node->io_fds->output_fd = -1;
        cmd_node->io_fds->stdin_backup = -1;
        cmd_node->io_fds->stdout_backup = -1;
    }
}

void handle_input_redirection(t_command **cmd_list, t_token **token_stream)
{
    t_token *current_token = *token_stream;
    t_command *target_cmd = get_last_command_node(*cmd_list);

    initialize_redirect_struct(target_cmd);

    if (current_token->next)
    {
        configure_input_file(
            target_cmd->io_fds,
            current_token->next->token_value,
            current_token->next->str_original);

        *token_stream = current_token->next->next ? current_token->next->next : current_token->next;
    }
}
