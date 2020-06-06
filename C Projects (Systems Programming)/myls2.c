#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
 
//Second version of the ls command in Linux
//Reads all files in folder specified
//Displays All files, or just non-hidden files
//in alphabetical order, or reverse alphabetical order
//based on flags given -- also formats the output to 
//match the output of the Linux ls command
 
 
char** do_ls(char [], int fileCount);
int count_ls(char dirname[]);
int maxLength(char *fileArray);


int main(int argc, char *argv[])
{
	int r_flag = 0;
	int s_flag = 0;
	int opt_flag = 0;
	int opt;
	int count = 1;
	int fileCount = 0;
	char** arr = 0;
	char temp[100];
	int maxLen = 0;
	struct winsize wbuf;
	
	while ((opt = getopt(argc, argv, "rs")) != -1){
		if (opt_flag == 0){
			count++;
			opt_flag = 1;
		}
			
        switch (opt)
        {
            case 'r':
                r_flag = 1;
                break;
			case 's':
				s_flag = 1;
				break;
            default:
                break;
        }
    }
	
	if (s_flag == 1 && r_flag == 1){
		printf("Cannot be in lexicographical order and reverse order\n");
		exit(1);
	}

	// If no args, call do_ls function using ".", which is the current directory
	if ( argc == count ){
		fileCount = count_ls(".");
		arr = do_ls( ".", fileCount);
	}
	else{
		for (int i = count; i < argc; i++)
		{
			fileCount = count_ls(argv[i]);
			arr = do_ls( argv[i], fileCount );
		}
	}
	
	int lenCounter = 0;
	int lenMax = 0;
	
	for(int i = 0; i < fileCount; i++){
		lenCounter = strlen(arr[i]);
		if (lenCounter > lenMax)
			lenMax = lenCounter;
		
	}
	
	maxLen = lenMax;
	int col = 0;
	if(ioctl(STDIN_FILENO, TIOCGWINSZ, &wbuf) != -1) {
		col = wbuf.ws_col;	
	}
	int row = 0;
	col = col / maxLen;
	row = (fileCount / col) + 1;
	
	//Selection sorting list 
	
	if(s_flag == 1 || r_flag == 1){
		for(int i = 0;i < fileCount;i++)
		  for(int j = i + 1;j < fileCount; j++){
			 if(strcmp(arr[i],arr[j])>0){
				strcpy(temp,arr[i]);
				strcpy(arr[i],arr[j]);
				strcpy(arr[j],temp);
			}
		}
	}
	
	int index = 0;
	if(r_flag == 0){
		for (int i = 0; i < row; i++){
			int index = i + 3;
			for (int j = 0; j < col; j++){
				if(index > fileCount - 1)
					break;
				if (arr[index][0] == '.'){
					
				}
				else
					printf("%-*s", maxLen, arr[index]);
				index = index + row;
				
			}
			printf("\n");
		}
	}
	
	if(r_flag == 1){
		for (int i = 0; i < row; i++){
			int index = (fileCount - 1) - i;
			for (int j = 0; j < col; j++){
				if(index < 0)
					break;
				if (arr[index][0] == '.'){}
				else
					printf("%-*s", maxLen, arr[index]);
				index = index - (row - i);
			}
			printf("\n");
		}
	}
	
	return 0;
}

//Finds longest filename in dirname
//used for formatting output

int maxLength(char *fileArray){
	int lenCounter = 0;
	int max = 0;
	int arraySize = sizeof(fileArray) / sizeof(fileArray[0]);	
	
	for(int i = 0; i < arraySize; i++){
		int j = 0;
	
		lenCounter = sizeof(fileArray[i]);
	
		if(lenCounter > max)
			max = lenCounter;
	
	}
	return max;
}

/*
 *  Counts all files in the directory
 */

int count_ls(char dirname[]){
	int fileCounter = 0;			/* total file count in directory*/
	DIR *dir_ptr;
	struct dirent *direntp;

	if ( ( dir_ptr = opendir( dirname ) ) == NULL ){
		fprintf(stderr,"ls2: cannot open %s\n", dirname);
		exit(1);
	}
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL )
				fileCounter++;
			
	}
		closedir(dir_ptr);
		
		return fileCounter;
}
	


/*
 *	list files in directory called dirname
 */
char** do_ls( char dirname[], int fileCount )
{
	DIR		*dir_ptr;		         /* the directory */
	struct dirent	*direntp;		/* each entry	 */
	char** fileArray = (char**)malloc(sizeof(char*) * (fileCount));
	int count = 0;
	
	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr,"ls1: cannot open %s\n", dirname);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL ){
			fileArray[count] = direntp->d_name;
			count++;
		}
		closedir(dir_ptr);
	}
	
	return fileArray;
}

