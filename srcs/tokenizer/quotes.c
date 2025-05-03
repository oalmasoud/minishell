#include "../../includes/minishell.h"

bool has_quotes(const char *str)
{
    while (*str)
    {
        if (*str == '\'' || *str == '"')
        {
            printf("🧪 Quote detected in: %s\n", str);
            return true;
        }
        str++;
    }
    return false;
}

int calculate_unquoted_length(const char *input)
{
    int len = 0;
    int i = 0;
    int quote = DEFAULT;

    while (input[i])
    {
        if ((input[i] == '"' || input[i] == '\'') && quote == DEFAULT)
        {
            quote = (input[i] == '\'') ? SQUOTE : DQUOTE;
        }
        else if ((input[i] == '\'' && quote == SQUOTE) || (input[i] == '"' && quote == DQUOTE))
        {
            quote = DEFAULT;
        }
        else
        {
            len++;
        }
        i++;
    }
    return len + 1;
}

void strip_quotes_from_token(t_token **token)
{
    int i = 0, j = 0;
    int quote = DEFAULT;
    int new_len = calculate_unquoted_length((*token)->token_value);
    char *cleaned = malloc(sizeof(char) * new_len);

    if (!cleaned)
        return;

    while ((*token)->token_value[i])
    {
        if (((*token)->token_value[i] == '\'' || (*token)->token_value[i] == '"') && quote == DEFAULT)
        {
            quote = ((*token)->token_value[i] == '\'') ? SQUOTE : DQUOTE;
        }
        else if (((*token)->token_value[i] == '\'' && quote == SQUOTE) || ((*token)->token_value[i] == '"' && quote == DQUOTE))
        {
            quote = DEFAULT;
        }
        else
        {
            cleaned[j++] = (*token)->token_value[i];
        }
        i++;
    }
    cleaned[j] = '\0';

    free_safe((void **)&((*token)->token_value));
    (*token)->token_value = cleaned;
}

int process_token_quotes(t_shell_data *data_shell)
{
    t_token *current = data_shell->tokens;

    while (current)
    {
        if (has_quotes(current->token_value) && (!current->prev || current->prev->token_type != HEREDOC))
        {
            printf("🔧 Cleaning quotes from token: %s\n", current->token_value);
            strip_quotes_from_token(&current);
            printf("✅ Cleaned token: %s\n", current->token_value);
        }
        current = current->next;
    }
    return 0;
}
