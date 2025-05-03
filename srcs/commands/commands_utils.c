#include "../../includes/minishell.h"

void append_command_node(t_command **cmd_list, t_command *new_cmd)
{
    t_command *current = *cmd_list;

    if (!current)
    {
        *cmd_list = new_cmd;
        return;
    }
    while (current->next)
        current = current->next;

    current->next = new_cmd;
    new_cmd->prev = current;
}

static void setup_command_defaults(t_command **cmd)
{
    (*cmd)->command = NULL;
    (*cmd)->path = NULL;
    (*cmd)->args = NULL;
    (*cmd)->pipe_output = false;
    (*cmd)->pipe_fd = 0;
    (*cmd)->prev = NULL;
    (*cmd)->next = NULL;
}

t_command *create_new_command(bool should_pipe)
{
    t_command *cmd = (t_command *)malloc(sizeof(t_command));
    if (!cmd)
        return NULL;

    ft_memset(cmd, 0, sizeof(t_command));
    cmd->pipe_output = should_pipe;

    setup_command_defaults(&cmd);
    return cmd;
}
