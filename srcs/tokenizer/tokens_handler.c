#include "../../includes/minishell.h"

static void save_separator(t_shell_data *data_shell, char *input, int index, int type)
{
    char sep[3] = {0};
    int length = (type == APPEND || type == HEREDOC) ? 2 : 1;
    strncpy(sep, &input[index], length);
    sep[length] = '\0';
    add_token_to_list(&data_shell->tokens, create_new_token(sep, type, DEFAULT));
}

static void save_token(t_shell_data *data_shell, char *input, int start, int end)
{
    char *token = strndup(input + start, end - start);
    add_token_to_list(&data_shell->tokens, create_new_token(token, WORD, DEFAULT));
    printf("Saved Token: %s\n", token); // Debugging output
    free(token);
}

static void handle_separator(t_shell_data *data_shell, char *input, int *index)
{
    int type = is_separator(input[*index], input[*index + 1]);
    if (type == APPEND || type == HEREDOC)
    {
        save_separator(data_shell, input, *index, type);
        (*index)++;
    }
    else
    {
        save_separator(data_shell, input, *index, type);
    }
}

static void update_parsing_status(int *status, char current)
{
    switch (*status)
    {
    case DEFAULT:
        if (current == '\"' || current == '\'')
            *status = current;
        break;
    case '\"':
    case '\'':
        if (current == *status)
            *status = DEFAULT;
        break;
    }
}

int tokenize_input(t_shell_data *data_shell, char *input)
{
    int i = 0, start = 0, status = DEFAULT;
    int sep_type;

    while (input[i] != '\0')
    {
        update_parsing_status(&status, input[i]);

        if (status == DEFAULT)
        {
            sep_type = is_separator(input[i], input[i + 1]);

            if (input[i] == '|' && input[i + 1] == '|')
            {
                error_msg("Unexpected token", "||", true);
                return FAILURE;
            }

            if (sep_type != NOT_SEPARATOR)
            {
                if (i > start)
                    save_token(data_shell, input, start, i);

                handle_separator(data_shell, input, &i);
                start = i + 1;
            }
        }
        i++;
    }

    if (start < i)
        save_token(data_shell, input, start, i);

    add_token_to_list(&data_shell->tokens, create_new_token("", END, DEFAULT));

    return (status == DEFAULT) ? 0 : 1;
}
