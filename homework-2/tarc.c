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
	return 0;
}

int tarc_close(tar_t *context)
{
	return close(context->file);
}


void tarc_chdir(tar_t *context, char *dir)
{
	char * curr_dir = context->current_dir;
	if (is_special_dot_file(dir)) {
		int len = strlen(curr_dir) - 1;

		if (curr_dir[len] == '/') {
			curr_dir[len] = '\0';
		}

		while (len >= 0) {
			if (curr_dir[len] == '/') {
				break;
			}
			curr_dir[len--] = '\0';
		} 
	}
	else if( curr_dir == NULL || strlen(curr_dir) == 0 ) {
		asprintf(&context->current_dir, "%s/", dir);
	} else { 
		asprintf(&context->current_dir, "%s%s", curr_dir, dir);
	} 
	chdir(dir);
}

int sum_field(char * field)
{
	int sum = 0;
	for (int i = 0, len = strlen(field); i < len; i++) {
		sum += (char) field[i];
	}
	return sum;
}

void checksum(tar_header_t *tar_header)
{
	int sum = 0;
	sum += sum_field(tar_header->name);
	sum += sum_field(tar_header->mode);
	sum += sum_field(tar_header->uid);
	sum += sum_field(tar_header->gid);
	sum += sum_field(tar_header->size);
	sum += sum_field(tar_header->mtime);
	sum += sum_field(tar_header->checksum);
	sum += sum_field(&tar_header->type);
	sum += sum_field(tar_header->linkname);
	sum += sum_field(tar_header->ustar);
	sum += sum_field(tar_header->uname);
	sum += sum_field(tar_header->gname);
	sum += sum_field(tar_header->devmajor);
	sum += sum_field(tar_header->devminor);
	sum += sum_field(tar_header->prefix);
	// sprintf(tar_header->checksum, "%014010\0 ");
	sprintf(tar_header->checksum, "%06o", sum);
	tar_header->checksum[6] = '\0';
	tar_header->checksum[7] = ' ';
}

void get_file_stats(tar_header_t *tar_header, struct stat* stat) {
	// printf("dev:  %d\n", (int)s->st_dev);
	// printf("file? %d\n", S_ISREG(s->st_mode));
	// printf("mode: %lo\n", (long)s->st_mode);
	// printf("uid:  %ld\n", (long)s->st_uid);
	// printf("gid:  %ld\n", (long)s->st_gid);
	// printf("size: %ld\n", s->st_size);
	// printf("mod:  %ld\n", s->st_mtime); 
	if (S_ISREG(stat->st_mode)) tar_header->type = REGULAR;
	else if (S_ISLNK(stat->st_mode)) tar_header->type = SYMLINK;
	else if (S_ISCHR(stat->st_mode)) tar_header->type = CHAR;
	else if (S_ISBLK(stat->st_mode)) tar_header->type = BLOCK;
	else if (S_ISDIR(stat->st_mode)) tar_header->type = DIRECTORY;
	else if (S_ISFIFO(stat->st_mode)) tar_header->type = FIFO;

	sprintf(tar_header->mode, "%07o", stat->st_mode & 0777);
	sprintf(tar_header->uid, "%07o",  stat->st_uid);
	sprintf(tar_header->gid, "%07o",  stat->st_gid);
	printf("dir type: %c \n", tar_header->type);
	if (tar_header->type == DIRECTORY) { 
    	sprintf(tar_header->size, "00000000000");
	} else { 
    	sprintf(tar_header->size, "%011o", (int) stat->st_size);
	}
    sprintf(tar_header->mtime, "%011o", (int) stat->st_mtime);
    memcpy(tar_header->ustar, "ustar\000", 8);
	memcpy(tar_header->checksum, "        ", 8);
	getlogin_r(tar_header->uname, 32);

	struct group * grp = getgrgid(stat->st_gid);	
	if (grp) strncpy(tar_header->gname, grp->gr_name, 32);


	checksum(tar_header);
}


int write_file(tar_t *tar_ctx, char *filename)
{ 
	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror(filename);
		exit(1);
	}

	char buf[BLOCK_SIZE];
	memset(buf, '\0', BLOCK_SIZE);

	int bytes_read;
	while ( (bytes_read = read(fd, buf, BLOCK_SIZE)) > 0) {
		if (write(tar_ctx->file, buf, BLOCK_SIZE) <= 0) {
			perror(filename);
			exit(1);
		} 
		memset(buf, '\0', BLOCK_SIZE);
	} 
	return 1;
}


