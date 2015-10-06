#include <iostream>
#include <utility> // std::pair
#include <string>
#include <vector> // For Tree<T>::search_all that fill a vector
using namespace std;


#define UNIQUE 1  // Use this if you want Unique key (set instead of multiset)

#define DEBUG 1 // Few debug outputs


class Contact {
	string name;
	string surname;
	string phone;

	public:
		Contact(const string & aName, const string & aSurname="", const string & aPhone=""):
			name(aName), surname(aSurname), phone(aPhone){}

		Contact(const Contact& c):
			name(c.name), surname(c.surname), phone(c.phone){}

		string getName() const{return name;}
		string getSurname() const{return surname;}
		string getPhone() const{return phone;}

		string str() const{return name+" "+surname+": "+phone;}

		bool contains(const string &aStr){
			if ( name.find(aStr) != string::npos )
				return true;
			if (surname.find(aStr) != string::npos)
				return true;
			return false;
		}

		friend bool operator< (const Contact & c1, const Contact & c2);
		friend bool operator== (const Contact & c1, const Contact & c2);
		friend ostream & operator<< ( ostream &output, const Contact & c );
};


bool operator== (const Contact & c1, const Contact & c2){
	if ( (c1.surname == c2.surname) && (c1.name == c2.name) )
		return true;
	else
		return false;
}

// This will allow to order them in a Tree structure
bool operator< (const Contact & c1, const Contact & c2){
	if (c1.name < c2.name)
		return true;
	else if (c1.name == c2.name)
		return (c1.surname < c2.surname);
	else
		return false;
}

ostream & operator<< ( ostream &output, const Contact & c ) {
	output << c.str();
	return output;
}



template<class T> class Node{
	public:
		T data;
		Node *left;
		Node *right;

		Node(const T& aData):
			data(aData), left(NULL), right(NULL){}

		static void print(Node *aNode){
			if (aNode)
				cout << aNode->data << endl;
			else
				cout << "Node is NULL...\n";
		}
};


template<class T> class Tree {
	public:
		Tree():root(NULL), nb(0){}
		Tree(const T &aData):root(NULL), nb(0){insert(aData);}

		~Tree();

		int size() const {return nb;}

		void insert(const T & aData);

		void traverse_in_order( void (*anAction)(Node<T>* aLeaf) );
		void traverse_pre_order( void (*anAction)(Node<T>* aLeaf) );
		void traverse_post_order( void (*anAction)(Node<T>* aLeaf) );

		void print(){traverse_in_order(&Node<T>::print);}
		void print_pre_order(){traverse_pre_order(&Node<T>::print);}
		void print_post_order(){traverse_post_order(&Node<T>::print);}


		Node<T> *search(const T & aData );

		// In order to be used, the T class should contain this function:
		// bool T::contains(const string &aStr);
		void search_all(const string &str, vector<Node<T>* > &output );

	private:
		Node<T> *root;
		int nb;

		void insert_node(const T & aData, Node<T> *aLeaf
#ifdef UNIQUE
, Node<T> *aParent=NULL
#endif
		);

		void destroy_tree(Node<T> *aLeaf);

		void traverse_in_order( void (*anAction)(Node<T>* aNode), Node<T> *aLeaf );
		void traverse_pre_order( void (*anAction)(Node<T>* aNode), Node<T> *aLeaf );
		void traverse_post_order( void (*anAction)(Node<T>* aNode), Node<T> *aLeaf );

		void search_all(const string &str, vector<Node<T>* > &output, Node<T> *aLeaf );
		Node<T> *search(const T & aData, Node<T> *aLeaf );
};



template<class T> Node<T> *  Tree<T>::search(const T &aData){
	search(aData, root);
}


template<class T> Node<T> *  Tree<T>::search(const T &aData, Node<T> *aLeaf ){
	if (aLeaf == NULL)
		return NULL;

	if (aLeaf->data == aData)
		return aLeaf;
	else if (aData < aLeaf->data)
		search(aData, aLeaf->left);
	else
		search(aData, aLeaf->right);
}


