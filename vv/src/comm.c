/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/
 
/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

/***************************************************************************
*     Fate 1.0 is copyright 1997-1998 Hui Kwong Tin (Takeda)               *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.										   *
*     											   *
*     Original Fate of Kingdom Mud (fate.gator.net 2000)			   *
***************************************************************************/

/* 
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>   /* for va_list at act_color() from CF */
#include <unistd.h> /* OLC -- for close read write etc */  

#include "merc.h"
#include "interp.h"
#include "recycle.h"

/* forked save */
#include <sys/wait.h>
#include <stdlib.h>

/* command procedures needed */
DECLARE_DO_FUN(do_save_clans	);
DECLARE_DO_FUN(do_read_clans	);
DECLARE_DO_FUN(do_help		);
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_skills	);
DECLARE_DO_FUN(do_outfit	);
DECLARE_DO_FUN(do_quit		);
#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif

#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "telnet.h"
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };

//char *get_stat_alias		args( (CHAR_DATA* ch, int which) );


/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fnctl.h>
#endif

#if	defined(linux)
/* int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
 */
int	close		args( ( int fd ) );
/* REDHAT 5.1 don't like this
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	listen		args( ( int s, int backlog ) );
RedHat end */
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
/* int read            args( ( int fd, char *buf, int nbyte ) ); */       
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
/* int write           args( ( int fd, char *buf, int nbyte ) ); */
#endif

#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct	timeval
{
	time_t	tv_sec;
	time_t	tv_usec;
};
#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static	long			theKeys	[4];

int	gettimeofday		args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined(MIPS_OS)
extern	int		errno;
#endif

#if	defined(MSDOS)
int	gettimeofday	args( ( struct timeval *tp, void *tzp ) );
int	kbhit		args( ( void ) );
#endif

#if	defined(NeXT)
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(sequent)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/* This includes Solaris Sys V as well */
#if defined(sun)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
#if !defined(__SVR4)
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if defined(SYSV)
int setsockopt		args( ( int s, int level, int optname,
			    const char *optval, int optlen ) );
#else
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
#endif
#endif

int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined(ultrix)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif



/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool		    wizlock;		/* Game is wizlocked		*/
bool		    newlock;		/* Game is newlocked		*/
char		    str_boot_time[MAX_INPUT_LENGTH];
time_t		    current_time;	/* time of this pulse */
int                 iNumPlayers = 0; /* The number of players on */
bool               MOBtrigger = TRUE;  /* act() switch                 */  
#define 	CURRENT_VERSION 6


/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void	game_loop_mac_msdos	args( ( void ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif

#if defined(unix)
void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( int port ) );
void	init_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif




/*
 * Other local functions (OS-independent).
 */
bool	check_parse_name	args( ( char *name ) );
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void    bust_a_prompt           args( ( CHAR_DATA *ch ) );
int     port,control;
void    signal_handler          args( ( int sig       ) );
void    init_signals            args( (		      ) );
void    shutdown_wgrace         args( (		      ) );

/*
 *  Function for saver process.  To prevent <defuncts>
 */
void exit_function( );

void exit_function( )
{
  dump_to_scr("Exiting from the player saver.\n\r");
  wait(NULL);
}


int main( int argc, char **argv )
{
    struct timeval now_time;
    /*int port;
    int control;*/

     bool fCopyOver = FALSE;

    /*
     * Memory debugging if needed.
     */
#if defined(MALLOC_DEBUG)
    malloc_debug( 2 );
#endif

/*
 *  save forker
 */
    /*atexit(exit_function);*/
    signal( SIGQUIT, exit_function );
    /*
     * Init time.
     */
    gettimeofday( &now_time, NULL );
    current_time 	= (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );


    /*
     * Reserve one channel for our use.
     */
    if ( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
    {
	perror( NULL_FILE );
	exit( 1 );
    }

    /*
     * Get the port number.
     */
    port = 4000;
    if ( argc > 1 )
    {
	if ( !is_number( argv[1] ) )
	{
	    fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
	    exit( 1 );
	}
	else if ( ( port = atoi( argv[1] ) ) <= 1024 )
	{
	    fprintf( stderr, "Port number must be above 1024.\n" );
	    exit( 1 );
	}
    if (argv[2] && argv[2][0])
        {
        fCopyOver = TRUE;
        control = atoi(argv[3]);
        }
        else 
	fCopyOver = FALSE;
    }

    /*
     * Run the game.
     */
      if (!fCopyOver)
    control = init_socket( port );
    init_signals();    
    boot_db( );
    sprintf( log_buf, "Fate is ready to rock on port %d.", port );
    log_string( log_buf );

       if (fCopyOver)
        copyover_recover();

    game_loop_unix( control );
    close (control);

{
FILE *fp;
AREA_DATA *area;
extern AREA_DATA *area_first;

    /* Log area popularity */
    fp = fopen(AREASTAT_FILE, "a");
    fprintf(fp,"\nBooted %sArea popularity statistics (in char * ticks)\n",
            str_boot_time);

    for (area = area_first; area != NULL; area = area->next) {
      if (area->count >= 5000000)
        fprintf(fp,"%-60s overflow\n",area->name);
      else
        fprintf(fp,"%-60s %lu\n",area->name,area->count);
    }
    fclose(fp);

}
    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}



#if defined(unix)
int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
    int x = 1;
    int fd;

    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof(x) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close(fd);
	exit( 1 );
    }

#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
	    close(fd);
	    exit( 1 );
	}
    }
#endif

    sa		    = sa_zero;
    sa.sin_family   = AF_INET;
    sa.sin_port	    = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
	perror("Init socket: bind" );
	close(fd);
	exit(1);
    }


    if ( listen( fd, 3 ) < 0 )
    {
	perror("Init socket: listen");
	close(fd);
	exit(1);
    }

    return fd;
}
#endif


#if defined(unix)
void game_loop_unix( int control )
{
    static struct timeval null_time;
    struct timeval last_time;

    signal( SIGPIPE, SIG_IGN );
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

#if defined(MALLOC_DEBUG)
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
	for ( d = descriptor_list; d; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    init_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character && d->character->level > 1)
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL && d->character->level > 1)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if (d->character != NULL && d->character->daze > 0)
		--d->character->daze;

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

	/* OLC */
	if ( d->showstr_point )
	    show_string( d, d->incomm );
	else
	if ( d->pString )
	    string_add( d->character, d->incomm );
	else
	    switch ( d->connected )
	    {
	        case CON_PLAYING:
		    if ( !run_olc_editor( d ) )
    		        substitute_alias( d, d->incomm );
		    break;
	        default:
		    nanny( d, d->incomm );
		    break;
	    }

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
	    &&   FD_ISSET(d->descriptor, &out_set) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL && d->character->level > 1)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif


#if defined(unix)
void init_descriptor( int control )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    struct sockaddr_in sock;
    struct hostent *from;
    int desc;
    int size;

    size = sizeof(sock);
    getsockname( control, (struct sockaddr *) &sock, &size );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }

    /*
     * Cons a new descriptor.
     */
