#define MAIN
#include "proc.h"
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

void sleep ();
void yieldControl ();
void pauseProcess (int segment, int *result);
void resumeProcess (int segment, int *result);
void killProcess (int segment, int *result);

void handleTimerInterrupt(int segment, int stackPointer);
void handleInterrupt21 (int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
int mod(int a, int b);
int div(int a, int b);

void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);

void readFile(char *buffer, char *path, int *result, char parentIndex);
void clear(char *buffer, int length);
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void executeProgram(char *path, int segment, int *success, char parentIndex);

char equalString(char *str1, char  *str2, int str1length);
int findEntry(char *name, char *sector);
void tracePath(char *path, char *filename, int *pathDirIndex, char parentIndex);

void terminateProgram (int *result);
void makeDirectory(char *path, int *result, char parentIndex);
void deleteFile(char *path, int *result, char parentIndex);
void deleteDirectory(char *path, int *success, char parentIndex);
void delFiles(char *files, char *sectors, char *map, char delIndex); //fungsi rekursif
void delDir(char *dirs, char *files, char *sectors, char *map, char delIndex); //fungsi rekursif
void putArgs (char curdir, char argc, char **argv);
void getCurdir (char *curdir);
void getArgc (char *argc);
void getArgv (char index, char *argv);
void handleTimerInterrupt(int segment, int stackPointer);
void yieldControl();
void sleep();
//void goToDir (char* path, int *success, char *curdir);

int main() {
	//char buffer[SECTOR_SIZE];
	//char *buffer2 = "ghjkl";
	int sect = 1;
	char success = FALSE;
	char *arg[2];
	arg[0] = "03";
	arg[1] = "01";
	//int sec = 1;
	//clearScreen();
	//displayLogo();
	//interrupt(0x16, 0, 0, 0, 0);

<<<<<<< HEAD
	//clearScreen();
	initializeProcStructures();
	makeInterrupt21();	
	makeTimerInterrupt();

=======
	initializeProcStructures();
	makeInterrupt21();
	makeTimerInterrupt();

	printString("Hello, there!\r\n");
>>>>>>> 8094065fd3bbc3743d40aaf8e644b93e052fed6d
	putArgs(0xFF, 0, arg);

	// interrupt(0x21, 0xFF << 8 | 0x06, "shell", 0x2000, &success);
	
	// makeDirectory("K301", &success, 0xFF);
	// makeDirectory("K301/in", &success, 0xFF);
	// writeFile("0zwxQji9","K301/in/code.txt", &sect, 0xFF);
	
	// putArgs(0xFF, 2, arg);
	// interrupt(0x21, 0xFF << 8 | 0x06, "keyproc2", 0x2000, &success);
	
	executeProgram("shell", 0x2000, &success, 0xFF);
	/*readFile(buffer, "key.txt", &success, 0xFF);
	
	
	printString("read done\r\n");
	if (success != SUCCESS) {
		printString("belum ada key.txt\r\n");	
  		executeProgram("keyproc2", 0x2000, &success, 0xFF);
	
	} else {	
		printString("Isi file key.txt :");
		printString(buffer);
	}/**/
	//printString("\r\n --Finished");
  while (1);
}

void handleTimerInterrupt(int segment, int stackPointer) {
  struct PCB *currPCB;
  struct PCB *nextPCB;
 
  setKernelDataSegment();
  currPCB = getPCBOfSegment(segment);
  currPCB->stackPointer = stackPointer;
  if (currPCB->state != PAUSED) {
    currPCB->state = READY;
    addToReady(currPCB);
  }
 
  do {
    nextPCB = removeFromReady();
  }  
  while (nextPCB != NULL && (nextPCB->state == DEFUNCT || nextPCB->state == PAUSED));

  if (nextPCB != NULL) {
    nextPCB->state = RUNNING;
    segment = nextPCB->segment;
    stackPointer = nextPCB->stackPointer;
    running = nextPCB;
  }
  else {
    running = &idleProc;
  }
  restoreDataSegment();
 
  returnFromTimer(segment, stackPointer);
}

void yieldControl () {
  interrupt(0x08, 0, 0, 0, 0);
}

void handleTimerInterrupt(int segment, int stackPointer) {
	struct PCB *currPCB;
	struct PCB *nextPCB;
	setKernelDataSegment();
	currPCB = getPCBOfSegment(segment);
	currPCB->stackPointer = stackPointer;
	if (currPCB->state != PAUSED) {
		currPCB->state = READY;
		addToReady(currPCB);
	}
	do {
		nextPCB = removeFromReady();
	}
	while (nextPCB != NULL && (nextPCB->state == DEFUNCT ||
		nextPCB->state == PAUSED));
		if (nextPCB != NULL) {
		nextPCB->state = RUNNING;
		segment = nextPCB->segment;
		stackPointer = nextPCB->stackPointer;
		running = nextPCB;
	}
	else {
		running = &idleProc;
	}
	restoreDataSegment();
	returnFromTimer(segment, stackPointer);
}


