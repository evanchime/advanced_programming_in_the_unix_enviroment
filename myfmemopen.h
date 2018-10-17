FILE *fmemopen(void *restrict buf, size_t size, const char *restrict type){
	FILE *fp = tmpfile(); // The stream
	    
	// The cookie
	static struct{
		FILE *fp; // The stream
		int malloc_flag; // Buffer allocated dynamically if 1, 0 otherwise
		void *buf; // The buffer
	}cookie;

	// initialize cookie
	cookie.fp = fp;
	if(buf == NULL){	
		if((buf = malloc(size)) == NULL)
			err_sys("malloc error for buffer");
		cookie.malloc_flag = 1;
	}else{
		cookie.malloc_flag = 0;
	}
	if (strlen(buf) != size)
		err_sys("error: buf length and argument size don't match");

	buf[0] = '\0';
	cookie.buf = buf;

	fp->_p = fp->_bf._base; // Set current position in _bf._base buffer
	fp->_cookie = &cookie; // cookie passed to the custom io functions

	// Test type
	if (strcmp(type, "r") == 0 | strcmp(type, "rb") == 0) {
		fp->_flags |= __SRD; // open for reading	
	}
	else if(strcmp(type, "w") == 0 | strcmp(type, "wb") == 0)
	{
		fp->_flags |= __SWR; // open for writing	        
	}
	else if (strcmp(type, "a") == 0 | strcmp(type, "ab") == 0)
	{
		append_mode_function(fp); // append
		fp->_flags |= __SWR; // and open for writing at ﬁrst null byte
	}
	else if (strcmp(type, "r+") == 0 | strcmp(type, "r+b") == 0 | strcmp(type, "rb+") == 0)
	{
		fp->_flags |= __SRD | __SWR; //open for reading and writing	
	}
	else if (strcmp(type, "w+") == 0 | strcmp(type, "w+b") == 0 | strcmp(type, "wb+") == 0)
	{
		fp->_bf._base[0] = '\0';  // truncate to 0 length 
		fp->_flags |= __SRD | __SWR; // and open for reading and writing    
	}
	else if (strcmp(type, "a+") == 0 | strcmp(type, "a+b") == 0 | strcmp(type, "ab+") == 0)
	{
		append_mode_function(fp); // append
		fp->_flags |= __SRD | __SWR; // and open for reading and writing at first null byte	   
	}
	else
		err_dump("unknown type %s", type);

	return funopen(&cookie, myread_fn, mywrite_fn, myseek_fn, myclose_fn); 
}
