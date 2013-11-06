#include<semaforo.h>

int main (void)
{
	get_sem(5);
	p_sem(4);
	v_sem(3);
	free_sem(2);

	return 0;
}