/*
    dnew = new_descriptor();

    dnew->descriptor	= desc;
    dnew->connected	= CON_GET_NAME;
    dnew->showstr_head	= NULL;
    dnew->showstr_point = NULL;
    dnew->outsize	= 2000;
    dnew->pEdit		= NULL;			
    dnew->pString	= NULL;			
    dnew->editor	= 0;			
    dnew->outbuf	= alloc_mem( dnew->outsize );
     */
    dnew = new_descriptor(); /* new_descriptor now also allocates things */
    dnew->descriptor = desc;

    size = sizeof(sock);
    if ( getpeername( desc, (struct sockaddr *) &sock, &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	dnew->host = str_dup( "(unknown)" );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;

	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
	sprintf( log_buf, "Sock.sinaddr:  %s", buf );
	log_string( log_buf );
	from = gethostbyaddr( (char *) &sock.sin_addr,
	    sizeof(sock.sin_addr), AF_INET );
	dnew->host = str_dup( from ? from->h_name : buf );
    }

    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    if ( check_ban(dnew->host,BAN_ALL))
    {
   write_to_descriptor( desc,
                     "Your site has been banned from this mud.\n\r"
                     "Your system administrator or access provider will be"
                     "notified if continue to access this system without"
                     " authorization.\n\r"
                     "Please send email to takeda@mathlab.sunysb.edu \n\r"
                     "to request access authorization.\n\r", 0 );
	close( desc );
	free_descriptor(dnew);
	return;
    }

    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
        write_to_buffer( dnew, "\033[2J\033[0;0H\033[0;37;40m\n\r", 0 );

	if ( help_greeting[0] == '.' )
	    write_to_buffer( dnew, help_greeting+1, 0 );
	else
	    write_to_buffer( dnew, help_greeting  , 0 );
    }

    return;
}
#endif



void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );
 	if ( (dclose->connected == CON_PLAYING) ||
 	  ((dclose->connected >= CON_NOTE_TO) &&
 	   (dclose->connected <= CON_NOTE_FINISH)))
     {
	    act( "{G$n has lost $s link.{x", ch, NULL, NULL, TO_ROOM );
	    wiznet("[ Net death has claimed $N. ]",ch,NULL,WIZ_LINKS,0,0);
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_descriptor(dclose);
    return;
}



bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( iStart >= sizeof(d->inbuf) - 10 )
    {
	sprintf( log_buf, "%s input overflow!", d->host);
	log_string( log_buf );
	write_to_descriptor( d->descriptor,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
	return FALSE;
    }

    for ( ; ; )
    {
	int nRead;

	nRead = read( d->descriptor, d->inbuf + iStart,
	    sizeof(d->inbuf) - 10 - iStart );
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
	    log_string( "EOF encountered on read." );
	    return FALSE;
	}
	else if ( errno == EWOULDBLOCK )
	    break;
	else
	{
	    perror( "Read_from_descriptor" );
	    return FALSE;
	}
    }

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
    int i, j, k;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	    d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */

    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
	{
	    d->repeat = 0;
	}
	else
	{
	    if ( ++d->repeat >= 25 )
	    {
		sprintf( log_buf, "%s input spamming!", d->host);
		log_string( log_buf );
		wiznet("[ Spam spam spam $N spam spam spam spam spam! OUT! ]",
		       d->character,NULL,WIZ_SPAM,0,get_trust(d->character));
		if (d->incomm[0] == '!')
		    wiznet(d->inlast,d->character,NULL,WIZ_SPAM,0,
			get_trust(d->character));
		else
		    wiznet(d->incomm,d->character,NULL,WIZ_SPAM,0,
			get_trust(d->character));

		d->repeat = 0;
/*
		write_to_descriptor( d->descriptor,
		    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
		strcpy( d->incomm, "quit" ); */
		if(get_trust(d->character) < LEVEL_IMMORTAL) {
	        write_to_descriptor( d->descriptor,
		"\n\r**DO NOT SPAM**\n\r",0);
//		do_quit(d->character,"");
//		close_socket( d ); 
		} 
	    }
	}
    }


    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
	strcpy( d->incomm, d->inlast );
    else
	strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}



/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
    if ( !merc_down )
    {
        if ( d->showstr_point )
	    write_to_buffer( d,"[(C)ontinue, (R)efresh, (B)ack, (Q)uit, or RETURN]:  ", 0);
        else if ( fPrompt && d->pString && d->connected == CON_PLAYING )
            write_to_buffer( d, "> ", 2 );
        else if ( fPrompt && d->connected == CON_PLAYING )
        {
	    CHAR_DATA *ch;
	CHAR_DATA *victim;

	ch = d->character;

        /* battle prompt */
        if ((victim = ch->fighting) != NULL && can_see(ch,victim))
        {
            int percent;
            char wound[100];
	    char *pbuff;
	    char buf[ MAX_STRING_LENGTH ];
	    char buffer[ MAX_STRING_LENGTH*2 ];
	    pbuff=buffer;
		if (victim->max_hit > 0)
                percent = victim->hit * 100 / victim->max_hit;
	        else
                percent = -1;

            if (percent >= 100)
                sprintf(wound,"{Gis in perfect health.{w");
            else if (percent >= 90)
                sprintf(wound,"{ghas a few scratches.{g");
            else if (percent >= 75)
                sprintf(wound,"{Mhas some small but disgusting cuts.{w");
            else if (percent >= 50)
                sprintf(wound,"{mis covered with bleeding wounds.{w");
            else if (percent >= 30)
                sprintf(wound,"{Yis gushing blood.{w");
            else if (percent >= 15)
                sprintf(wound,"{Ris writhing in agony.{w");
            else if (percent >= 0)
                sprintf(wound,"{ris convulsing on the ground.{w");
            else
                sprintf(wound,"{yis nearly dead.{w");

            sprintf(buf,"{c%s %s {x\n\r",
	            IS_NPC(victim) ? victim->short_descr :
                    (is_affected(victim,gsn_doppelganger) &&
                     !IS_SET(ch->act,PLR_HOLYLIGHT)) ?
                    PERS(victim->doppel,ch) : PERS(victim,ch)
		    ,wound);
	    buf[0] = UPPER(buf[0]);
            if (!is_affected(victim,gsn_mirror)) pbuff = buffer;
	    colourconv( pbuff, buf, d->character );
            write_to_buffer( d, buffer, 0 );

        }


	ch = d->original ? d->original : d->character;
	if (!IS_SET(ch->comm, COMM_COMPACT) )
	    write_to_buffer( d, "\n\r", 2 );


        if ( IS_SET(ch->comm, COMM_PROMPT) )
            bust_a_prompt( d->character );

/*
 	{
	    char *pbuff;
	    char buffer[ 80 ];
 	    char buf[40];

 	    ch = d->character;
	    sprintf( buf, "{Y<{R%dhp {%dm %dmv>{x ",
	    ch->hit,ch->mana,ch->move );
	    pbuff = buffer;
	    colourconv( pbuff, buf, ch );
	    write_to_buffer( d, buffer, 0 );
 	}
*/
	if (IS_SET(ch->comm,COMM_TELNET_GA))
	    write_to_buffer(d,go_ahead_str,0);
    }
}
    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by != NULL )
    {
	if (d->character != NULL)
	    write_to_buffer( d->snoop_by, d->character->name,0);
	write_to_buffer( d->snoop_by, "> ", 2 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     */
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }
}

