#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TADArbol.h"

void Initialize(Tree *t)
{
	*t = NULL;

	return;
}

void Destroy(Tree *t)
{
	if((*t)->right != NULL)
		Destroy(&((*t)->right));
	if((*t)->left != NULL)
		Destroy(&((*t)->left));

	free(*t);

	return;
}

void NewRightSon(Tree *t, Position p, element e)
{
	if(Empty(t))
	{	
		*t = malloc(sizeof(node));
		(*t)->e = e;
		(*t)->right = NULL;
		(*t)->left = NULL;
	}
	else
	{
		p->right = malloc(sizeof(node));
		p->right->e = e;
		p->right->right = NULL;
		p->right->left = NULL;
	}

	return;
}

void NewLeftSon(Tree *t, Position p, element e)
{
	if(Empty(t))
	{	
		*t = malloc(sizeof(node));
		(*t)->e = e;
		(*t)->right = NULL;
		(*t)->left = NULL;
	}
	else
	{
		p->left = malloc(sizeof (node));
		p->left->e = e;
		p->left->right = NULL;
		p->left->left = NULL;
	}

	return;
}

void DeleteRightSon(Position p)
{
	Destroy(&(p->right));
	p->right = NULL;
	return;
}

void DeleteLeftSon(Position p)
{
	Destroy(&(p->left));
	p->left = NULL;
	return;
}

void DeleteNode(Tree *t, Position p)
{
	Position father = Father(t, p);
	
	if (father->right == p)
		father->right = NULL;
	else 
		if (father->left == p)
			father->left = NULL;

	Destroy(&p);

	return;
}

void ReplaceNode(Position p, element e)
{
    p->e = e;

	return;
}

Position Root(Tree *t)
{
	return *t;
}

Position Father(Tree *t, Position p)
{
	Position father = NULL;

	if((*t)->right == p || (*t)->left == p)
		return *t;

	if((*t)->right != NULL)
		father = Father(&((*t)->right), p);

	if((*t)->left != NULL && father == NULL)
		father = Father(&((*t)->left), p);

	return father;	
}

Position RightSon(Position p)
{
	return p->right;	
}

Position LeftSon(Position p)
{
	return p->left;
}

Position Search(Tree *t, element e)
{
	Position p = NULL;

	if (memcmp(&((*t)->e), &e, sizeof(element)) == 0)
		return *t;
	if((*t)->right != NULL)
		p = Search(&((*t)->right), e);
	if((*t)->left != NULL && p == NULL)
		p = Search(&((*t)->left), e);
	
	return p;
}


bool Empty(Tree *t)
{
	if(*t != NULL)
		return false;
	else
		return true;	
}

bool NullNode(Tree *t, Position position)
{
	bool b = true; 

	if (*t == position)
		return false;
	if((*t)->right != NULL)
		b = NullNode(&((*t)->right), position);
	if((*t)->left != NULL && b == true)
		b = NullNode(&((*t)->left), position);

	return b;
}

element ReadNode(Position p)
{
  return p->e;
}

//|+|+|+|+|+|+|+|+|+|RECENTLY CREATED
Position NextSon(Position father, Position position, bool *son_visited)
{
	if(father->left != position && son_visited[2] != true)
	{
		son_visited[2] = true;
		return father->left;
	}

	return NULL;
}

Position SearchNonVisited(Tree *t, int levels)
{
	Position position = NULL;

	if ((*t)->e.visited == false && (*t)->e.level == levels)
		return *t;
	if((*t)->right != NULL)
		position = SearchNonVisited(&((*t)->right), levels);
	if((*t)->left != NULL && position == NULL)
		position = SearchNonVisited(&((*t)->left), levels);

	return position;
}

Position SearchInLevel(Tree *t, int levels)
{
	Position position = NULL;

	if ((*t)->e.level == levels)
		return *t;

	if((*t)->right != NULL)
		position = SearchInLevel(&((*t)->right), levels);

	if((*t)->left != NULL && position == NULL)
		position = SearchInLevel(&((*t)->left), levels);

	return position;
}

bool IsSon(Position father, Position position)
{
	if(father->right == position || father->left == position)
		return true;

	return false;	
}

void PreOrder(Tree *t, Position position)
{
	element e;
	if(!NullNode(t, position))
	{
		e = ReadNode(position);
		printf(" %c", e.c);
		PreOrder(t, LeftSon(position));
		PreOrder(t, RightSon(position));
	}
	return;
}

Position SearchNode(Tree *t, char ch, int levels)
{
	Position p = NULL;

	if(ch == 'A')
	{
		if((*t)->e.level == levels)
			return *t;
	}
	else if((*t)->e.level == levels && (*t)->e.c == ch)
	{
		return *t;
	}

	if((*t)->right != NULL)
		p = SearchNode(&((*t)->right), ch, levels);

	if((*t)->left != NULL && p == NULL)
		p = SearchNode(&((*t)->left), ch, levels);

	return p;
}