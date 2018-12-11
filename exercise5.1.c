



void
setbuf(FILE *restrict fp,char *restrict buf){
	size_t size = BUFSIZE;
	int mode = _IOFBF;
	if(buf == NULL){
		size = 0;
		mode = _IONBF;
	}else{ // set line buffering if the stream is associated with a terminal device, otherwise full buffering
		struct stat sbuf;
		if(fstat(fp, &sbuf) < 0)
			err_ret("fstat error");
		else if(S_ISBLK(buf.st_mode))
			mode = _IOLBF;
	}
	setvbuf(fp, buf, mode, size);
}

