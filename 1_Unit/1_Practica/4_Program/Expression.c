//This program assumes that the parentheses are balanced
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "TADStackInt.h"
#include "TADStackChar.h"
#include "TADStackTree.h"

void Initialize(Tree *t);
void Destroy(Tree *t);
void NewRightSon(Tree *t, Position p, element e);
void NewLeftSon(Tree *t, Position p, element e);
element ReadNode(Position p);
Position RightSon(Position p);
Position LeftSon(Position p);
Position Father(Tree *t, Position p);
bool NullNode(Tree *t, Position p);
void InOrder(Tree *t, Position p);

int main(void)
{
	char expression[100];
	short i, j, t = 1, no_trees = 0;
	StackInt stack_operands;
	StackChar stack_operators;
	StackTree stack_trees;
	Tree tree, main_tree;
	Position position, main_position;
	elementc ec;
	elementi ei;
	elementt et;
	element e;
	int counter = 0;
	char operand[2];

	InitializeC(&stack_operators);
	InitializeI(&stack_operands);
	InitializeT(&stack_trees);

	printf("Enter an expression: ");
	scanf("%s", expression);

	i = 0;
	j = 0;
	while(expression[i] != '\0')
	{
		if(isdigit(expression[i]))
		{
			operand[0] = expression[i];
			operand[1] = '\0';
			PushI(&stack_operands, (elementi) {.value = atoi(operand)});
		}
		else if(expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/')
			PushC(&stack_operators, (elementc) {.value = expression[i]});
		else if(expression[i] == ')')
		{
			if(t == 1)
			{
				Initialize(&tree);
				ec = PopC(&stack_operators);
				NewRightSon(&tree, position, (element) {.operatore = ec.value, .operand = -1});
				position = Root(&tree);
				ei = PopI(&stack_operands);
				NewLeftSon(&tree, position, (element) {.operatore = '-', .operand = ei.value});
				ei = PopI(&stack_operands);
				NewRightSon(&tree, position, (element) {.operatore = '-', .operand = ei.value}); 
				PushT(&stack_trees, (elementt) {.value = tree});
				Destroy(&tree);		
				t = 0;
				no_trees++;
			}
			else
			{
				Initialize(&tree);
				ec = PopC(&stack_operators);
				NewRightSon(&tree, position, (element) {.operatore = ec.value, .operand = -1});
				position = Root(&tree);
				ei = PopI(&stack_operands);
				NewRightSon(&tree, position, (element) {.operatore = '-', .operand = ei.value});
				PushT(&stack_trees, (elementt) {.value = tree});
				Destroy(&tree);		
				no_trees++;
			}
		}
		i++;
	}

	t = 1;
	Initialize(&main_tree);
	while(!EmptyT(&stack_trees))
	{
		et = PopT(&stack_trees);
		position = Root(&et.value);
		e = ReadNode(position);

		if(t == 1)
		{
			NewRightSon(&main_tree, main_position, e);

			main_position = Root(&main_tree);
			position = RightSon(position);
			e = ReadNode(position);
			printf("%d\n", e.operand);
			NewRightSon(&main_tree, main_position, e);

			t = 0;	
			no_trees--;
		}
		else if(no_trees == 1)
		{
			NewLeftSon(&main_tree, main_position, e);
			main_position = LeftSon(main_position);
					
			position = RightSon(position);
			e = ReadNode(position);
			NewRightSon(&main_tree, main_position, e);
			
			position = Root(&et.value);
			position = LeftSon(position);
			e = ReadNode(position);
			NewLeftSon(&main_tree, main_position, e);
		}
		else
		{
			NewLeftSon(&main_tree, main_position, e);
			main_position = LeftSon(main_position);

			position = RightSon(position);
			e = ReadNode(position);
			NewRightSon(&main_tree, main_position, e);
			no_trees--;
		}
	}

	main_position = Root(&main_tree);
	InOrder(&main_tree, main_position);

	return 0;
}

void InOrder(Tree *t, Position p)
{
	element e;
	if(!NullNode(t, p))
	{
		InOrder(t, LeftSon(p));
		e = ReadNode(p);
		printf("%c ", e.operatore);
		printf("%d ", e.operand);
		InOrder(t, RightSon(p));
	}
	return;
}

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

Position Root(Tree *t)
{
	return *t;
}

bool Empty(Tree *t)
{
	if(*t != NULL)
		return false;
	else
		return true;	
}

element ReadNode(Position p)
{
  return p->e;
}

Position RightSon(Position p)
{
	return p->right;	
}

Position LeftSon(Position p)
{
	return p->left;
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

bool NullNode(Tree *t, Position p)
{
	bool b = true; 

	if (*t == p)
		return false;
	if((*t)->right != NULL)
		b = NullNode(&((*t)->right), p);
	if((*t)->left != NULL && b == true)
		b = NullNode(&((*t)->left), p);

	return b;
}