template<class T> void  Tree<T>::search_all( const string &str, vector<Node<T>* > &output){
	search_all(str, output, root);
}


template<class T> void  Tree<T>::search_all(const string &str, vector<Node<T>* > &output, Node<T> *aLeaf ){
	if (aLeaf == NULL)
		return;

	if (aLeaf->left != NULL)
		search_all(str, output, aLeaf->left);

	if (aLeaf->data.contains(str)){
		output.push_back(aLeaf);
#ifdef DEBUG
		cout << "[debug][search_all]<str: " << str << "> MATCH: " << aLeaf->data.str() << endl;
#endif
	}

	if (aLeaf->right != NULL)
		search_all(str, output, aLeaf->right);
}


template<class T> void  Tree<T>::traverse_in_order( void (*anAction)(Node<T>* aNode)){
	traverse_in_order(anAction, root);
}


template<class T> void  Tree<T>::traverse_in_order( void (*anAction)(Node<T>* aNode), Node<T> *aLeaf ){
	if (aLeaf == NULL)
		return;

	if (aLeaf->left != NULL)
		traverse_in_order(anAction, aLeaf->left);

	anAction(aLeaf);

	if (aLeaf->right != NULL)
		traverse_in_order(anAction, aLeaf->right);
}

template<class T> void  Tree<T>::traverse_pre_order( void (*anAction)(Node<T>* aNode)){
	traverse_pre_order(anAction, root);
}


template<class T> void  Tree<T>::traverse_pre_order( void (*anAction)(Node<T>* aNode), Node<T> *aLeaf ){
	if (aLeaf == NULL)
		return;

	anAction(aLeaf);

	if (aLeaf->left != NULL)
		traverse_pre_order(anAction, aLeaf->left);

	if (aLeaf->right != NULL)
		traverse_pre_order(anAction, aLeaf->right);
}

template<class T> void  Tree<T>::traverse_post_order( void (*anAction)(Node<T>* aNode)){
	traverse_post_order(anAction, root);
}


template<class T> void  Tree<T>::traverse_post_order( void (*anAction)(Node<T>* aNode), Node<T> *aLeaf ){
	if (aLeaf == NULL)
		return;

	if (aLeaf->left != NULL)
		traverse_post_order(anAction, aLeaf->left);

	if (aLeaf->right != NULL)
		traverse_post_order(anAction, aLeaf->right);

	anAction(aLeaf);
}

template<class T> Tree<T>::~Tree(){
	destroy_tree(root);
}

template<class T> void Tree<T>::insert(const T & aData){
	if (root == NULL){
		root = new Node<T>(aData);
	} else {
		insert_node(aData, root);
	}
	++nb;
}


template<class T>  void Tree<T>::insert_node(const T & aData, Node<T>* aLeaf
#ifdef UNIQUE
, Node<T> *aParent
#endif
){
	if (aData < aLeaf->data){
		if (aLeaf->left != NULL){
			insert_node(aData, aLeaf->left
#ifdef UNIQUE
, aLeaf
#endif
			);
		} else {
			aLeaf->left = new Node<T>(aData);
		}
	}
// if UNIQUE, we replace existing matching node with new data
#ifdef UNIQUE
	else if (aData == aLeaf->data){
#ifdef DEBUG
		cout << "[insert_node], match found\n";
#endif
		Node<T> * newNode = new Node<T>(aData);
		newNode->left = aLeaf->left;
		newNode->right = aLeaf->right;

		// We are updating the root...
		if (aParent == NULL){
			root = newNode;
		} else {

			if (aParent->left && (aParent->left == aLeaf)){
				aParent->left = newNode;
#ifdef DEBUG
				cout << "[insert_node][match] node was left of parent\n";
#endif
			}
			else if (aParent->right && (aParent->right == aLeaf)){
				aParent->right = newNode;
#ifdef DEBUG
				cout << "[insert_node][match] node was right of parent\n";
#endif
			}
		}

		delete aLeaf;
		--nb;

#ifdef DEBUG
		cout << "[insert_node][match] node updated...\n";
#endif
	}
#endif
	else {
		if (aLeaf->right != NULL){
			insert_node(aData, aLeaf->right
#ifdef UNIQUE
, aLeaf
#endif
			);
		} else {
			aLeaf->right =new Node<T>(aData);
		}
	}
}


