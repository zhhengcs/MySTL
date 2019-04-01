template<typename T>
class LinkedList
{
public:
	using Comparator=int(*)(T*,T*);
	class iterator;
	LinkedList();
	~LinkedList();
	bool push_front(T*);
	bool push_back(T*);
	void clear();
	bool empty();
	iterator begin();
	iterator end();
	iterator rbegin();
	iterator rend();
	T* front();
	T* back();
	void pop_back();
	void pop_front();
	iterator insert(iterator,T*);
	void sort();
	void sort(Comparator);
	void reverse();
	size_t size();
	private:
	size_t tSize,nSize,lSize;
	struct lNode
	{
		lNode* next;
		lNode* back;
		T* data;
		lNode() {
			next = back = NULL;
			data = NULL;
		}
		~lNode()
		{
			next = back = NULL;
			delete data;
			delete next;
			delete back;
		}
	};

	lNode* start=NULL;
	lNode* tail=NULL;
	void deleteList();
	lNode* createNode(const T*);
};

template<typename T>
class LinkedList<T>::iterator
{
public:
	iterator(typename LinkedList<T>::lNode* pos, bool forward)
	{
		this->pos = pos;
		this->forward = forward;
	}
	T operator*() const { return *(pos->data); }
	iterator& operator++(){
		if(forward)
			pos=pos->next;
		else
			pos=pos->back;
		return *this;
	}
	iterator& operator++(int){
		if(forward)
			pos=pos->next;
		else
			pos=pos->back;
		return *this;
	}
	iterator& operator--(){
		if(forward)
			pos=pos->back;
		else
			pos=pos->next;
		return *this;
	}
	iterator& operator--(int){
		if(forward)
			pos=pos->back;
		else
			pos=pos->next;
		return *this;
	}
	bool operator!=(const iterator& a){
		return this->pos!=(a.pos);
	}
	typename LinkedList<T>::lNode *pos;
	bool forward;
};

template<typename T>
LinkedList<T>::LinkedList(){
	tSize=sizeof(T);
	nSize=sizeof(lNode);
	lSize=0;
}
template<typename T>
LinkedList<T>::~LinkedList(){
	deleteList();
}
template<typename T>
bool LinkedList<T>::push_front(T* data)
{
	lNode *p = createNode(data);
	if(p==NULL)
		return false;
	if(start)
		start->back=p;
	p->next=start;
	start = p;

	if(tail==NULL)
		tail=p;

	p->next = start;
	lSize++;
	return true;
}
template<typename T>
bool LinkedList<T>::push_back(T* data)
{
	lNode *p = createNode(data);
	if(p==NULL)
		return false;
	if(tail==NULL)
	{
		start=tail=p;
	}
	else
	{
		p->back=tail;
		tail = tail->next = p;
	}
	lSize++;
	return true;
}

template<typename T>
typename LinkedList<T>::iterator LinkedList<T>::insert(iterator it,T* data)
{
	if(it.pos&&it.pos!=start)
	{
		lNode* newNode = createNode(data);
		if(newNode!=NULL){
			newNode->back=it.pos->back;
			newNode->next=it.pos;
			it.pos->back=newNode;
			newNode->back->next=newNode;
			iterator newIter(newNode,it.forward);
			lSize++;
			return newIter;
		}
	}
	else{
		//
		if(!it.foward||it.pos==start){
			if(push_front(data)){
				iterator newIter(start,it.forward);
				return newIter;
			}
		}
		else {
			if(push_back(data)){
				iterator newIter(tail,it.forward);
				return newIter;
			}
		}
	}
	return it.forward()?end():rend();
}
template<typename T>
void LinkedList<T>::clear()
{
	deleteList();
}
template<typename T>
bool LinkedList<T>::empty()
{
	return (start==NULL);
}

template<typename T>
size_t LinkedList<T>::size()
{
	return lSize;
}
template<typename T>
typename LinkedList<T>::iterator LinkedList<T>::begin()
{
	return iterator(start,true);
}
template<typename T>
typename LinkedList<T>::iterator LinkedList<T>::end()
{
	return iterator(NULL,true);
}

template<typename T>
typename LinkedList<T>::iterator LinkedList<T>::rbegin() 
{
	return iterator(tail, false);
}
template<typename T>
typename LinkedList<T>::iterator LinkedList<T>::rend() {
	return iterator(NULL, false);
}
template<typename T>
T* LinkedList<T>::front()
{
	if(start)
		return start->data;
	else
		return NULL;
}
template<typename T>
T* LinkedList<T>::back()
{
	if(tail)
		return tail->data;
	else
		return NULL;
}

template<typename T>
void LinkedList<T>::pop_front()
{
	if(start)
	{
		lNode* p = start;
		start = p->next;
		start->back = NULL;
		delete p;
		lSize--;
	}
}
template<typename T>
void LinkedList<T>::pop_back()
{
	if(tail)
	{
		lNode* p = tail;
		if(tail->back){
			tail = p->back;
			tail->next = NULL;
			delete p;
			lSize--;
		}else{
			start = tail = NULL;
			delete p;
			lSize = 0;
		}
	}
}

template<typename T>
void LinkedList<T>::reverse() 
{
	iterator fw = begin();
	iterator bw = rbegin();
	T* temp;
	for (; fw.pos != bw.pos && fw.pos->back != bw.pos; fw++, bw++) {
		temp = bw.pos->data;
		bw.pos->data = fw.pos->data;
		fw.pos->data = temp;
	}
}
template<typename T>
void LinkedList<T>::sort(Comparator comFunc)
{
	lNode *nLoop,*nTest;
	T* temp;
	bool swp;
	if(tail == start)
		return;
	nTest = start->next;
	while(nTest){
		if(comFunc(nTest->data,nTest->back->data)<0){
			swp=true;
			nLoop = start;
			while(swp){
				swp = false;
				for(;nLoop!=nTest;nLoop=nLoop->next)
				{
					if(comFunc(nTest->data,nLoop->data)<0)
					{
						temp=nLoop->data;
						nLoop->data=nTest->data;
						nTest->data = temp;
						swp = true;
						break;
					}
				}
			}
		}
		nTest = nTest->next;
	}
}

template<typename T>
void LinkedList<T>::sort()
{
	lNode *nTest,*nLoop;
	T* temp;
	bool swp;
	if(start == tail)
		return;
	nTest = start->next;
	while(nTest!=NULL)
	{
		if(*nTest->data < *nTest->back->data)
		{
			swp = true;
			nLoop = start;
			while(swp){
				swp = false;
				for(;nLoop!=nTest;nLoop=nLoop->next){
					if(*nTest->data < *nLoop->data){
						temp = nLoop->data;
						nLoop->data = nTest->data;
						nTest->data = temp;
						swp = true;
						break;
					}
				}
			}
		}
		nTest = nTest->next;
	}
}

template<typename T>
typename LinkedList<T>::lNode* LinkedList<T>::createNode(const T* data)
{
	lNode* p = new lNode;
	if(p!=NULL){
		p->data = new T;
		if (p->data == NULL)
			return NULL;
		memcpy(p->data, data, tSize);
	//	p->next = NULL;
		if(p->data == NULL)
			return NULL;
	}
	return p;
}

template<typename T>
void LinkedList<T>::deleteList()
{
	lNode* p=start;
	while(p!=NULL)
	{
		start = start->next;
		
		delete p;
		p = start;
	}
	start = tail = NULL;
	lSize = 0;
}