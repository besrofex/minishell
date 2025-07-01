#include "mini.h"

// Ajouter en tête de fichier
static int handle_redirection(t_cmd *cmd, t_token **current)
{
    if (!(*current)->next || (*current)->next->type != TOKEN_WORD)
    {
        fprintf(stderr, "minishell: syntax error near token `%s'\n", (*current)->value);
        return (0);
    }
    if (!add_redirection(cmd, *current))
        return (0);
    *current = (*current)->next->next;
    return (1);
}

// Cette fonction est utilisée mais pas définie
static int is_redirection(t_token_type type)
{
   return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || 
            type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

t_cmd *parse_tokens(t_token *tokens)
{
   t_cmd *cmd_list = NULL;
   t_cmd *current_cmd = NULL;
   t_token *current = tokens;
   
   while (current && current->type != TOKEN_EOF)
   {
      if (!cmd_list)
      {
         cmd_list = init_cmd();
         current_cmd = cmd_list;
         if (!cmd_list)
            return (NULL);
      }
      else if (current->type == TOKEN_PIPE)
      {
         current_cmd->next = init_cmd();
         if (!current_cmd->next)
         {
            free_commands(cmd_list);
            return (NULL);
         }
         current_cmd = current_cmd->next;
         current = current->next;
         continue;
      }

      if (is_redirection(current->type))
      {
         if (!handle_redirection(current_cmd, &current))
    		{
        		free_commands(cmd_list);
        		return (NULL);
    		}
    		continue;
      }
      else if (current->type == TOKEN_WORD)
      {
			if (current->value[0] == '\0')  // Ignorer les mots vides
    		{
        		current = current->next;
        		continue;
    		}
    		if (!add_argument(current_cmd, current))
    		{
        		free_commands(cmd_list);
        		return (NULL);
    		}
    		current = current->next;
      }
      else
      {
         current = current->next; // Skip unexpected tokens
      }
   }
   
   return cmd_list;
}
