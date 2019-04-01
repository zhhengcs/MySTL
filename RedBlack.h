enum Colour { RED = 1, BLACK };
struct rbNode
{
	uint8_t key;
	uint8_t colour;
	rbNode *left, *right, *parent;
	void *data;
};
rbNode* root;
enum Players { WHT, BLK };
uint8_t nextMove = WHT;
enum Pieces { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
char pNames[][7] = { "Pawn", "Rook", "Knight", "Bishop",
"Queen", "King" };
char pSymbol[] = { 'P', 'R', 'N', 'B', 'Q', 'K' };
struct cBoard {
	uint8_t player;
	uint8_t piece;
};
typedef void(*ClBk)(uint8_t, struct cBoard*);
typedef uint8_t U_INT

void rbDoInsert(U_INT key, cBoard* square)
{
	rbNode* temp = makeNode(key,square);
	root = rbInsert(temp, root);
	rbPostInsertFix(temp, root);
}

rbNode* makeNode(U_INT key, cBoard* square)
{
	rbNode* temp = (rbNode*)malloc(sizeof(rbNode));
	temp->key = key;
	temp->left = temp->right = temp->parent = NULL;
	temp->colour = RED;
	temp->data = malloc(sizeof(cBoard));
	memcpy(temp->data, square, sizeof(cBoard));
}

rbNode* rbInsert(rbNode* node, rbNode* leaf)
{
	if (leaf == NULL)
		return node;
	if (node->key == leaf->key)
	{
		return leaf;
	}
	if (node->key < leaf->key){
		leaf->left = rbInsert(node, leaf->left);
		leaf->left->parent = leaf;
	}
	else if (node->key > leaf->key) {
		leaf->right = rbInsert(node, leaf->right);
		leaf->right->parent = leaf;
	}
	return leaf;
}

void rbPostInsertFix(rbNode* node, rbNode*& root)
{
	rbNode* grandma = NULL;
	rbNode* aunty = NULL;
	while (node != root && node->parent->color == RED)
	{
		grandma = node->parent->parent;
		if (node->parent == grandma->left)
		{
			aunty = grandma->right;
			if(aunty && aunty->colour == RED)
			{
				node->parent->colour = BLACK;
				aunty->colour = BLACK;
				grandma->colour = RED;
				node=grandma;
			}else{
				if(node==node->parent->right){
					node=node->parent;
					rbRotateLeft(node,root);
				}
				node->parent->colour=BLACK;
				grandma->colour = RED;
				rbRotateRight(grandma,root);
			}
		}
		else{
			aunty=grandma->left;
			if(aunty&&aunty->colour==RED)
			{
				node->parent->colour=BLACK;
				aunty->colour=BLACK;
				grandma->colour = RED;
				node=grandma;
			}else{
				if(node == node->parent->left){
					node=node->parent;
					rbRotateRight(node,root);
				}
				node->parent->colour = BLACK;
				grandma->colour=RED;
				rbRotateLeft(grandma,root);
			}
		}
	}
	root->colour = BLACK;
}

void rbRotateLeft(rbNode* node,rbNode*& root)
{
	rbNode* child = node->right;
	node->right = child->left;
	if(child->left)
	{
		child->left->parent=node;
	}
	child->parent=node->parent;
	if(node == root){
		root = child;
	}
	else if(node == node->parent->left){
		node->parent->left = child;
	}else{
		node->parent->right = child;
	}
	child->left = node;
	node->parent = child;
}

void rbRotateRight(rbNode* node,rbNode*& root)
{
	rbNode* child = node->left;
	node->left = child->right;
	if(child->right!=NULL)
	{
		child->right->parent = node;
	}
	child->parent = node->parent;
	if(node == root)
		root = child
	else if(node == node->parent->right)
	{
		node->parent->right = child;
	}else{
		node->parent->left = child;
	}
	child->right = node;
	node->parent = child;
}

cBoard* rbSearch(U_INT key,rbNode* leaf)
{
	if(leaf){
		if(leaf->key==key){
			return (cBoard*) leaf->data;
		}
		if(key < leaf->key){
			return rbSearch(key,leaf->left);
		}
		return rbSearch(key,leaf->right);
	}
	return NULL;
}
rbNode* rbFind(uint8_t key, rbNode* leaf) 
{
	if(leaf) {
		if(leaf->key == key) {
			return leaf;
		}
		if(key < leaf->key) {
			return rbFind(key, leaf->left);
		}
		return rbFind(key, leaf->right);
	}
	return NULL;
}

rbNode* rbMinimun(rbNode* node)
{
	for(;node->left;node = node->left);
	return node;
}

void rbDeleteTree(rbNode* leaf)
{
	if(leaf){
		rbDeleteTree(leaf->left);
		rbDeleteTree(leaf->right);
		free(leaf->data);
		free(leaf);
	}
}

void rbIterate(rbNode* root,ClBk callBack)
{
	if(root==NULL)
		return ;
	rbIterate(root->left,callBack);
	callBack(root->key,(cBoard*)root->data);
	rbIterate(root->right,callBack);
}

void rbSwap(U_INT* a,U_INT* b)
{
	*a = *a^*b;
	*b = *a^*b;
	*a = *b^*a;
}