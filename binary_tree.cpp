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
		T *data;
		Node *left;
		Node *right;

		Node(const T& aData):
			data(NULL), left(NULL), right(NULL){data = new T(aData);}

		~Node(){
			if (data){
#ifdef DEBUG
				cout << "[debug][Node] Deleting data: " << *data << endl;
#endif
				delete data;
			}
		}

		static void print(Node *aNode){
			if (aNode)
				cout << *(aNode->data) << endl;
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
		int erase(const T & aData);

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

		int insert_node(const T & aData, Node<T> *aLeaf);

		void destroy_tree(Node<T> *aLeaf);

		void traverse_in_order( void (*anAction)(Node<T>* aNode), Node<T> *aLeaf );
		void traverse_pre_order( void (*anAction)(Node<T>* aNode), Node<T> *aLeaf );
		void traverse_post_order( void (*anAction)(Node<T>* aNode), Node<T> *aLeaf );

		void search_all(const string &str, vector<Node<T>* > &output, Node<T> *aLeaf );

		Node<T> *search(const T & aData, Node<T> *aLeaf );

		int erase(const T & aData, Node<T> *aLeaf, Node<T> *aParent=NULL);

		// replace aLeaf by aOtherLeaf (aParent must be the real parent of aLeaf)
		void replace(Node<T> *aLeaf, Node<T> *aParent, Node<T> *aOtherLeaf);

		// move aLeaf to the bottom aDirection of aDest
		void moveToBottom(Node<T> *aLeaf, Node<T> *aDest, const string & aDirection);

		// return the minimum from aStart
		Node<T>* getMinParent(Node<T> *const aStart);

		// update the data of aNode
		void updateNode(Node<T> *aNode, const T & aData);
};



template<class T> Node<T> * Tree<T>::search(const T &aData){
	search(aData, root);
}


template<class T> Node<T> *  Tree<T>::search(const T &aData, Node<T> *aLeaf ){
	if (aLeaf == NULL)
		return NULL;

	if (aData == *(aLeaf->data))
		return aLeaf;
	else if (aData < *(aLeaf->data))
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

	if (aLeaf->data->contains(str)){
		output.push_back(aLeaf);
#ifdef DEBUG
		cout << "[debug][Tree::search_all] <str: " << str << "> MATCH: " << aLeaf->data->str() << endl;
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
		++nb;
	} else {
		nb += insert_node(aData, root);
	}
}

template<class T> int Tree<T>::erase(const T & aData) {
	if (!root)
		return 0;

	int nbDeleted = erase(aData, root);
	nb-=nbDeleted;

	return nbDeleted;
}

template<class T> void Tree<T>::replace(Node<T> *aLeaf, Node<T> *aParent, Node<T> *aOtherLeaf) {
	if (aLeaf == root){
		root = aOtherLeaf;
#ifdef DEBUG
		cout << "[debug][Tree::replace] replacing root '"<< *(aLeaf->data) << "' by '";
		if (aOtherLeaf)
			cout << *(aOtherLeaf->data) << "'\n";
		else
			cout << " NULL '\n";
#endif
	} else if (aParent->left == aLeaf){
		aParent->left = aOtherLeaf;
#ifdef DEBUG
		cout << "[debug][Tree::replace] replacing parent '"<< *(aParent->data) << "' left side: '";
		cout << *(aLeaf->data) << "' by node: '";
		if (aOtherLeaf)
			cout << *(aOtherLeaf->data) << "'\n";
		else
			cout << " NULL '\n";
#endif
	} else if (aParent->right == aLeaf){
		aParent->right = aOtherLeaf;
#ifdef DEBUG
		cout << "[debug][Tree::replace] replacing parent '"<< *(aParent->data) << "' right side: '";
		cout << *(aLeaf->data) << "' by node: '";
		if (aOtherLeaf)
			cout << *(aOtherLeaf->data) << "'\n";
		else
			cout << " NULL '\n";
#endif
	} else {
		cout << "[debug][Tree::replace] Error, node: '"<< *(aParent->data) << "' is not parent of '" << *(aLeaf->data) << "'\n";
	}
}