int write_header(tar_t *tar_ctx, char *filename, struct stat *stat_buf) 
{
	tar_header_t *tar_header = (tar_header_t *) malloc(sizeof(tar_header_t)); 

	char name[100];
	memset(name, '\0', 100);
	if (tar_ctx->current_dir == NULL || strlen(tar_ctx->current_dir) == 0) {
		sprintf(name, "%s", filename);
	} else {
		sprintf(name, "%s%s", tar_ctx->current_dir, filename);
	}

	// if (S_ISDIR(stat_buf->st_mode)) {
	// 	sprintf(tar_header->name, "%s/", name);
	// } else { 
		memcpy(tar_header->name, name, sizeof(name));
	// }
	printf("header name: %s\n", tar_header->name);
	get_file_stats(tar_header, stat_buf);

	char buf[BLOCK_SIZE];
	memset(buf, '\0', BLOCK_SIZE);

	int offset = 0;
	memcpy(&buf[offset], tar_header->name, sizeof(tar_header->name));
	offset += sizeof(tar_header->name);

	memcpy(&buf[offset], tar_header->mode, sizeof(tar_header->mode));
	offset += sizeof(tar_header->mode);

	memcpy(&buf[offset], tar_header->uid, sizeof(tar_header->uid));
	offset += sizeof(tar_header->uid);

	memcpy(&buf[offset], tar_header->gid, sizeof(tar_header->gid));
	offset += sizeof(tar_header->gid);

	memcpy(&buf[offset], tar_header->size, sizeof(tar_header->size));
	offset += sizeof(tar_header->size);

	memcpy(&buf[offset], tar_header->mtime, sizeof(tar_header->mtime));
	offset += sizeof(tar_header->mtime);

	memcpy(&buf[offset], tar_header->checksum, sizeof(tar_header->checksum));
	offset += sizeof(tar_header->checksum);

	memcpy(&buf[offset], &tar_header->type, sizeof(tar_header->type));
	offset += sizeof(tar_header->type);

	memcpy(&buf[offset], tar_header->linkname, sizeof(tar_header->linkname));
	offset += sizeof(tar_header->linkname);

	memcpy(&buf[offset], tar_header->ustar, sizeof(tar_header->ustar));
	offset += sizeof(tar_header->ustar);

	memcpy(&buf[offset], tar_header->uname, sizeof(tar_header->uname));
	offset += sizeof(tar_header->uname);

	memcpy(&buf[offset], tar_header->gname, sizeof(tar_header->gname));
	offset += sizeof(tar_header->gname);

	// int sum = 0;
	// for (int i = 0; i < 512; i++) {
	// 	sum += (char) buf[i];
	// }
	// printf("checksum: %06o\n", sum);
	// printf("tar_header checksum: %s\n", tar_header->checksum);
	free(tar_header);
	return write(tar_ctx->file, buf, BLOCK_SIZE);
}

int tar_file(tar_t *tar_ctx, char *filename, struct stat *stat_buf)
{
	write_header(tar_ctx, filename, stat_buf);
	write_file(tar_ctx, filename);
	return 1;
}


int tar_directory(tar_t *tar_ctx, char *filename, struct stat *stat_buf)
{ 
	write_header(tar_ctx, filename, stat_buf);
	return 1;
}

int tar_it(tar_t *tar_ctx, char *filename) 
{

	struct stat *stat_buf = (struct stat *)malloc(sizeof(struct stat));
	if (stat(filename, stat_buf) == -1) {
		perror(filename);
		return errno;
	};

	if (!S_ISDIR(stat_buf->st_mode)) {
		if(tar_file(tar_ctx, filename, stat_buf) != 1) {
			perror(filename);
			return 1;
		}
		return 0;
	}
	if (filename[strlen(filename)-1] != '/') {
		asprintf(&filename, "%s/", filename);
	}
	printf("filename: %s\n", filename);

	// int error = tar_directory(tar_ctx, filename);
	if (tar_directory(tar_ctx, filename, stat_buf) != 1) {
		perror(filename);
		return 1; 
	}

	DIR *dir;
	if ( (dir = opendir(filename)) == NULL) {
		printf("error\n");
		return -1;
	}

	struct dirent *file; 
	int error;
	while ((file = readdir(dir)) != NULL) {
		if (is_special_dot_file(file->d_name)) continue; 

		tarc_chdir(tar_ctx, filename);

		if ((error = tar_it(tar_ctx, file->d_name)) != 0) {
			printf("Error in: %s\n", filename);
			perror(file->d_name);
			return -1;
		};
	
		tarc_chdir(tar_ctx, "..");
	}

	return 0;
}

// For the rewrite -- should implement tar_it to just tar a single file
// then add directory recursion after
// tar_it(filname) -> allocate the buffer, lstat file, set_[field] x 13
// for recursion, don't chdir just append the path on each iteration.
// write an append function? allocate buffer, append, tar_it, free
// handle errors during rewrite. 