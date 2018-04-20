
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

char equalString(char *str1, char  *str2, int str1length);
char findEntry(char *name, char *sector, char parentIndex);

int main() {
	int i, j;
	int result;
	char argc;
	char curdir;
	char argv[3][50];
	char dirs [512];
	char dirname[15];
	
	interrupt(0x21, 0x02, dirs, DIRS_SECTOR, 0); //read sector dirs
	interrupt(0x21, 0x21, &curdir, 0, 0); //getcurdir
	interrupt(0x21, 0x22, &argc, 0, 0); //getargc
	for (i=0; i<argc; ++i) {
		interrupt(0x21, 0x23, i, argv[i], 0);	//getargs
	}
	
	if (argc == 1 ){
		j = 0;
		for (i = 0; argv[0][i] != '\0'; i++) {
			//interrupt(0x21, 0x0, &argv[0][i], 0, 0); //debug
			if (argv[0][i] != '/') {
				dirname[j] = argv[0][i];
				j++;
			} else {
				dirname[j] = '\0';
				if (j == 2 && dirname[0] == '.' && dirname[1] == '.') {
					curdir = dirs[curdir * 16];
				} else {
					curdir = findEntry(dirname, dirs, curdir);
					if (curdir == NOT_FOUND) {
						break;
					}
				}
				j = 0;
			}
		}
		if (curdir != NOT_FOUND) {
			dirname[j] = '\0';
			if (dirname[0] == '.' && dirname[1] == '.') {
				curdir = dirs[curdir * 16];
			} else {
				curdir = findEntry(dirname, dirs, curdir);
			}
		}
		if (curdir != NOT_FOUND) {
			interrupt(0x21, 0x20, curdir, 0, 0); //putargs
		} else {
			interrupt(0x21, 0x0, "Direktori tidak ditemukan\r\n", 0, 0);
		}
	} else {
		interrupt(0x21, 0x0, "Invalid aruments\r\n", 0, 0);
	}
	interrupt(0x21, 0x07, &result, 0, 0);
}

char equalString(char *str1, char  *str2, int str1length) {
	int i = 0;	
	while (i < str1length && str2[i] != '\0' && str1[i] != '\0') {
		if (str1[i] != str2[i]) {
			return FALSE;
		}
		i++;
	}
	return (i > 0 && (str2[i] == str1[i] || i == str1length));
}


char findEntry(char *name, char *sector, char parentIndex) {
//mencari suatu entry pada dirs atau files dan mengembalikan indeks nya
	char i;
	char found;
	found = FALSE;
	i = 0;
	while (i*DIRS_ENTRY_LENGTH < SECTOR_SIZE && !found) {
		if (sector[i*DIRS_ENTRY_LENGTH] == parentIndex && equalString(sector + i*DIRS_ENTRY_LENGTH + 1, name, MAX_FILENAME)) {
			found = TRUE;
		} else {
			i++;
		}
	}
	//interrupt(0x21, 0x0, "\r\nsearching..\r\n", 0, 0);
	if (found) {
		//printString("TRUE");
		return i;
	} else {
		//printString("FALSE");
		return NOT_FOUND;
	}
}