template<class T> void Tree<T>::moveToBottom(Node<T> *aLeaf, Node<T> *aDest, const string & aDirection) {
	// we move aLeaf to the bottom left of aDest
	if (aDirection.compare("left") == 0 ){
		if (!aDest->left){
#ifdef DEBUG
			cout << "[debug][Tree::moveToBottom] moving node '" << *(aLeaf->data) << "' on the left side of '" << *(aDest->data) << "'\n";
#endif
			aDest->left = aLeaf;
		} else
			moveToBottom(aLeaf, aDest->left, aDirection);

	// we move aLeaf to the bottom right of aDest
	} else {
		if (!aDest->right){
#ifdef DEBUG
			cout << "[debug][Tree::moveToBottom] moving node '" << *(aLeaf->data) << "' on the right side of '" << *(aDest->data) << "'\n";
#endif
			aDest->right = aLeaf;
		} else
			moveToBottom(aLeaf, aDest->right, aDirection);
	}
}

template<class T> Node<T>* Tree<T>::getMinParent(Node<T> *aStart){
	if (!aStart)
		return NULL;
	if (!aStart->left)
		return NULL;

	if (!aStart->left->left)
		return aStart;
	else
		return getMinParent(aStart->left);
}

template<class T> void Tree<T>::updateNode(Node<T> *aNode, const T & aData){
	if (!aNode)
		return;

	delete aNode->data;
	aNode->data = new T(aData);
}


template<class T> int Tree<T>::erase(const T & aData, Node<T> *aLeaf, Node<T> *aParent) {
	if (!aLeaf)
		return 0;

	int nbDeleted = 0;
	if (aData == *(aLeaf->data)){

#ifndef UNIQUE
		// multiset => multiple values where inserted on the right,
		// => we first delete the farest ones
		nbDeleted += erase(aData, aLeaf->right, aLeaf);
#endif

		// if nothing left, the right leaf take its place
		if (!aLeaf->left){
			replace(aLeaf, aParent, aLeaf->right);
			delete aLeaf;
		}
		// if nothing right, left take its place
		else if (!aLeaf->right){
			replace(aLeaf, aParent, aLeaf->left);
			delete aLeaf;
		}


		// - Replace the data of the Node by the minimum of its right side
		// (i.e: the most left starting from the leaf->right)
		// - remove this minimum
		else{
			Node<T> *minParent = getMinParent(aLeaf->right);

			// The minimum on the aLeaf right side is its right side (that's why no parent
			if (!minParent){
#ifdef DEBUG
			cout << "[debug][Tree::erase] Node '" << *(aLeaf->data) << "' has 2 leaves...\n";
			cout << "[debug][Tree::erase] and the minimum on the right its his right leaf: '" << *(aLeaf->right->data) << "'\n";
#endif
				updateNode(aLeaf, *(aLeaf->right->data));
				delete aLeaf->right;
				aLeaf->right = NULL;

			// We got the minimum parent => we delete it and set the parent pointer to NULL
			} else {
#ifdef DEBUG
			cout << "[debug][Tree::erase] Node '" << *(aLeaf->data) << "' has 2 leaves...\n";
			cout << "[debug][Tree::erase] The minimum from its right branch is: '" << *(minParent->left->data) << "'\n";
#endif
				updateNode(aLeaf, *(minParent->left->data));
				delete minParent->left;
				minParent->left = NULL;
			}


#ifdef DEBUG
			cout << "[debug][Tree::erase] New node value: '" << *(aLeaf->data) << "'\n";
#endif
		}



/*  WRONG ALGO.... THE HEIGHT WILL COULD INCREASE BY ONE INSTEAD OF STAYING THE SAME...

		// If the leaf has both children left and right,
		// Deleting a node from the left of parent means:
		//   - its right leaf takes takes his position
		//   - its left leaf goes at bottom left of its right side
		else if (!aParent || aParent->left == aLeaf){
			replace(aLeaf, aParent, aLeaf->right);

			moveToBottom(aLeaf->left, aLeaf->right, "left");
			delete aLeaf;



		// If the leaf has both children left and right,
		// Deleting a node from the right of parent means:
		//   - its left leaf takes takes his position
		//   - its right leaf goes at bottom right of its right side
		} else {
			replace(aLeaf, aParent, aLeaf->left);

			moveToBottom(aLeaf->right, aLeaf->left, "right");
			delete aLeaf;
		}

*/

		++nbDeleted;

	} else if (aData < *(aLeaf->data)){
		nbDeleted += erase(aData, aLeaf->left, aLeaf);
	} else {
		nbDeleted += erase(aData, aLeaf->right, aLeaf);
	}

	return nbDeleted;
}

