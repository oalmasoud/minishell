#include "../../includes/minishell.h"

int save_separator(t_token **token_lst, char *str, int index, int type)
{
    int i;
    char *sep;

    i = 0;
    if (type == APPEND || type == HEREDOC)
    {
        sep = malloc(sizeof(char) * 3);
        if (!sep)
            return (1);
        while (i < 2)
            sep[i++] = str[index++];
        sep[i] = '\0';
        add_token_to_list(token_lst, create_new_token(sep, NULL, type, DEFAULT));
    }
    else
    {
        sep = malloc(sizeof(char) * 2);
        if (!sep)
            return (1);
        while (i < 1)
            sep[i++] = str[index++];
        sep[i] = '\0';
        add_token_to_list(token_lst, create_new_token(sep, NULL, type, DEFAULT));
    }
    return (0);
}

int store_token_word(t_token **token_list, char *input, int end_index, int begin_index)
{
    int char_index = 0;
    char *word_segment;

    word_segment = malloc(sizeof(char) * (end_index - begin_index + 1));
    if (!word_segment)
        return (1);

    while (begin_index < end_index)
    {
        word_segment[char_index] = input[begin_index];
        begin_index++;
        char_index++;
    }

    word_segment[char_index] = '\0';
    add_token_to_list(token_list, create_new_token(word_segment, ft_strdup(word_segment), WORD, DEFAULT));

    return (0);
}

int extract_token_or_delimiter(int *index_ptr, char *input, int token_start, t_shell_data *shell)
{
    int token_type;

    token_type = is_separator(input, (*index_ptr));
    if (token_type)
    {
        if ((*index_ptr) != 0 && is_separator(input, (*index_ptr) - 1) == 0)
            store_token_word(&shell->tokens, input, (*index_ptr), token_start);
        if (token_type == APPEND || token_type == HEREDOC || token_type == PIPE || token_type == INPUT || token_type == TRUNC || token_type == END)
        {
            save_separator(&shell->tokens, input, (*index_ptr), token_type);
            if (token_type == APPEND || token_type == HEREDOC)
                (*index_ptr)++;
        }
        token_start = (*index_ptr) + 1;
    }

    return token_start;
}

int update_parsing_status(int status, char *str, int i)
{
    if (str[i] == '\'' && status == DEFAULT)
        status = SQUOTE;
    else if (str[i] == '\"' && status == DEFAULT)
        status = DQUOTE;
    else if (str[i] == '\'' && status == SQUOTE)
        status = DEFAULT;
    else if (str[i] == '\"' && status == DQUOTE)
        status = DEFAULT;
    return (status);
}

int tokenize_input(t_shell_data *shell, char *input_line)
{
    int index;
    int input_len;
    int token_start;
    int parse_state;

    index = -1;
    token_start = 0;
    input_len = ft_strlen(input_line);
    parse_state = DEFAULT;

    while (++index <= input_len)
    {
        parse_state = update_parsing_status(parse_state, input_line, index);
        if (parse_state == DEFAULT)
            token_start = extract_token_or_delimiter(&index, input_line, token_start, shell);
    }

    if (parse_state != DEFAULT)
    {
        if (parse_state == DQUOTE)
            error_msg("unexpected EOF while looking for matching", "\"", true);
        else if (parse_state == SQUOTE)
            error_msg("unexpected EOF while looking for matching", "\'", true);
        error_msg("syntax error", "unexpected end of file", false);
        return (1);
    }

    return (0);
}
