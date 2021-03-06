/*??????????????????????????????????????????????????*/
/*??????????????????????????????????????????????????*/

#include <lib.h>
#include <unistd.h>
#include <sys/cdefs.h>
#include <stdlib.h>
#include <stdio.h>
#include <minix/callnr.h>
#include <minix/endpoint.h>

PUBLIC int get_sem (int n) {
	int sid;
	message m;
	m.m1_i1 = n;

	sid = (_syscall(PM_PROC_NR, GET_SEM, &m) );

	if ( sid==-1 ) exit(0);

	return sid;
}

PUBLIC int p_sem (int sid) {
	message m;
	m.m1_i1 = sid;

	/* Faz a call e pode ser suspenso neste momento. */
	return _syscall(PM_PROC_NR, P_SEM, &m);
}

PUBLIC int v_sem (int sid) {
	int rec;
	message m;
	m.m1_i1 = sid;
	_syscall(PM_PROC_NR, V_SEM, &m);
}

PUBLIC int free_sem (int sid) {
	message m;
	m.m1_i1 = sid;
	return (_syscall(PM_PROC_NR, FREE_SEM, &m) );
}

PUBLIC int wait_sem (int sid) {
	message m;
	m.m1_i1 = sid;
	return (_syscall(PM_PROC_NR, WAIT_SEM, &m) );
}

/*??????????????????????????????????????????????????*/
/*??????????????????????????????????????????????????*/
