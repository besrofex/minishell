#include "mini.h"


t_cmd *init_cmd(void)
{
   t_cmd *cmd = malloc(sizeof(t_cmd));
   if (!cmd)
      return (NULL);

   cmd->args = NULL;
   cmd->redirections = NULL;
   cmd->next = NULL;
   return (cmd);
}

void add_redirection(t_cmd *cmd, t_token *token)
{
   t_redirection *new;
   t_redirection *last;
   
   if (!cmd || !token || !token->next)
      return;

   new = malloc(sizeof(t_redirection));
   if (!new)
      return;

   new->type = token->type;
   new->file = ft_strdup(token->next->value);
   if (!new->file)
   {
      free(new);
      return;
   }
   new->next = NULL;

   if (!cmd->redirections)
      cmd->redirections = new;
   else
   {
      last = cmd->redirections;
      while (last->next)
         last = last->next;
      last->next = new;
   }
}

void add_argument(t_cmd *cmd, t_token *token)
{
   int i = 0;
   char **new_args;

   if (!cmd || !token)
      return;

   if (!cmd->args)
   {
      cmd->args = malloc(sizeof(char *) * 2);
      if (!cmd->args)
         return;
      cmd->args[0] = ft_strdup(token->value);
      if (!cmd->args[0])
      {
         free(cmd->args);
         cmd->args = NULL;
         return;
      }
      cmd->args[1] = NULL;
      return;
   }

   while (cmd->args[i])
      i++;

   new_args = malloc(sizeof(char *) * (i + 2));
   if (!new_args)
      return;
      
   i = 0;
   while (cmd->args[i])
   {
      new_args[i] = cmd->args[i];
      i++;
   }
   new_args[i] = ft_strdup(token->value);
   if (!new_args[i])
   {
      free(new_args);
      return;
   }
   new_args[i + 1] = NULL;

   free(cmd->args);
   cmd->args = new_args;
}

void free_commands(t_cmd *commands)
{
   t_cmd *tmp_cmd;
   t_redirection *tmp_redir;
   int i;

   while (commands)
   {
      tmp_cmd = commands;
      commands = commands->next;

      // Free args
      if (tmp_cmd->args)
      {
         i = 0;
         while (tmp_cmd->args[i])
            free(tmp_cmd->args[i++]);
         free(tmp_cmd->args);
      }

      // Free redirections
      while (tmp_cmd->redirections)
      {
         tmp_redir = tmp_cmd->redirections;
         tmp_cmd->redirections = tmp_cmd->redirections->next;
         free(tmp_redir->file);
         free(tmp_redir);
      }

      free(tmp_cmd);
   }
}
