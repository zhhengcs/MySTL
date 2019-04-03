#ifndef RedBlack_h
#define RedBlack_h
template<typename T,typename K>
class RedBlack
{
public:
	using RBIter=bool(*)(K,T*);
	RedBlack();
	~RedBlack();
	bool deleteNode(K);
	bool insertNode(K,T*);
	int blackHeight();
	int nodeCount(bool);
	T* nodeSearch(K);
	void iterate(RBIter);
	void reverseIterator(RBIter);
private:
	enum Colour { RED, BLACK };
	struct rbNode {
		K key;
		uint8_t colour;
		rbNode *left, *right, *parent;
		void* data;
	};
	rbNode* root;
	uint8_t tSize;
	uint8_t nodeSize;
	bool memoryError;
	bool updateOK;
	bool rbRepeat;
	T* rbSearch(K, rbNode*);
	int rbCount(bool, rbNode*);
	rbNode* rbMinimum(rbNode*);
	rbNode* rbMaximum(rbNode*);
	int rbHeight(rbNode*);
	int rbMax(int, int);
	rbNode* rbFind(K, rbNode*);
	void rbDeleteTree(rbNode*);
	void rbRotateRight(rbNode*, rbNode*&);
	void rbRotateLeft(rbNode*, rbNode*&);
	rbNode* rbPreDeleteFix(rbNode*, rbNode*&);
	void rbPostInsertFix(rbNode*, rbNode*&);
	rbNode* rbInsert(rbNode*, rbNode*);
	rbNode* rbMakeNode(K key, T* data);
	void rbIterate(rbNode*, RBIter);
	void rbRevIterate(rbNode*, RBIter);
}


template<typename T, typename K>
RedBlack<T, K>::RedBlack(bool noUpdates = false) {
	root = NULL;
	nodeSize=sizeof(rbNode);
	tSize=sizeof(T);
	updateOK=!noUpdates;
}
template<typename T, typename K>
RedBlack<T, K>::~RedBlack() {
	rbDeleteTree(root);
}

template<typename T, typename K>
bool RedBlack<T, K>::insertNode(K key, T* data) {
	memoryError=false;
	rbNode* temp = rbMakeNode(key,data);
	root = rbInsert(temp,root);
	root = rbPostInsertFix(temp,root);
	return memoryError;
}
template<typename T, typename K>
bool RedBlack<T, K>::deleteNode(K key)
{
	rbNode* found = rbFind(key,root);
	if(found)
	{
		rbNode* del = rbPreDeleteFix(found,root);
		free(del->data);
		free(del);
		return true;
	}
	return false;
}
template<typename T, typename K>
T* RedBlack<T,K>::nodeSearch(K key)
{
	return rbSearch(key);
}
template<typename T, typename K>
void RedBlack<T,K>::iterate(RBIter callBack)
{
	rbRepeat=true;
	rbIterate(root, callBack);
}

template<typename T, typename K>
void RedBlack<T, K>::reverseIterate(RBIter callBack) {
	rbRepeat = true;
	rbRevIterate(root, callBack);
}

template<typename T, typename K>
int RedBlack<T, K>::blackHeight() {
	return rbHeight(root);
}

template<typename T, typename K>
int RedBlack<T, K>::nodeCount(bool blackOnly) {
	return rbCount(blackOnly, root);
}


// private methods
template<typename T, typename K>
typename RedBlack<T, K>::rbNode* RedBlack<T, K>::rbMakeNode(K key, T* data) {
	rbNode* temp = (rbNode*)malloc(nodeSize);
	if(temp == NULL)
		memoryError=true;
	temp->key=key;
	temp->left=temp->right=temp->parent=NULL;
	temp->colour=RED;
	temp->data=malloc(tSize);
	if(temp->data){
		memcpy(temp->data,data,tSize);
	}
	else{
		memoryError=true;
	}
	return temp;
}

template<typename T, typename K>
typename RedBlack<T,K>::rbNode* RedBlack<T,K>::rbInsert(rbNode* node,rbNode* leaf)
{
	if (leaf == NULL) {
		return node;
	}
	if(node->key==leaf->key){
		if(updateOK){
			memcpy(leaf->data,node->data,tSize);
			free(node->data);
			free(node);
		}
		else{
			memoryError=true;
		}
		return leaf
	}
	if(node->key < leaf->key){
		leaf->left = rbInsert(node,rb->leaf);
		leaf->left->parent = leaf;
	}
	else if(node->key > leaf->key){
		leaf->right=rbInsert(node,leaf->right);
		leaf->right->parent = leaf->right;
	}
	return leaf;
} 
template<typename T, typename K>
void RedBlack<T,K>::rbPostInsertFix(rbNode* node,rbNode*& root)
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
template<typename T, typename K>
typename RedBlack<T, K>::rbNode* RedBlack<T, K>::rbFind(K key, rbNode* leaf)
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

