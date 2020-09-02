#include <stdbool.h>

typedef struct elementi
{
    int value;
	struct elementi *down;
}elementi;

typedef elementi *StackInt;

//DECLARACIÓN DE FUNCIONES
void InitializeI(StackInt *stack);			//Inicializar pila (Iniciar una pila para su uso)
void PushI(StackInt *stack, elementi e);		//Empilar (Introducir un elementio a la pila)
elementi PopI(StackInt *stack);				//Desempilar (Extraer un elementio de la pila)
bool EmptyI(StackInt *stack);				//Vacia (Preguntar si la pila esta vacia)
elementi TopI(StackInt *stack);				//Tope (Obtener el "elementio" del tope de la pila si extraerlo de la pila)
int ValueTopI(StackInt *stack);				//Tamaño de la pila (Obtener el número de elementios en la pila)
void DestroyI(StackInt *stack);				//Elimina pila (Borra a todos los elementios y a la pila de memoria)
