#include <iostream>
#include "table.h"
using namespace std;

//helper function
EntryNode* nodeCreate(string* a){
	EntryNode* p = new EntryNode;
	p->entry = a;
	p->next = nullptr;
	p->prev = nullptr;
	return p;
}

Intermediate::Intermediate(const Table &table){
	attrs = table.attrs;
	numAttrs = table.numAttrs;

	if(table.numEntries == 0){
		head = nullptr;
		tail = nullptr;
	}
	else if(table.numEntries == 1){
		head = nodeCreate(table.entries[0]);
		tail = head;
		head->next = nullptr;
		head->prev = nullptr;
	}
	else{
		head = nodeCreate(table.entries[0]);

		EntryNode* b4 = nullptr;
		EntryNode* curr = head;

		for (int j = 1; j < table.numEntries; ++j) {
			curr->next = nodeCreate(table.entries[j]);
			curr-> prev = b4;
			b4 = curr;
			curr = curr->next;
		}
		curr->prev = b4;
		curr->next = nullptr;

		tail = curr;
	}
}

//helper function
void deleteNode(EntryNode*& head){
	if((head == nullptr)){
		return;
	}
	deleteNode(head->next);

	head->entry = nullptr;
	head->prev = nullptr;
	head->next = nullptr;
	delete head;
	head = nullptr;

	return;
}

Intermediate::~Intermediate(){
	attrs = nullptr;
	deleteNode(head);
	head = nullptr;
	tail = nullptr;
}

//helper function
int attrIndex(const string &attr, const string* attrs, int limit){
	int index = -1;

	for(int i = 0; i < limit; i ++){
		if (attrs[i] == attr)
			index = i;
	}

	return index;
}

//helper function
void deletecurr(EntryNode*& curr){
	curr->entry = nullptr;
	curr->next = nullptr;
	curr->prev = nullptr;
	delete curr;
	return;
}

Intermediate& Intermediate::where(const string &attr, enum compare mode, const string &value){

	EntryNode* b4 = nullptr;
	EntryNode* curr = head;
	int index = attrIndex(attr,attrs,numAttrs);

	if(attrIndex(attr,attrs,numAttrs) == -1)
		return *this;


	if(mode == EQ){
		while((head->entry[index] != value)){
			if(head == nullptr)
				break;
			head = head -> next;

			b4 = curr;
			curr = curr->next;
			deletecurr(b4);
		}


		if (head == nullptr){
			//head = nullptr;
			tail = nullptr;
			return *this;
		}

		head->prev=nullptr;

		for (curr = head, b4 = nullptr; curr->next != nullptr; ){
			if(curr->entry[index] != value){
				b4 = curr->prev;
				b4->next = curr->next;
				curr->next->prev = b4;
				b4 = curr;
				curr = curr->next;
				deletecurr(b4);
			}
			else{
				b4 = curr;
				curr = curr->next;
			}
		}

		if(curr->entry[index] == value){
			curr->next = nullptr;
			tail = curr;
		}

		else{
			tail = curr -> prev;
			tail->next = nullptr;
			deletecurr(curr);
		}
	}

	if(mode == CONTAINS){
		while((head->entry[index].string::find(value) == string::npos)){
			if(head == nullptr)
				break;
			head = head -> next;

			b4 = curr;
			curr = curr->next;
			deletecurr(b4);
		}

		if (head == nullptr){
			//head = nullptr;
			tail = nullptr;
			return *this;
		}

		head->prev=nullptr;

		for (curr = head, b4 = nullptr; curr->next != nullptr; ){
			if((curr->entry[index].find(value) == string::npos)){
				b4 = curr->prev;
				b4->next = curr->next;
				curr->next->prev = b4;
				b4 = curr;
				curr = curr->next;
				deletecurr(b4);
			}
			else{
				b4 = curr;
				curr = curr->next;
			}
		}
		if((curr->entry[index].find(value) != string::npos)){
			curr->next = nullptr;
			tail = curr;
		}

		else{
			tail = curr -> prev;
			tail->next = nullptr;
			deletecurr(curr);
		}
	}

	return *this;
}


