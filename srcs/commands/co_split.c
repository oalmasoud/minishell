#include "../../includes/minishell.h"

void free_string_array(char **arr)
{
    int i = 0;
    if (!arr)
        return;
    while (arr[i])
    {
        free_safe((void **)&arr[i]);
        i++;
    }
    free_safe((void **)&arr);
}

static void clear_token_list(t_token **lst, void (*del)(void *))
{
    t_token *tmp;

    tmp = NULL;
    while (*lst != NULL)
    {
        tmp = (*lst)->next;
        delete_token_node(*lst, del);
        *lst = tmp;
    }
}

static void append_token_node(t_token **token_list, t_token *new_token)
{
    if (!token_list || !new_token)
        return;

    if (!*token_list)
    {
        *token_list = new_token;
        return;
    }

    t_token *cursor = *token_list;
    while (cursor->next)
        cursor = cursor->next;

    cursor->next = new_token;
    new_token->prev = cursor;
}

static t_token *manual_token_create(char *str, char *str_backup, int type, int status)
{
    t_token *new_node;

    new_node = malloc(sizeof(t_token) * 1);
    if (!(new_node))
        return (NULL);
    new_node->token_value = str;
    new_node->str_original = str_backup;
    new_node->variable_exists = false;
    new_node->token_type = type;
    new_node->execution_status = status;
    new_node->join = false;
    new_node->prev = NULL;
    new_node->next = NULL;
    return (new_node);
}

void split_var_token_as_command(t_command *last_command, char *cmd_string)
{
    t_token *token_list = NULL;
    t_token *original_head = NULL;
    char **split_parts;
    int i;

    split_parts = ft_split(cmd_string, ' ');
    if (!split_parts)
        return;

    last_command->command = ft_strdup(split_parts[0]);

    if (split_parts[1])
        token_list = manual_token_create(ft_strdup(split_parts[1]), NULL, WORD, DEFAULT);

    original_head = token_list;
    i = 1;

    while (split_parts[++i])
    {
        append_token_node(&token_list,
                          manual_token_create(ft_strdup(split_parts[i]), NULL, WORD, DEFAULT));
    }

    append_token_node(&token_list,
                      manual_token_create(NULL, NULL, END, DEFAULT));

    build_command_arguments(&token_list, last_command);
    clear_token_list(&original_head, &free_safe_2);
    free_string_array(split_parts);
}