//Echo
int main() {
	int i;
	int result;
	char argc;
	char argv[4][20];
	char input[50];
	
	interrupt(0x21, 0x22, &argc, 0, 0); //getargc
	for (i=0; i<argc; ++i) {
		interrupt(0x21, 0x23, i, argv[i], 0);	//getargs
	}
	//print argv
	interrupt(0x21, 0x00, argv[0], 0, 0);
	interrupt(0x21, 0x00, "\n\r", 0, 0);
	
	//terminate
	interrupt(0x21, 0x07, &result, 0, 0);
	return 0;
}