//helper function
void swapEntry(EntryNode*& a, EntryNode*& b){
	string* temp = a->entry;
	a->entry = b->entry;
	b->entry = temp;
}

Intermediate& Intermediate::orderBy(const string &attr, enum order order){

	EntryNode* b4 = nullptr;
	EntryNode* curr = head;
	int index{attrIndex(attr,attrs,numAttrs)};

	if(index == -1)
		return *this;

	if((order == ASCENDING)){
		for(b4 = head; b4 != nullptr; b4 = b4->next){
			for(curr = b4->next; curr != nullptr; curr = curr->next){
				if((b4->entry[index]).compare(curr->entry[index]) <= 0)
					swapEntry(curr, b4);
			}
		}

		for (curr = tail, b4 = head; !((b4->prev == curr) || (b4 == curr)); b4 = b4->next, curr = curr->prev){
			swapEntry(curr, b4);
		}
		return *this;
	}

	if((order == DESCENDING)){
		for(b4 = head; b4 != nullptr; b4 = b4->next){
			for(curr = b4->next; curr != nullptr; curr = curr->next){
				if(b4->entry[index].compare(curr->entry[index])>=0)
					swapEntry(curr, b4);
			}
		}
		for (curr = tail, b4 = head; !((b4->prev == curr) || (b4 == curr)); b4 = b4->next, curr = curr->prev){
				swapEntry(curr, b4);
		}
		return *this;
	}
}

Intermediate& Intermediate::limit(unsigned int limit){
	if(head == nullptr)
		return *this;
	if (limit == 0){
		deleteNode(head);
		head = nullptr;
		tail = nullptr;
		return *this;
	}
	else{
		EntryNode* b4 = nullptr;
		EntryNode* curr = head;
		for(unsigned int i = 0; i < limit; ++i){
			b4 = curr;
			curr = curr-> next;
		}
		deleteNode(curr);
		b4->next = nullptr;
		b4 = tail;
	}
	return *this;
}

void Intermediate::update(const string &attr, const string &new_value) const{

	EntryNode* curr = head;
	int index{attrIndex(attr, attrs, numAttrs)};

	if(index == -1){
		return;
	}
	else{
		for(; curr != nullptr; curr = curr->next)
			curr->entry[index] = new_value;
	}
}

void Intermediate::select(const string *attrs, int numAttrs) const{

	if(this->numAttrs == 0)
		return;
	if(head == nullptr){
		for(int i = 0; i < this->numAttrs; ++i)
			if(i == this->numAttrs - 1)
				cout << this->attrs[i];
			else
				cout << this->attrs[i] << " | ";
		cout << endl;
		return;
	}

	EntryNode* curr = head;
	int index[numAttrs];
	unsigned int maxLength[this->numAttrs];

	for(int i = 0; i < this->numAttrs; ++i){
		curr = head;
		for(maxLength[i] = this->attrs[i].length(); curr != nullptr; curr = curr->next){
			maxLength[i] = (maxLength[i] > curr->entry[i].length())? maxLength[i]: curr->entry[i].length();
		}
	}

	if (attrs == nullptr || numAttrs >= this->numAttrs){
		numAttrs = this->numAttrs;
		for(int i = 0; i < numAttrs; i ++){
			index[i] = i;
		}
	}

	else{
		for(int i = 0; i < numAttrs; i++){
			index[i] = attrIndex(attrs[i], this->attrs, this->numAttrs);
		}
	}


	for(int i = 0; i < numAttrs; i ++){
		if (index[i] == -1)
			continue;
		if(i == numAttrs - 1)
			cout << _left_pad_until(this->attrs[index[i]], maxLength[index[i]]);
		else
			cout << _left_pad_until(this->attrs[index[i]], maxLength[index[i]]) << " | ";
	}

	cout << endl;

	for (curr = head; curr != nullptr; curr = curr->next){
		for(int i = 0; i < numAttrs; i ++){
			if (index[i] == -1)
				continue;
			if(i == numAttrs - 1)
				cout << _left_pad_until(curr->entry[index[i]], maxLength[index[i]]);
			else
				cout << _left_pad_until(curr->entry[index[i]], maxLength[index[i]]) << " | ";
		}
		cout << endl;
	}

	return;
}
