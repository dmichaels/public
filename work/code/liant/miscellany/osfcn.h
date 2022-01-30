/**************************************************************************** 
/* Operating System Call definitions -- Sequoia

   These provide function prototypes for all the operating system calls.

   Copyright 1990, 1991 by Language Processors, Inc.  ALL RIGHTS RESERVED
   Copyright 1988, 1989 by TauMetric Corporation      ALL RIGHTS RESERVED


   @(#)osfcn.h	1.2 9/8/89
*****************************************************************************/

#ifndef _OSFCN_H_
#define _OSFCN_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/stat.h>

extern "C" {
int	accept( int s, struct sockaddr *addr, int *addrlen );
int	access( char *path, int mode );
int	acct( char *file );
int	bind( int s, struct sockaddr *name, int namelen );
caddr_t	brk( caddr_t addr );
int	chdir( char *path );
int	chmod( char *path, int mode );
int	chown( char *path, int owner, int group );
int	chroot( char *dirname );
int	close( int fd );
int	connect( int s, struct sockaddr *name, int namelen );
int	creat( char *name, int mode );
int	dup( int oldfd );
int	dup2( int oldfd, int newfd );
int	execve( char *name, char *argv[], char *envp[] );
void	_exit( int status );
int	fchmod( int fd, int mode );
int	fchown( int fd, int owner, int group );
int	fcntl( int fd, int cmd, int arg );
int	fsync(int fd);
int	ftruncate( int fd, unsigned long length );
int	getdirent( int fd, char *buf, int nbytes, long *basep );
int	getdomainname( char *name, int namelen );
int	getgroups( int n, int *gidset );
int	gethostname( char *name, int namelen );
int	getpeername( int s, struct sockaddr *name, int *namelen );
int	getpgrp( int pid );
int	getpriority( int which, int who );
int	getsockname( int s, struct sockaddr *name, int *namelen );
int	getsockopt( int s, int level, int optname, char *optval, int *optlen );
int	ioctl( int d, int request, char *argp );
int	kill( int pid, int sig );
int	killpg( int pgrp, int sig );
int	link( char *toname, char *fromname );
int	listen( int s, int backlog );
int	lseek( int d, long offset, int whence );
int	mdir( char *path, int mode );
int	mknod( char *path, int mode, int dev );
int	mmap( caddr_t addr, int len, int prot, int share, int fd, off_t off );
int	mount( int type, char *dir, int flags, caddr_t data );
int	munmap( caddr_t addr, int len );
int	open( char* name, int flags, ... );
int	pipe( int filedes[2] );
int	profil( char *buff, int bufsiz, int offset, int scale );
int	ptrace(int req, int pid, char *addr, int data, char *addr2);
int	quota( int cmd, int uid, int arg, caddr_t addr );
int	quotactl( int cmd, char *special, int uid, caddr_t addr );
int	read( int fd, char *buf, int nbytes );
int	readlink( char *path, char *buf, int bufsiz );
int	reboot( int howto );
int	recv( int s, char *buf, int len, int flags );
int	recvfrom( int s, char *buf, int len, int flags,
		  struct sockaddr *from, int *fromlen );
int	rmdir( char *path );
caddr_t	sbrk( int incr );
int	send( int s, char *msg, int len, int flags );
int	sendto( int s, char *msg, int len, int flags,
		struct sockaddr *to, int tolen );
int	setdomainname( char *name, int namelen );
int	setegid( int rgid, int egid );
int	setgroups( int ngroups, int *gidset );
int	sethostname( char *name, int namelen );
int	setpgrp( int pid, int pgrp );
int	setpriority( int which, int who, int prio );
int	setquota( char *special, char *file );
int	setreuid( int ruid, int euid );
int	setsockopt( int s, int level, int optname, char *optval, int *optlen );
int	shutdown( int s, int how );
int	sigblock( int mask );
void	sigpause( int sigmask );
int	sigsetmask( int mask );
int	socket( int af, int type, int protocol );
int	socketpair( int d, int type, int protocol, int sv[2] );
void	swapon( char *special );
int	symlink( char *toname, char *fromname );
int	syscall( int number, ... );
int	truncate( char *path, unsigned long length );
int	umask( int mask );
int	unlink( char *path );
int	unmount( char *name );
int	write( int fd, char *buf, int nbytes );
int	flock( int fd, int operation );
int	fork( void );
int	fstat( int fd, struct stat *buf );
int	getdtablesize( void );
int	getegid( void );
int	geteuid( void );
int	getgid( void );
int	gethostid( void );
int	getpagesize( void );
int	getpid( void );
int	getppid( void );
int	getuid( void );
int	lstat( char *path, struct stat *buf );
int	stat( char *path, struct stat *buf );
int	sync( void );
int	vfork( void );
void	vhangup( void );
}

#endif	/* _OSFCN_H_ */
