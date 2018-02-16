#include <string.h>
#include <dirent.h> 
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef enum {
	true = 1,
	false = 0
} Bool;

Bool is_special_dot_file(char *filename) {
	return !strncmp(filename, ".", strlen(filename)) || !strncmp(filename, "..", strlen(filename));
}

Bool is_directory(int fd) {
	return false;
}

Bool tar_it(char *filename, int target_file) {
	int fd = open(filename, O_RDONLY);
	if (!is_directory(fd)) {
		return tar_file(fd, target_file);
	}

	tar_directory(fd, target_file);
	DIR *dir = opendir(fd);
	if (!dir) {
		printf("error\n");
		return true;
	}

	struct dirent *file;
	while ((file = readdir(dir)) != NULL) {
		tar_it(file, target_file);
	} 
}