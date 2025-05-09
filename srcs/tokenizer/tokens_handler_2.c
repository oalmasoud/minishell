#include "../../includes/minishell.h"

int is_separator(char *str, int i)
{
    if (((str[i] > 8 && str[i] < 14) || str[i] == 32))
        return (SPACES);
    else if (str[i] == '|')
        return (PIPE);
    else if (str[i] == '<' && str[i + 1] == '<')
        return (HEREDOC);
    else if (str[i] == '>' && str[i + 1] == '>')
        return (APPEND);
    else if (str[i] == '<')
        return (INPUT);
    else if (str[i] == '>')
        return (TRUNC);
    else if (str[i] == '\0')
        return (END);
    else
        return (0);
}

t_token *create_new_token(char *str, char *str_backup, int type, int status)
{
    t_token *new_token = malloc(sizeof(t_token) * 1);
    if (!(new_token))
        return NULL;

    new_token->token_value = str;
    new_token->str_original = str_backup;
    new_token->variable_exists = false;
    new_token->token_type = type;
    new_token->execution_status = status;
    new_token->join = false;
    new_token->prev = new_token->next = NULL;

    return new_token;
}

void add_token_to_list(t_token **list, t_token *new_token)
{
    t_token *current;

    current = *list;
    if (current == NULL)
    {
        *list = new_token;
        return;
    }
    if (list && *list && new_token)
    {
        while (current->next != NULL)
            current = current->next;
        current->next = new_token;
        new_token->prev = current;
    }
}