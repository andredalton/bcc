#include<semaforo.h>
#include<sys/time.h>

int main (void)
{
	int i;
	int v[300];

	for (i=0; i<300; i++) {
		v[i] = get_sem(i+1);
		if ((i+1)%7==0)
			free_sem(i-3);
	}

	return 0;
}
