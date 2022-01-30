/*
 * ------------------------------------------------------------------------
 *
 * Program:
 *
 *      Program to test enhancements to be done to LPI-DEBUG
 *      involving restarting (normally or abnormally) terminated 
 *      user programs, and handling keyboard interrupts.
 *
 * Usage:
 *
 *      minidb [-i] [ -r | -R ] [-d] user_program [user_arguments]...
 *
 *      This program will load the user program but not execute it until
 *      the user enters the CONTINUE command ("c" or "C").  If during the
 *      execution of the user program, an abnormal termination condition
 *      occurs, the user program will enter a stopped state and print out
 *      the signal number which caused the condition.  At that point, the
 *      user should be able to continue execution, or reload the program
 *      and start again.  This also applies if the user interrupts (i.e.
 *      hits CTRL-C) their program in execution.  If the user program exits
 *      normally, then the user must reload their program to execute it again.
 *
 *      The RELOAD command ("r" or "R") is used to reload the user program.  
 *      An optional command line argument may be given.  If just "*" is
 *      given as the command line argument, then the original command
 *      line arguments are used.  If just "&" is given, then the last
 *      command line argument which was assigned is used.  To change the
 *      name of the program being debugged (i.e. argv[0]), use the
 *      character "%" followed immediately (no spaces) by the new name
 *      as the first argument to RELOAD.  In order to run the program
 *      after this command, use the CONTINUE command.  To see what command
 *      line arguments are currently being used, give just "?" as an
 *      argument to RELOAD.  More extensive command line editing features
 *      could easily be added.
 *
 *      To execute a shell command, enter "!" followed immediately
 *      (no spaces) by the command (and arguments) to be executed.
 *      
 *      To quit, the user should enter the QUIT command ("q" or "Q").
 *
 *      To list the commands available, use the HELP command ("h" or "H").
 *
 *      The "-i" flag will cause interrupts to be
 *      ignored rather than (attempted to be) caught.
 *
 *      If the "-r" option is given, then the user program will be
 *      reloaded automatically after it exits.  If the "-R" option
 *      is given, then in addition to the above, programs which have
 *      entered a stopped state will also be reloaded.
 *
 *      The "-d" flag will give miscellaneous debugging information.
 *
 * History:
 *
 *      David G. Michaels, LPI, February 1986.
 *
 * ------------------------------------------------------------------------
 */

#include                <a.out.h>
#include                <stdio.h>
#include                <fcntl.h>
#include                <signal.h>
#include                <errno.h>

#define FALSE           (0)
#define TRUE            (!FALSE)
#define EOS             ('\0')

#define MAXLINE         256
#define MAXARGS         64
#define ARGDELIM        " \t"


#define TRACEME_PTRACE          0
#define CONTINUE_PTRACE         7
#define KILL_PTRACE             8
#define SINGLESTEP_PTRACE       9

/*
 * User program state (UPS) definitions; assigned to ups_state.
 * The global variable "ups_status" always contains the
 * status returned by the "wait" system call.
 */

#define UPS_EXIT        1       /* Exited normally                      */
                                /*    exit status is in "ups_exitno"    */
#define UPS_STOP        2       /* Stopped                              */
                                /*    signal number is in "ups_signo"   */
#define UPS_INTR        3       /* Interrupted                          */
#define UPS_TERM        4       /* Terminated abnormally                */
                                /*    signal number is in "ups_signo"   */
                                /*    THIS SHOULD NEVER HAPPEN          */
#define UPS_UNKNOWN     5       /* Unknown state                        */
                                /*    THIS SHOULD NEVER HAPPEN          */


#define strmatch(a,b)   (strcmp((a),(b)) == 0)
#define prompt()        fprintf(stderr,"MINIDB-> ")

typedef short   bool;

extern int      execv(), exit(), fork(), ptrace(), wait();

extern char     *sys_errlist[];
extern int       sys_nerr;

static char      *programname = NULL;
static char      *NAME_USERPROG = NULL;
static char     **ARGS_USERPROG = NULL;
static char     **ORIG_USERARGS = NULL;
static char     **ENVP_USERPROG = NULL;

static int      SON_ID = 0;

static int      ups_state, ups_status, ups_exitno, ups_signo;

static bool     ignoreintr = FALSE;
static bool     debugflag = FALSE;
static bool     reloadexit = FALSE;
static bool     reloadstop = FALSE;

