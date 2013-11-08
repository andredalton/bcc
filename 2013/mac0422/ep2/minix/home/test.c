#include<semaforo.h>
#include<sys/time.h>

int main (void)
{
	int sid;

	sid = get_sem(3);
	sid = get_sem(8);
	sid = get_sem(2);

	p_sem(sid);
	p_sem(sid);

	v_sem(sid);
	v_sem(sid);

	p_sem(sid);
	p_sem(sid);
	v_sem(sid);

	return 0;
}
