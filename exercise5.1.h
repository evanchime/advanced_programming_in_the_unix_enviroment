/* 
	* Implement setbuf using setvbuf.
	* This implementation sets line buffering if the stream is associated with a terminal device
*/

void
my_setbuf(FILE *restrict fp,char *restrict buf){
	size_t size = BUFSIZ;
	int mode = _IOFBF;
	if(buf == NULL){
		size = 0;
		mode = _IONBF;
	}else if(fp == stdin || fp == stdout || fp == stderr){// set line buffering if the stream is associated with a terminal device, otherwise full buffering
		mode = _IOLBF;
	}
	setvbuf(fp, buf, mode, size);
}