main(argc, argv, envp)
int      argc;
char    *argv[];
char    *envp[];
{
        void    LOAD_USERPROG(), DEB(), usage();

        programname = argv[0];

        if (argc <= 1)
                usage();

        for (argc--, argv++ ; argc > 0 ; argc--, argv++) {
                if (**argv == '-') {
                        switch ((*argv)[1]) {
                        case 'i':
                                ignoreintr = TRUE;
                                break;
                        case 'd':
                                debugflag = TRUE;
                                break;
                        case 'r':
                                reloadexit = TRUE;
                                break;
                        case 'R':
                                reloadstop = TRUE;
                                break;
                        default:
                                fprintf(stderr,
                                "Illegal flag (%s)\n",*argv);
                                usage();
                        }
                }
                else
                        break;
        }

        printf("\nLPI-MINIDB   Starting up with \"%s\".  Wait...\n",argv[0]);

        NAME_USERPROG = argv[0];
        ORIG_USERARGS = &argv[0];
        ARGS_USERPROG = &argv[0];
        ENVP_USERPROG = &envp[0];

        LOAD_USERPROG(NAME_USERPROG,ARGS_USERPROG,ENVP_USERPROG);
        DEB();
        exit(0);
}

void
DEB()
{
        char *gets();
        int wordvec();
        void docommand();
        void RESUME_USERPROG(), printargvector(), printcommands();

        bool reload = FALSE;
        int nwords, i;
        char line[MAXLINE+1], *linevec[MAXARGS];

        static char saveline[MAXLINE+1], *argvec[MAXARGS];

        printcommands();
        prompt();

getcommands:

        while (gets(line) != NULL) {
                if (((line[0] == 'C') || (line[0] == 'c')) ||
                    ((line[0] == 'S') || (line[0] == 's'))) {
                        if ((line[0] == 'S') || (line[0] == 's')) {
                                fprintf(stderr,"Single-step user program.\n");
                                RESUME_USERPROG(SINGLESTEP_PTRACE);
                        }
                        else {
                                fprintf(stderr,"Continue user program.\n");
                                RESUME_USERPROG(CONTINUE_PTRACE);
                        }
                        fprintf(stderr,"User program has ");
                        switch (ups_state) {
                        case UPS_EXIT:
                                fprintf(stderr,"exited (status %d)",ups_exitno);
                                if (reloadexit || reloadstop)
                                        reload = TRUE;
                                break;
                        case UPS_STOP:
                                fprintf(stderr,"stopped (signal %d)",ups_signo);
                                if (reloadstop)
                                        reload = TRUE;
                                break;
                        case UPS_INTR:
                                fprintf(stderr,
                                "been stopped due to an interrupt");
                                if (reloadstop)
                                        reload = TRUE;
                                break;
                        case UPS_TERM:
                                fprintf(stderr,
                                "died (signal %d)...THIS SHOULD NOT HAPPEN",
                                ups_signo);
                                break;
                        case UPS_UNKNOWN:
                                fprintf(stderr,
                                "stopped for UNKNOWN reasons");
                                break;
                        default:
                                fprintf(stderr,
                                "stopped for UNDEFINED reasons");
                                break;
                        }
                        if (reload) {
                                reload = FALSE;
                                fprintf(stderr,"...Reloading.\n");
                                LOAD_USERPROG(NAME_USERPROG,
                                              ARGS_USERPROG,
                                              ENVP_USERPROG);
                        }
                        else
                                fprintf(stderr,"...Use RELOAD.\n");
                }
                else if ((line[0] == 'r') || (line[0] == 'R')) {
                        nwords = wordvec(line,linevec,MAXARGS,ARGDELIM);
                        if (nwords == 2 && strmatch(linevec[1],"?")) {
                                fprintf(stderr,
                                "Current argument vector:\n");
                                printargvector(ARGS_USERPROG);
                        }
                        else {
                                if ((nwords == 2) &&
                                     strmatch(linevec[1],"*")) {
                                        ARGS_USERPROG = ORIG_USERARGS;
                                }
                                else if ((nwords == 2) &&
                                         strmatch(linevec[1],"&")) {
                                        ;  /* Use last argument list */
                                }
                                else {  /* Use a new argument list */
                                        int n = 0;
                                        for (i = 0 ; i <= MAXLINE ; i++)
                                                saveline[i] = line[i];
                                        if (linevec[1][0] == '%') {
                                                n = 1;
                                                if (linevec[1][1] != EOS)
                                                        argvec[0] =
                                                         &saveline
                                                          [linevec[1]
                                                           -line] + 1;
                                                else
                                                        argvec[0] =
                                                         NAME_USERPROG;
                                        }
                                        else
                                                argvec[0] = NAME_USERPROG;
                                        for (i = 1 ; i < MAXARGS ; i++) {
                                                if (linevec[i+n] == NULL)
                                                        argvec[i] = NULL;
                                                else
                                                        argvec[i] =
                                                         &saveline
                                                          [linevec[i+n]
                                                           -line];
                                        }
                                        ARGS_USERPROG = argvec;
                                }
                                fprintf(stderr,
                                "Reloading user program with argument list:\n");
                                printargvector(ARGS_USERPROG);
                                LOAD_USERPROG(NAME_USERPROG,
                                              ARGS_USERPROG,
                                              ENVP_USERPROG);
                        }
                }
                else if ((line[0] == 'h') ||
                         (line[0] == 'H') ||
                         (line[0] == '?')) {
                        printcommands();
                }
                else if (line[0] == '!') {
                        docommand(&line[1]);
                }
                else if ((line[0] == 'q') || (line[0] == 'Q')) {
                        fprintf(stderr,"Quit...Bye!\n");
                        exit(0);
                }
                prompt();
        }

        if (errno == EINTR) {
                /*
                 * Got CTRL-C during read system
                 * call (causing gets to return NULL).
                 */
                goto getcommands;
        }

        fprintf(stderr,"MINIDB  Quit (errno=%d)...Bye!\n",errno);
}

