/**
 * @Author: Nonrustknife <chris>
 * @Date:   17-Oct-2021
 * @Email:  cm656879@outlook.com
 * @Last modified by:   chris
 * @Last modified time: 27-Oct-2021
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<thread.h>
#define likely(x) __builtin_expect(!!(x), 1) //gcc内置函数, 帮助编译器分支优化
#define unlikely(x) __builtin_expect(!!(x), 0)

#define RED 1
#define BLACK 2

typedef struct RbTreeNode
{
	struct RbTreeNode *left;
	struct RbTreeNode *right;
	struct RbTreeNode *parent;
	unsigned char 	  color;
	int 			  			key;
	void			  			*value;
}RbTreeNode;

typedef struct RbTree
{
	RbTreeNode *root;
	RbTreeNode *nil;
}RbTree;

RbTreeNode* AllocRbTreeNode()
{
	RbTreeNode *rbTreeNode = NULL;
	rbTreeNode = malloc(sizeof(RbTreeNode));
	if(rbTreeNode == NULL)
		return NULL;
	memset(rbTreeNode, 0, sizeof(RbTreeNode));

	return rbTreeNode;
}

void DelRbTreeNode(RbTree *rbTree, RbTreeNode *delNode)
{
	if(delNode->parent == rbTree->nil)
		free(delNode);

	if(delNode == delNode->parent->left)
	{
		delNode->parent->left = rbTree->nil;
	}
	else
	{
		delNode->parent->right = rbTree->nil;
	}
	free(delNode);

	return;
}

void LeftRotate(RbTree *rbTree, RbTreeNode *rotNode)
{
	RbTreeNode *rightNode = rotNode->right;

	rotNode->right = rightNode->left;
	if(rightNode->left != rbTree->nil)
	{
		rightNode->left->parent = rotNode;
	}

	if(rotNode->parent == rbTree->nil)
	{
		rbTree->root = rightNode;
	}
	else if(rotNode == rotNode->parent->left)
	{
		rotNode->parent->left = rightNode;
		rightNode->parent = rotNode->parent;
	}
	else
	{
		rotNode->parent->right = rightNode;
		rightNode->parent = rotNode->parent;
	}

	rightNode->left = rotNode;
	rotNode->parent = rightNode;
	return;
}

void RightRotate(RbTree *rbTree, RbTreeNode *rotNode)
{
	RbTreeNode *leftNode = rotNode->left;
	rotNode->left = leftNode->right;
	if(leftNode->right != rbTree->nil)
	{
		leftNode->right->parent = rotNode;
	}

	if(rotNode->parent == rbTree->nil)
	{
		rbTree->root = leftNode;
	}
	else if(rotNode == rotNode->parent->left)
	{
		rotNode->parent->left = leftNode;
		leftNode->parent = rotNode->parent;
	}
	else
	{
		rotNode->parent->right = leftNode;
		leftNode->parent = rotNode->parent;
	}

	leftNode->right = rotNode;
	rotNode->parent = leftNode;
}

void RbTreeInsertFixup(RbTree *rbTree, RbTreeNode *fixNode)
{
	while((fixNode->parent != rbTree->nil) && (fixNode->parent->color == RED))
	{
		if(fixNode->parent == fixNode->parent->parent->left)
		{
			RbTreeNode *y = fixNode->parent->parent->right;

			if((y != rbTree->nil) && (y->color == RED))
			{
				fixNode->parent->color = BLACK;
				y->color = BLACK;
				fixNode->parent->parent->color = RED;
				fixNode = fixNode->parent->parent;
			}
			else
			{
				// LL型-->右旋
				if(fixNode == fixNode->parent->left)
				{
					fixNode->parent->color = BLACK;
					fixNode->parent->parent->color = RED;
					RightRotate(rbTree, fixNode->parent->parent);
				}
				//LR型--左旋+右旋
				else
				{
					fixNode->color = BLACK;
					fixNode->parent->parent->color = RED;
					LeftRotate(rbTree, fixNode->parent);
					RightRotate(rbTree, fixNode->parent);
				}
			}
		}
		else
		{
			RbTreeNode *y = fixNode->parent->parent->left;
			if((y != rbTree->nil) && (y->color == RED))
			{
				fixNode->parent->color = BLACK;
				y->color = BLACK;
				fixNode->parent->parent->color = RED;
				fixNode = fixNode->parent->parent;
			}
			else
			{
				//RR型-->左旋
				if(fixNode == fixNode->parent->right)
				{
					fixNode->parent->color = BLACK;
					fixNode->parent->parent->color = RED;
					LeftRotate(rbTree, fixNode->parent->parent);
				}
				//RL型-->右旋+左旋
				else
				{
					fixNode->color = BLACK;
					fixNode->parent->parent->color = RED;
					RightRotate(rbTree, fixNode->parent);
					LeftRotate(rbTree, fixNode);
				}
			}
		}
	}

	rbTree->root->color = BLACK;
	rbTree->root->parent = rbTree->nil;
}

void RbTreeInsert(RbTree *rbTree, RbTreeNode *insertNode)
{
	//插入一个节点包括：寻找插入节点的位置；自平衡调整；
	RbTreeNode *x = rbTree->root;
	RbTreeNode *y = rbTree->nil;
	while(x != rbTree->nil)
	{
		y = x;
		if(insertNode->key < x->key)
		{
			x = x->left;
		}
		else if(insertNode->key > x->key)
		{
			x = x->right;
		}
		else
		{
			return;
		}
	}

	insertNode->parent = y;
	if(y == rbTree->nil)
	{
		rbTree->root = insertNode;
	}
	else if(insertNode->key < y->key)
	{
		y->left = insertNode;
	}
	else
	{
		y->right = insertNode;
	}
	insertNode->left = rbTree->nil;
	insertNode->right = rbTree->nil;
	insertNode->color = RED;

	RbTreeInsertFixup(rbTree, insertNode);
}

void RbTreeDelBlackNoleaf(RbTree *rbTree, RbTreeNode *delNode)
{
	RbTreeNode *delNodeSon;
	delNodeSon = (delNode->left == rbTree->nil)?(delNode->right):(delNode->left);

	if(unlikely((delNode->color != BLACK) || (delNodeSon->color != RED)))
	{
		printf("[%d][%s]Not a rbTree!\n", __LINE__, __FUNCTION__);
	}

	if(delNode->parent == rbTree->nil)
	{
		rbTree->root = delNode->left;
	}
	else
	{
		if(delNode == delNode->parent->left)
		{
			delNode->parent->left = delNodeSon;
		}
		else
		{
			delNode->parent->right = delNodeSon;
		}

		delNodeSon->parent = delNode->parent;
		delNodeSon->color = BLACK;
		free(delNode);
	}

	return;
}

void RbTreeDelRedLeaf(RbTree *rbTree, RbTreeNode *delNode)
{
	DelRbTreeNode(rbTree, delNode);
}

void RbTreeDelBlackLeaf(RbTree *rbTree, RbTreeNode *delNode)
{
	while(delNode->parent != rbTree->nil)
	{
		RbTreeNode *broNode;
		if(delNode == delNode->parent->left)
		{
			broNode = delNode->parent->right;
			if(broNode->color == RED)
			{
				delNode->parent->color = BLACK;
				broNode->color = BLACK;
				LeftRotate(rbTree, delNode->parent);
				continue;
			}
			else
			{
				if((broNode->right != rbTree->nil) && (broNode->right->color == RED))
				{
					int tmpColor = delNode->parent->color;
					delNode->parent->color = broNode->color;
					broNode->color = tmpColor;
					broNode->right->color = BLACK;
					LeftRotate(rbTree, delNode->parent);
					break;
				}
				else
				{
					if((broNode->left != rbTree->nil) && (broNode->left->color == RED))
					{
						int tmpColor = delNode->parent->color;
						delNode->parent->color = BLACK;
						broNode->left->color = tmpColor;
						RightRotate(rbTree, broNode);
						LeftRotate(rbTree, delNode->parent);
						continue;
					}
					else
					{
						if(delNode->parent->color == RED)
						{
							delNode->parent->color = BLACK;
							broNode->color = RED;
							break;
						}
						else
						{
							broNode->color = RED;
							delNode = delNode->parent;
							continue;
						}
					}
				}
			}
		}
		else
		{
			broNode = delNode->parent->left;
			if(broNode->color == RED)
			{
				delNode->parent->color = BLACK;
				broNode->color = BLACK;
				RightRotate(rbTree, delNode->parent);
				continue;
			}
			else
			{
				if(broNode->left->color == RED)
				{
					int tmpColor = delNode->parent->color;
					delNode->parent->color = broNode->color;
					broNode->color = tmpColor;
					broNode->left->color = BLACK;
					LeftRotate(rbTree, delNode->parent);
					break;
				}
				else
				{
					if(broNode->right->color == RED)
					{
						int tmpColor = delNode->parent->color;
						delNode->parent->color = BLACK;
						broNode->right->color = tmpColor;
						LeftRotate(rbTree, broNode);
						RightRotate(rbTree, delNode->parent);
						continue;
					}
					else
					{
						if(delNode->parent->color == RED)
						{
							delNode->parent->color = BLACK;
							broNode->color = RED;
							break;
						}
						else
						{
							broNode->color = RED;
							delNode = delNode->parent;
							continue;
						}
					}
				}
			}
		}
	}

	DelRbTreeNode(rbTree, delNode);
}

void RbTreeDel(RbTree *rbTree, RbTreeNode *delNode)
{
	int tag = 0;

	while(tag != 2)
	{
		if(delNode->left == rbTree->nil)//左子树为空
		{
			if(delNode->right == rbTree->nil)//右子树也为空
			{
				if(delNode->color == RED)
				{
					RbTreeDelRedLeaf(rbTree, delNode);
				}
				else if(delNode->color == BLACK)
				{
					RbTreeDelBlackLeaf(rbTree, delNode);
				}
				else
				{
					printf("[%d][%s]ERR:node color err!\n", __LINE__, __FUNCTION__);
				}
			}
			else//右子树不为空，此时仅有一种情况，即删除节点为黑色，右子节点为红色；
			{
				RbTreeDelBlackNoleaf(rbTree, delNode);
			}
		}
		else//左子树不为空
		{
			if(delNode->right == rbTree->nil)//右子树为空
			{
				RbTreeDelBlackNoleaf(rbTree, delNode);
			}
			else//左右子树都不为空
			{
				RbTreeNode *tempNode = delNode->right;
				while(tempNode != rbTree->nil)
				{
					tempNode = tempNode->left;
				}

				int tempKey = delNode->key;
				delNode->key = tempNode->key;
				delNode->value = tempNode->value;
				tempNode->key = tempKey;
				delNode = tempNode;

				tag++;
			}
		}

	}

	return;
}

RbTreeNode *RbTreeNodeCreate(int key, void* value)
{
	RbTreeNode *rbTreeNode = malloc(sizeof(RbTreeNode));
	memset(rbTreeNode, 0 , sizeof(RbTreeNode));
	rbTreeNode->key = key;
	rbTreeNode->value = value;

	return rbTreeNode;
}

void PrintRbTreeNode(RbTreeNode *rbTreeNode)
{
	printf("Addr:%x|key:%d|color:%x|value:%s|left:%x|right:%x|parent:%x\n",
	(int)rbTreeNode, rbTreeNode->key, rbTreeNode->color, rbTreeNode->value, (int)rbTreeNode->left, (int)rbTreeNode->right, (int)rbTreeNode->parent);
	return;
}

int main()
{
	RbTree *rbTree1 = malloc(sizeof(RbTree));
	rbTree1->root = NULL;
	rbTree1->nil = NULL;

	RbTreeNode *rbTreeNode1 = RbTreeNodeCreate(1234, "hello");
	RbTreeNode *rbTreeNode2 = RbTreeNodeCreate(12, "hellokitty");
	RbTreeNode *rbTreeNode3 = RbTreeNodeCreate(123, "hellonimabidekitty");

	RbTreeInsert(rbTree1, rbTreeNode1);
	RbTreeInsert(rbTree1, rbTreeNode2);
	RbTreeInsert(rbTree1, rbTreeNode3);

	PrintRbTreeNode(rbTreeNode1);
	PrintRbTreeNode(rbTreeNode2);
	PrintRbTreeNode(rbTreeNode3);

	return 0;
}
