#include <iostream>
#include <list>
#include <string>

// Needed for main to store stats
#include <map>
#include <vector>

using namespace std;

static int debug_merge_sort = 1;
static int debug_quick_sort = 1;

class Person {
	string iName;
	string iSurname;

	public:
		Person(const string & aName, const string & aSurname): iName(aName), iSurname(aSurname){}

		string getName(){return iName;}
		string getSurname(){return iSurname;}

		friend bool operator< (const Person & p1, const Person & p2);
		friend class CompareByFisrtName;

		friend ostream & operator<< ( ostream &output, const Person & p );

	private:
//		Person(const Person & aPerson);
//		Person & operator =(const Person & aPerson);
};


// Functor to compare by surname
class CompareByFisrtName {
	public:
		bool operator() (const Person &p1, const Person &p2){
			if (p1.iName < p2.iName)
				return true;
			else if (p1.iName == p2.iName)
				return (p1.iSurname < p2.iSurname);
			else
				return false;
		}
};

// Needed to use std::less
bool operator< (const Person & p1, const Person & p2){
	if (p1.iSurname < p2.iSurname)
		return true;
	else if (p1.iSurname == p2.iSurname)
		return (p1.iName < p2.iName);
	else
		return false;
}


ostream & operator<< ( ostream &output, const Person & p ) {
	output << p.iName << " " << p.iSurname;
	return output;
}


// like std::min_element from the algorithms
template<class FwdIter, class Compare>
FwdIter getMin(FwdIter begin, FwdIter end, Compare cmp)
{
	if (begin == end)
		return end;

	FwdIter min = begin++;
    for (; begin != end; ++begin) {
		if (cmp(*begin, *min))
			min = begin;
    }

	return min;
}

template<class FwdIter, class Compare>
int selection_sort(FwdIter begin, FwdIter end, Compare cmp)
{
	int nbIter = 0;
	if (begin == end)
		return nbIter;

    for (; begin != end; ++begin) {
//        FwdIter min = std::min_element(begin, end, cmp);
//        FwdIter min = getMin(begin, end, cmp);
		FwdIter curr = begin, min = begin;
		++curr;
		for (; curr != end; ++curr) {
			if (cmp(*curr, *min))
				min = curr;
			++nbIter;
		}
		std::iter_swap(min, begin);
    }

	return nbIter;
}

template<class BidirIter, class Compare>
int insertion_sort(BidirIter begin, BidirIter end, Compare cmp)
{
	int nbIter = 0;
	if (begin == end)
		return nbIter;

	BidirIter it = begin;
	++it;
    for (; it != end; ++it) {
//		cout << "- it: " << *it << endl;
		BidirIter curr = it;
		BidirIter prev = it;
		--prev;
		while ( (curr != begin) && cmp(*curr, *prev) ){
//			cout << "\t curr: " << *curr << ", prev: " << *prev << endl;
			std::iter_swap(prev, curr);
			--prev, --curr;
			++nbIter;
		}
    }

	return nbIter;
}


template<class FwdIter, class Compare>
int bubble_sort(FwdIter begin, FwdIter end, Compare cmp)
{
	int nbIter = 0;
	if (begin == end)
		return nbIter;

	bool swapped;
	do {
		swapped =false;
		FwdIter curr = begin;
		FwdIter next = begin;
		++next;
		while (next != end){
			if (cmp(*next, *curr) ){
				std::iter_swap(curr, next);
				swapped = true;
			}
			++curr, ++next;
			++nbIter;
		}

	} while (swapped);

	return nbIter;
}



//include <algorithm> // std::inplace_merge
template<class RandomIter, class Compare>
int merge_sort(RandomIter begin, RandomIter end, Compare cmp)
{
	int nbIter = 0;

	int dist = end - begin;
	if (debug_merge_sort){
		cout << "[merge_sort] dist: " << dist << endl;
		cout << "[merge_sort] list: ";
		print(begin, end);
		cout << endl;
	}

	if (dist > 1){
		RandomIter middle = begin + dist/2;

		// next one start also from middle due to the way iterators work:
		// the intervals are [begin, end[
		// so we are taking first [begin, middle[ then [middle, end[
		merge_sort(begin, middle, cmp);
		merge_sort(middle, end, cmp);

//		std::inplace_merge(begin, middle, end, cmp);
		nbIter += my_inplace_merge(begin, middle, end, cmp);
	}
	return nbIter;
}