template<class T> void Tree<T>::destroy_tree(Node<T> *aLeaf){
	if( aLeaf != NULL){
		destroy_tree(aLeaf->left);
		destroy_tree(aLeaf->right);
#ifdef DEBUG
		cout << "[debug][destroy_tree] delete leaf: " << aLeaf->data << endl;
#endif
		delete aLeaf;
	}
}

template<class T> void print(vector<T> v){
	for (typename vector<T>::const_iterator it=v.begin(); it!=v.end(); ++it){
		cout << (*it)->data << endl;
	}
}


void try_search(Tree<Contact> &tree, const string & name, const string & surname=""){
	cout << "\nTry search("<< name << ", "<< surname << ")\n";
	Node<Contact> *res = tree.search(Contact(name, surname));
	if (res)
		cout << "Found: " << res->data << endl;
	else
		cout << "not found...\n";
}

int main(){

	Tree<Contact> tree;

	tree.insert(Contact("Matthieu", "Bruel", "07554433222"));
	tree.insert(Contact("Spilios", "Alexopoulos", "07222222222"));
	tree.insert(Contact("John", "Smith", "07111111111"));
	tree.insert(Contact("Yannis", "Papadopulos", "07987654321"));
	tree.insert(Contact("Bob", "Dupont", "07123456789"));
	tree.insert(Contact("Danae", "Rivera", "07555555555"));
	tree.insert(Contact("Pablo", "Tonto", "07666666666"));
	tree.insert(Contact("Barby", "", "07999999999"));


	cout << "Traverse pre order:\n";
	tree.traverse_pre_order(&Node<Contact>::print);

	cout << "\n\nTraverse post order:\n";
	tree.traverse_post_order(&Node<Contact>::print);

	cout << "\n\nContacts in order:\n";
	tree.print();
//	tree.traverse_in_order(&Node<Contact>::print);



	cout << "\n\nTry search_all(\"B\")\n";
	vector<Node<Contact>*> s;
	tree.search_all(string("B"), s);
	print(s);


	try_search(tree, "Barby");
	try_search(tree, "Matthieu");
	try_search(tree, "Matthieu", "Bruel");
	try_search(tree, "Pablo", "Tonto");


	cout << "Size of the Tree: " << tree.size() << endl;
#ifdef UNIQUE
	cout << "\n\nLet's try to enter an existing contact (should overwrite)\n";
	cout << "Let's change my details (Matthieu Bruel to phone number 07000000000)\n";
	tree.insert(Contact("Matthieu", "Bruel", "07000000000"));
	try_search(tree, "Matthieu", "Bruel");

	cout << "\nLet's change my details (Pablo Tonto to phone number 07777777777)\n";
	tree.insert(Contact("Pablo", "Tonto", "07777777777"));
	try_search(tree, "Pablo", "Tonto");
#else
	cout << "Let's insert a Duplicate for Matthieu Bruel with phone number 07000000000\n";
	cout << "It should now be twice in the Tree...\n";
	tree.insert(Contact("Matthieu", "Bruel", "07000000000"));
	tree.print();
#endif
	cout << "Size of the Tree: " << tree.size() << endl;


#ifdef DEBUG
	cout << "\n\nDestruction of the Tree\n";
#endif
	return 0;
}



