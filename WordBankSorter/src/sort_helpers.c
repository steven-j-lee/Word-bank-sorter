#include "A4_sort_helpers.h"
#include <stdlib.h>
#define BUFFER_SIZE 100
#define TRUE 1

// Function: read_all() 
// Provided to read an entire file, line by line.
// No need to change this one.

void read_all( char *filename ){
    
    FILE *fp = fopen( filename, "r" );

    int curr_line = 0;
	
    while( curr_line < MAX_NUMBER_LINES && 
           fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) )
    {
        curr_line++;
    }
	
    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// Function: read_all() 
// Provided to read only the lines of a file staring with first_letter.
// No need to change this one.

void read_by_letter( char *filename, char first_letter ){

    FILE *fp = fopen( filename, "r" );
    int curr_line = 0;
    text_array[curr_line][0] = '\0';
	
    while( fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) ){
        if( text_array[curr_line][0] == first_letter ){
            curr_line++;
        }

        if( curr_line == MAX_NUMBER_LINES ){
            sprintf( buf, "ERROR: Attempted to read too many lines from file.\n" );
            write( 1, buf, strlen(buf) );
            break;
        }
    }
	
    text_array[curr_line][0] = '\0';
    fclose(fp);
}

sem_t* global[100]; 


void sort_words( ){

char* sort;
sort = (char*)malloc(MAX_NUMBER_LINES);

	for(int a = 0; text_array[a][0] != '\0'; a++){

		for(int b = a+1; text_array[b][0] !='\0'; b++){ 

			if(strcmp(text_array[a], text_array[b]) > 0){

					strcpy(sort, text_array[a]);

					strcpy(text_array[a], text_array[b]);

					strcpy(text_array[b], sort);			
				}			
	 	}
	}

free(sort);
    
}


int initialize( ){

char* buffer;

buffer = (char*)malloc(BUFFER_SIZE);

	for(int i=0; i<27; i++){

		sprintf(buffer, "SEM_%C", 'a'+i);

		sem_unlink(buffer);

			if(i==0){

				global[i] = sem_open(buffer, O_CREAT, 0666, 1);

				} else{

				       global[i] = sem_open(buffer, O_CREAT, 0666, 0);

				}

	}

free(buffer);

    return 0;
}


// YOU MUST COMPLETE THIS FUNCTION FOR Q2 and Q3.   

int process_by_letter( char* input_filename, char first_letter ){
	
sem_wait(global[(first_letter-'a')]);

read_by_letter(input_filename, first_letter);

sort_words();

FILE* file = fopen("sorted.txt", "ab+");

	/*sprintf( buf, "This process will sort the letter %c.\n",  first_letter );
    	write(1,buf,strlen(buf)); */
	
	for(int n = 0; text_array[n][0] !='\0'; n++){
	
			fprintf(file, "%s", text_array[n]);

		}

		fclose(file);

	if((first_letter-'a') != 26){

		sem_post(global[(first_letter-'a')+1]);

		  } 

    return 0;
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2 and Q3.
int finalize( ){

sem_wait(global[26]);

char* buffer;
buffer = (char*)malloc(MAX_LINE_LENGTH);

FILE* file = fopen("sorted.txt", "rb+");
	
	while(TRUE) { //infinite loop until it breaks

		  fgets(buffer, MAX_LINE_LENGTH, file);
		  
		  if(feof(file)){
					break; //feof indicates when the file reaches the end ==> then it breaks
				} 

		  fprintf(stdout, "%s", buffer);

		  fflush(stdout); //flushes out the output of the buffer

		  }

		fclose(file);

		remove("sorted.txt"); //more convenient to test (no need to manually delete the temporary file)

    sprintf( buf, "Sorting complete!\n" );

    write( 1, buf, strlen(buf) );

free(buffer);


    return 0;
}

