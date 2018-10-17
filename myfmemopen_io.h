#ifndef _MYFMEMOPEN_IO_H 
#define _MYFMEMOPEN_IO_H

// The custom io functions

int fflush(FILE *fp){
	int n;
	static int stream_byte_count = 0; // Number of bytes on stream 
	if ((n = fp->_p - fp->_bf._base) > stream_byte_count) { // if stream have increased, flush
		*fp->_p = '\0'; // A null byte is written at the current position in the stream
		memmove(fp->_cookie->buf + stream_byte_count, fp->_bf._base + stream_byte_count, n - stream_byte_count);
		stream_byte_count = n;  
	}
	return 0;
}

int mywrite_fn(void *cookie, const char *buf, int nbytes){
	if (write(fileno(cookie->fp), buf, nbytes) != nbytes) 
		err_sys("write error");
	return nbytes;
}

int myread_fn(void *cookie, char *buf, int nbytes){
	int n;
	if ((n = read(fileno(cookie->fp), buf, nbytes)) < 0) 
		err_sys("read error");
	return n;
}

fpos_t myseek_fn(void *cookie, fpos_t offset, int whence){
	if (fflush(cookie->fp) != 0) {
		err_sys("fflush error");
	}    
	return lseek(fileno(cookie->fp), offset, whence);
}

int myclose_fn(void *cookie){
	if (fflush(cookie->fp) != 0) { // Flush before closing
		err_sys("fflush error");
	}
	if (cookie->malloc_flag == 1) {// Test if cookie's buf was allocated dynamically
		free(cookie->buf); 
	}
	return close(fileno(cookie->fp));
}

#endif /* _MYFMEMOPEN_IO_H */