void handleInterrupt21 (int AX, int BX, int CX, int DX) {
  char AL, AH;
  AL = (char) (AX);
  AH = (char) (AX >> 8);

  switch (AL) {
    case 0x00:
      printString(BX);
      break;
    case 0x01:
      readString(BX);
      break;
    case 0x02:
      readSector(BX, CX);
      break;
    case 0x03:
      writeSector(BX, CX);
      break;
    case 0x04:
      readFile(BX, CX, DX, AH);
      break;
    case 0x05:
      writeFile(BX, CX, DX, AH);
      break;
    case 0x06:
      executeProgram(BX, CX, DX, AH);
      break;
    case 0x07:
      terminateProgram(BX);
      break;
    case 0x08:
      makeDirectory(BX, CX, AH);
      break;
    case 0x09:
      deleteFile(BX, CX, AH);
      break;
    case 0x0A:
      deleteDirectory(BX, CX, AH);
      break;
    case 0x20:
      putArgs(BX, CX, DX);
      break;
    case 0x21:
      getCurdir(BX);
      break;
    case 0x22:
      getArgc(BX);
      break;
    case 0X23:
      getArgv(BX, CX);
      break;
<<<<<<< HEAD
    case 0x30:
    	yieldControl();
    	break;
    case 0x31:
    	sleep();
    	break;
    case 0x32:
    	pauseProcess(BX, CX);
    	break;
    case 0x33:
    	resumeProcess(BX, CX);
    	break;
    case 0x34:
    	killProcess(BX, CX);
    	break;
=======
     case 0x30:
     	yieldControl();
     	break;
     case 0x31:
     	sleep();
     	break;
>>>>>>> 8094065fd3bbc3743d40aaf8e644b93e052fed6d
    //case 0x50:
      //goToDir(BX, CX, DX);
    default:
      printString("Invalid interrupt");
  }
}

void sleep () {
  setKernelDataSegment();
  running->state = PAUSED;
  restoreDataSegment();  
  yieldControl();
}


void pauseProcess (int segment, int *result) {
  struct PCB *pcb;
  int res;
   
  setKernelDataSegment();
  pcb = getPCBOfSegment(segment);
  if (pcb != NULL && pcb->state != PAUSED) {
    pcb->state = PAUSED;
    res = SUCCESS;
  }
  else {
    res = NOT_FOUND;
  }
  restoreDataSegment();
   
  *result = res;
}


void resumeProcess (int segment, int *result) {
  struct PCB *pcb;
  int res;
   
  setKernelDataSegment();
  pcb = getPCBOfSegment(segment);
  if (pcb != NULL && pcb->state == PAUSED) {
    pcb->state = READY;
    addToReady(pcb);
    res = SUCCESS;
  }
  else {
    res = NOT_FOUND;
  }
  restoreDataSegment();
   
  *result = res;
}

void killProcess (int segment, int *result) {
  struct PCB *pcb;
  int res;
   
  setKernelDataSegment();
  pcb = getPCBOfSegment(segment);
  if (pcb != NULL) {
    releaseMemorySegment(pcb->segment);
    releasePCB(pcb);
    res = SUCCESS;
  }
  else {
    res = NOT_FOUND;
  }
  restoreDataSegment();
   
  *result = res;
}



void putArgs (char curdir, char argc, char **argv) {
  char args[SECTOR_SIZE];
  int i, j, p;
  clear(args, SECTOR_SIZE);

  args[0] = curdir;
  args[1] = argc;
  i = 0;
  j = 0;
  for (p = 2; p < ARGS_SECTOR && i < argc; ++p) {
    args[p] = argv[i][j];
    if (argv[i][j] == '\0') {
      ++i;
      j = 0;
    }
    else {
      ++j;
    }
  }
   
  writeSector(args, ARGS_SECTOR);
}
 
void getCurdir (char *curdir) {
  char args[SECTOR_SIZE];
  readSector(args, ARGS_SECTOR);
  *curdir = args[0];
}


void getArgc (char *argc) {
  char args[SECTOR_SIZE];
  readSector(args, ARGS_SECTOR);
  *argc = args[1];
}
 
