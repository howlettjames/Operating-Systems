#include <stdbool.h>

typedef struct element
{
    char value;
	struct element *down;
}element;


typedef element *Stack;

//DECLARACIÓN DE FUNCIONES
void Initialize(Stack *stack);			//Inicializar pila (Iniciar una pila para su uso)
void Push(Stack *stack, element e);		//Empilar (Introducir un elemento a la pila)
element Pop(Stack *stack);				//Desempilar (Extraer un elemento de la pila)
bool Empty(Stack *stack);				//Vacia (Preguntar si la pila esta vacia)
element Top(Stack *stack);				//Tope (Obtener el "elemento" del tope de la pila sin extraerlo de la pila)
int ValueTop(Stack *stack);				//Tamaño de la pila (Obtener el número de elementos en la pila)
void Destroy(Stack *stack);				//Elimina pila (Borra a todos los elementos y a la pila de memoria)
