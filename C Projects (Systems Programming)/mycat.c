#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

//Follows the same behaviors as the 'cat' command
//concatonates files together

int main(int argc, char *argv[]){

    char current;
    char previous;
	int count = 2;
	int b_flag = 0;
	int e_flag = 0;
	int opt;
    
    /* check args 	*/
    if ( argc < 2 ){
        printf("usage: program_name [-be] file1 [file2 ...]\n");
        exit(1);
    }
    
	while ((opt = getopt(argc, argv, "be")) != -1){
        switch (opt)
        {
            case 'b':
                b_flag = 1;
				if(e_flag == 0)
					count = count + 1;
                break;
            case 'e':
                e_flag = 1;
				if(b_flag == 0)
					count = count + 1;
                break;
            default:
                opt = -1;
        }
    }
	// line counter -- placed outside of the loop to count ALL files lines without starting at 1 for each file
	// instructions weren't perfectly clear, so I made this decision to count total lines rather than just lines in each file
	int line_number = 0;
	
	
	//loop runs while there are still file arguments to concatonate
	while( count <= argc){
		/* open file and check for potential errors */
		int fd = open(argv[(count - 1)], O_RDONLY);
		
		//checking for file
		if (fd == -1) {
			printf("Error opening file: No such file or directory");
			exit(1);
		}
		
		int n_chars;
		
		
		/* read file and print out chars with proper concatonation */
		while ( (n_chars = read(fd, &current, sizeof(current))) > 0){
			if(b_flag == 1 && ((previous == '\n' && current != '\n') || line_number == 0)){
				printf("\t%d ", ++line_number);
			}
			
			if(e_flag == 1 && (current == '\n'))
				printf("$");
			
			printf("%c", current);
			previous = current;
		}

		/* close file	*/
		if ( close(fd) == -1 ){
			perror("Error closing file");
			exit(1);
		}
		else
			close(fd);
		
		//increment counter in case of multiple files
		count++;
		
	}
    return 0;
}
