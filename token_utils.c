#include "mini.h"

t_token *create_token(t_token_type type, char *value)
{
   t_token *token;
   
	token = malloc(sizeof(t_token));
   if (!token)
      return (NULL);
   token->type = type;
   token->value = ft_strdup(value);
   if (!token->value)
   {
      free(token);
      return (NULL);
   }
   token->quoted = 0;
   token->next = NULL;
   return (token);
}

void add_token_to_list(t_token **head, t_token *new_token)
{
   t_token *current;

   if (!new_token)
      return;
   if (!*head)
   {
      *head = new_token;
      return;
   }
   current = *head;
   while (current->next)
      current = current->next;
   current->next = new_token;
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;

   while (tokens)
   {
      tmp = tokens;
      tokens = tokens->next;
      free(tmp->value);
      free(tmp);
   }
}
