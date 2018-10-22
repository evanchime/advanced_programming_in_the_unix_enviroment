/*
 	* input/output functions for the implementation of  fmemopen for FreeBSD and Mac OS X. 
*/ 

#ifndef _MYFMEMOPEN_IO_H 
#define _MYFMEMOPEN_IO_H

#include "myfmemopen_aux.h"

// The custom io functions

int fflush(FILE *fp){
	int n;
	struct cookie *cookie = fp->_cookie;
	static int stream_byte_count = 0; // Number of bytes on stream 
	if ((n = fp->_p - fp->_bf._base) > stream_byte_count) { // if stream have increased, flush
		*fp->_p = '\0'; // A null byte is written at the current position in the stream
	memmove(cookie->buf + stream_byte_count, fp->_bf._base + stream_byte_count, n - stream_byte_count);
	stream_byte_count = n;
	}	
	return 0;
} 

int mywrite_fn(void *cookie, const char *buf, int nbytes){
	struct cookie *mycookie = cookie;
	if (write(fileno(mycookie->fp), buf, nbytes) != nbytes) 
		err_sys("write error");
	return nbytes;
}

int myread_fn(void *cookie, char *buf, int nbytes){
	int n;
	struct cookie *mycookie = cookie;
	if ((n = read(fileno(mycookie->fp), buf, nbytes)) < 0) 
		err_sys("read error");
	return n;
}

fpos_t myseek_fn(void *cookie, fpos_t offset, int whence){
	struct cookie *mycookie = cookie;
	if (fflush(mycookie->fp) != 0) {
		err_sys("fflush error");
	}    
	return lseek(fileno(mycookie->fp), offset, whence);
} 
    
int myclose_fn(void *cookie){
	struct cookie *mycookie = cookie;
	if (fflush(mycookie->fp) != 0) { // Flush before closing
		err_sys("fflush error");
	}
	if (mycookie->malloc_flag == 1) {// Test if cookie's buf was allocated dynamically
		free(mycookie->buf); 
	}
	return close(fileno(mycookie->fp));
}

#endif /* _MYFMEMOPEN_IO_H */
