#include "../../includes/minishell.h"

static void assign_command_args_if_missing(t_shell_data *data_shell)
{
    t_command *current = data_shell->cmd;

    if (!data_shell || !current)
        return;

    while (current)
    {
        if (current->command && !current->args)
        {
            current->args = malloc(sizeof(char *) * 2);
            if (!current->args)
                return;
            current->args[0] = ft_strdup(current->command);
            current->args[1] = NULL;
        }
        current = current->next;
    }
}

void build_command_list(t_shell_data *data_shell, t_token *tokens)
{
    t_token *current_token = tokens;

    if (!current_token || current_token->token_type == END)
        return;

    while (current_token)
    {
        if (current_token == tokens)
            append_command_node(&data_shell->cmd, create_new_command(false));

        if (current_token->token_type == WORD || current_token->token_type == VAR)
            parse_command_tokens(&data_shell->cmd, &current_token);
        else if (current_token->token_type == INPUT)
            handle_input_redirection(&data_shell->cmd, &current_token);
        else if (current_token->token_type == TRUNC)
            handle_output_truncation(&data_shell->cmd, &current_token);
        else if (current_token->token_type == HEREDOC)
            handle_heredoc_redirection(data_shell, &data_shell->cmd, &current_token);
        else if (current_token->token_type == APPEND)
            handle_output_append(&data_shell->cmd, &current_token);
        else if (current_token->token_type == PIPE)
            handle_pipe_token(&data_shell->cmd, &current_token);
        else if (current_token->token_type == END)
            break;
    }
    assign_command_args_if_missing(data_shell);
}
