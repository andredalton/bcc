#include<semaforo.h>
#include<sys/time.h>

int main (int argc, char *argv[])
{
	int i;
	int r;
	int sid = get_sem(4);

	if (argc==1) {
		for(i=0; i<10; i++) {
			r = fork();
			if ( !r ) {
				printf("\nOla papai [%d].\n", i);
				p_sem(sid);
				sleep(i);
				v_sem(sid);
				break;
			}
			else {
				printf("\nCriando filho %d [%d].\n", r, i);
			}
		}
	}

	wait_sem(sid);

	return 0;
}
