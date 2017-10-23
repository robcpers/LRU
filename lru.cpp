/*	Simple implemenation of an LRU cache with hashmap and doubly linked list
	Based off of:	https://alaindefrance.wordpress.com/2014/10/05/lru-cache-implementation/

*/



#include <iostream>
#include <unordered_map>



// basic of doubly linked list (with a key and data
class 	doubleListNode{
	public:
		int key;
		int data;
		doubleListNode *prev;
		doubleListNode *next;
};



// LRU cache class with get and set functions
class 	LRU{
	public:
		LRU(int capacity);
		~LRU();
	private:
		int capacity;
		doubleListNode *head;
		doubleListNode *end;
		std::unordered_map<int, doubleListNode *> hmap;
		void	addToHead(doubleListNode *node);
		void	removeNode(doubleListNode *node);
		void	moveToHead(doubleListNode *node);
		bool	removeLast();

	public:
		void	set(int key, int data);
		int	get(int key);

};



// constructor
LRU::LRU(int capacity = 3){
	this->capacity = capacity;
	this->head = NULL;
	this->end = NULL;
}


// destructor
LRU::~LRU(){
	while(removeLast()){}
	this->hmap.clear();
}




// adds a node to the start of the list
void	LRU::addToHead(doubleListNode *node){
	// if empty list
	if(this->head == NULL){
		this->head = node;
		this->end = node;
		node->next = NULL;
		node->prev = NULL;
	}
	// else rearrange
	else{
		this->head->prev = node;
		node->next = this->head;
		this->head = node;
	}

	return;
}



// removes a particular node if present
void	LRU::removeNode(doubleListNode *node){
	if(this->head == NULL || node == NULL){
		return;
	}

	// if head and only item
	if(this->head == node && this->end == node){
		this->head = NULL;
		this->end = NULL;
	}
	// head and >1 item
	else if(this->head == node){
		this->head = node->next;
		node->next->prev = NULL;
	}
	// end
	else if(this->end == node){
		this->end = node->prev;
		node->prev->next = NULL;
	}
	// middle
	else{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

}



// moves a node to start of list (eg if just accessed)
void	LRU::moveToHead(doubleListNode *node){
	this->removeNode(node);
	this->addToHead(node);

	return;
}



// remove  last (eg if cache is full)
bool	LRU::removeLast(){
	doubleListNode *node = this->end;
	if(node != NULL){
		this->removeNode(node);
		delete node;
		return true;
	}

	return false;
}



// set a new value for cache
void	LRU::set(int key, int data){
	auto it = this->hmap.find(key);

	// if already there
	if(it != hmap.end()){
		doubleListNode *node = it->second;
		this->moveToHead(node);
		node->data = data;
		return;
	
	}
	// else if full
	else if(this->hmap.size() == this->capacity){
		int oldKey = this->end->key;
		this->removeLast();
		this->hmap.erase(oldKey);
	}


	// then add in new node
	doubleListNode *node = new doubleListNode();
	node -> key = key;
	node -> data = data;
	this->addToHead(node);
	this->hmap.insert(std::make_pair(key, node));

	return;
}



// recall a value from cache
int	LRU::get(int key){

	auto it = this->hmap.find(key);

	// if here
	if(it != hmap.end()){
		doubleListNode *node = it->second;
		this->moveToHead(node);
		return node->data;
	}

	return -1;

}



int main(){

	// testing
	LRU *cache = new LRU();
	cache->set(2, 3);
	cache->set(3, 4);
	cache->set(4, 5);
	std::cout << cache->get(2) << "\n";
	cache->set(5, 6);
	std::cout << cache->get(2) << "\n";
	std::cout << cache->get(3) << "\n";
	std::cout << cache->get(4) << "\n";
	cache->set(6, 7);
	cache->set(7, 8);
	std::cout << cache->get(4) << "\n";
	std::cout << cache->get(2) << "\n";

	delete cache;

	return 0;
}
