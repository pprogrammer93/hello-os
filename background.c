int main() {
	int a;
	enableInterrupts();
	a = 0;
	while (1) {
		a++;
		interrupt(0x21,0x30,0,0,0);
		if (a == 60) {
			a = 0;
			interrupt(0x21,0x0, "background\r\n",0,0);
		}
	}
}