void
LOAD_USERPROG(name,argvector,environ)
char *name;
char *argvector[];
char *environ[];
{
        void KILL_USERPROG(), sighandler(), intrhandler();
        int status;
        static int firsttime = TRUE;

        if (!firsttime)
                KILL_USERPROG(SON_ID);

        SON_ID = fork();

        if (SON_ID == 0) {                      /* Child */
                /*
                 * Load but do not begin to
                 * execute the user process.
                 */
                (void)ptrace(TRACEME_PTRACE,0,0,0);
                (void)execve(name,argvector,environ);
                exit(0);
        }
        else if (SON_ID < 0) {
                fprintf(stderr,"Fatal error (fork failed)...Exit.\n");
                exit(1);
        }
        else {                                  /* Parent */
                signal(SIGQUIT,sighandler);
                signal(SIGILL,sighandler);
                signal(SIGTRAP,sighandler);
                signal(SIGIOT,sighandler);
                signal(SIGEMT,sighandler);
                signal(SIGFPE,sighandler);
                signal(SIGBUS,sighandler);
                signal(SIGSEGV,sighandler);
                signal(SIGSYS,sighandler);
                if (ignoreintr)
                        signal(SIGINT,SIG_IGN);
                else
                        signal(SIGINT,intrhandler);

                /*
                 * Make sure we're loaded
                 * in and ready to go.
                 */

                if (!firsttime) {
                        wait(&status);
                        if ((status & 0x00FF) != 0x007F) {
                                fprintf(stderr,
                                "Fatal error (failed to load \"%s\")...Exit.\n",
                                name);
                                exit(1);
                        }
                }

                if (firsttime)
                        firsttime = FALSE;
        }
}

void
RESUME_USERPROG(mode)
int mode;
{
        void nointr(), mayintr();
        int waitreturn;

        if (debugflag) {
                fprintf(stderr,"***:Waiting on child (pid=%d)\n",SON_ID);
        }

        ptrace(mode,SON_ID,1,0);        /* resume child execution */
        nointr();                       /* disable interrupts */
        waitreturn = wait(&ups_status); /* parent patiently waits */
        mayintr();                      /* enable interrupts */

        if (debugflag) {
                fprintf(stderr,
                "***:Done waiting (pid=%d,stat=%d,0%o,0x%x,ret=%d,err=%d)\n",
                SON_ID,ups_status,ups_status,ups_status,waitreturn,errno);
        }

        /*
         * The following should not happen if interrupts
         * are being ignored by wait() as they are above.
         */
        if ((waitreturn == -1) && (errno == EINTR)) {   /* interrupted */
                ups_state = UPS_INTR;
        }
        else if ((ups_status & 0x00FF) == 0) {          /* normal exit */
                ups_state = UPS_EXIT;
                ups_exitno = (ups_status & 0xFF00) >> 8;

        }
        else if ((ups_status & 0x00FF) == 0x7F) {       /* stopped */
                ups_state = UPS_STOP;
                ups_signo = (ups_status & 0xFF00) >> 8;
        }
        /*
         * The following sould not happen;  abnormal termination
         * conditions in a ptraced process should cause the process
         * to enter a stopped state instead of actually terminating.
         */
        else if ((ups_status & 0xFF00) == 0) {          /* died */
                ups_state = UPS_TERM;
                ups_signo = (ups_status & 0x00FF);
        }
        else {                                          /* unknown */
                ups_state = UPS_UNKNOWN;
        }
}

