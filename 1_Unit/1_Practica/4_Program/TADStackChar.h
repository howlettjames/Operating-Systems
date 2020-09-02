#include <stdbool.h>

typedef struct elementc
{
    char value;
	struct elementc *down;
}elementc;


typedef elementc *StackChar;

//DECLARACIÓN DE FUNCIONES
void InitializeC(StackChar *stack);			//Inicializar pila (Iniciar una pila para su uso)
void PushC(StackChar *stack, elementc e);		//Empilar (Introducir un elemento a la pila)
elementc PopC(StackChar *stack);				//Desempilar (Extraer un elemento de la pila)
bool EmptyC(StackChar *stack);				//Vacia (Preguntar si la pila esta vacia)
elementc TopC(StackChar *stack);				//Tope (Obtener el "elemento" del tope de la pila si extraerlo de la pila)
int ValueTopC(StackChar *stack);				//Tamaño de la pila (Obtener el número de elementos en la pila)
void DestroyC(StackChar *stack);				//Elimina pila (Borra a todos los elementos y a la pila de memoria)
