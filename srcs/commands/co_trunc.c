#include "../../includes/minishell.h"

static void configure_output_trunc(t_io_redirect *redirect, char *filepath, char *original_name)
{
    if (!clear_previous_file_ref(redirect, false))
        return;

    redirect->output_file = ft_strdup(filepath);
    if (redirect->output_file && redirect->output_file[0] == '\0')
    {
        display_redirect_error(original_name, NULL, "unknown redirect", false);
        return;
    }

    redirect->output_fd = open(redirect->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (redirect->output_fd == -1)
        display_redirect_error(redirect->output_file, NULL, strerror(errno), false);
}

void handle_output_truncation(t_command **command_list, t_token **token_stream)
{
    t_token *current = *token_stream;
    t_command *target_cmd = get_last_command_node(*command_list);

    initialize_redirect_struct(target_cmd);
    configure_output_trunc(
        target_cmd->io_fds,
        current->next->token_value,
        current->next->str_original);

    *token_stream = current->next->next ? current->next->next : current->next;
}
