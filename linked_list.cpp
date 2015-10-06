#include <iostream>
using std::cout;
using std::endl;

struct Node{
	int data;
	struct Node *next;
};
typedef struct Node Node;

void print(Node *head){
	if (head){
		cout << head->data << " -> ";
		Node *n = head;
		while (n->next){
			n = n->next;
			cout << n->data << " -> ";
		}
	}
	cout << "NULL\n";
}


void print_reverse(Node *head){
	if (head != NULL){
		// print last one and get the size
		int size = 0;
		Node * n = head;
		while (n->next != NULL){
			n = n->next;
			++size;
		}
		cout << n->data << " -> ";

        // print the other ones
		while (size > 0){
			n = head;
			for (int k=1; k <size; ++k){
				n = n->next;
			}
			cout << n->data << " -> ";
			--size;
		}
	}
	cout << "NULL\n";
}

Node* push_front(Node *head,int data){
	Node *n = new Node;
	n->data = data;
	n->next = head;
	return n;
}


Node* push_back(Node *head,int data) {
	Node *n = new Node;
	n->data = data;
	n->next = NULL;

	if (!head){
		return n;
	} else {
		Node *tmp = head;
		while (tmp->next){
			tmp = tmp->next;
		}
		tmp->next = n;
		return head;
	}
}


Node* insert_at(Node *head, int data, int position){
	Node * n = new Node;
	n->data = data;

	if (position == 0){
		n->next = head;
		return n;
	}

	if ((head == NULL) && (position > 0)){
		cout << "Empty list, can't insert data in position "<< position << endl;
		return NULL;
	}

	Node * previous = NULL;
	Node * current = head;
	for (int k=0; k<position; ++k){
		previous = current;
		if (previous == NULL){
			cout <<  "can't insert data in position "<< position;
			cout << ", list size is only: "<< k << endl;
			return head;
		}
		current = previous->next;
	}
	n->next = current;
	previous->next = n;
	return head;
}

Node* delete_at(Node *head, int position){
	if (head == NULL)
		return NULL;

	Node *next = head->next;

	if (position == 0){
		delete head;
		return next;
	} else {
		head->next=delete_at(next, position -1);
		return head;
	}
}

Node* reverse(Node *head){
	Node *prev = NULL;
	Node *current = head;
	Node *next;
	while (current != NULL){
		next  = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	return prev;
}

void destroy(Node ** head){
	while(*head=delete_at(*head, 0)){}
	*head = NULL;
}

int main(){

	Node *list = NULL;
	for (int k=1; k<5; ++k){
		list = push_front(list, k);
		list = push_back(list, k);
	}
	cout << "Initial list: ";
	print(list);
	cout << endl;

	list = delete_at(list, 0);
	list = delete_at(list, 0);
	list = delete_at(list, 8);
	list = delete_at(list, 2);
	list = delete_at(list, 1);
	cout << "After Deletions: ";
	print(list);
	cout << endl;

	cout << "let's reverse it: ";
	list = reverse(list);
	print(list);
	cout << endl;

	list = insert_at(list, 0, 0);
	list = insert_at(list, 0, 0);
	list = insert_at(list, 12, 12);
	list = insert_at(list, 12, 3);
	cout << "After Insertion at certain positions: ";
	print(list);
	cout << endl;

	cout << "print_reverse: ";
	print_reverse(list);
	cout << endl;

	cout << "destroy the list: ";
	destroy(&list);
	print(list);
	cout << endl;


	return 0;
}
