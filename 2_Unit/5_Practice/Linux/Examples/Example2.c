#include <stdio.h>
#include <pthread.h>

void *hilo(void *arg);

int main(void)
{
	pthread_t id_hilo;
	char *mensaje = "Hola a todos desde el hijo";
	int devolucion_hilo;

	pthread_create(&id_hilo, NULL, hilo, mensaje);
	pthread_join(id_hilo, (void *)&devolucion_hilo);
	printf("valor = %i\n", devolucion_hilo);

	return 0;
}

void *hilo(void *arg)
{
	char *men = arg;
	int resultado_hilo = 0;

	printf("%s\n", men);
	resultado_hilo = 100;

	pthread_exit((void *)resultado_hilo);
}