void getArgv (char index, char *argv) {
  char args[SECTOR_SIZE];
  int i, j, p;
  readSector(args, ARGS_SECTOR);
   
  i = 0;
  j = 0;
  for (p = 2; p < ARGS_SECTOR; ++p) {
    if (i == index) {
      argv[j] = args[p];
       ++j;
    }
    if (args[p] == '\0') {
      if (i == index) {
        break;
       }
      else {
        ++i;
       }
    }
  }
}



void printString(char *string) {
	int i;
	char c;
	i = 0;
	while (string[i] != '\0') { 	
		c = string[i];	
		interrupt(0x10, 0xE00 + c, 0, 0, 0);
		i++;
	}
}

void readString(char *string) {
	int i;
	char c;
	i = 0;
	c = interrupt(0x16, 0, 0, 0, 0);
	while (c != '\r') {
		if (c == '\b' && i > 0) {
			i--;
			string[i] = '\0';
			interrupt(0x10, 0xE00 + '\b', 0, 0, 0);
			interrupt(0x10, 0xE00 + '\0', 0, 0, 0);
			interrupt(0x10, 0xE00 + '\b', 0, 0, 0);
		} else {
			interrupt(0x10, 0xE00 + c, 0, 0, 0);
			string[i] = c;
			i++;
		}
		c = interrupt(0x16, 0, 0, 0, 0);
	}
	string[i] = '\0';
	interrupt(0x10, 0xE00 + '\r', 0, 0, 0); //newline	
	interrupt(0x10, 0xE00 + '\n', 0, 0, 0); //newline
}

int mod(int a, int b) {
  while(a >= b){
    a = a - b;
  }
  return a;
}

int div(int a, int b) {
  int q = 0;
  while(q*b <=a) {
    q = q+1;
  }
  return q-1;
}

