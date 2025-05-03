#include "../../includes/minishell.h"

static void configure_output_append(t_io_redirect *redirect, char *filepath, char *original_filename)
{
    if (!clear_previous_file_ref(redirect, false))
        return;

    redirect->output_file = ft_strdup(filepath);
    if (redirect->output_file && redirect->output_file[0] == '\0' && original_filename)
    {
        display_redirect_error(original_filename, NULL, "unknown redirect", false);
        return;
    }

    redirect->output_fd = open(redirect->output_file, O_WRONLY | O_CREAT | O_APPEND, 0664);
    if (redirect->output_fd == -1)
        display_redirect_error(redirect->output_file, NULL, strerror(errno), false);
}

void handle_output_append(t_command **command_list, t_token **token_stream)
{
    t_token *current_token = *token_stream;
    t_command *current_command = get_last_command_node(*command_list);

    initialize_redirect_struct(current_command);

    configure_output_append(
        current_command->io_fds,
        current_token->next->token_value,
        current_token->next->str_original);

    *token_stream = current_token->next->next ? current_token->next->next : current_token->next;
}
