#define FILES_SECTOR 0x102
#define FILES_ENTRY_LENGTH 16
#define SECTOR_SIZE 512

#include "proc.h"

char* getFileName(char index);
void readSector(char * buffer, int sector);
void printString(char *str);

int main() {
	char idx;
	char idxStr[2];
	int status;
	struct PCB *currPCB = running;

	printString("PS: \n\r");
	while (currPCB != NULL) {
		idx = currPCB->index;
		printString("Process Name: ");
		printString(getFileName(idx));

		idxStr[0] = idx; idxStr[1] = '\0';
		printString("\n\rPID: ");
		printString(idxStr); printString("\n\r");

		status = currPCB->state;
		if (status==0)
			printString("DEFUNC");
		else if (status==1)
			printString("RUNNING");
		else if (status==2)
			printString("STARTING");
		else if (status==3)
			printString("READY");
		else
			printString("PAUSED");
		printString("\n\r\n\r");
		currPCB = currPCB->next;
	}
	return 0;
}

char* getFileName(char index) {
	char filename[15];
	char files[SECTOR_SIZE];
	int i;
	int j;
	int k;
	readSector(files, FILES_SECTOR);
	i = 0;	j = 0;  k = 0;
	while (i*FILES_ENTRY_LENGTH < SECTOR_SIZE) {
		if (files[i*FILES_ENTRY_LENGTH] == index) {
			j = i*FILES_ENTRY_LENGTH+1;
			while (files[i] != '\0') {
				filename[k] = files[i];
				j++; k++;
			}
		} else {
			i++;
		}
	}
	filename[k] = '\0';
	return filename;
}

void readSector(char * buffer, int sector) {
	interrupt(0x21, 0x02, buffer, sector, 0);
}

void printString(char *str) {
	interrupt(0x21, 0x00, str, 0, 0);
}