/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    const char *str;
    const char *i;
    char *point;
    char *pbuff;
    char buffer[ MAX_STRING_LENGTH*2 ];
    char doors[MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    const char *dir_name[] = {"N","E","S","W","U","D"};
    int door;
    int percent;
    const char *graph;

    point = buf;
    str = ch->prompt;

    if ( str == NULL && !IS_NPC(ch) )
      str = "{Y<{R%hhp {B%mm {C%vmv {G%X{Y>{w "; 

    if (!str || str[0] == '\0')
    {
        sprintf( buf, "{Y<{R%dhp {B%dm {C%dmv {G%s {Y>{w",
	    ch->hit,ch->mana,ch->move,ch->prefix);
	send_to_char(buf,ch);
	return;
    }

   if (IS_SET(ch->comm,COMM_BUSY))
   {
	send_to_char("[BUSY] ",ch);
	return;
   }

   if (IS_SET(ch->comm,COMM_AFK) && IS_SET(ch->comm,COMM_OLC))
   {
        send_to_char("{p<AFK>{m[OLC]{x ",ch);
        return;
   }

   if (IS_SET(ch->comm,COMM_AFK) && !IS_SET(ch->comm,COMM_OLC))
   {
   	send_to_char("{p<AFK>{x ",ch);
	return;
   } 

   if (IS_SET(ch->comm,COMM_OLC) && !IS_SET(ch->comm,COMM_AFK))
   {
        send_to_char("{m[OLC]{x ",ch);
        return;
   }

/*
   if (IS_SET(ch->comm,COMM_AFK))
   {
	send_to_char("[AFK] ",ch);
	return;
   }
*/
   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
	case 'e':
	    found = FALSE;
	    doors[0] = '\0';
	    for (door = 0; door < 6; door++)
	    {
		if ((pexit = ch->in_room->exit[door]) != NULL
		&&  pexit ->u1.to_room != NULL
		&&  (can_see_room(ch,pexit->u1.to_room)
		||   (IS_AFFECTED(ch,AFF_INFRARED)
		&&    !IS_AFFECTED(ch,AFF_BLIND)))
		&&  !IS_SET(pexit->exit_info,EX_CLOSED))
		{
		    found = TRUE;
		    strcat(doors,dir_name[door]);
		}
	    }
	    if (!found)
	 	strcat(doors,"none");
	    sprintf(buf2,"%s",doors);
	    i = buf2; break;
 	 case 'c' :
	    sprintf(buf2,"%s","\n\r");
	    i = buf2; break;

	case '1' :
      if (ch->max_hit > 0)
          percent = ch->hit * 100 / ch->max_hit;
      else
          percent = -1;

      if (percent >= 100)
            graph = "{Y[{R##########{Y]{x";
      else if (percent >= 90)
            graph = "{Y[{R#########{r-{Y]{x";
      else if (percent >= 80)
            graph = "{Y[{R########{r--{Y]{x";
      else if (percent >= 70)
            graph = "{Y[{R#######{r---{Y]{x";
      else if (percent >= 58)
            graph = "{Y[{R######{r----{Y]{x";
      else if (percent >= 45)
            graph = "{Y[{R#####{r-----{Y]{x";
      else if (percent >= 30)
            graph = "{Y[{R####{r------{Y]{x";
      else if (percent >= 28)
            graph = "{Y[{R###{r-------{Y]{x";
      else if (percent >= 15)
            graph = "{Y[{R##{r--------{Y]{x";
        else if (percent >= 8)
       graph = "{Y[{R#{r---------{Y]{x";
      
       else
          graph = "{Y[{r----------{Y]{x";
	sprintf(buf2,"%s",graph);
	i = buf2; break;

	case '2' :
      if (ch->max_mana > 0)
          percent = ch->mana * 100 / ch->max_mana;
      else
          percent = -1;

      if (percent >= 100)
            graph = "{Y[{B##########{Y]{x";
      else if (percent >= 90)
            graph = "{Y[{B#########{b-{Y]{x";
      else if (percent >= 80)
            graph = "{Y[{B########{b--{Y]{x";
      else if (percent >= 70)
            graph = "{Y[{B#######{b---{Y]{x";
      else if (percent >= 58)
            graph = "{Y[{B######{b----{Y]{x";
      else if (percent >= 45)
            graph = "{Y[{B#####{b-----{Y]{x";
      else if (percent >= 30)
            graph = "{Y[{B####{b------{Y]{x";
      else if (percent >= 28)
            graph = "{Y[{B###{b-------{Y]{x";
      else if (percent >= 15)
            graph = "{Y[{B##{b--------{Y]{x";
        else if (percent >= 8)
       graph = "{Y[{B#{b---------{Y]{x";
      
       else
          graph = "{Y[{b----------{Y]{x";
	sprintf(buf2,"%s",graph);
	i = buf2; break;
	case '3' :
      if (ch->max_move > 0)
          percent = ch->move * 100 / ch->max_move;
      else
          percent = -1;

      if (percent >= 100)
            graph = "{Y[{G##########{Y]{x";
      else if (percent >= 90)
            graph = "{Y[{G#########{g-{Y]{x";
      else if (percent >= 80)
            graph = "{Y[{G########{g--{Y]{x";
      else if (percent >= 70)
            graph = "{Y[{G#######{g---{Y]{x";
      else if (percent >= 58)
            graph = "{Y[{G######{g----{Y]{x";
      else if (percent >= 45)
            graph = "{Y[{G#####{g-----{Y]{x";
      else if (percent >= 30)
            graph = "{Y[{G####{g------{Y]{x";
      else if (percent >= 28)
            graph = "{Y[{G###{g-------{Y]{x";
      else if (percent >= 15)
            graph = "{Y[{G##{g--------{Y]{x";
        else if (percent >= 8)
       graph = "{Y[{G#{g---------{Y]{x";
      
       else
          graph = "{Y[{g----------{Y]{x";
	sprintf(buf2,"%s",graph);
	i = buf2; break;
         case 'h':
              sprintf( buf2, "%d%s",
                ch->level>=40?ch->hit:PERCENT(ch->hit,ch->max_hit),
		ch->level>=40?"":"%" );
            i = buf2; break;
         case 'H' :
            sprintf( buf2, "%d", ch->max_hit );
            i = buf2; break;
         case 'm' :
	    if ( ch->level >= 40 )
              sprintf( buf2, "%d", ch->mana );
	    else
	      sprintf( buf2, "%d%%", PERCENT(ch->mana, ch->max_mana ));
            i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana );
            i = buf2; break;
         case 'v' :
	    if ( ch->level >= 40 )
              sprintf( buf2, "%d", ch->move );
	    else
	      sprintf( buf2, "%d%%", PERCENT( ch->move, ch->max_move) );
            i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move );
            i = buf2; break;
	case 'X' :
	sprintf(buf2, "%d",  IS_NPC(ch) ? 0 :
        exp_to_level(ch,ch->pcdata->points) );
	i = buf2; break;	
