#include "../../includes/minishell.h"

int append_additional_arguments(t_token **token_list, t_command *cmd)
{
    int old_arg_count = 0;
    int new_arg_count = 0;
    char **new_array;
    t_token *temp = *token_list;

    while (temp && (temp->token_type == WORD || temp->token_type == VAR))
    {
        new_arg_count++;
        temp = temp->next;
    }

    while (cmd->args && cmd->args[old_arg_count])
        old_arg_count++;

    new_array = malloc(sizeof(char *) * (old_arg_count + new_arg_count + 1));
    if (!new_array)
        return FAILURE;

    new_array = extend_argument_array(old_arg_count, new_array, cmd, token_list);
    free(cmd->args);
    cmd->args = new_array;
    *token_list = temp;
    return SUCCESS;
}

int count_arguments(t_token *node)
{
    int count = 0;

    while (node && (node->token_type == WORD || node->token_type == VAR))
    {
        count++;
        node = node->next;
    }
    return count;
}

t_command *get_last_command_node(t_command *command_list)
{
    while (command_list->next)
        command_list = command_list->next;
    return command_list;
}

static bool token_contains_space(char *str)
{
    while (*str)
    {
        if (*str == ' ')
            return true;
        str++;
    }
    return false;
}

void parse_command_tokens(t_command **command_list, t_token **token_stream)
{
    t_token *current_token = *token_stream;
    t_command *active_cmd;

    while (current_token->token_type == WORD || current_token->token_type == VAR)
    {
        active_cmd = get_last_command_node(*command_list);

        bool is_first_word = (!current_token->prev || current_token->prev->token_type == PIPE || active_cmd->command == NULL);

        if (is_first_word)
        {
            if (current_token->token_type == VAR && token_contains_space(current_token->token_value))
                split_var_token_as_command(active_cmd, current_token->token_value);
            else
                active_cmd->command = ft_strdup(current_token->token_value);
            current_token = current_token->next;
        }
        else
        {
            build_command_arguments(&current_token, active_cmd);
        }
    }
    *token_stream = current_token;
}
