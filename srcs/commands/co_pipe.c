#include "../../includes/minishell.h"

void handle_pipe_token(t_command **command_list, t_token **token_stream)
{
    t_command *current_command;

    current_command = get_last_command_node(*command_list);
    current_command->pipe_output = true;
    append_command_node(&current_command, create_new_command(false));
    *token_stream = (*token_stream)->next;
}