/*         case 'x' :
            sprintf( buf2, "%d", ch->exp );
            i = buf2; break;
	 case 'X' :
	    sprintf(buf2, "%d",  IS_NPC(ch) ? 0 :
	    (ch->level + 1) * exp_per_level(ch,ch->pcdata->points) - ch->exp);
	    i = buf2; break; */
         case 'g' :
            sprintf( buf2, "%ld", ch->gold);
            i = buf2; break;
	 case 's' :
	    sprintf( buf2, "%ld", ch->silver);
	    i = buf2; break;
         case 'a' :
            if( ch->level > 9 )
               sprintf( buf2, "%d", ch->alignment );
            else
               sprintf( buf2, "%s", IS_GOOD(ch) ? "good" : IS_EVIL(ch) ?
                "evil" : "neutral" );
            i = buf2; break;
         case 'r' :
            if( ch->in_room != NULL )
               sprintf( buf2, "%s",
		((!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT)) ||
		 (!IS_AFFECTED(ch,AFF_BLIND) && !room_is_dark( ch->in_room ,ch)))
		? ch->in_room->name : "darkness");
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'R' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%d", ch->in_room->vnum );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'z' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->area->name );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case '%' :
            sprintf( buf2, "%%" );
            i = buf2; break;
         case 'o' :
            sprintf( buf2, "%s", olc_ed_name(ch) );
            i = buf2; break;
	 case 'O' :
	    sprintf( buf2, "%s", olc_ed_vnum(ch) );
	    i = buf2; break;
      }
      ++str;
      while( (*point = *i) != '\0' )
         ++point, ++i;
   }
  *point       = '\0';
   pbuff        = buffer;
   colourconv( pbuff, buf, ch );
   write_to_buffer( ch->desc, buffer, 0 );

  /* write_to_buffer( ch->desc, buf, point - buf ); */
 

   if (ch->prefix[0] != '\0')
        write_to_buffer(ch->desc,ch->prefix,0);
   return;
}




/*
 * Append onto an output buffer.
 */
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
    
    /*
     * Find length in case caller didn't.
     */
    if ( length <= 0 )
	length = strlen(txt);

    /*
     * Initial \n\r if needed.
     */
    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]	= '\n';
	d->outbuf[1]	= '\r';
	d->outtop	= 2;
    }

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize )
    {
	char *outbuf;

        if (d->outsize >= 32000)
	{
	    bug("Buffer overflow. Closing.\n\r",0);
	    close_socket(d);
	    return;
 	}
	outbuf      = alloc_mem( 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop );
	free_mem( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
    }


    /*
     * Copy.
     */
    strncpy( d->outbuf + d->outtop, txt, length );
    d->outtop += length;
    return;
}

/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
	    { perror( "Write_to_descriptor" ); return FALSE; }
    }

    return TRUE;
}




/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    /*
     * Reserved words.
     */
/*
    if ( is_full_name( name,
"all auto immortal self someone something the you demise balance circle loner honor savant strife scribe amity exile advisor ") )
	return FALSE;
 
    if ( is_name( name,
"immortal self someone something the you demise balance circle loner honor \
savant strife scribe amity exile advisor dark death shadow heart bane \
sword") )
	return FALSE;
*/ 
    if (!str_cmp(capitalize(name),"Takeda") )
	return TRUE;

    /*
     * Length restrictions.
     */

    if ( strlen(name) <  3 )
	return FALSE;

    if ( strlen(name) > 12 )
	return FALSE;

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll,adjcaps = FALSE,cleancaps = FALSE;
 	int total_caps = 0;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) )
		return FALSE;

	    if ( isupper(*pc)) /* ugly anti-caps hack */
	    {
		if (adjcaps)
		    cleancaps = TRUE;
		total_caps++;
		adjcaps = TRUE;
	    }
	    else
		adjcaps = FALSE;

	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;

	if ((total_caps > (strlen(name)) / 2 && strlen(name) < 3))
	    return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_full_name( name, pMobIndex->player_name ) )
		    return FALSE;
	    }
	}
    }

  if ( check_illegal( name ) )
    return FALSE;

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&&   (!fConn || ch->desc == NULL)
	&&   !str_cmp( d->character->name, ch->name ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		OBJ_DATA *obj;

		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char(
		    "{CReconnecting. Type replay to see missed tells.{x\n\r", ch );
		act( "{G$n has reconnected.{x", ch, NULL, NULL, TO_ROOM );
		if ((obj = get_eq_char(ch,WEAR_LIGHT)) != NULL
		&&  obj->item_type == ITEM_LIGHT && obj->value[2] != 0)
		    --ch->in_room->light;

	        if (!str_cmp (ch->name, "Dermos"))
	          d->host = "ZX814562.ipt.aol.com";

   		if (!str_cmp (ch->name, "Agron"))
	 	  d->host = "31234.ras.fusp.nl";
	 
   		if (!str_cmp (ch->name, "Niljako"))
	 	  d->host = "31234.ras.fusp.nl";
	 	  	 
   		if (!str_cmp (ch->name, "Khazkar"))
	 	  d->host = "31234.ras.fusp.nl";


		sprintf( log_buf, "%s@%s reconnected.", ch->name,d->host);
		log_string( log_buf );
		wiznet("[ $N groks the fullness of $S link. ]",
		    ch,NULL,WIZ_LINKS,0,0);
		d->connected = CON_PLAYING;
/*  board code added */
/* Inform the character of a note in progress and the possbility of continuation! */
 		if (ch->pcdata->in_progress)
		{
 			send_to_char ("{CYou have a note in progress. Type NOTE WRITE to continue it.{x\n\r",ch);
		   if (IS_SET(ch->comm,COMM_BUSY))
		      REMOVE_BIT(ch->comm,COMM_BUSY);

		}
/*  end */

         }
	    return TRUE;
	}
    }

    return FALSE;
}