#include <list>
template<class RandomIter, class Compare>
int my_inplace_merge(RandomIter begin, RandomIter middle, RandomIter end, Compare cmp)
{
	typedef typename iterator_traits<RandomIter>::value_type T;

	if (debug_merge_sort){
		cout << "[inplace_merge] list(begin, middle):";
		print(begin, middle);
		cout <<endl;
		cout << "[inplace_merge] list(middle, end):";
		print(middle, end);
		cout <<endl;
	}

	int nbIter = 0;
	list<T> q1(begin, middle);
	list<T> q2(middle, end);
	if (debug_merge_sort){
		cout << "[inplace_merge] queue1: ";
		print(q1);
		cout << endl;
		cout << "[inplace_merge] queue2: ";
		print(q2);
		cout << endl;
	}

	RandomIter it = begin;
	do {
		if ( cmp(q1.front(), q2.front()) ){
			*begin = q1.front();
			q1.pop_front();
		} else {
			*begin = q2.front();
			q2.pop_front();
		}
		++begin;
		++nbIter;
	} while( !(q1.empty() || q2.empty()) );

	// Finishing queue 1
	while (!q1.empty()){
		*begin = q1.front();
		q1.pop_front();
		++begin;
	}

	// or finishing queue 2
	while (!q2.empty()){
		*begin = q2.front();
		q2.pop_front();
		++begin;
	}

	return nbIter;
}


template<class BidirIter, class Compare>
int quick_sort(BidirIter begin, BidirIter end, Compare cmp)
{
	int nbIter = 0;

	int nbElem = std::distance(begin, end);
	if (nbElem > 1){
		if (debug_quick_sort){
			cout << "quick_sort: " << nbElem << " elements, list: ";
			print(begin, end);
			cout << endl;
		}

		// we could choose the pivot randomly but let's take the last element
		BidirIter pivot = end;
		--pivot;

		BidirIter firstHigher = begin;
		for (BidirIter current = begin; current != end; ++current){
			if (cmp(*current, *pivot)){ // if pivot current smaller than pivot
				if (debug_quick_sort){
					cout << "[quick_sort] Pivot: " << *pivot << " > " << *current;
					cout << " => let's put it left and swap it with firstHigher: " << *firstHigher << endl;
				}
				// we move it on the left;
				std::iter_swap(firstHigher, current);
				++firstHigher; // increment the futur place of the pivot

				if (debug_quick_sort){
					cout << "List is now: ";
					print(begin, end);
					cout << endl;
				}
			}
			++nbIter;
		}

		// let's put the pivot (value) at his position
		std::iter_swap(pivot, firstHigher);
		if (debug_quick_sort){
			cout << "Let's put the pivot in the middle (on his left, all smaller): ";
			print(begin, end);
			cout << endl << endl;
		}


		// pivot is not included [begin, pivot[
		quick_sort(begin, firstHigher, cmp);
		// ]pivot, end[
		quick_sort(++firstHigher, end, cmp);
	}

	return nbIter;
}


template<typename T> void print(const vector<T> & v){
	for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it){
		cout << "\t- " << *it << endl;
	}
}

template<typename T> void print(const list<T> & l){
	for (typename list<T>::const_iterator it = l.begin(); it != l.end(); ++it){
		cout << *it << ", ";
	}
}

template<typename RandomIter> void print(RandomIter begin, RandomIter end){
	for (RandomIter it = begin; it != end; ++it){
		cout << *it << ", ";
	}
}


void init_vector(vector<Person> & v){
	v.clear();
	v.push_back(Person("Matthieu", "Bruel"));
	v.push_back(Person("Spilios", "Alexopoulos"));
	v.push_back(Person("John", "Smith"));
	v.push_back(Person("Yannis", "Papadopulos"));
	v.push_back(Person("Bob", "Dupont"));
	v.push_back(Person("Danae", "Rei"));
}



