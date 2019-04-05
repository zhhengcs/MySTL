#ifndef RedBlack_h
#define RedBlack_h
#include <stdlib.h>           
#include   <string.h>
#include <iostream>
using namespace std;
// std::swap is available in this context
//template <class T> inline void swap(T& a, T& b)
//{
//	T c(a); a = b; b = c;
//};
template<typename T, typename K>
class RedBlack {
public:
	using RBIter = bool(*)(K, T*);
	RedBlack(bool);
	~RedBlack();
	bool deleteNode(K);
	bool insertNode(K, T*);
	int blackHeight();
	int nodeCount(bool);
	T* nodeSearch(K);
	void iterate(RBIter);
	void reverseIterate(RBIter);
	void levelIterate(RBIter);

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
	void rbLvIterate(rbNode*, RBIter);
	void printColour(Colour);
};
// Constructot / destructor
template<typename T, typename K>
RedBlack<T, K>::RedBlack(bool noUpdates) {
	root = NULL;
	nodeSize = sizeof(rbNode);
	tSize = sizeof(T);
	updateOK = !noUpdates;
}
template<typename T, typename K>
RedBlack<T, K>::~RedBlack() {
	rbDeleteTree(root);
}
template<typename T, typename K>
bool RedBlack<T, K>::insertNode(K key, T* data) {
	memoryError = false;
	rbNode* temp = rbMakeNode(key, data);
	root = rbInsert(temp, root);
	rbPostInsertFix(temp, root);
	return memoryError;
}
template<typename T, typename K>
bool RedBlack<T, K>::deleteNode(K key) {
	rbNode* found = rbFind(key, root);
	if (found) {
		rbNode* del = rbPreDeleteFix(found, root);
		free(del->data);
		free(del);
		return true;
	}
	return false;
}
template<typename T, typename K>
T* RedBlack<T, K>::nodeSearch(K key) {
	return rbSearch(key, root);
}

template<typename T, typename K>
void RedBlack<T, K>::iterate(RBIter callBack) {
	rbRepeat = true;
	rbIterate(root, callBack);
}

template<typename T, typename K>
void RedBlack<T, K>::reverseIterate(RBIter callBack) {
	rbRepeat = true;
	rbRevIterate(root, callBack);
}
template<typename T, typename K>
void RedBlack<T, K>::levelIterate(RBIter callback) {
	rbRepeat = true;
	rbLvIterate(root, callback);
}