void
KILL_USERPROG(pid)
int pid;
{
        int kill();
        int killreturn, waitreturn, status;

        if (pid > 0) {

                if (debugflag) {
                        fprintf(stderr,
                        "***:Killing child(pid=%d) with ptrace(8)\n",pid);
                }

                ptrace(KILL_PTRACE,pid,0,0);

                if (debugflag) {
                        fprintf(stderr,"***:Waiting for child to die\n");
                }

                waitreturn = wait(&status);

                if (debugflag) {
                        fprintf(stderr,
                        "***:Done waiting (ret=%d,err=%d,stat=%d,0x%x)\n",
                        waitreturn,errno,status,status);
                }

                if (kill(pid,0) != 0) {
                        if (debugflag) {
                                fprintf(stderr,
                                "***:Child killed by ptrace(8)\n");
                        }
                }
                else {
                        fprintf(stderr,
                        "Error (can't kill child)...Continue.\n");
                        if (debugflag) {
                                fprintf(stderr,
                                "***:Child still alive...ptrace(8) failed!\n");
                        }
                }
        }
        else {
                if (debugflag) {
                        fprintf(stderr,"***:Child already dead\n");
                }
        }
}

void
sighandler(i)
int i;
{
        fprintf(stderr,"\nFatal error (signal %d)...Exit.\n",i);
        signal(SIGQUIT,SIG_IGN);
        signal(SIGILL,SIG_IGN);
        signal(SIGTRAP,SIG_IGN);
        signal(SIGIOT,SIG_IGN);
        signal(SIGEMT,SIG_IGN);
        signal(SIGFPE,SIG_IGN);
        signal(SIGBUS,SIG_IGN);
        signal(SIGSEGV,SIG_IGN);
        signal(SIGSYS,SIG_IGN);
        exit(i);
}

void
intrhandler(i)
int i;
{
        void intrhandler();

        fprintf(stderr,"\nInterrupt...Caught.\n",i);
        signal(SIGINT,intrhandler);
}

void
nointr()
{
        signal(SIGINT,SIG_IGN);
}

void
mayintr()
{
        signal(SIGINT,intrhandler);
}
void
docommand(cmd)
char *cmd;
{
        int system();

        fprintf(stderr,"Executing shell command:\n");
        fprintf(stderr,"         \"%s\"\n",cmd);
        if (system(cmd) < 0)
                fprintf(stderr,
                "Error (shell command failed)...Continue.\n");
}

void
printargvector(argvector)
char **argvector;
{
        register char **p;

        fprintf(stderr,"------->");
        for (p = argvector ; *p != NULL ; p++)
                fprintf(stderr," \"%s\"",*p);
        fprintf(stderr,"\n");
}

void
printcommands()
{
        fprintf(stderr,"\nMini-Debugger Commands:\n\n");
        fprintf(stderr,
                " [C]  \"Continue\"\n");
        fprintf(stderr,
                " [R]  \"Reload [ * | & | ? | [%%progname] arguments ]\"\n");
        fprintf(stderr,
                " [H]  \"Help\"\n");
        fprintf(stderr,
                " [!]   !<shell command>\n");
        fprintf(stderr,
                " [Q]  \"Quit\"\n\n");
}

/*
 * ------------------------------------------------------------------------
 * wordvec
 *
 * Construct a word vector consisting of words in te given string "str",
 * delimited by characters in the character set "delim".  Pointers to the
 * beginning of each word are placed in order in the character pointer
 * array "vec".  Return the number of words actually put into the vector.
 * The string "str" is munged since EOS characters actually replace every
 * delimiter character.  The character pointer array "vec" is assumed to
 * have at least "nword" elements allocated for its use.  If less than
 * "nword" word pointers are put into "vec", then the remaining "vec"
 * pointers will be set to NULL.
 */

int
wordvec(str,vec,nword,delim)
register char *str, **vec;
register int nword;
char *delim;
{
        bool indelim = TRUE;
        register int i, j;

        for (i = 0 ; (i < nword) && (*str != EOS) ; str++) {
                if (strchr(delim,*str)) {
                        *str = EOS;
                        indelim = TRUE;
                }
                else if (indelim == TRUE) {
                        vec[i++] = str;
                        indelim = FALSE;
                }
        }
        for (j = i ; j < nword ; j++)
                vec[j] = NULL;
        while ((*str != EOS) && (strchr(delim,*str) == NULL))
                str++;
        *str = EOS;
        return(i);
}

/*
 * ------------------------------------------------------------------------
 * Print usage and die.
 */

void
usage()
{
   fprintf(stderr,
   "usage: %s [-i] [-r] [-d] user_program [user_arguments]...\n",programname);
   exit(1);
}
