int equalString(char *str1, char *str2, int str1len);
void readString(char *string, int disableProcessControls);
void printString(char *string);
void parseInput(char * input, char **output);

int main() {
	int i;
	char curdir;
	char argc;
	int isParallel;
	int success;
	char argv[20][20];
	char input[100];
	char * command[20];
	enableInterrupts();
	while (1) {
	interrupt(0x21, 0x00, "$ ", 0, 0);

	interrupt(0x21, 0x21, &curdir, 0, 0); //get curdir

	
	readString(input, 1);
	parseInput(input, command);

	interrupt(0x21, 0x22, &argc, 0, 0); //getargc
	interrupt(0x21, 0x23, argc-1, argv[0], 0);	//getargs
	isParallel = argv[0][0] == '&';
	
	if (command[0][0] == '.' && command[0][1] == '/') {
		interrupt(0x21, curdir << 8 | 0x6, &command[0][2], isParallel, &success); //execute dari curdir
	} else if (equalString(command[0], "resume")) {
		interrupt(0x21, 0x33, command[1][0]-'0', &success, 0);
	} else if (equalString(command[0], "pause"))  {
		interrupt(0x21, 0x32, command[1][0]-'0', &success, 0);
	} else if (equalString(command[0], "kill"))  {
		interrupt(0x21, 0x34, command[1][0]-'0', &success, 0);	
	} else if (equalString(command[0], "ps")){
		interrupt(0x21, 0x35, 0, 0, 0);
	} else {
		interrupt(0x21, 0xFF << 8 | 0x6, command[0], isParallel, &success);
	}
	

	/*
	if (equalString(command[0], "echo")) {
		interrupt(0x21, 0xFF << 8 | 0x6, "echo", 0, &success); //execute dari root
	} else if (equalString(command[0], "mkdir")) {
		interrupt(0x21, 0xFF << 8 | 0x6, "mkdir", 0, &success);
	} else if (equalString(command[0], "ls")) {
		interrupt(0x21, 0xFF << 8 | 0x6, "ls", 0, &success);
	} else if (equalString(command[0], "rm")) {
		interrupt(0x21, 0xFF << 8 | 0x6, "rm", 0, &success);
	} else if (equalString(command[0], "cat")) {
		interrupt(0x21, 0xFF << 8 | 0x6, "cat", 0, &success);
	} else if (equalString(command[0], "cd")) {
		interrupt(0x21, 0xFF << 8 | 0x6, "cd", 0, &success);
	} else {
		interrupt(0x21, curdir << 8 | 0x6, command[0], 0, &success);
	}*/
	if (success < 0) {
		printString("command failed\r\n");
	}
	}
	return 0;
}

int equalString(char *str1, char *str2) {
	int i = 0;	
	while (str2[i] != '\0' && str1[i] != '\0') {
		if (str1[i] != str2[i]) {
			return 0;
		}
		i++;
	}
	if (i>0 && str2[i] == '\0' && str1[i] == '\0' )
		return 1;
}

void readString(char *string, int disableProcessControls) {
	interrupt(0x21, 0x1, string, disableProcessControls, 0);
}

void printString(char *string) {
	interrupt(0x21, 0x0, string, 0, 0);
}



void parseInput(char * input, char **output) {
	int i, j, k;
	char curdir;
	j = 0;
	output[j] = input;
	for (i=0; input[i] != '\0'; i++) {
		if (input[i] == ' ') {
			input[i] = '\0';
			j++;
			output[j] = &input[i+1];
		}
	}
	
	
	if (j > 0) {
		interrupt(0x21, 0x21, &curdir,0,0); //get curdir
		interrupt(0x21, 0x20, curdir, j, &output[1]); //put args
	}
}
