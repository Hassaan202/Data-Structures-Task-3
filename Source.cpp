#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include "minHeap.h"
using namespace std;


ostream& operator<<(ostream& out, reachabilityInformation& r1) {
	out << r1.id << " , " << r1.time;
	return out;
}


template <class T>
void printList(list<T>& l1) {
	typename list<T>::iterator it = l1.begin();
	while (it != l1.end()) {
		cout << *it << endl;
		it++;
	}
}


void tokenizeFileLine(list<reachabilityInformation>& l1, string line) {
	reachabilityInformation tmp;
	int j = 0, i = 0;
	bool isFirst = true;
	if (line == "") { //return if empty string
		return;
	}
	else {
		for (i = 0; i < line.length(); i++) {
			if (line[i] == ',' || line[i] == '\n' || line[i] == '\0') {
				string val = line.substr(i - j, j);
				if (isFirst) {
					tmp.id = stoi(val);
					isFirst = false;
				}
				else {
					tmp.time = stoi(val);
					l1.push_back(tmp);
					isFirst = true;
				}
				j = 0;
			}
			else {
				j++;
			}
		}
	}
	//for the last value as the loop will end by then
	string val = line.substr(i - j, j);
	if (isFirst) {
		tmp.id = stoi(val);
		isFirst = false;
	}
	else {
		tmp.time = stoi(val);
		l1.push_back(tmp);
		isFirst = true;
	}
}


void readReachabilityDataFromFile(string fileName, vector<list<reachabilityInformation>>& l1, int& numHosp) {
	string currWord;
	ifstream fin(fileName);
	if (!fin) {
		cout << "File 1 has not succesfully opened!\n";
	}
	else {
		fin >> currWord; //read the total number of hospitals stored on the first line of the file
		numHosp = stoi(currWord);
		list<reachabilityInformation> tmp;
		int i = 0;
		while (!fin.eof()) {
			fin >> currWord;
			l1.push_back(tmp); //dummy value to create space in the vector
			tokenizeFileLine(l1[i], currWord);
			i++;
		}
	}
	fin.close();
}


int main() {
	//ofstream out("input1.txt");
	//if (!out) {
	//	cout << "File not opened!";
	//}
	//int numHosp = 0;
	//cout << "Enter the total number of hospitals in the war zone: ";
	//cin >> numHosp;
	//while (numHosp < 1) {
	//	cout << "Enter a positive integer greater than one: ";
	//	cin >> numHosp;
	//}
	//out << numHosp;

	//char choice = 'y';
	//int i = 0;
	//while (i <= numHosp) {
	//	out << endl;
	//	choice = 'y';
	//	int j = 0;
	//	while (choice == 'y') {
	//		int time, id;
	//		if (i == 0) {
	//			cout << "Enter the reachability details for UN:\n";
	//			cout << j + 1<< "-id: ";
	//			cin >> id;
	//			cout << j + 1 << "-time: ";
	//			cin >> time;
	//		}
	//		else {
	//			cout << "Enter the reachability details for hospital " << i << ":\n";
	//			cout << j + 1 << "-id: ";
	//			cin >> id;
	//			cout << j + 1 << "-time: ";
	//			cin >> time;
	//		}
	//		//save to the file
	//		string tmp = to_string(id) + "," + to_string(time);
	//		if (j != 0) {
	//			tmp = ',' + tmp;
	//		}
	//		out << tmp;
	//		j++;
	//		cout << "Do you want to enter more data (y/n): ";
	//		cin >> choice;
	//		choice = tolower(choice);
	//		while (choice != 'y' && choice != 'n') {
	//			cout << "Enter a correct choice(y/n): ";
	//			cin >> choice;
	//			choice = tolower(choice);
	//		}
	//	}
	//	i++;
	//}
	cout << endl << endl;
	cout << "---------------REACHABILITY INFORMATION---------------\n";

	vector<list<reachabilityInformation>> rList;
	int numOfHospitals = 0;
	//stores data from the file to the vector/list data structure
	readReachabilityDataFromFile("input.txt", rList, numOfHospitals);

	MinHeap<heapStruct> shortestPath(numOfHospitals + 1);
	shortestPath.findBestPaths(rList);

	return 0;
}