template<typename T, typename K>
typename RedBlack<T, K>::rbNode* RedBlack<T,K>::rbPreDeleteFix(rbNode* node,rbNode*& root)
{
	rbNode* nodeToDelete = NULL;
	rbNode* child = NULL;
	rbNode* parentNode = NULL;
	if(nodeToDelete->left==NULL){
		child = nodeToDelete->right;
	}
	else{
		if(nodeToDelete->right == NULL)
			child = nodeToDelete->left;
		else{
			nodeToDelete = rbMinimum(nodeToDelete->right);
			child = nodeToDelete->right;
		}
	}
	if(nodeToDelete != node){
		node->left->parent = nodeToDelete->parent;
		nodeToDelete->left = node->left;
		if(nodeToDelete!=node->right){
			parentNode=nodeToDelete->parent;
			if(child){
				child->parent=nodeToDelete->parent;
			}
			nodeToDelete->parent->left=child;
			nodeToDelete->right = node->right;
			node->right->parent = nodeToDelete;
		}
		else{
			parentNode=nodeToDelete;
		}
		if(root == node){
			root = nodeToDelete;
		}
		else if(node->parent->left==node){
			node->parent->left = nodeToDelete;
		}
		else{
			node->parent->right = nodeToDelete;
		}
		nodeToDelete->parent = node->parent;
		swap(nodeToDelete->colour,node->colour);
		nodeToDelete=node;
	}
	else{
		parentNode=nodeToDelete->parent;
		if(child){
			child->parent=nodeToDelete->parent;
		}
		if(root == node)
			root = child;
		else{
			if(node->parent->left == node){
				node->parent->left = child;
			}
			else{
				node->parent->right = child;
			}
		}
	}

	if(nodeToDelete->colour!=RED){
		while (child != root && (child == NULL || child->colour == BLACK)) {
			if (child == parentNode->left) {
				rbNode* sisterNode = parentNode->right;
				if (sisterNode->colour == RED) {
					sisterNode->colour = BLACK;
					parentNode->colour = RED;
					rbRotateLeft(parentNode, root);
					sisterNode = parentNode->right;
				}
				if ((sisterNode->left == NULL ||
					sisterNode->left->colour == BLACK) &&
					(sisterNode->right == NULL ||
						sisterNode->right->colour == BLACK)) {
					sisterNode->colour = RED;
					child = parentNode;
					parentNode = parentNode->parent;
				}
				else {
					if (sisterNode->right == NULL ||
						sisterNode->right->colour == BLACK) {
						if (sisterNode->left) sisterNode->left->colour = BLACK;
						sisterNode->colour = RED;
						rbRotateRight(sisterNode, root);
						sisterNode = parentNode->right;
					}
					sisterNode->colour = parentNode->colour;
					parentNode->colour = BLACK;
					if (sisterNode->right) {
						sisterNode->right->colour = BLACK;
					}
					rbRotateLeft(parentNode, root);
					break;
				}
			}
			else {                  // same as above but the other way about
				rbNode* sisterNode = parentNode->left;
				if (sisterNode->colour == RED) {
					sisterNode->colour = BLACK;
					parentNode->colour = RED;
					rbRotateRight(parentNode, root);
					sisterNode = parentNode->left;
				}
				if ((sisterNode->right == NULL ||
					sisterNode->right->colour == BLACK) &&
					(sisterNode->left == NULL ||
						sisterNode->left->colour == BLACK)) {
					sisterNode->colour = RED;
					child = parentNode;
					parentNode = parentNode->parent;
				}
				else {
					if (sisterNode->left == NULL ||
						sisterNode->left->colour == BLACK) {
						if (sisterNode->right) {
							sisterNode->right->colour = BLACK;
						}
						sisterNode->colour = RED;
						rbRotateLeft(sisterNode, root);
						sisterNode = parentNode->left;
					}
					sisterNode->colour = parentNode->colour;
					parentNode->colour = BLACK;
					if (sisterNode->left) {
						sisterNode->left->colour = BLACK;
					}
					rbRotateRight(parentNode, root);
					break;
				}
			}
		}
		if (child) {
			child->colour = BLACK;
		}

	}
	return nodeToDelete;
}


template<typename T,typename K>
void RedBlack<T,K>::rbRotateLeft(rbNode* node,rbNode*& root)
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
template<typename T,typename K>
void RedBlack<T,K>::rbRotateRight(rbNode* node,rbNode*& root)
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
template<typename T, typename K>
T* RedBlack<T,K>::rbSearch(K key,rbNode* leaf)
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

template<typename T, typename K>
typename RedBlack<T, K>::rbNode* RedBlack<T, K>::rbMinimum(rbNode* node) {

	for(;node->left;node = node->left);
	return node;
}

template<typename T, typename K>
typename RedBlack<T, K>::rbNode* RedBlack<T, K>::rbMaximum(rbNode* node) {
	while (node->right) {
		node = node->right;
	}
	return node;
}

template<typename T, typename K>
void RedBlack<T, K>::rbIterate(rbNode* leaf, RBIter callBack) {
	if(root==NULL)
		return ;
	rbIterate(root->left,callBack);
	callBack(root->key,(cBoard*)root->data);
	rbIterate(root->right,callBack);
}

template<typename T, typename K>
void RedBlack<T, K>::rbRevIterate(rbNode* leaf, RBIter callBack) {
	if (leaf && rbRepeat) {
		rbRevIterate(leaf->right, callBack);
		if (rbRepeat) {
			rbRepeat = callBack(leaf->key, (T*)leaf->data);
			rbRevIterate(leaf->left, callBack);
		}
	}
}


template<typename T, typename K>
void RedBlack<T, K>::rbDeleteTree(rbNode* leaf) {
	if(leaf){
		rbDeleteTree(leaf->left);
		rbDeleteTree(leaf->right);
		free(leaf->data);
		free(leaf);
	}
}
template<typename T, typename K>
int RedBlack<T, K>::rbHeight(rbNode* leaf) {
	if (leaf) {
		int subMax = rbMax(rbHeight(leaf->left), rbHeight(leaf->right));
		if (leaf->colour == BLACK) { subMax++; }
		return subMax;
	}
	return 0;
}

template<typename T, typename K>
int RedBlack<T, K>::rbMax(int a, int b) {
	return (a <= b) ? b : a;
}

#endif
