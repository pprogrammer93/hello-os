#define SECTOR_SIZE 512
#define DIRS_ENTRY_LENGTH 16
#define DIRS_SECTOR 0x101
#define FILES_SECTOR 0x102
#define SECTORS_SECTOR 0x103
//LS

void readSector(char * buffer, int sector);
void printString(char *str);

int main() {
	int i;
	int result;
	char dirs[SECTOR_SIZE];
	char files[SECTOR_SIZE];
	char curdir;
	char filename[15];
	char input[50];
	
	interrupt(0x21, 0x21, &curdir, 0, 0); //get curdir
	//ls
	readSector(dirs, DIRS_SECTOR);
	readSector(files, FILES_SECTOR);
	i = 0;
	while (i*DIRS_ENTRY_LENGTH < SECTOR_SIZE) {
		if (dirs[i*DIRS_ENTRY_LENGTH] == curdir && dirs[i*DIRS_ENTRY_LENGTH+1] != '\0') {
			printString("DIR: ");
			printString(dirs + i*DIRS_ENTRY_LENGTH + 1);
			printString("\r\n");
		}
		i++;
	}
	i = 0;
	while (i*DIRS_ENTRY_LENGTH < SECTOR_SIZE) {
		if (files[i*DIRS_ENTRY_LENGTH] == curdir && files[i*DIRS_ENTRY_LENGTH+1] != '\0') {
			printString("FILE: ");
			printString(files + i*DIRS_ENTRY_LENGTH + 1);
			printString("\r\n");
		}
		i++;
	}
	//terminate
	interrupt(0x21, 0x07, &result, 0, 0);
	return 0;
}

void readSector(char * buffer, int sector) {
	interrupt(0x21, 0x02, buffer, sector, 0);
}

void printString(char *str) {
	interrupt(0x21, 0x00, str, 0, 0);
}/**/