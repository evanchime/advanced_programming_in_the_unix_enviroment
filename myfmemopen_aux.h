/*
 	*The auxilliary function and data structure for the implemeantion of fmemopen 
*/

#ifndef _MYFMEMOPEN_AUX_H 
#define _MYFMEMOPEN_AUX_H

/*
	*whenever a memory stream is opened for append, 
	*the current ﬁle position is set to the ﬁrst null byte in the buffer.
	*If the buffer contains no null bytes, then the current position is set to one byte past the end of the buffer
*/
void 
append_mode_function(FILE *fp)
{
	unsigned char *base_copy = fp->_bf._base;
	while (*base_copy != '\0')
		base_copy++;
	fp->_p = base_copy;
}

// The cookie
struct cookie{
	FILE *fp; // The stream
	int malloc_flag; // Buffer allocated dynamically if 1, 0 otherwise
	char *buf; // The buffer
};

#endif /* _MYFMEMOPEN_AUX_H */
