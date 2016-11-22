//compressR_LOLS.c
//compressR_LOLS(<file to compress>, <number of parts>)
//uses processes to split compression of file into a specific number of parts
#include "rle.c"
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

char * fileName;
//struct for holding arguments for compressToFile
typedef struct args
{
	char * input;
	int segSize;
	int place;
	int iteration;	
} args;

//uses rle encoding on input string, then writes the code to a new file
/*
void compressToFile(void* arguments){
	//retrieve and encode input

	args* in = (args *) arguments;
	
	char* segment = (char*)malloc(in->segSize + 1);

	strncpy(segment, in->input + in->place, in->segSize);

	printf("Segment: %s\n", segment);

	char * encoded = encode(segment);

	printf("Encoded: %s\n", encoded);

	//now write encoded to file
	//contruct the new file's name
	int fileNameSize = sizeof(fileName) + 3;
	
	char * number = (char*)malloc(3);
	char * newFileName = (char*)(malloc(fileNameSize));
	
	strcpy(newFileName, fileName); 
	sprintf(number, "%d", in->iteration + 1);
	strcat(newFileName, number);

	printf("File name: %s\n", newFileName);

	//create a new file with the name
	FILE *file;
	file = fopen(newFileName, "w+");
	printf("File opened\n");
	fprintf(file, encoded);

	fclose(file);

	free(newFileName);
	free(number);
	return;
}
*/

int main(int argc, char *argv[]){
	
	int childCount;	//number of threads specified
	int child_status;
		//specified file name

	FILE * file;		//file with uncompressed input

	int docLength;		//character length of file

	char * input;		//uncompressed string

	int segSize;		//size of uncompressed string segments
	//get comand line args
	if (argc == 3){
		fileName = argv[1];
		childCount = atoi(argv[2]);
	}else{
		printf("ERROR not enough arguments\n");
		return 0;
	}

	/*get uncompressed string from file*/
	file = fopen(fileName, "r");

	docLength = get_file_length(file);

	input = (char*)malloc(docLength);

	fgets(input, docLength + 1, file);

	fclose(file);

	printf("Input : %s\n", input);

	//
	
	int place = 0; 					//used to save index where process leaves off/starts again
	int i;
	
	for(i = 0; i < childCount; i++){
		pid_t child_pid = fork();
		
		segSize = floor(docLength/childCount);

		args* arguments =  (args*)malloc(sizeof(args));

		arguments->segSize = segSize;
			printf("ARGUMNETS SEGSIZE %d\n", segSize);
		arguments->place = place;
			printf("ARGUMENTS PLACE %d\n", place);
		arguments->iteration = i;
			printf("ARGUMENTS ITERATION %d\n", i);
		
		
	    char *argPassing[5];
		printf("INPUT %s\n", input);
		argPassing[0] = input;
		char *str1 = malloc(sizeof(char*));
		char *str2 = malloc(sizeof(char*));
		char *str3 = malloc(sizeof(char*));
		
		sprintf(str1, "%d", segSize);
		argPassing[1] = str1;
		sprintf(str2, "%d", place);
		argPassing[2] = str2;
		sprintf(str3, "%d", i);
		argPassing[3] = str3;
	
		argPassing[4] = fileName;
		
		printf("ARGPASSING 0 %s\n", argPassing[0]);
		printf("ARGPASSING 1 %s\n", argPassing[1]);
		printf("ARGPASSING 2 %s\n", argPassing[2]);
		printf("ARGPASSING 3 %s\n", argPassing[3]);
		printf("ARGPASSING 4 %s\n", argPassing[4]);
		
		
		
		printf("PASSING TO THE WORKER\n");
		if(child_pid != 0){
			
		}else{
			execvp("./compressR_worker_LOLS", argPassing);
			exit(0);
		}
		
		place += segSize;
	}

    
	printf("Waiting on process\n");
	/* Wait for children to exit. */
	int child_status;
	pid_t pid;
	int c = childCount
	while (c > 0) {
	  pid = wait(&child_status);
	  printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
	  --n;  // TODO(pts): Remove pid from the pids array.
	}

	// for(i = 0; i < childCount; i++){
	// 	pthread_join(pth[i],NULL);
	// }

	printf("process done\n");
	return 0;

}

int get_file_length( FILE *file ) {
    fpos_t position; 
    int length; 

    fgetpos( file, &position ); // save previous position in file

    if ( fseek( file, 0, SEEK_END ) // seek to end
        || ( length = ftell( file ) ) == -1 ) { // determine offset of end
        perror( "Finding file length" ); // handle overflow
    }

    fsetpos( file, &position ); // restore position

    return length;
}








