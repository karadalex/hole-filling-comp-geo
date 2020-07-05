#include "utils.h"

using namespace std;

void printVector(vector<int> vec, string vec_name) {
	cout << vec_name << " = ";
	for (int i = 0; i < vec.size(); i++) {
		cout << vec.at(i) << " ";
	}
	cout << endl;
}