template<class T>  int Tree<T>::insert_node(const T & aData, Node<T>* aLeaf){
	if (aData < *(aLeaf->data)){
		if (aLeaf->left != NULL){
			insert_node(aData, aLeaf->left);
		} else {
			aLeaf->left = new Node<T>(aData);
			return 1;
		}
	}
// if UNIQUE, we replace existing matching node with new data
#ifdef UNIQUE
	else if (aData == *(aLeaf->data)){
#ifdef DEBUG
		cout << "[debug][Tree::insert_node], match found\n";
#endif
		updateNode(aLeaf, aData);
		return 0;

#ifdef DEBUG
		cout << "[debug][Tree::insert_node][match] node updated...\n";
#endif
	}
#endif
	else {
		if (aLeaf->right != NULL){
			insert_node(aData, aLeaf->right);
		} else {
			aLeaf->right =new Node<T>(aData);
			return 1;
		}
	}
}


template<class T> void Tree<T>::destroy_tree(Node<T> *aLeaf){
	if( aLeaf != NULL){
		destroy_tree(aLeaf->left);
		destroy_tree(aLeaf->right);
#ifdef DEBUG
		cout << "[debug][Tree::destroy_tree] delete leaf: " << *(aLeaf->data) << endl;
#endif
		delete aLeaf;
	}
}

template<class T> void print(vector<T> v){
	for (typename vector<T>::const_iterator it=v.begin(); it!=v.end(); ++it){
		cout << *((*it)->data) << endl;
	}
}


void try_search(Tree<Contact> &tree, const string & name, const string & surname=""){
	cout << "\nTry search("<< name << ", "<< surname << ")\n";
	Node<Contact> *res = tree.search(Contact(name, surname));
	if (res)
		cout << "Found: " << *(res->data) << endl;
	else
		cout << "not found...\n";
}

int main(){

	Tree<Contact> tree;

	tree.insert(Contact("Matthieu", "Bruel", "07554433222"));
	tree.insert(Contact("Spilios", "Alexopoulos", "07222222222"));
	tree.insert(Contact("Bob", "Dupont", "07123456789"));
	tree.insert(Contact("John", "Smith", "07111111111"));
	tree.insert(Contact("Yannis", "Papadopulos", "07987654321"));
	tree.insert(Contact("Danae", "Rivera", "07555555555"));
	tree.insert(Contact("Pablo", "Tonto", "07666666666"));
	tree.insert(Contact("Barby", "", "07999999999"));
	tree.insert(Contact("Alex", "", "07000000000"));
	tree.insert(Contact("Ben", "", "07000000000"));
	tree.insert(Contact("Nico", "", "07000000000"));
	tree.insert(Contact("Ron", "", "07000000000"));

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



	cout << "\n\nTraverse pre order:\n";
	tree.traverse_pre_order(&Node<Contact>::print);

	cout << "\nLet's delete Bob\n";
	tree.erase(Contact("Bob", "Dupont"));
	tree.traverse_pre_order(&Node<Contact>::print);


	cout << "\nLet's delete Spilios\n";
	tree.erase(Contact("Spilios", "Alexopoulos"));
	tree.traverse_pre_order(&Node<Contact>::print);


	cout << "\nLet's delete Ron\n";
	tree.erase(Contact("Ron"));
	tree.traverse_pre_order(&Node<Contact>::print);

	cout << "\nLet's delete Ben\n";
	tree.erase(Contact("Ben"));
	tree.traverse_pre_order(&Node<Contact>::print);

	cout << "\nLet's delete Barby\n";
	tree.erase(Contact("Barby"));
	tree.traverse_pre_order(&Node<Contact>::print);

	cout << "\nFinally Let's delete the head (Matthieu)\n";
	tree.erase(Contact("Matthieu", "Bruel"));
	tree.traverse_pre_order(&Node<Contact>::print);

	cout << "Size of the Tree: " << tree.size() << endl;

#ifdef DEBUG
	cout << "\n\nDestruction of the Tree\n";
#endif
	return 0;
}