/*
 * Check if already playing.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold; dold = dold->next )
    {
	if ( dold != d
	&&   dold->character != NULL
	&&   dold->connected != CON_GET_NAME
	&&   dold->connected != CON_GET_OLD_PASSWORD
	&&   !str_cmp( name, dold->original
	         ? dold->original->name : dold->character->name ) )
	{
	    write_to_buffer( d, "That character is already playing.\n\r",0);
	    write_to_buffer( d, "Do you wish to connect anyway (Y/N)?",0);
	    d->connected = CON_BREAK_CONNECT;
	    return TRUE;
	}
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   ch->desc->connected != CON_PLAYING
    ||   ch->was_in_room == NULL
    ||   ch->in_room != get_room_index(ROOM_VNUM_LIMBO))
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "{G$n has returned from the void.{x", ch, NULL, NULL, TO_ROOM );
    return;
}

/*
 * Write to one char, new colour version, by Lope.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
    const	char 	*point;
    		char 	*point2;
    		char 	buf[ MAX_STRING_LENGTH*4 ];
		int	skip = 0;

    buf[0] = '\0';
    point2 = buf;
    if( txt && ch->desc )
	{
	    if( IS_SET( ch->act, PLR_COLOR ) )
	    {
		for( point = txt ; *point ; point++ )
	        {
		    if( *point == '{' )
		    {
			point++;
			skip = colour( *point, ch, point2 );
			while( skip-- > 0 )
			    ++point2;
			continue;
		    }
		    *point2 = *point;
		    *++point2 = '\0';
		}
		*point2 = '\0';
        	write_to_buffer( ch->desc, buf, point2 - buf );
	    }
	    else
	    {
		for( point = txt ; *point ; point++ )
	        {
		    if( *point == '{' )
		    {
			point++;
			continue;
		    }
		    *point2 = *point;
		    *++point2 = '\0';
		}
		*point2 = '\0';
        	write_to_buffer( ch->desc, buf, point2 - buf );
	    }
	}
    return;
}

/** =>
 ** center_to_char("Text centered to 40 columns", ch, 40);
 **/
void center_to_char(char *argument, CHAR_DATA *ch, int columns)
{
char centered[MAX_INPUT_LENGTH];
int spaces;

columns = (columns < 2) ? 79 : columns;
spaces=(columns-strlen(argument))/2;

sprintf(centered,"%*c%s",spaces,' ',argument);
send_to_char(centered,ch);

return;
}


void do_font( CHAR_DATA *ch, char *argument)
{
int  place, size;
char buf[10];

    size = strlen(argument);

/* top border */
center_to_char( "{c={D-",ch,72-(2*size) );

for (place=2;place<size;place++) 
send_to_char("{c={D-",ch);
send_to_char("{c=\n\r",ch);

/* middle */
sprintf(buf,"{C%c",UPPER(argument[0]));
center_to_char(buf,ch,72-(2*size+2) );

for (place=1;place<size;place++) 
	{
        sprintf(buf," %c",UPPER(argument[place]));
	send_to_char(buf,ch);
	}
send_to_char("{x\n\r",ch);

/* bottom border */
center_to_char( "{c={D-",ch,72-(2*size) );

for (place=2;place<size;place++) 
send_to_char("{c={D-",ch);
send_to_char("{c={w\n\r",ch);

return;
}


/*
 * Write to one char.
 */
void send_to_char_bw( const char *txt, CHAR_DATA *ch )
{
    if ( txt != NULL && ch->desc != NULL )
        write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}

/*
 * Send a page to one char.
 */
void page_to_char_bw( const char *txt, CHAR_DATA *ch )
{
    if ( txt == NULL || ch->desc == NULL)
	return;

    if (ch->lines == 0 )
    {
	send_to_char(txt,ch);
	return;
    }

    ch->desc->showstr_head = alloc_mem(strlen(txt) + 1);
    strcpy(ch->desc->showstr_head,txt);
    ch->desc->showstr_point = ch->desc->showstr_head;
    show_string(ch->desc,"");
}

/*
 * Page to one char, new colour version, by Lope.
 */
void page_to_char( const char *txt, CHAR_DATA *ch )
{
   const	char	*point;
    		char	*point2;
    		char	buf[ MAX_STRING_LENGTH * 4 ];
		int	skip = 0;

    buf[0] = '\0';
    point2 = buf;
   if( txt && ch->desc )
	{
	    if( IS_SET( ch->act, PLR_COLOR ) )
	    {
		for( point = txt ; *point ; point++ )
	        {
		    if( *point == '{' )
		    {
			point++;
			skip = colour( *point, ch, point2 );
			while( skip-- > 0 )
			    ++point2;
			continue;
		    }
		    *point2 = *point;
		    *++point2 = '\0';
		}
		*point2 = '\0';
		free_string( ch->desc->showstr_head );
		ch->desc->showstr_head  = str_dup( buf );
		ch->desc->showstr_point = ch->desc->showstr_head;
		show_string( ch->desc, "" );
	    }
	    else
	    {
		for( point = txt ; *point ; point++ )
	        {
		    if( *point == '{' )
		    {
			point++;
			continue;
		    }
		    *point2 = *point;
		    *++point2 = '\0';
		}
		*point2 = '\0';
		free_string( ch->desc->showstr_head );
		ch->desc->showstr_head  = str_dup( buf );
		ch->desc->showstr_point = ch->desc->showstr_head;
		show_string( ch->desc, "" );
	    }
	}
    return;
}

void show_string( DESCRIPTOR_DATA *d, char *input )
{
    char               *start, *end;
    char                arg[MAX_INPUT_LENGTH];
    int                 line = 0, pagelen;

    /* Set the page length */
    /* ------------------- */

    pagelen = d->original ? d->original->lines
                          : d->character->lines;

    /* Check for the command entered */
    /* ----------------------------- */

    one_argument( input, arg );

    switch( UPPER( *arg ) )
    {
        /* Show the next page */

        case '\0':
        case 'C': line = 0;
                  break;

        /* Scroll back a page */

        case 'B': line = -2 * pagelen;
                  break;

        /* Help for show page */

        case 'H': write_to_buffer( d, "B     - Scroll back one page.\n\r",0 );
                  write_to_buffer( d, "C     - Continue scrolling.\n\r", 0);
                  write_to_buffer( d, "H     - This help menu.\n\r", 0 );
                  write_to_buffer( d, "R     - Refresh the current page.\n\r",
                             0 );
                  write_to_buffer( d, "Enter - Continue Scrolling.\n\r", 0);
                  return;

        /* refresh the current page */

        case 'R': line = -1 - pagelen;
                  break;

        /* stop viewing */

        default:  free_string( d->showstr_head );
                  d->showstr_head  = NULL;
                  d->showstr_point = NULL;
                  return;
    }

    /* do any backing up necessary to find the starting point */
    /* ------------------------------------------------------ */

    if ( line < 0 )
    {
        for( start= d->showstr_point; start > d->showstr_head && line < 0;
             start-- )
            if ( *start == '\r' )
                line++;
    }
    else
        start = d->showstr_point;

    /* Find the ending point based on the page length */
    /* ---------------------------------------------- */

    line  = 0;

    for ( end= start; *end && line < pagelen; end++ )
        if ( *end == '\r' )
            line++;

    d->showstr_point = end;

    if ( end - start )
        write_to_buffer( d, start, end - start );

    /* See if this is the end (or near the end) of the string */
    /* ------------------------------------------------------ */

    for ( ; isspace( *end ); end++ );

    if ( !*end )
    {
        free_string( d->showstr_head );
        d->showstr_head  = NULL;
        d->showstr_point = NULL;
    }

    return;
}


/* quick sex fixer */
void fix_sex(CHAR_DATA *ch)
{
    if (ch->sex < 0 || ch->sex > 2)
    	ch->sex = IS_NPC(ch) ? 0 : ch->pcdata->true_sex;
}

void act (const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2,
	  int type)
{
    /* to be compatible with older code */
    act_color(format,ch,arg1,arg2,type,POS_RESTING);
}


