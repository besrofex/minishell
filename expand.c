#include "mini.h"

/* Fonctions auxiliaires pour expand_tokens */

static char	*expand_var(char *str, int *i, char **envp, int exit_status)
{
	int		start;
	char	*var_name;
	char	*var_value;
	char	*result;

	(*i)++; // Skip $
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(exit_status));
	}
	else if (str[*i] == '$')
	{
		(*i)++;
		return (ft_itoa(getpid()));
	}
	else if (!ft_isalnum(str[*i]) && str[*i] != '_')
	{
		// $ suivi d'un caractère non-alphanumérique
		return (ft_strdup("$"));
	}
	else
	{
		start = *i;
		while (ft_isalnum(str[*i]) || str[*i] == '_')
			(*i)++;
		var_name = ft_substr(str, start, *i - start);
		var_value = get_env_value(var_name, envp);
		free(var_name);
		if (!var_value)
			return (ft_strdup(""));
		result = ft_strdup(var_value);
		free(var_value);
		return (result);
	}
}

static char	*handle_quotes_expansion(char *str, int *i, char **envp, int exit_status)
{
	char	*result;
	char	*expanded_part;
	char	*temp;
	char	quote;
	int		start;
	// int		j;

	quote = str[*i];
	(*i)++; // Skip opening quote
	start = *i;
	
	if (quote == '\'') // Single quotes - no expansion
	{
		while (str[*i] && str[*i] != quote)
			(*i)++;
		if (str[*i] != quote)
		{
			fprintf(stderr, "minishell: unclosed quote `%c'\n", quote);
			return (NULL);
		}
		result = ft_substr(str, start, *i - start);
		(*i)++; // Skip closing quote
		return (result);
	}
	else // Double quotes - expand variables
	{
		result = ft_strdup("");
		while (str[*i] && str[*i] != quote)
		{
			if (str[*i] == '$')
			{
				expanded_part = expand_var(str, i, envp, exit_status);
				temp = ft_strjoin(result, expanded_part);
				free(result);
				free(expanded_part);
				result = temp;
			}
			else
			{
				temp = malloc(2);
				temp[0] = str[*i];
				temp[1] = '\0';
				expanded_part = ft_strjoin(result, temp);
				free(result);
				free(temp);
				result = expanded_part;
				(*i)++;
			}
		}
		if (str[*i] != quote)
		{
			fprintf(stderr, "minishell: unclosed quote `%c'\n", quote);
			free(result);
			return (NULL);
		}
		(*i)++; // Skip closing quote
		return (result);
	}
}

// static char	*process_dollar_in_dquote(char *str, int *i, char **envp, int exit_status)
// {
// 	char	*before;
// 	char	*var;
// 	char	*after;
// 	char	*result;

// 	before = ft_substr(str, 0, *i);
// 	var = expand_var(str, i, envp, exit_status);
// 	after = ft_strdup(str + *i);
// 	result = ft_strjoin_free(ft_strjoin_free(before, var, 1), after, 3);
// 	return (result);
// }

/* Fonction principale */

void expand_tokens(t_token *tokens, char **envp, int exit_status)
{
    t_token *current = tokens;
    char *expanded;
    char *tmp;

   while (current)
   {
      if (current->type == TOKEN_WORD && current->quoted != 1)
      {
         expanded = expand_str(current->value, envp, exit_status);
         if (!expanded)
         {
            current = current->next;
            continue;
         }
         if (ft_strchr(expanded, '*') && current->quoted != 2)
         {
            tmp = handle_globbing(expanded);
            free(expanded);
            expanded = tmp;
            if (!expanded)
               continue;
         }

         free(current->value);
         current->value = expanded;
      }
      current = current->next;
   }
}

/* Fonction d'expansion principale */

char	*expand_str(char *str, char **envp, int exit_status)
{
	int		i;
	char	*result;
	char	*tmp;
	char	*part;

	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
        {
            part = handle_quotes_expansion(str, &i, envp, exit_status);
            if (!part)  // Vérification ajoutée
            {
                free(result);
                return (NULL);
            }
        }
		if (str[i] == '\'' && !ft_strchr(str + i + 1, '\''))
			break ;
		else if (str[i] == '\'')
			part = handle_quotes_expansion(str, &i, envp, exit_status);
		else if (str[i] == '"')
			part = handle_quotes_expansion(str, &i, envp, exit_status);
		else if (str[i] == '$')
			part = expand_var(str, &i, envp, exit_status);
		else
		{
			part = ft_substr(str, i, 1);
			i++;
		}
		tmp = ft_strjoin(result, part);
		free(result);
		free(part);
		result = tmp;
	}
	return (result);
}
