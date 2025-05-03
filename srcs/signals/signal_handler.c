#include "../../includes/minishell.h"

void handleInteractiveInterrupt(int sigNumber)
{
    (void)sigNumber;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void deactivateSigquit()
{
    signal(SIGQUIT, SIG_IGN);
}

void configureInteractiveSignals()
{
    deactivateSigquit();
    signal(SIGINT, handleInteractiveInterrupt);
}
void newLine(int sigName)
{
    (void)sigName;
    rl_on_new_line();
}

void ignoreCommandSignals()
{
    signal(SIGINT, newLine);
    signal(SIGQUIT, newLine);
}

void ignore_sigquit(void)
{
    struct sigaction act;

    ft_memset(&act, 0, sizeof(act));
    act.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &act, NULL);
}

void set_signals_interactive(void)
{
    struct sigaction act;

    ignore_sigquit();
    ft_memset(&act, 0, sizeof(act));
    act.sa_handler = &handleInteractiveInterrupt;
    sigaction(SIGINT, &act, NULL);
}

void set_signals_noninteractive(void)
{
    struct sigaction act;

    ft_memset(&act, 0, sizeof(act));
    act.sa_handler = &newLine;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
}