void readSector(char *buffer, int sector){
	interrupt(0x13, 0x201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}
void writeSector(char *buffer, int sector){
	interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

int findEntry(char *name, char *sector, char parentIndex) {
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
	if (found) {
		//printString("TRUE");
		return i;
	} else {
		//printString("FALSE");
		return NOT_FOUND;
	}
}

void tracePath(char *path, char *filename, int *pathDirIndex, char parentIndex) {
	// input: path dan index direktori awal
	// output: nama file dan indeks direktori di ujung path
	int i,j;
	int entry_index;
	char dirs[SECTOR_SIZE];
	readSector(dirs, DIRS_SECTOR);
	entry_index = parentIndex;
	i = 0;
	j = 0;
	while (path[i] != '\0') {
		if (path[i] == '/') {
			filename[j] = '\0';
			j = 0;
			//printString(filename);
			//printString("/\r\n");
			entry_index = findEntry(filename, dirs, entry_index);
			if (entry_index == NOT_FOUND) {
				filename[0] = '\0';
				*pathDirIndex = NOT_FOUND;
				return;
			} 
		} else {
			filename[j] = path[i];
			j++;
		}
		i++;
	}
	filename[j] = '\0';
	*pathDirIndex = entry_index;

}


void readFile(char *buffer, char *path, int *result, char parentIndex){
	int i, j;	
	int entry_index;
	int file_index;
	char sector[SECTOR_SIZE];
	char temp[MAX_FILENAME];

	tracePath(path, temp, &entry_index, parentIndex); //temp = nama file di ujung path, entry_index = idx direktori file
	if (entry_index == NOT_FOUND) {
		*result = NOT_FOUND;
		return;
	}

	//printString("\r\nFile: ");//debug
	//printString(temp);


	readSector(sector, FILES_SECTOR); //sector = files
	file_index = findEntry(temp, sector, entry_index); //mencari file dengan nama = temp
	//printString(temp);
	clear(sector, SECTOR_SIZE);
	if (file_index == NOT_FOUND) {
		//printString("NF\r\n");
		*result = NOT_FOUND;
		return;
	} else {
		//printString("F\r\n");
		*result = SUCCESS;
		readSector(sector, SECTORS_SECTOR); //sector = sectors
		//printString("test");
		i = file_index * DIRS_ENTRY_LENGTH;
		//printString("\r\nF2");
		j = 0;
		while (sector[i] != EMPTY && j < MAX_SECTORS) {
			//printString("\r\nxy");
			readSector(buffer + (j * SECTOR_SIZE), sector[i]);
			//printString("z");  
			i++; 
			j++;
		}
		//printString("\r\ndone");
		return;
	}	
}

void clear(char *buffer, int length){
  int i;
  for(i = 0; i < length; ++i){
    buffer[i] = EMPTY;
  }
}

void writeFile(char *buffer, char *path, int *sectors, char parentIndex){
	char map[SECTOR_SIZE];
	char files[SECTOR_SIZE];
	char sectors[SECTOR_SIZE];
	char sectorBuffer[SECTOR_SIZE];
	char temp[MAX_FILENAME];
	int dirIndex;
	int fileIndex;
	int i, j, sectorCount;

	readSector(map, MAP_SECTOR);
	readSector(files, FILES_SECTOR);
	readSector(sectors, SECTORS_SECTOR);

	//mengecek kecukupan jumlah sector
	for (i = 0, sectorCount = 0; i < MAX_BYTE && sectorCount < *sectors; ++i) {
	  if (map[i] == EMPTY) {
	    ++sectorCount;
	  }
	}
	if (sectorCount < *sectors || *sectors > MAX_SECTORS) {
	  *sectors = INSUFFICIENT_SECTORS;
	  return;
	}

	//mengecek entry file yang boleh ditimpa
	for (fileIndex = 0; fileIndex < MAX_FILES; ++fileIndex) {
		if (files[fileIndex * DIRS_ENTRY_LENGTH + 1] == '\0') {
		  break;
		}
	}
	if (fileIndex >= MAX_FILES) {
		*sectors = INSUFFICIENT_ENTRIES;
		return;
	}

	//mengecek validitas path
	tracePath(path, temp, &dirIndex, parentIndex);
	if (dirIndex == NOT_FOUND) {
		*sectors = NOT_FOUND;
		return;
	} 

	//mengecek file bernama sama di direktori tersebut
	if (findEntry(temp, files, dirIndex) != NOT_FOUND) {
	 	*sectors = ALREADY_EXISTS;
	 	return;
	};

	 // memulai penulisan: menulis nama di sector files
	clear(files + fileIndex * DIRS_ENTRY_LENGTH, DIRS_ENTRY_LENGTH);
	files[fileIndex * DIRS_ENTRY_LENGTH] = dirIndex;

	for (i = 0; i < MAX_FILENAME; ++i) {
	    if (temp[i] != '\0') {
	      files[fileIndex * DIRS_ENTRY_LENGTH + 1 + i] = temp[i];
	    }
	    else {
	      break;
	    }
	  }
	
	// penulisan sectors di sector sectors
	  clear(sectors + fileIndex * DIRS_ENTRY_LENGTH, DIRS_ENTRY_LENGTH);
	  for (i = 0, sectorCount = 0; i < MAX_BYTE && sectorCount < *sectors; ++i) {
	    if (map[i] == EMPTY) {
	      map[i] = USED;
	      sectors[fileIndex * DIRS_ENTRY_LENGTH + sectorCount] = i;
	      clear(sectorBuffer, SECTOR_SIZE);
	      for (j = 0; j < SECTOR_SIZE; ++j) {
	      	
	        sectorBuffer[j] = buffer[sectorCount * SECTOR_SIZE + j];
	      }
	      //printString("\r\nwritten to file : ");
	      	//printString(sectorBuffer); //debug
	      writeSector(sectorBuffer, i);
	      ++sectorCount;
	    }        
	  }

	writeSector(map, MAP_SECTOR);
	writeSector(files, FILES_SECTOR);
	writeSector(sectors, SECTORS_SECTOR);
	return;
}


void executeProgram(char *path, int segment, int *success, char parentIndex){
	char buffer[MAX_SECTORS * SECTOR_SIZE];
	readFile(buffer, path, success, parentIndex);
	if (*success == SUCCESS) {
		int i;
		
		for (i = 0; i < MAX_SECTORS * SECTOR_SIZE; i++) {
			putInMemory(segment, i, buffer[i]);
		} 
		launchProgram(segment);
	}
	 
}

void terminateProgram (int *result) {
  char shell[6];
  shell[0] = 's';
  shell[1] = 'h';
  shell[2] = 'e';
  shell[3] = 'l';
  shell[4] = 'l';
  shell[5] = '\0';
  executeProgram(shell, 0x2000, result, 0xFF);
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


void makeDirectory(char *path, int *result, char parentIndex) {
	char dirs[SECTOR_SIZE];
	char dirName[MAX_DIRNAME];
	int dirIndex;
	char makeIndex;
	int i;

	tracePath(path, dirName, &dirIndex, parentIndex);
	if (dirIndex == NOT_FOUND) {
		*result = NOT_FOUND;
		return;
	}
	readSector(dirs, DIRS_SECTOR);
	// mengecek dirs kosong untuk dijadikan indeks dir baru
	for (makeIndex = 0; makeIndex < MAX_FILES; makeIndex++) {
		if (dirs[makeIndex * DIRS_ENTRY_LENGTH + 1] == '\0') {
			break;
		}
	}
	if (makeIndex >= MAX_FILES) {
		*result = INSUFFICIENT_ENTRIES;
		return;
	}

	//memastikan dir unik
	if (findEntry(dirName, dirs, dirIndex) != NOT_FOUND) {
		*result = ALREADY_EXISTS;
		return;
	} 
	clear(dirs + makeIndex * DIRS_ENTRY_LENGTH, DIRS_ENTRY_LENGTH);
	dirs[makeIndex * DIRS_ENTRY_LENGTH] = dirIndex;
	for(i = 0; i < MAX_DIRNAME && dirName[i] != '\0'; i++) {
		dirs[makeIndex * DIRS_ENTRY_LENGTH + 1 + i] = dirName[i];
	}
	writeSector(dirs, DIRS_SECTOR);
	*result = SUCCESS;
	return;

}

void deleteFile(char *path, int *result, char parentIndex) {
	int i;
	char files[SECTOR_SIZE];
	char filename[MAX_FILENAME];
	int fileIndex;
	char map[SECTOR_SIZE];
	char sectors[SECTOR_SIZE];
	char dirIndex;

	readSector(files, FILES_SECTOR);
	readSector(map, MAP_SECTOR);
	readSector(sectors, SECTORS_SECTOR);

	tracePath(path, filename, &dirIndex, parentIndex);
	if (dirIndex == NOT_FOUND) {
		*result = NOT_FOUND;
		return;
	}

	fileIndex = findEntry(filename, files, dirIndex);
	if (fileIndex == NOT_FOUND) {
		*result = NOT_FOUND;
		return;
	}

	files[fileIndex * DIRS_ENTRY_LENGTH + 1] = '\0';
	for (i = 0; i < DIRS_ENTRY_LENGTH; i++) {
		int temp = sectors[fileIndex * DIRS_ENTRY_LENGTH + i]; 
		if (temp != '\0') {
			map[temp] = EMPTY;
		} else {
			break;
		}
	}

	writeSector(files, FILES_SECTOR);
	writeSector(map, MAP_SECTOR);
}

void delFiles(char *files, char *sectors, char *map, char delIndex) {
	int i,j;
	for (i = 0; i < MAX_FILES; i++) {
		if (files[i * DIRS_ENTRY_LENGTH] == delIndex) {
			files[i * DIRS_ENTRY_LENGTH + 1] = '\0';
			for (j = 0; j < DIRS_ENTRY_LENGTH; j++) {
				char temp = sectors[delIndex * DIRS_ENTRY_LENGTH + j]; 
				if (temp != '\0') {
					map[temp] = EMPTY;
				} else {
					break;
				}
			}
		}
	}
}

void delDir(char *dirs, char *files, char *sectors, char *map, char delIndex) {
	int i;
	dirs[delIndex*DIRS_ENTRY_LENGTH + 1] = '\0';
	delFiles(files, sectors, map, delIndex);
	for (i = 0; i < MAX_FILES; i++) {
		if (dirs[i * DIRS_ENTRY_LENGTH] == delIndex) {
			delDir(dirs, files, sectors, map, i);
		}
	}

}

void deleteDirectory(char *path, int *success, char parentIndex) {
	int i;
	char dirs[SECTOR_SIZE];
	char map[SECTOR_SIZE];
	char sectors[SECTOR_SIZE];
	char files[SECTOR_SIZE];
	char dirname[MAX_FILENAME];
	int delIndex;
	int dirIndex;

	readSector(files, FILES_SECTOR);
	readSector(map, MAP_SECTOR);
	readSector(dirs, DIRS_SECTOR);
	readSector(sectors, SECTORS_SECTOR);

	tracePath(path, dirname, &dirIndex, parentIndex);
	if (dirIndex == NOT_FOUND) {
		*success = NOT_FOUND;
		return;
	}
	delIndex = findEntry(dirname, dirs, dirIndex);
	if (delIndex == NOT_FOUND) {
		*success = NOT_FOUND;
		return;
	}

	delDir(dirs, files, sectors, map, delIndex);

	writeSector(files, FILES_SECTOR);
	writeSector(map, MAP_SECTOR);
	writeSector(dirs, DIRS_SECTOR);
	*success = SUCCESS;
}

void yieldControl() {
	interrupt(0x08, 0, 0, 0, 0);
}

void sleep() {
	setKernelDataSegment();
	running->state = PAUSED;
	restoreDataSegment();
	yieldControl();
}