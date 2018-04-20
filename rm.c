

int main() {
	int i;
	int result;
	char argc;
	char curdir;
	char argv[3][50];
	char input[10];

	interrupt(0x21, 0x21, &curdir, 0, 0); //getcurdir
	interrupt(0x21, 0x22, &argc, 0, 0); //getargc
	for (i=0; i<argc; ++i) {
		interrupt(0x21, 0x23, i, argv[i], 0);	//getargs
	}

	if (argc == 1) {
		interrupt(0x21, curdir << 8 | 0x09, argv[0], &result, 0);
		
		if (result != -1) {
			interrupt(0x21, 0x00, "file deleted\r\n", 0 ,0);
		} else {
			interrupt(0x21, 0x00, "file delete failed\r\n",0,0);
		}
	} else if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'd') {
		interrupt(0x21, curdir << 8 | 0x0A, argv[0], &result, 0);
		
		if (result != -1) {
			interrupt(0x21, 0x00, "directory deleted\r\n", 0, 0);
		} else {
			interrupt(0x21, 0x00, "directory delete failed\r\n", 0, 0);
		}
	}
	interrupt(0x21, 0x07, &result, 0, 0);
}