void Xcolourconv( char *buffer, const char *txt , CHAR_DATA *ch )
{
    const	char	*point;
		int	skip = 0;
		int	XXX = 1;

    if( ch->desc && txt )
    {
	if( IS_SET( ch->act, PLR_COLOR ) && (XXX != 1))
	{
	    for( point = txt ; *point ; point++ )
	    {
		if( *point == '{' )
		{
		    point++;
		    skip = colour( *point, ch, buffer );
		    while( skip-- > 0 )
			++buffer;
		    continue;
		}
		*buffer = *point;
		*++buffer = '\0';
	    }
	    *buffer = '\0';
	}
	else
	{
	    for( point = txt ; *point ; point++ )
	    {
		if( *point == '{' )
		{
		    point++;
		    continue;
		}
		*buffer = *point;
		*++buffer = '\0';
	    }
	    *buffer = '\0';
	}
    }
    return;
}



void act_color( const char *format, CHAR_DATA *ch, const void *arg1,
	      const void *arg2, int type, int min_pos, ... )
{
    static char * const he_she  [] = { "it",  "he",  "she" };
    static char * const him_her [] = { "it",  "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };

    CHAR_DATA *to;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;
    char 		*pbuff;
    char 		buf[ MAX_STRING_LENGTH   ];
    char 		buffer[ MAX_STRING_LENGTH*2 ];
    char 		fname[ MAX_INPUT_LENGTH  ];
    bool		fColour = FALSE;



    /*
     * Discard null and zero-length messages.
     */
 if ( format == NULL )
        return;
    if (sizeof(format) ==0)
        return;


    /* discard null rooms and chars */
    if (ch == NULL || ch->in_room == NULL)
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
        if ( vch == NULL )
        {
            bug( "Act: null vch with TO_VICT.", 0 );
            return;
        }

	if (vch->in_room == NULL)
	    return;

        to = vch->in_room->people;
    }

    for ( ; to != NULL; to = to->next_in_room )
    {

       if ( (!IS_NPC(to) 
        && to->desc == NULL )
        ||   ( IS_NPC(to) && !HAS_TRIGGER_MOB(to, TRIG_ACT) && to->desc==NULL )  
        ||    to->position < min_pos ) 
		continue;

        if ( type == TO_CHAR && to != ch )
            continue;
        if ( type == TO_VICT && ( to != vch || to == ch ) )
            continue;
        if ( type == TO_ROOM && to == ch )
            continue;
        if ( type == TO_NOTVICT && (to == ch || to == vch) )
            continue;

        point   = buf;
        str     = format;
        while ( *str != '\0' )
        {
            if ( *str != '$' )
            {
		    fColour = TRUE;
              *point++ = *str++;
               continue;
            }
            ++str;

            if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
            {
                bug( "Act: missing arg2 for code %d.", *str );
                i = " <@@@> ";
            }
            else
            {

                switch ( *str )
                {
                default:  bug( "Act: bad code %d.", *str );
                          i = " <@@@> ";                                break;
                /* Thx alex for 't' idea */
                case 't': i = (char *) arg1;                            break;
                case 'T': i = (char *) arg2;                            break;
                case 'n': i =
		  (is_affected(ch,gsn_doppelganger) &&
		   !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    PERS(ch->doppel,to) : PERS( ch,  to );
		  break;
                case 'N': i =
		  (is_affected(vch,gsn_doppelganger) &&
		   !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    PERS(vch->doppel,to):PERS(vch,  to );
		  break;
                case 'e': i =
		  (is_affected(ch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    he_she [URANGE(0,ch->doppel->sex,2)] :
		    he_she  [URANGE(0, ch  ->sex, 2)];
		  break;
                case 'E': i =
		  (is_affected(vch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    he_she  [URANGE(0, vch->doppel->sex, 2)] :
		    he_she  [URANGE(0, vch->sex, 2)];
		  break;
                case 'm': i =
		  (is_affected(ch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    him_her [URANGE(0,ch->doppel->sex,2)] :
		    him_her [URANGE(0, ch->sex, 2)];
		  break;
                case 'M': i =
		  (is_affected(vch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    him_her  [URANGE(0, vch->doppel->sex, 2)] :
		    him_her  [URANGE(0, vch->sex, 2)];
		  break;
                case 's': i =
		  (is_affected(ch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    his_her [URANGE(0,ch->doppel->sex,2)] :
		    his_her [URANGE(0, ch  ->sex, 2)];
		  break;
                case 'S': i =
		  (is_affected(vch, gsn_doppelganger) &&
		    !IS_SET(to->act,PLR_HOLYLIGHT)) ?
		    his_her  [URANGE(0, vch->doppel->sex, 2)] :
		    his_her  [URANGE(0, vch->sex, 2)];
		  break;

                case 'p':
                    i = can_see_obj( to, obj1 )
                            ? obj1->short_descr
                            : "something";
                    break;

                case 'P':
                    i = can_see_obj( to, obj2 )
                            ? obj2->short_descr
                            : "something";
                    break;

                case 'd':
                    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
                    {
                        i = "door";
                    }
                    else
                    {
                        one_argument( (char *) arg2, fname );
                        i = fname;
                    }
                    break;
                }
            }

            ++str;
            while ( ( *point = *i ) != '\0' )
                ++point, ++i;
        }

        *point++	= '\n';
        *point++	= '\r';
        *point		= '\0';
	buf[0]		= UPPER( buf[0] );
	pbuff		= buffer;
	colourconv( pbuff, buf, to );
/* board code added, not sure here */


     /* This new TRIG_ACT Recode be kazmir*/
/*    if (( to->desc != NULL )&& (to->desc->connected == CON_PLAYING))
         write_to_buffer( to->desc, buffer, 0 );
*/
    if ( !IS_NPC(ch) )
    {
       CHAR_DATA *mob, *mob_next;
       for ( mob = ch->in_room->people; mob != NULL; mob = mob_next )
       {
           mob_next = mob->next_in_room;
           if ( IS_NPC(mob) && HAS_TRIGGER_MOB( mob, TRIG_ACT )
           &&   mob->position == mob->pIndexData->default_pos )
//               mp_act_trigger( buf, mob, ch, arg1, arg2, TRIG_ACT);
                 p_act_trigger( buf, mob, NULL, NULL, ch, NULL, NULL, TRIG_ACT );



       }
     }

      /* This old TRIG_ACT Recode be kazmir */
	if (to->desc && (to->desc->connected == CON_PLAYING))
        write_to_buffer( to->desc, buffer, 0);
        else 
       if ( MOBtrigger )
//           p_act_trigger( buf, to, ch, arg1, arg2, TRIG_ACT ); */
           p_act_trigger( buf, to, NULL, NULL, ch, NULL, NULL, TRIG_ACT );
    if ( type == TO_ROOM || type == TO_NOTVICT )
    {
	OBJ_DATA *obj, *obj_next;
	CHAR_DATA *tch, *tch_next;

	 point   = buf;
	 str     = format;
	 while( *str != '\0' )
	 {
	     *point++ = *str++;
	 }
	 *point   = '\0';
	 
	for( obj = ch->in_room->contents; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( HAS_TRIGGER_OBJ( obj, TRIG_ACT ) )
		p_act_trigger( buf, NULL, obj, NULL, ch, NULL, NULL, TRIG_ACT );
	}

	for( tch = ch; tch; tch = tch_next )
	{
	    tch_next = tch->next_in_room;

	    for ( obj = tch->carrying; obj; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( HAS_TRIGGER_OBJ( obj, TRIG_ACT ) )
		    p_act_trigger( buf, NULL, obj, NULL, ch, NULL, NULL, TRIG_ACT );
	    }
	}

	 if ( HAS_TRIGGER_ROOM( ch->in_room, TRIG_ACT ) )
	     p_act_trigger( buf, NULL, NULL, ch->in_room, ch, NULL, NULL, TRIG_ACT );
    }


    }
    return;
}

void act_new( const char *format, CHAR_DATA *ch, const void *arg1,
	      const void *arg2, int type, int min_pos)
{
    /* to be compatible with older code */
    act_color(format,ch,arg1,arg2,type,min_pos);
    return;
}

int colour( char type, CHAR_DATA *ch, char *string )
{
    char	code[ 20 ];
    char	*p = '\0';

    if( IS_NPC( ch ) )
	return( 0 );

    switch( type )
    {
	default:
	    sprintf( code, CLEAR );
	    break;
	case 'x':
	    sprintf( code, CLEAR );
	    break;
	case 'b':
	    sprintf( code, C_BLUE );
	    break;
	case 'c':
	    sprintf( code, C_CYAN );
	    break;
	case 'g':
	    sprintf( code, C_GREEN );
	    break;
	case 'm':
	    sprintf( code, C_MAGENTA );
	    break;
	case 'r':
	    sprintf( code, C_RED );
	    break;
	case 'w':
	    sprintf( code, C_WHITE );
	    break;
	case 'y':
	    sprintf( code, C_YELLOW );
	    break;
	case 'B':
	    sprintf( code, C_B_BLUE );
	    break;
	case 'C':
	    sprintf( code, C_B_CYAN );
	    break;
	case 'G':
	    sprintf( code, C_B_GREEN );
	    break;
	case 'M':
	    sprintf( code, C_B_MAGENTA );
	    break;
	case 'R':
	    sprintf( code, C_B_RED );
	    break;
	case 'W':
	    sprintf( code, C_B_WHITE );
	    break;
	case 'Y':
	    sprintf( code, C_B_YELLOW );
	    break;
	case 'D':
	    sprintf( code, C_D_GREY );
	    break;
	case '*':
	    sprintf( code, "%c", 007 );
	    break;
/*	case '/':
	    sprintf( code, "%c", 012 );
	    break;*/
        case '%':
            strcpy( code, STYLE_FLASH );
            break;
        case 'u':
            strcpy( code, C_UNDERSCORE );
            break;            
        case 'i':
            strcpy( code, C_ITALIC );
            break;            
	case '{':
	    sprintf( code, "%c", '{' );
	    break;
    }

    p = code;
    while( *p != '\0' )
    {
	*string = *p++;
	*++string = '\0';
    }

    return( strlen( code ) );
}

void colourconv( char *buffer, const char *txt , CHAR_DATA *ch )
{
    const	char	*point;
		int	skip = 0;

    if( ch->desc && txt )
    {
	if( IS_SET( ch->act, PLR_COLOR ) )
	{
	    for( point = txt ; *point ; point++ )
	    {
		if( *point == '{' )
		{
		    point++;
		    skip = colour( *point, ch, buffer );
		    while( skip-- > 0 )
			++buffer;
		    continue;
		}
		*buffer = *point;
		*++buffer = '\0';
	    }
	    *buffer = '\0';
	}
	else
	{
	    for( point = txt ; *point ; point++ )
	    {
		if( *point == '{' )
		{
		    point++;
		    continue;
		}
		*buffer = *point;
		*++buffer = '\0';
	    }
	    *buffer = '\0';
	}
    }
    return;
}


void dump_to_scr( char *text )
{
int a;

  a = strlen( text );
  write(1, text, a);
  return;
}

/*
char *get_stat_alias( CHAR_DATA *ch, int which )
{
char *stat;
int istat;

    if ( which == STAT_STR)  {
      istat=get_curr_stat(ch,STAT_STR);
      if      ( istat >  23 ) stat = "  Titantic  ";
      else if ( istat >= 21 ) stat = " Remarkable ";
      else if ( istat >= 18 ) stat = "   Strong   ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 10 ) stat = "    Weak    ";
      else                    stat = "  Infantile ";
      return(stat);
    }

    if ( which == STAT_WIS)  {
      istat=get_curr_stat(ch,STAT_WIS);
      if      ( istat >  23 ) stat = "  Profound  ";
      else if ( istat >= 21 ) stat = " Insightful ";
      else if ( istat >= 18 ) stat = "  Sensible  ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 10 ) stat = "    Dim     ";
      else                    stat = "  Foolish   ";
      return(stat);
    }

    if ( which == STAT_CON)  {
      istat=get_curr_stat(ch,STAT_CON);
      if      ( istat >  23 ) stat = "  Vigorous  ";
      else if ( istat >= 21 ) stat = "   Robust   ";
      else if ( istat >= 18 ) stat = "  Healthy   ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 10 ) stat = "    Weak    ";
      else                    stat = "   Sickly   ";
      return(stat);
    }

    if ( which == STAT_INT)  {
      istat=get_curr_stat(ch,STAT_INT);
      if      ( istat >  23 ) stat = "   Genius   ";
      else if ( istat >= 21 ) stat = " Brilliant  ";
      else if ( istat >= 18 ) stat = "   Astute   ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 10 ) stat = "    Slow    ";
      else                    stat = "Incompetent ";
      return(stat);
    }

    if ( which == STAT_DEX)  {
      istat=get_curr_stat(ch,STAT_DEX);
      if      ( istat >  23 ) stat = "   Expert   ";
      else if ( istat >= 21 ) stat = "   Adroit   ";
      else if ( istat >= 18 ) stat = "   Quick    ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 10 ) stat = "   Clumsy   ";
      else                    stat = "  Hopeless  ";
      return(stat);
    }

   bug( "Stat_alias: Bad stat number.", 0 );
   return(NULL);

}
*/
void logf (char * fmt, ...)
{
        char buf [2*MAX_STRING_LENGTH];
        va_list args;
        va_start (args, fmt);
        vsprintf(buf,fmt,args);
        va_end(args);

        log_string(buf);
}

int strlencolor(char *argument)
{
  char *str;
  int strlength;

  if (argument == NULL || argument[0] == '\0')
    return 0;

  strlength = 0;
  str = argument;

  while (*str != '\0')
  {
    if ( *str != '{' ) 
    {                                         
      str++;
      strlength++;
      continue;
    }
  if ( *str == '{' )
      {
        str++;
        if ( *str == '{' )
        {
          strlength++;
          str++;
          continue;
        }
        else
        {
          str++;
        }
      }
    }
  return strlength;
}


/* source: EOD, by John Booth <???> */

void printf_to_char (CHAR_DATA *ch, char *fmt, ...)
{
       char buf [MAX_STRING_LENGTH];
       va_list args;
       va_start (args, fmt);
       vsprintf (buf, fmt, args);
       va_end (args);

       send_to_char (buf, ch);
}

void bugf (char * fmt, ...)
{
       char buf [2*MSL];
       va_list args;
       va_start (args, fmt);
       vsprintf (buf, fmt, args);
       va_end (args);

       bug (buf, 0);
}

volatile sig_atomic_t fatal_error_in_process = 0;

/* Copyover note:
 * Whenever a program is executed from exec (as in the case of copyover)
 * all signals from the previous execution of the mud return to their
 * SIG_DEF (default) signal handler, since copyover starts the mud
 * process all over again, init_signals will get called and properly
 * reset the signal handler.
 */

/** Function: signal_handler
  * Descr   : Our signal handler, to shutdown gracefully (if possible)
  *         : upon reciept of a signal signifying we should shutdown.
  * Returns : void
  * Syntax  : (n/a)
  */
void signal_handler(int sig)
{
  /* Since our 'shutdown_wgrace' is not reentrant, and may (likely) access
   * non-volatile global variables, be damn sure it's not called twice.
   */
  if ( fatal_error_in_process )
     raise(sig);

  switch(sig)
  {
    case SIGBUS:
    case SIGTERM:
    case SIGABRT:
    case SIGSEGV:
      {
         fatal_error_in_process = 1;         /* Yes, this IS a fatal error */
         bugf("BUG: Signal handler in %s.",  /* Log signal to bug log file */
               (sig == SIGBUS)  ? "SIGBUS"  :
               (sig == SIGTERM) ? "SIGTERM" :
               (sig == SIGABRT) ? "SIGABRT" : "SIGSEGV");

         shutdown_wgrace();                  /* Attempt a graceful shutdown  */
         raise(sig);                         /* set return status of process */
         break;
      }
      break;

  }

}

/** Function: init_signals
  * Descr   : Initialize signals that we trap, setting up a handler for them.
  * Returns : void
  * Syntax  : void
  * Note    : By default, signals sent to sighandler are blocked until the
  *         : handler returns, but other signals are not blocked. We need
  *         : to block any other signal we recieve that we normally trap,
  *         : until we are done trying to be graceful...
  */

void init_signals()
{
  struct sigaction sigact;
  sigset_t mask;

    /* NOTE: We inherit any current signal actions by default.
     * Dont install a signal handler for any ignored signals.
     */
    sigaction ( SIGBUS, NULL, &sigact );

    if ( sigact.sa_handler != SIG_IGN )
    {
      sigact.sa_handler = signal_handler;
      sigemptyset(&mask);
      /* block these signals to the handler while it's running */
      sigaddset(&mask, SIGTERM);
      sigaddset(&mask, SIGABRT);
      sigaddset(&mask, SIGSEGV);
     sigprocmask(SIG_BLOCK, &mask, NULL);  /*JLR*/
      sigact.sa_mask = mask;
      sigact.sa_flags = SA_RESETHAND;
      sigaction ( SIGBUS, &sigact, NULL );
    } else
      logf("Init: Signal SIGBUS ignored.");

    sigaction ( SIGTERM, NULL, &sigact );

    if ( sigact.sa_handler != SIG_IGN )
    {
      sigact.sa_handler = signal_handler;
      sigemptyset(&mask);
      /* block these signals to the handler while it's running */
      sigaddset(&mask, SIGBUS);
      sigaddset(&mask, SIGABRT);
      sigaddset(&mask, SIGSEGV);
      sigact.sa_mask = mask;
      sigact.sa_flags = SA_RESETHAND;
      sigaction ( SIGTERM, &sigact, NULL );
    } else
      logf("Init: Signal SIGTERM ignored.");

    sigaction ( SIGABRT, NULL, &sigact );

    if ( sigact.sa_handler != SIG_IGN )
    {
      sigact.sa_handler = signal_handler;
      sigemptyset(&mask);
      /* block these signals to the handler while it's running */
      sigaddset(&mask, SIGBUS);
      sigaddset(&mask, SIGTERM);
      sigaddset(&mask, SIGSEGV);
      sigact.sa_mask = mask;
     sigact.sa_flags = SA_RESETHAND;
      sigaction ( SIGABRT, &sigact, NULL );
    } else
      logf("Init: Signal SIGABRT ignored.");

    sigaction ( SIGSEGV, NULL, &sigact );

    if ( sigact.sa_handler != SIG_IGN )
    {
      sigact.sa_handler = signal_handler;
      sigemptyset(&mask);
      /* block these signals to the handler while it's running */
      sigaddset(&mask, SIGBUS);
      sigaddset(&mask, SIGTERM);
      sigaddset(&mask, SIGABRT);
      sigact.sa_mask = mask;
      sigact.sa_flags = SA_RESETHAND;
     sigaction ( SIGSEGV, &sigact, NULL );
    } else
      logf("Init: Signal SIGSEGV ignored.");

    logf("Init: Signals initialized.");

}


/** Function: shutdown_wgrace
  * Descr   : Upon receipt of a fatal signal, attempt a graceful shutdown.
  */
void shutdown_wgrace()
{
    DESCRIPTOR_DATA *d,*d_next;

    #if defined(WWWSERVER)
    shutdown_web();     /* be sure we close the webserver port  */
    #endif

    /* save_mud_data();    Save the mud configuration [WoT Only]*/

//    do_asave(NULL, ""); /* Attempt to save any changed areas    */

    /* Notify players of impending crash, and save all pfiles   */
    for ( d = descriptor_list; d != NULL; d = d_next )
    {

      if( d->character )
      {
     if ( d->connected == CON_PLAYING )
          send_to_char("\n\r-*- System Crash... Saving your data. -*-\n\r",
                       d->character);

//        do_save (d->character, "");
      }

        d_next = d->next;
        close_socket(d);
    }

}

void send_to_desc (const char *txt, DESCRIPTOR_DATA * d)
{
    const char *point;
    char *point2;
    char buf[MAX_STRING_LENGTH * 4];
    int skip = 0;

    buf[0] = '\0';
    point2 = buf;
    if (txt && d)
    {

      //  if (d->ansi == TRUE)
        if (TRUE)
        {
            for (point = txt; *point; point++)
            {
                if (*point == '{')
                {
                    point++;
                    skip = colour (*point, NULL, point2);
                    while (skip-- > 0)
                        ++point2;
                    continue;
                }
                *point2 = *point;
                *++point2 = '\0';
            }
            *point2 = '\0';
            write_to_buffer (d, buf, point2 - buf);
        }
        else
        {
            for (point = txt; *point; point++)
            {
                if (*point == '{')
                {
                    point++;
                    continue;
                }
                *point2 = *point;
                *++point2 = '\0';
            }
            *point2 = '\0';
            write_to_buffer (d, buf, point2 - buf);
        }
    }
    return;
}
