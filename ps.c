void printString(char *str);

int main() {
	printString("Processes with Running Status: \r\n");
	return 0;
}

void printString(char *str) {
	interrupt(0x21, 0x00, str, 0, 0);
}
