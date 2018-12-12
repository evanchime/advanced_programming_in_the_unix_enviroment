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
	}else{ // set line buffering if the stream is associated with a terminal device, otherwise full buffering
		struct stat sbuf;
		if(fstat(fileno(fp), &sbuf) < 0)
			err_ret("fstat error");
		else if(S_ISBLK(sbuf.st_mode) || fp == stdin || fp == stdout || fp == stderr)
			mode = _IOLBF;
	}
	setvbuf(fp, buf, mode, size);
}

