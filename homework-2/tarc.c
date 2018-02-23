#include "tarc.h"

int is_special_dot_file(char *filename)
{
	return !strncmp(filename, ".", strlen(filename)) || !strncmp(filename, "..", strlen(filename));
}


char* checksum(char *buffer)
{
	char *checksum_value = (char *) malloc(CHECKSUM_SIZE);
	int sum = 0;
	for (int i = 0; i < 500; i++) {
		sum += (int) buffer[i];
	}

	sprintf(checksum_value, "%06o", sum);
	checksum_value[6] = '\0';
	checksum_value[7] = ' ';
	return checksum_value;
}


int tar_header(int archive, char *filename, stat_t *stat_buf)
{
	char buffer[BLOCK_SIZE]; 
	memset(buffer, '\0', BLOCK_SIZE);

	memcpy(&buffer[NAME_OFFSET], filename, strlen(filename));
	sprintf(&buffer[MODE_OFFSET], "%07o", stat_buf->st_mode & 0777); 
	sprintf(&buffer[UID_OFFSET], "%07o", stat_buf->st_uid); 
	sprintf(&buffer[GID_OFFSET], "%07o", stat_buf->st_gid); 
	sprintf(&buffer[MTIME_OFFSET], "%011o", (int) stat_buf->st_mtime); 
	memcpy(&buffer[CHECKSUM_OFFSET], "        ", CHECKSUM_SIZE); 
	memcpy(&buffer[TYPE_OFFSET], filename, TYPE_SIZE); 

	if (S_ISREG(stat_buf->st_mode)) {
		buffer[TYPE_OFFSET] = REGULAR;
		sprintf(&buffer[SIZE_OFFSET], "%011o", (int) stat_buf->st_size); 
	}
	else if (S_ISLNK(stat_buf->st_mode)) {
		buffer[TYPE_OFFSET] = SYMLINK; 
		sprintf(&buffer[SIZE_OFFSET], "%011o", (int) 0); 
		readlink(filename, &buffer[LINKNAME_OFFSET], LINKNAME_SIZE);
	}
	else if (S_ISCHR(stat_buf->st_mode)) buffer[TYPE_OFFSET] = CHAR;
	else if (S_ISBLK(stat_buf->st_mode)) buffer[TYPE_OFFSET] = BLOCK;
	else if (S_ISDIR(stat_buf->st_mode)) { 
		sprintf(&buffer[SIZE_OFFSET], "%011o", (int) 0); 
		buffer[TYPE_OFFSET] = DIRECTORY;
	}
	else if (S_ISFIFO(stat_buf->st_mode)) buffer[TYPE_OFFSET] = FIFO;

	memcpy(&buffer[USTAR_OFFSET], "ustar", USTAR_SIZE);
	memcpy(&buffer[USTAR_V_OFFSET], "00", USTAR_V_SIZE);
	getlogin_r(&buffer[UNAME_OFFSET], UNAME_SIZE);
	struct group * grp = getgrgid(stat_buf->st_gid);	
	if (grp) {
		memcpy(&buffer[GNAME_OFFSET], grp->gr_name, strlen(grp->gr_name));
	}
	sprintf(&buffer[DEVMAJOR_OFFSET], "0000000");
	sprintf(&buffer[DEVMINOR_OFFSET], "0000000");
	// memcpy(&buf[PREFIX_OFFSET], filename, PREFIX_SIZE);

	char *checksum_value = checksum(buffer); 
	memcpy(&buffer[CHECKSUM_OFFSET], checksum_value, CHECKSUM_SIZE); 

	return write(archive, buffer, BLOCK_SIZE);
}


int tar_file(int archive, char *filename, stat_t *stat_buf) 
{ 
	char buf[CONTENT_SIZE];
	memset(buf, '\0', CONTENT_SIZE);
	// we should ignore symlinks and directories since they don't have contents
	if (S_ISLNK(stat_buf->st_mode) || S_ISDIR(stat_buf->st_mode)) {
		return write(archive, buf, CONTENT_SIZE); 
	}

	int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror(filename);
		exit(1);
	}

	int bytes_read;
	while ( (bytes_read = read(fd, buf, CONTENT_SIZE)) > 0) {
		if (write(archive, buf, CONTENT_SIZE) <= 0) {
			perror(filename);
			exit(1);
		} 
		memset(buf, '\0', CONTENT_SIZE);
	} 
	return 1;
}


char * append(char *pathname, char *filename)
{
	char * path = (char *) malloc(strlen(pathname) + strlen(filename));
	sprintf(path, "%s%s", pathname, filename);
	return path;
}


int tar_it(int archive, char *filename)
{ 
	stat_t *stat_buf = (stat_t *) malloc(sizeof(stat_t));  // get file info
	if (lstat(filename, stat_buf) == -1) {
		perror(filename);
		return errno;
	};

	int is_dir = S_ISDIR(stat_buf->st_mode);
	if (is_dir && filename[strlen(filename)-1] != '/') {  // add a backslash for dirs if needed
		filename = append(filename, "/");
	}

	tar_header(archive, filename, stat_buf);
	tar_file(archive, filename, stat_buf); 

	if (!is_dir) {   // stop recursing if we've reached a file
		return 1;
	}

	DIR *dir;
	if ( (dir = opendir(filename)) == NULL) {
		printf("error\n");
		return -1;
	}

	struct dirent *file; 
	while ((file = readdir(dir)) != NULL) {
		if (is_special_dot_file(file->d_name)) continue; 

		char * path = append(filename, file->d_name); // concat path with filename
		if (tar_it(archive, path) != 1) {
			printf("Error in: %s\n", filename);
			perror(file->d_name);
			return -1;
		};
		free(path);
	}
	return 1; 
}