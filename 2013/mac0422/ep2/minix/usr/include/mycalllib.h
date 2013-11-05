#include <lib.h>
#include <unistd.h>

PUBLIC int mycall () {
	message m;
	return (_syscall(PM_PROC_NR, MYCALL, &m) );
}
