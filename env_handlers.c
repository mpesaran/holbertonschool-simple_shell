#include "shell.h"

/* CUSTOM GETENV FUNCTION TO BYPASS CHECKER */
char *_getenv(const char *name)
{
	int i = 0;
	size_t name_len;

	if (name == NULL || environ == NULL)
		return NULL;
	
	name_len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
		{
			return &environ[i][name_len + 1];
		}
		i++;
	}
	return NULL;
}

