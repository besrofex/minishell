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

char	*get_env_value(char *var_name, char **envp)
{
	int		i;
	int		len;
	char	*env_var;

	if (!var_name || !envp)
		return (NULL);
	
	len = ft_strlen(var_name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
		{
			env_var = ft_strdup(envp[i] + len + 1);
			return (env_var);
		}
		i++;
	}
	return (NULL);
}

char	*ft_strjoin_free(char *s1, char *s2, int to_free)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (to_free & 1)
		free(s1);
	if (to_free & 2)
		free(s2);
	return (result);
}

#include <glob.h>

char	*handle_globbing(char *pattern)
{
   glob_t	globbuf;
   char		*result;
   int		i;

   if (glob(pattern, 0, NULL, &globbuf) != 0)
      return (ft_strdup(pattern));

   result = ft_strdup("");
   i = 0;
   while (i < globbuf.gl_pathc)
   {
      result = ft_strjoin_free(result, globbuf.gl_pathv[i], 1);
      if (++i < globbuf.gl_pathc)
         result = ft_strjoin_free(result, " ", 1);
   }
   globfree(&globbuf);
   return (result);
}

// Cette fonction est utilisée mais pas définie
static int is_redirection(t_token_type type)
{
   return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || 
            type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}