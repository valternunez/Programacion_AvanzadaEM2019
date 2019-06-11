/*----------------------------------------------------------------

*

* Programación avanzada: Normalización de archivos

* Fecha: 22-febrero-2019

* Autor: A01206138 Valter Núñez

*

*--------------------------------------------------------------*/

//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <float.h>


//Normalize Function
void normalize (char* filename, char* newfilename, char* program){
    
    //Variable declaration
    FILE* file;
	int aux, res, i, j = 0;
	float num, nval, values[61][2];
	
	
	//ERROR detection
	file = fopen(filename, "r");
	if (!file)
	{
		perror(program);
		exit(-1);
	}

	FILE *newfile;
    
    //ERROR Detection
	newfile = fopen(newfilename, "w");
	if (!newfile)
	{
		perror(program);
		exit(-1);
	}


    //CALCULATION FOR's
	for (i = 0; i < 61; i++)
	{
		fscanf(file, "%i,%i", &aux, &res);

		values[i][0] = FLT_MAX;
		values[i][1] = FLT_MIN;

		for (j = 0; j < 625; j++)
		{
			fscanf(file, ",%f", &num);

			if (num > values[i][1])
			{
				values[i][1] = num;
			}

			if (num < values[i][0])
			{
				values[i][0] = num;
			}
		}
	}
	
	fseek(file, 0, SEEK_SET);
	j = 0;

	for (j = 0; j < 61; j++)
	{
		fscanf(file, "%i,%i", &aux, &res);

		fscanf(file, ",%f", &num);
			
		nval = (num - values[j][0]) / (values[j][1] - values[j][0]);

		fprintf(newfile, "%.2f", nval);

		for (i = 1; i < 625; i++)
		{
			fscanf(file, ",%f", &num);
			
			nval = (num - values[j][0]) / (values[j][1] - values[j][0]);

			fprintf(newfile, ",%.2f", nval);
		}
		
		fprintf(newfile, "\n");	
	}

	fclose(file);
	fclose(newfile);
}

//LIST Method
void list (char* path, char* input, char* output, char* program){
    
    //Variable declaration
    DIR* dir;
	DIR* out_dir;

	char input_directory[PATH_MAX + NAME_MAX + 1];
	char output_directory[PATH_MAX + NAME_MAX + 1];

    
	sprintf(input_directory, "%s/%s", path, input);
	sprintf(output_directory, "%s/%s", path, output);

    //Directory opening error detection
	if ( (dir = opendir(input_directory)) == NULL )
	{
		perror(program);
		exit(-1);
	}
	
	if ( (out_dir = opendir(output_directory)) == NULL )
	{
		perror(program);
		exit(-1);
	}

    //Struct declaration
	struct dirent* direntry;
	struct stat info;

	char filename[PATH_MAX + NAME_MAX + 1], newfilename[PATH_MAX + NAME_MAX + 1];
	
	while ((direntry = readdir(dir)) != NULL)
	{
		if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0)
		{
			sprintf(filename, "%s/%s", input_directory, direntry->d_name);
			lstat(filename, &info);

			if (!S_ISDIR(info.st_mode))
			{
				sprintf(newfilename, "%s/%s", output_directory, direntry->d_name);
				normalize(filename, newfilename, program);
			}
		}
	}
}



//Main Function
int main (int argc, char* argv []){
    
    //Error amount arguments
    if (argc != 3)
	{
		printf("usage: %s input_directory output_directory\n", argv[0]);
		return -1;
	}

    //Variable Declaration
	char *input_directory, *output_directory;
	char path[NAME_MAX + 1];
    
    //Scans
	getcwd(path, NAME_MAX);
	input_directory = argv[1];
	output_directory = argv[2];

    //Run the program
	list(path, input_directory, output_directory, argv[0]);
    
    //END
	printf("Done.\n");

	return 0;
}