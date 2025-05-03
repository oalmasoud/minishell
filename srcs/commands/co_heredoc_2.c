#include "../../includes/minishell.h"

static bool is_var_enclosed_by_quotes(char *input, int idx)
{
    if (idx > 0)
    {
        if (input[idx - 1] == '\"' && input[idx + 1] == '\"')
            return true;
        else
            return false;
    }
    return false;
}

static bool is_separator_after_dollar(char c)
{
    return (c == '$' || c == ' ' || c == '=' || c == '\0');
}

static char *expand_variables_in_heredoc(t_shell_data *data, char *input)
{
    int idx = 0;

    while (input[idx])
    {
        if (input[idx] == '$' && !is_separator_after_dollar(input[idx + 1]) && !is_var_enclosed_by_quotes(input, idx))
            input = expand_variable_in_heredoc(input, resolve_variable_value(NULL, input + idx, data), idx);
        else
            idx++;
    }
    return input;
}

static char *expand_line_variables(t_shell_data *data, char *line)
{
    char **segments;
    int idx;

    segments = ft_split(line, ' ');
    if (!segments)
        return NULL;

    idx = 0;
    while (segments[idx])
    {
        if (ft_strchr(segments[idx], '$'))
        {
            segments[idx] = expand_variables_in_heredoc(data, segments[idx]);
            if (!segments[idx])
                return NULL;
        }
        idx++;
    }
    return join_string_array(segments);
}

bool evaluate_heredoc_input(t_shell_data *data, char **line, t_io_redirect *redirect, bool *stop_reading)
{
    if (*line == NULL)
    {
        display_redirect_error("warning", "delimited : wanted", redirect->heredoc_tag, true);
        *stop_reading = true;
        return false;
    }
    if (ft_strncmp(*line, redirect->heredoc_tag, ft_strlen(redirect->heredoc_tag) + 1) == 0)
    {
        *stop_reading = true;
        return false;
    }
    if (!redirect->heredoc_is_quoted && ft_strchr(*line, '$'))
    {
        *line = expand_line_variables(data, *line);
        if (!(*line))
        {
            free_safe_2(*line);
            *stop_reading = false;
            return false;
        }
    }
    return true;
}