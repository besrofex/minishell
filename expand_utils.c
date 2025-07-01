#include "mini.h"

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
