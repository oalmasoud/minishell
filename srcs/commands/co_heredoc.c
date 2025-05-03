#include "../../includes/minishell.h"

bool write_heredoc_input(t_shell_data *data, t_io_redirect *redirect, int fd)
{
    char *line = NULL;
    bool stop_reading = false;

    while (1)
    {
        set_signals_interactive();
        line = readline(">");
        set_signals_noninteractive();

        if (!evaluate_heredoc_input(data, &line, redirect, &stop_reading))
            break;

        ft_putendl_fd(line, fd);
        free_safe_2(line);
    }
    free_safe_2(line);
    return stop_reading;
}

bool create_and_fill_heredoc(t_shell_data *data, t_io_redirect *redirect)
{
    int fd;
    bool success;

    success = true;
    fd = open(redirect->input_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    success = write_heredoc_input(data, redirect, fd);
    close(fd);
    return success;
}

static char *extract_delimiter(char *raw_delim, bool *is_quoted)
{
    int len = ft_strlen(raw_delim) - 1;

    if ((raw_delim[0] == '"' && raw_delim[len] == '"') || (raw_delim[0] == '\'' && raw_delim[len] == '\''))
    {
        *is_quoted = true;
        return ft_strtrim(raw_delim, "\'\"");
    }
    return ft_strdup(raw_delim);
}

static char *generate_heredoc_filename(void)
{
    static int counter = 0;
    char *filename;
    char *suffix;

    suffix = ft_itoa(counter);
    if (!suffix)
        return NULL;
    filename = ft_strjoin(HEREDOC_NAME, suffix);
    free(suffix);
    counter++;
    return filename;
}

void handle_heredoc_redirection(t_shell_data *data, t_command **command_list, t_token **token_stream)
{
    t_token *current_token = *token_stream;
    t_command *current_command = get_last_command_node(*command_list);
    t_io_redirect *redirect;

    initialize_redirect_struct(current_command);
    redirect = current_command->io_fds;

    if (!clear_previous_file_ref(redirect, true))
        return;

    redirect->input_file = generate_heredoc_filename();
    redirect->heredoc_tag = extract_delimiter(current_token->next->token_value, &(redirect->heredoc_is_quoted));

    if (create_and_fill_heredoc(data, redirect))
        redirect->input_fd = open(redirect->input_file, O_RDONLY);
    else
        redirect->input_fd = -1;

    if (current_token->next->next)
        current_token = current_token->next->next;
    else
        current_token = current_token->next;

    *token_stream = current_token;
}