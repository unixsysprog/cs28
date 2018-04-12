#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/wait.h>
#include	"smsh.h"
#include	"splitline.h"
#include	"varlib.h"
#include	"process.h"

/**
 **	small-shell version 5
 **		first really useful version after prompting shell
 **		this one parses the command line into strings
 **		uses fork, exec, wait, and ignores signals
 **
 **     hist: 2017-04-12: changed comment to say "version 5" 
 **/

#define	DFL_PROMPT	"> "

void	setup();

int main(int argc, char ** argv)
{
	char	*cmdline, *prompt, **arglist;
	int	result;

	prompt = DFL_PROMPT ;
	FILE *input = stdin;
	setup();

	if (argc > 1) {
		prompt = "";
		if ( (input = fopen(argv[1], "r")) == NULL ) {
			perror("smsh");
			exit(1);
		}
	}

	if (argc > 2) {
		char key[10];
		for (int i = 2; i < argc; i++) {
			sprintf(key, "%d", i-1);
			VLstore(key, argv[i]);
		}
	}

	while ( (cmdline = next_cmd(prompt, input)) != NULL ){
		cmdline = substitute_variables(&cmdline);
		printf("next command %s\n", cmdline);

		if ( (arglist = splitline(cmdline)) != NULL  ){
			result = process(arglist);
			freelist(arglist);
		}
		free(cmdline);
	}
	return result;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
	extern char **environ;

	VLenviron2table(environ);
	signal(SIGINT,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr,"Error: %s,%s\n", s1, s2);
	exit(n);
}
