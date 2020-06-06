#include	<stdio.h>
#include 	<stdlib.h>
#include	<fcntl.h>
#include 	<unistd.h>
#include 	<string.h>
#include 	<errno.h>
#include 	<sys/stat.h>
#include 	<time.h>
#include	<utmp.h>

#define SHOWHOST


char *getHost(struct utmp *, char *);
char *getTty(struct utmp *, char *);

int main( int argc, char *argv[]){
	struct stat infobuf;
	int	fd;
	char buf[BUFSIZ]; 
	char message[400];
	char *mytty = ttyname(STDIN_FILENO);
	char *myhost = malloc(100);
	char *target_tty = malloc(200);
	struct utmp utbuf; 
    int utmpfd;
	char *username;
	char *myusername;
	int idlecomp1 = 99999999;
	int idlecomp2 = 0;
	int idletime = 0;
	time_t now;
	struct tm *now_tm;
	int hour;
	int minute;
	time_t current_time;
	char *held_tty = malloc(13);
	
	
	int i=0;
	
	now = time(NULL);
	now_tm = localtime(&now);
	hour = now_tm->tm_hour;
	minute = now_tm->tm_min;
	
	//get current username
	myusername = getlogin();
	
	
	if(argc < 2 || argc > 3){
		printf("Use incorrect, please use: mywrite [username] [optional ttyname]\n");
		exit(1);
	}
    if ( (utmpfd = open(UTMP_FILE, O_RDONLY)) == -1 ){
        perror(UTMP_FILE);
        exit(1);
    }
	username = argv[1];
	if(argc == 3){
		target_tty = argv[2];
	}
	else{
		//find latest tty based on input username
		while( read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf) ){
			if ( utbuf.ut_type == USER_PROCESS ){
				//if myusername shows up, get the hostname
				if(strcmp(utbuf.ut_name, myusername) == 0 )
					myhost = utbuf.ut_host;
				//if the username shows up, figure out the lastest
				// terminal tty used
				if( strcmp(utbuf.ut_name, username) == 0 ){
					//get last time used
					char *held2_tty = malloc(13);
					strcpy(held2_tty, "");
					strcat(held2_tty, "/dev/");
					strcat(held2_tty, utbuf.ut_line);
					if(stat(held2_tty, &infobuf) == -1)
						perror(utbuf.ut_line);
					else {
						current_time = time(NULL);
						idletime = (int)infobuf.st_atime;
					}
					idlecomp2 = current_time - idletime;
					//compare with currently chosen username/tty combo
					if(idlecomp2 < idlecomp1){
						strcpy(held_tty, held2_tty);
						idlecomp1 = idlecomp2;
					}
					free(held2_tty);
				}
			}
		}
		strcpy(target_tty, held_tty);
	}
	//creating message for output
	sprintf(message, "Message from %s@%s on %s at %d:%d \n", myusername, myhost, mytty, hour, minute);

	
	
	/* open devices */
	fd = open( target_tty, O_WRONLY );
	if ( fd == -1 ){
		printf("file not found ERROR\n"); 
                exit(1);
	}
	
	write(fd, message, strlen(message));
	/* loop until EOF on input */
	
	while(fgets(buf, BUFSIZ, stdin) != NULL){
		if(write(fd, buf, strlen(buf)) == -1){
			break;
		}
	}
	
	//write "EOF" to other user's terminal and close file connection when user disconnects.
	write(fd, "EOF\n", 4);
	close(fd);

	return 0;
}