template<typename T, typename K>
int RedBlack<T, K>::blackHeight() {
	return rbHeight(root);
}
template<typename T, typename K>
int RedBlack<T, K>::nodeCount(bool blackOnly) {
	return rbCount(blackOnly, root);
}
//private methods
template<typename T, typename K>
typename RedBlack<T, K>::rbNode* RedBlack<T, K>::rbMakeNode(K key, T* data) {
	rbNode* temp = (rbNode*)malloc(nodeSize);
	if (temp == NULL) {
		memoryError = true;
	}
	temp->key = key;
	temp->left = temp->right = temp->parent = NULL;
	temp->colour = RED;
	temp->data = malloc(tSize);
	if (temp->data) {
		memcpy(temp->data, data, tSize);
	}
	else {
		memoryError = true;
	}
	return temp;
}
template<typename T, typename K>
typename RedBlack<T, K>::rbNode* RedBlack<T, K>::rbInsert(rbNode* node, rbNode* leaf) {
	if (leaf == NULL) {
		return node;
	}
	if (node->key == leaf->key) {
		if (updateOK) {
			memcpy(leaf->data, node->data, tSize);
			free(node->data);
			free(node);
		}
		else {
			memoryError = true; // well sort of
		}
		return leaf;
	}
	if (node->key < leaf->key) {
		leaf->left = rbInsert(node, leaf->left);
		leaf->left->parent = leaf;
	}
	else if (node->key > leaf->key) {
		leaf->right = rbInsert(node, leaf->right);
		leaf->right->parent = leaf;
	}
	return leaf;
}
template<typename T, typename K>
void RedBlack<T, K>::rbPostInsertFix(rbNode* node, rbNode*& root) {
	rbNode* grandMa = NULL;
	rbNode* aunty = NULL;
	while (node != root && node->parent->colour == RED) {
		grandMa = node->parent->parent;
		if (node->parent == grandMa->left) {
			aunty = grandMa->right;
			if (aunty && aunty->colour == RED) {
				node->parent->colour = BLACK;
				aunty->colour = BLACK;
				grandMa->colour = RED;
				node = grandMa;
			}
			else {
				if (node == node->parent->right) {
					node = node->parent;
					rbRotateLeft(node, root);
				}
				node->parent->colour = BLACK;
				grandMa->colour = RED;
				rbRotateRight(grandMa, root);
			}
		}
		else {
			aunty = grandMa->left;
			if (aunty && aunty->colour == RED) {
				node->parent->colour = BLACK;
				aunty->colour = BLACK;
				grandMa->colour = RED;
				node = grandMa;
			}
			else {
				if (node == node->parent->left) {
					node = node->parent;
					rbRotateRight(node, root);
				}
				node->parent->colour = BLACK;
				grandMa->colour = RED;
				rbRotateLeft(grandMa, root);
			}
		}
	}
	root->colour = BLACK;
}
template<typename T, typename K>
typename RedBlack<T, K>::rbNode* RedBlack<T, K>::rbFind(K key, rbNode* leaf) {
	if (leaf) {
		if (leaf->key == key) {
			return leaf;
		}
		if (key < leaf->key) {
			return rbFind(key, leaf->left);
		}
		return rbFind(key, leaf->right);
	}
	return NULL;
}
template<typename T, typename K>
typename RedBlack<T, K>::rbNode* RedBlack<T, K>::rbPreDeleteFix(rbNode* node, rbNode*& root) {
	rbNode* nodeToDelete = node;
	rbNode* child = NULL;
	rbNode* parentNode = NULL;
	if (nodeToDelete->left == NULL) {    // node has one or no children
		child = nodeToDelete->right;       // which might be NULL
	}
	else {
		if (nodeToDelete->right == NULL)   // node has one child
			child = nodeToDelete->left;      // child is not NULL
		else {                         // node has two children
			nodeToDelete = rbMinimum(nodeToDelete->right);
			// so we reset to the minimum key node
			// from the right child (so next largest key)
			child = nodeToDelete->right;
		}
	}
	if (nodeToDelete != node) {
		// if the nodeToDelete got changed above
		node->left->parent = nodeToDelete;
		nodeToDelete->left = node->left;
		if (nodeToDelete != node->right) {
			parentNode = nodeToDelete->parent;
			if (child) {
				child->parent = nodeToDelete->parent;
			}
			nodeToDelete->parent->left = child;
			nodeToDelete->right = node->right;
			node->right->parent = nodeToDelete;
		}
		else {
			parentNode = nodeToDelete;
		}
		if (root == node) {
			root = nodeToDelete;
		}
		else if (node->parent->left == node) {
			node->parent->left = nodeToDelete;
		}
		else {
			node->parent->right = nodeToDelete;
		}
		nodeToDelete->parent = node->parent;
		std::swap(nodeToDelete->colour, node->colour);
		nodeToDelete = node;
		// nodeToDelete now deinately points to node to be deleted
	}
	else {
		parentNode = nodeToDelete->parent;
		if (child) {
			child->parent = nodeToDelete->parent;
		}
		if (root == node) {
			root = child;
		}
		else {
			if (node->parent->left == node) {
				node->parent->left = child;
			}
			else {
				node->parent->right = child;
			}
		}
	}
	if (nodeToDelete->colour != RED) {
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
template<typename T, typename K>
void RedBlack<T, K>::rbRotateLeft(rbNode* node, rbNode*& root) {
	rbNode* child = node->right;
	node->right = child->left;
	if (child->left) {
		child->left->parent = node;
	}
	child->parent = node->parent;
	if (node == root) {
		root = child;
	}
	else if (node == node->parent->left) {
		node->parent->left = child;
	}
	else {
		node->parent->right = child;
	}
	child->left = node;
	node->parent = child;
}
template<typename T, typename K>
void RedBlack<T, K>::rbRotateRight(rbNode* node, rbNode*& root) {
	rbNode* child = node->left;
	node->left = child->right;
	if (child->right != 0) {
		child->right->parent = node;
	}
	child->parent = node->parent;
	if (node == root) {
		root = child;
	}
	else if (node == node->parent->right) {
		node->parent->right = child;
	}
	else {
		node->parent->left = child;
	}
	child->right = node;
	node->parent = child;
}
template<typename T, typename K>
T* RedBlack<T, K>::rbSearch(K key, rbNode* leaf) {
	if (leaf) {
		if (leaf->key == key) {
			return (T*)leaf->data;
		}
		if (key < leaf->key) {
			return rbSearch(key, leaf->left);
		}
		return rbSearch(key, leaf->right);
	}
	return NULL;
}
template<typename T, typename K>
void RedBlack<T, K>::rbIterate(rbNode* leaf, RBIter callBack) {
	if (leaf && rbRepeat) {
		rbIterate(leaf->left, callBack);
		if (rbRepeat) {
			//cout << leaf->colour << ' ';
			printColour(Colour(leaf->colour));
			rbRepeat = callBack(leaf->key, (T*)leaf->data);
			rbIterate(leaf->right, callBack);
		}
	}
}
template<typename T, typename K>
void RedBlack<T,K>::printColour(Colour e){
	switch (e) {
		case RED:cout << "RED "; break;
		case BLACK:cout << "Black "; break;
	}
}
template<typename T, typename K>
void RedBlack<T, K>::rbLvIterate(rbNode* leaf,RBIter callback) {
	if (leaf&&rbRepeat) {
		if (rbRepeat){
			//cout << leaf->colour << ' ';
			printColour(Colour(leaf->colour));
			rbRepeat = callback(leaf->key, (T*)leaf->data);
		}
		rbLvIterate(leaf->left, callback);
		rbLvIterate(leaf->right, callback);
	}

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
typename RedBlack<T, K>::rbNode* RedBlack<T, K>::rbMinimum(rbNode* node) {
	while (node->left) {
		node = node->left;
	}
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
template<typename T, typename K>
int RedBlack<T, K>::rbCount(bool blackOnly, rbNode* leaf) {
	if (leaf) {
		int rVal = 1;
		if (blackOnly && leaf->colour == RED) { rVal--; }
		return rVal + rbCount(blackOnly, leaf->left) + rbCount(blackOnly, leaf->right);
	}
	return 0;
}
template<typename T, typename K>
void RedBlack<T, K>::rbDeleteTree(rbNode* leaf) {
	if (leaf) {
		rbDeleteTree(leaf->left);
		rbDeleteTree(leaf->right);
		free(leaf->data);
		free(leaf);
	}
}
#endif