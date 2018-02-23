#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tarc.h"

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("please provide a directory name and target files\n");
		return 1;
	}

	int file_count = argc - 2;
	char **files = &argv[2];

	char *archive_name = argv[1];
	tar_t *tar_ctx = (tar_t *)malloc(sizeof(tar_t));
	tar_ctx->current_dir = "";

	if (tarc_open(tar_ctx, archive_name) != 0) {
		return 2;
	}

	for (int i = 0; i < file_count; i++) {
		if (tar_it(tar_ctx, files[i]) == 0) {
			printf("cannot tar file\n");
			break;
		}
	}

	tarc_close(tar_ctx);
	free(tar_ctx);
	return 0;
}