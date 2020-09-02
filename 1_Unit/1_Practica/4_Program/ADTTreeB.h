#include <stdbool.h>

typedef struct
{
	char operatore;
	int operand;
	bool visited;
	int level;
}element;

typedef struct node
{
	element e;
	struct node *left;
	struct node *right;
}node;

typedef node *Tree;

typedef node *Position;

void Initialize(Tree *t); //*
void Destroy(Tree *t); //*
Position Root(Tree *t); //*
Position Father(Tree *t, Position p); //*
Position RightSon(Position p); //*
Position LeftSon(Position p);//*
Position Search(Tree *t, element e);//*
bool Empty(Tree *t);//*
bool NullNode(Tree *t, Position p);//
element ReadNode(Position p);
void NewRightSon(Tree *t, Position p, element e); //*
void NewLeftSon(Tree *t, Position p, element e); //*
void DeleteRightSon(Position p); //*
void DeleteLeftSon(Position p); //*
void DeleteNode(Tree *t, Position p); //*
void ReplaceNode(Position p, element e);//*

//RECENTLY ADDED
Position NextSon(Position father, Position position, bool *son_visited);
Position SearchNonVisited(Tree *t, int levels);
Position SearchInLevel(Tree *t, int levels);
Position SearchNode(Tree *t, char ch, int level);
bool IsSon(Position father, Position position);
void PreOrder(Tree *t, Position position);