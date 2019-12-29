int
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;
	lock.l_type = type;  /* F_RDLCK, F_WRLCK, F_UNLCK */
	lock.l_start = offset; /* byte offset, relative to l_whwnce
}
