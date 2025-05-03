#include "../../includes/minishell.h"

bool process_input_command(t_shell_data *data_shell)
{
    if (data_shell->input_line == NULL)
    {
        execute_exit_command(data_shell, NULL);
    }
    else if (ft_strlen(data_shell->input_line) == 0 || is_all_whitespace(data_shell->input_line))
    {
        return false;
    }

    add_history(data_shell->input_line);

    if (tokenize_input(data_shell, data_shell->input_line) == FAILURE)
    {
        return false;
    }

    if (!data_shell->tokens || data_shell->tokens->token_type == END)
    {
        return false;
    }
    if (validateAndProcessVar(&data_shell->tokens) == FAILURE)
        return (false);

    expand_env_variables(data_shell, &data_shell->tokens);
    process_token_quotes(data_shell);
    build_command_list(data_shell, data_shell->tokens);

    return true;
}
