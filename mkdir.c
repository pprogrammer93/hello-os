
#define MAX_BYTE 256
#define SECTOR_SIZE 512
#define MAX_FILES 32
#define MAX_FILENAME 15
#define MAX_SECTORS 16
#define DIRS_ENTRY_LENGTH 16
#define MAX_DIRNAME 15
#define MAP_SECTOR 0x100
#define DIRS_SECTOR 0x101
#define FILES_SECTOR 0x102
#define SECTORS_SECTOR 0x103
#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define INSUFFICIENT_SECTORS 0
#define NOT_FOUND -1
#define ALREADY_EXISTS -2
#define INSUFFICIENT_ENTRIES -3
#define EMPTY 0x00
#define USED 0xFF
#define ROOT 0xFF
#define ARGS_SECTOR 512


int main() {
	int i;
	int result;
	char argc;
	char curdir;
	char argv[4][20];
	char input[50];
	
	interrupt(0x21, 0x21, &curdir, 0, 0); //getcurdir
	interrupt(0x21, 0x22, &argc, 0, 0); //getargc
	for (i=0; i<argc; ++i) {
		interrupt(0x21, 0x23, i, argv[i], 0);	//getargs
	}
	if (argc == 1) {
		//makeDirectory
		interrupt(0x21,curdir << 8 | 0x08, argv[0], &result, 0);
	}

	//terminate
	interrupt(0x21, 0x07, &result, 0, 0);
	return 0;
}

