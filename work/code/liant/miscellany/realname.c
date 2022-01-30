
#include <stdio.h>
#include <pwd.h>
#include <ctype.h>

#define NAME_LEN	64

main(argc,argv)
int argc;
char *argv[];
{
	int	i, realname();
	char	name[NAME_LEN];

	if(argc < 2){
		fprintf(stderr,"usage: realname login_name...\n");
		exit(1);
	}
	for(i = 1 ; i < argc ; i++){
		realname(argv[i],name);
		if(*name == '\0')
			printf("%-20s** invalid user\n",argv[i]);
		else
			printf("%-20s%s\n",argv[i],name);
	}
}

realname(login,real)	/* Puts in string real, the full */
char *login, *real;	/* name of the given login name. */
{
	struct	passwd *p, *getpwnam();
	char	*nmptr;

	if((p = getpwnam(login)) == NULL){
		*real = '\0';
		return;
	}
	for(nmptr = p->pw_gecos ; *nmptr && *nmptr != ',' ; nmptr++){
		if(*nmptr == '&'){
			*real++ = toupper(*login);
			for(login++ ; *login ; login++)
				*real++ = *login;
		}
		else
			*real++ = *nmptr;
	}
	*real = '\0';
}
