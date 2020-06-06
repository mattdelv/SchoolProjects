#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
 
//First version of the ls command in Linux
//Reads all files in folder specified
//Displays All files, or just non-hidden files
//in alphabetical order, or reverse alphabetical order
//based on flags given.
 
char** do_ls(char [], int fileCount);
int count_ls(char dirname[]);


int main(int argc, char *argv[])
{
	int a_flag = 0;
	int r_flag = 0;
	int s_flag = 0;
	int opt_flag = 0;
	int opt;
	int count = 1;
	int fileCount = 0;
	char** arr = 0;
	char temp[100];
	
	
	while ((opt = getopt(argc, argv, "ars")) != -1){
		if (opt_flag == 0){
			count++;
			opt_flag = 1;
		}
			
        switch (opt)
        {
            case 'a':
                a_flag = 1;
                break;
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
	
	int printFlag = 0;
	
	if (r_flag == 1){
		int stop_index = 2;
	if(a_flag == 1){
     stop_index = 0;
	}
	for (int i = fileCount - 1; i >= stop_index; i--)
		printf("%s\n", arr[i]);
 
 printFlag = 1;
}
	if(a_flag == 1 && printFlag == 0){
		for (int i = 0; i < fileCount; i++)
			printf("%s\n", arr[i]);
	}
	else if (printFlag == 0){
		for (int i = 0; i < fileCount; i++) {
			if (arr[i][0] == '.'){}
				
			else
				printf("%s\n", arr[i]);
		}
	}
	
	return 0;
}

int count_ls(char dirname[]){
	int fileCounter = 0;			/* total file count in directory*/
	DIR *dir_ptr;
	struct dirent *direntp;

	if ( ( dir_ptr = opendir( dirname ) ) == NULL ){
		fprintf(stderr,"ls1: cannot open %s\n", dirname);
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

