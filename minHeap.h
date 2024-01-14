#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
using namespace std;


struct reachabilityInformation {
	int id;
	int time;

	reachabilityInformation(int i = 0, int t = 0) {
		id = i; time = t;
	}
};


struct heapStruct {
	int id;
	int timeTaken;
	vector<int> prevLocation;
	vector<int> individualTimeArray;

	heapStruct() {
		id = 0; timeTaken = INT_MAX;
	}
	heapStruct(int i, int t) {
		id = i; timeTaken = t;
	}
};

ostream& operator<<(ostream& out, heapStruct& h) {
	out << "Hospital ID: " << h.id << endl;
	out << "Total Time Taken: " << h.timeTaken << endl;
	out << "Previous Hospital in the Path: " << endl;
	for (int i = 0; i < h.prevLocation.size(); i++) {
		if (i != h.prevLocation.size() - 1) {
			cout << h.prevLocation[i] << "->";
		}
		else {
			cout << h.prevLocation[i];
		}
	}
	cout << endl << "Individual Path Times Between Hospitals:" << endl;
	for (int i = 0; i < h.individualTimeArray.size(); i++) {
		if (i != h.individualTimeArray.size() - 1) {
			cout << h.individualTimeArray[i] << "->";
		}
		else {
			cout << h.individualTimeArray[i];
		}
	}
	cout << endl;

	return out;
}


template <typename T>
class MinHeap {
	T* arr;
	int currSize;
	int maxCapacity;

	void reheapUp(int, int);
	void reheapDown(int, int);

public:
	MinHeap(int = 100);
	MinHeap(T*, int);
	~MinHeap();
	void buildMinHeap();
	void insert(T);
	bool isEmpty();
	T getMin();
	void deleteMin(T&);
	void heapSort();
	void findBestPaths(vector<list<reachabilityInformation>>&);
};


template<typename T>
inline void MinHeap<T>::reheapUp(int root, int last)
{
	if (last > root) {
		int parent = (last - 1) / 2;
		if (arr[parent] > arr[last]) {
			swap(arr[parent], arr[last]);
			reheapUp(root, parent);
		}
	}
}


template<typename T>
inline void MinHeap<T>::reheapDown(int curr, int last)
{
	int rightChild = 2 * curr + 2, leftChild = 2 * curr + 1, minChild = 0;
	if (leftChild <= last) {
		if (leftChild == last) {
			minChild = leftChild;
		}
		else {
			minChild = (arr[leftChild] < arr[rightChild]) ? leftChild : rightChild;
		}
		if (arr[minChild] < arr[curr]) {
			swap(arr[minChild], arr[curr]);
			reheapDown(minChild, last);
		}
	}
}


template<typename T>
inline MinHeap<T>::MinHeap(int cap)
{
	maxCapacity = cap;
	currSize = 0;
	arr = new T[maxCapacity];
}


template<typename T>
inline MinHeap<T>::MinHeap(T* data, int size)
{
	maxCapacity = 2 * size;
	arr = new T[maxCapacity];
	currSize = size;
	for (int i = 0; i < currSize; i++) {
		arr[i] = data[i];
	}
}

template<typename T>
inline MinHeap<T>::~MinHeap()
{
	if (arr != nullptr) {
		delete[] arr;
	}
}

template<typename T>
inline void MinHeap<T>::buildMinHeap()
{
	for (int i = currSize / 2 - 1; i >= 0; i--) {
		//calls reheapdown from the first non-leaf node towards the root
		reheapDown(i, currSize - 1);
	}
}

template<typename T>
inline void MinHeap<T>::insert(T d)
{
	if (currSize < maxCapacity) {
		arr[currSize++] = d;
		reheapUp(0, currSize - 1);
	}
}

template<typename T>
inline bool MinHeap<T>::isEmpty()
{
	return currSize == 0;
}

template<typename T>
inline T MinHeap<T>::getMin()
{
	return arr[0];
}

template<typename T>
inline void MinHeap<T>::deleteMin(T& item)
{
	if (!isEmpty()) {
		item = arr[0];
		arr[0] = arr[--currSize];
		reheapDown(0, currSize - 1);
	}
}

template<typename T>
inline void MinHeap<T>::heapSort()
{
	for (int i = currSize - 1; i >= 0; i--) {
		swap(arr[i], arr[0]);
		reheapDown(0, i - 1);
	}
}

//specialized
template<>
inline void MinHeap<heapStruct>::reheapUp(int root, int last)
{
	if (last > root) {
		int parent = (last - 1) / 2;
		if (arr[parent].timeTaken > arr[last].timeTaken) {
			swap(arr[parent], arr[last]);
			reheapUp(root, parent);
		}
	}
}


template<>
inline void MinHeap<heapStruct>::reheapDown(int curr, int last)
{
	int rightChild = 2 * curr + 2, leftChild = 2 * curr + 1, minChild = 0;
	if (leftChild <= last) {
		if (leftChild == last) {
			minChild = leftChild;
		}
		else {
			minChild = (arr[leftChild].timeTaken < arr[rightChild].timeTaken) ? leftChild : rightChild;
		}
		if (arr[minChild].timeTaken < arr[curr].timeTaken) {
			swap(arr[minChild], arr[curr]);
			reheapDown(minChild, last);
		}
	}
}

template<>
inline void MinHeap<heapStruct>::findBestPaths(vector<list<reachabilityInformation>>& l1)
{
	//the indexes of the heap array tells the hospital number, 0 being UN
	//initialization
	arr[0].timeTaken = 0;
	for (int i = 0; i < maxCapacity; i++) {
		arr[i].id = i;
	}
	currSize = maxCapacity;
	//data is already in min heap form
	//start processing the data
	for (int i = currSize - 1; i >= 0; i--) {
		buildMinHeap();
		swap(arr[0], arr[i]); //remove the min element from the heap like heapsort
		int currID = arr[i].id; //also the index of the vector array
		list<reachabilityInformation>::iterator it = l1[currID].begin();
		while (it != l1[currID].end()) {
			int hospitalID = (*it).id;
			int timeToReach = (*it).time;
			//look for the id in the min heap
			bool found = false;
			for (int j = 0; j < i && !found; j++) { //less than i as no need to check the ones already removed
				if (hospitalID == arr[j].id) {
					found = true; //to end the loop
					if (arr[j].timeTaken > arr[i].timeTaken + timeToReach) {
						//replace with the shorter time
						arr[j].timeTaken = arr[i].timeTaken + timeToReach;
						//update previous location
						arr[j].prevLocation = arr[i].prevLocation;
						arr[j].prevLocation.push_back(arr[i].id); //adds the current hospital through which the shortest path
						arr[j].individualTimeArray = arr[i].individualTimeArray;
						arr[j].individualTimeArray.push_back(timeToReach);
					}
				}
			}
			it++; //point towards next hospital in the list of vector index
		}
		currSize--; //for the build heap to work correctly
	}
	//adds the final destination hospital in the prev location list
	for (int i = 0; i < maxCapacity; i++) {
		arr[i].prevLocation.push_back(arr[i].id);
	}
	//printing the final minHeap
	for (int i = 0; i < maxCapacity; i++) {
		cout << arr[i] << endl;
	}
}

