#include <stdio.h>
#include <fcntl.h>

main (ac, av)
int	 ac;
char	*av[];
{
	int i;

	if (ac < 2) {
		printf ("usage: %s file_name...\n", av[0]);
		exit (1);
	}
	for (i = 1 ; i < ac ; i++) {
		printf ("%s:", av[i]);
		if (access(av[i],F_OK) == 0)
			printf (" file");
		if (access(av[i],X_OK) == 0)
			printf (" executable");
		if (access(av[i],R_OK) == 0)
			printf (" readable");
		if (access(av[i],W_OK) == 0)
			printf (" writable");
		printf ("\n");
	}
	exit (0);
}
