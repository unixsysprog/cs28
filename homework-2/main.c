#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tarc.c"

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("please provide a directory name and target files\n");
		return 1;
	}

	int file_count = argc - 2;
	char **files = (char**) &argv[2];

	char *archive_name = argv[1];
	int target_file = open(archive_name, O_RDWR|O_CREAT, 0644);
	if (target_file == -1) {
		printf("unable to create file: %s\n", archive_name);
		return 2;
	} 

	for (int i = 0; i < file_count; i++) { 
		if (!tar_it(files[i], target_file)) {
            printf("cannot tar file\n");
            return 3;
        }
	}

	return 0;
} 

