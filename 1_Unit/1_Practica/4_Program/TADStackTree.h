#include <stdbool.h>
#include "TADArbol.h"

typedef struct elementt
{
    Tree value;
	struct elementt *down;
}elementt;

typedef elementt *StackTree;

//DECLARACIÓN DE FUNCIONES
void InitializeT(StackTree *stack);			//Inicializar pila (Iniciar una pila para su uso)
void PushT(StackTree *stack, elementt e);		//Empilar (Introducir un elementto a la pila)
elementt PopT(StackTree *stack);				//Desempilar (Extraer un elementto de la pila)
bool EmptyT(StackTree *stack);				//Vacia (Preguntar si la pila esta vacia)
elementt TopT(StackTree *stack);				//Tope (Obtener el "elementto" del tope de la pila si extraerlo de la pila)
int ValueTopT(StackTree *stack);				//Tamaño de la pila (Obtener el número de elementtos en la pila)
void DestroyT(StackTree *stack);				//Elimina pila (Borra a todos los elementtos y a la pila de memoria)
