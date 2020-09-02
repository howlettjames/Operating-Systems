#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	struct stat file_status;
	char file_name[FILENAME_MAX];

	printf("Enter the filename: ");
	scanf("%s", file_name);

	//OBTENEMOS EL STATUS DEL ARCHIVO
	if((stat(file_name, &file_status)) == -1)
	{
		printf("Error: Cannot open file status.\n");
		exit(EXIT_FAILURE);
	}

	//IMPRIMIMOS INFORMACIÓN ACERCA DEL ARCHIVO
	printf("ID of device containing the file: %ld\n", file_status.st_dev);
	printf("File type: ");
	switch(file_status.st_mode & S_IFMT)
	{
		case S_IFBLK: printf("block device\n"); break;
		case S_IFCHR: printf("character device\n"); break;
		case S_IFDIR: printf("directory\n"); break;
		case S_IFIFO: printf("FIFO/pipe\n"); break;
		case S_IFLNK: printf("symlink\n"); break;
		case S_IFREG: printf("regular file\n"); break;
		case S_IFSOCK: printf("socket\n"); break;
		default: printf("unknown?\n"); break;
	}
	printf("I-node number: %ld\n", (long) file_status.st_ino);
	printf("Mode: %lo (octal)\n", (unsigned long) file_status.st_mode);
	printf("Link count: %ld\n", (long) file_status.st_nlink);
	printf("Ownership: UID=%ld   GID=%ld\n", (long) file_status.st_uid, (long) file_status.st_gid);
	printf("Preferred I/O block size: %ld bytes\n", (long) file_status.st_blksize);
	printf("File size: %lld bytes\n", (long long) file_status.st_size);
	printf("Blocks allocated: %lld\n", (long long) file_status.st_blocks);
	printf("Last status change: %s", ctime(&file_status.st_ctime));
	printf("Last file access: %s", ctime(&file_status.st_atime));
	printf("Last file modification: %s", ctime(&file_status.st_mtime));

	return 0;
}