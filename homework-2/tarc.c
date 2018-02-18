#include "tarc.h"

int is_special_dot_file(char *filename)
{
	return !strncmp(filename, ".", strlen(filename)) || !strncmp(filename, "..", strlen(filename));
}

int is_directory(int fd)
{
	return 0;
}

int tarc_open(tar_t *tar_ctx, char *filename)
{
	int fd = open(filename, O_RDWR | O_CREAT, 0644);
	if (fd == -1) {
		perror(filename);
		close(fd);
		return -1;
	}

	tar_ctx->file = fd;
	tar_ctx->current_dir = "";
	return 0;
}

int tarc_close(tar_t *context)
{
	return close(context->file);
}

int tar_it(tar_t *tar_ctx, char *filename) 
{
	struct stat *stat_buf = (struct stat *)malloc(sizeof(struct stat));
	if (stat(filename, stat_buf) == -1) {
		perror(filename);
		return errno;
	};

	if (!S_ISDIR(stat_buf->st_mode)) {
		printf("I tarred the file: %s %s\n", tar_ctx->current_dir, filename);
		return 0;
	}

	// int error = tar_directory(tar_ctx, filename);
	printf("I tar-ed the dir:  %s %s\n", tar_ctx->current_dir, filename);
	int error;
	// if (error != 0) {
	// 	return error;
	// }

	DIR *dir;
	if ( (dir = opendir(filename)) == NULL) {
		printf("error\n");
		return -1;
	}

	struct dirent *file;

	while ((file = readdir(dir)) != NULL) {
		if (is_special_dot_file(file->d_name)) continue; 

		tar_ctx->current_dir = filename;
		chdir(filename);
		if ((error = tar_it(tar_ctx, file->d_name)) != 0) {
			printf("Error in: %s\n", filename);
			perror(file->d_name);
			return -1;
		};
		chdir("..");
	}

	return 0;
}