/*
 * Write a program that calculates the dot product of two vectors of up to 10 dimensions, doing the
 * multiplications in parallel with the sum at the end.
 *
 * g++ dotProduct.cpp -std=c++11 -pthread -o dotP
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <stack>

using namespace std;

// Just to see the threads execution order
#define THREAD_TRACE 1


// Template class to do the dot product of vector of any type supporting multiplication
// and with any dimension.
// (can use it with main numbers primitive types)
template<typename T, unsigned short N> class dotProduct{
	public:
		// Calculate the dot product of to vector stored in C style array
		T calculate(const T * const v1,const T * const v2){
			// Only one handle can run this method at a time (thread safe)
			std::lock_guard<std::mutex> lock(mCalculate);

			std::vector<std::thread> threads;

			// Create the threads to do the multiplications in parallel
			for (int i=0; i<N; ++i){
				threads.push_back(std::thread(&dotProduct::doMultiplication, this, v1[i], v2[i]));
			}

			// Join them all (to be sure to have all the results
			for (int i=0; i<N; ++i){
				threads[i].join();
			}

			// Do the sum
			T product{};
			while(!results.empty()){
				product += results.top();
				results.pop();
			}

			return product;
		}


		// Same funtion but using vectors to allow easy call without array initialisation
		T calculate(const std::vector<T> & v1,const std::vector<T> & v2){
			// Only one handle can run this method at a time (thread safe)
			std::lock_guard<std::mutex> lock(mCalculate);

			vector<std::thread> threads;

			// Create the threads to do the multiplications in parallel
			for (int i=0; i<N; ++i){
				threads.push_back(std::thread(&dotProduct::doMultiplication, this, v1[i], v2[i]));
			}

			// Join them all (to be sure to have all the results
			for (int i=0; i<N; ++i){
				threads[i].join();
			}

			// Do the sum
			T product{};
			while(!results.empty()){
				product += results.top();
				results.pop();
			}

			return product;
		}


		// Nice Output for dot products of Arrays
		T printProduct(const T * const v1,const T * const v2){
			T result = calculate(v1, v2);
			printArray(v1);
			cout << ".";
			printArray(v2);
			cout << " = " << result << endl;
			return result;
		}


	private:
		std::mutex    mCalculate; // To make the calculate function thread safe
		std::mutex    mResults;   // To protect the resource
		std::stack<T> results;    // stack where every thread will drop its result

		// Do the dot product calculation for a dimension and push the result on the stack
		void doMultiplication(const T & n1, const T &n2){
			T res = n1*n2;

			// Only lock when shared resources are used
			mResults.lock();
#ifdef THREAD_TRACE
			cout << "Multiplying: "<< n1 << " and " << n2 << ": " << res << endl;
#endif
			results.push(res);
			mResults.unlock();
		}

		void printArray(const T * const arr){
			cout << "(";
			for (int i=0; i<N; ++i){
				if (i!=0)
					cout << ", ";
				cout << arr[i];
			}
			cout << ")";
		}
};



int main(){

	// Initialise an instance for vectors of double of 5 dimensions
	dotProduct<int, 5> dotProd1;
	int res1 = dotProd1.calculate({1,2,3,4,5}, {11,12,13,14,15});
	cout << res1 << endl;


	// Initialise an instance for arrays of double of 3 dimensions
	// (like in the example given)
	dotProduct<double, 3> dotProd2;
	double A[3]{3.0, 4.04, 7};
	double B[3]{6, 9, 11.1};
	double res = dotProd2.printProduct(A, B);

	return 0;
}