int main(){

	vector<Person> l;
	vector<Person>::iterator min;
	int nbIter;
	map<string, vector<int> > stats;

	cout << "Initial List:\n";
	init_vector(l);
	print(l);
	cout << endl;

	min = getMin(l.begin(), l.end(), std::less<Person>());
	cout << "Min with simple < : " << *min << endl;
	min = getMin(l.begin(), l.end(), CompareByFisrtName());
	cout << "Min with CompareByFisrtName : " << *min << endl;
	cout << endl;

	cout << "\n\nI.: Selecting sort: ";
//	stats.insert("Selection Sort", vector<int>);
	nbIter = selection_sort(l.begin(), l.end(), std::less<Person>());
	stats["Selection Sort"].push_back(nbIter);
	cout << "After Selection sort with std::less, nbIter = " << nbIter << endl;
	print(l);
	cout << endl;

	nbIter = selection_sort(l.begin(), l.end(), CompareByFisrtName());
	stats["Selection Sort"].push_back(nbIter);
	cout << "After Selection sort with CompareByFisrtName, nbIter = " << nbIter << endl;
	print(l);
	cout << endl;



	cout << "\n\nII.: Insertion sort";
	cout << "Initial List:\n";
	init_vector(l);
	print(l);
	cout << endl;

	nbIter = insertion_sort(l.begin(), l.end(), std::less<Person>());
	stats["Insertion Sort"].push_back(nbIter);
	cout << "After Insertion sort with std::less, nbIter = " << nbIter << endl;
	print(l);
	cout << endl;

	nbIter = insertion_sort(l.begin(), l.end(), CompareByFisrtName());
	stats["Insertion Sort"].push_back(nbIter);
	cout << "After Insertion sort with CompareByFisrtName, nbIter = " << nbIter << endl;
	print(l);
	cout << endl;



	cout << "\n\nIII.: Bubble sort";
	cout << "Initial List:\n";
	init_vector(l);
	print(l);
	cout << endl;

	nbIter = bubble_sort(l.begin(), l.end(), std::less<Person>());
	stats["Bubble Sort"].push_back(nbIter);
	cout << "After Bubble sort with std::less, nbIter = " << nbIter << endl;
	print(l);
	cout << endl;

	nbIter = bubble_sort(l.begin(), l.end(), CompareByFisrtName());
	stats["Bubble Sort"].push_back(nbIter);
	cout << "After Bubble sort with CompareByFisrtName, nbIter = " << nbIter << endl;
	print(l);
	cout << endl;


	cout << "\n\nIV.: Merge sort";
	cout << "Initial List:\n";
	init_vector(l);
	print(l);
	cout << endl;

	nbIter = merge_sort(l.begin(), l.end(), std::less<Person>());
	stats["Merge Sort"].push_back(nbIter);
	cout << "After Merge sort with std::less, nbIter = " << nbIter << endl;
	print(l);
	cout << endl;

	nbIter = merge_sort(l.begin(), l.end(), CompareByFisrtName());
	stats["Merge Sort"].push_back(nbIter);
	cout << "After Merge sort with CompareByFisrtName, nbIter = " << nbIter << endl;
	print(l);
	cout << endl;



	cout << "\n\nV.: Quick sort";
	cout << "Initial List:\n";
	init_vector(l);
	print(l);
	cout << endl;

	nbIter = quick_sort(l.begin(), l.end(), std::less<Person>());
	stats["Quick Sort"].push_back(nbIter);
	cout << "After Quick sort with std::less, nbIter = " << nbIter << endl;
	print(l);
	cout << endl;


	nbIter = quick_sort(l.begin(), l.end(), CompareByFisrtName());
	stats["Quick Sort"].push_back(nbIter);
	cout << "After Quick sort with CompareByFisrtName, nbIter = " << nbIter << endl;
	print(l);
	cout << endl;



	cout << "\n\nNumber of iteration for the different sorting algorithm:\n";
	for (map<string, vector<int> >::const_iterator it =  stats.begin(); it != stats.end(); ++it){
		cout << it->first;

		const vector<int> &v=it->second;
		for (int k = 0; k < v.size(); ++k)
			cout << ", " << v[k];
		cout << endl;
	}
	return 0;
}

