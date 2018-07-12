#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
	bool isRun = true;
	string str;
	stringstream ss;

	while (isRun)
	{
		cout << "input : "; cout.flush();
		getline(cin, str);
		ss.clear();
		ss << str;
		cout << "read : ";
		while (ss >> str) {
			cout << str <<  (ss.eof() ? "" : ", ");
			transform(str.begin(), str.end(), str.begin(), tolower);
			if ("exit" == str) {
				cout.flush();
				cin.clear();
				cout << endl << "Are you sure to exit ? (Y/N) "; cout.flush();
				cin >> str;
				transform(str.begin(), str.end(), str.begin(), tolower);
				if ("y" == str) {
					isRun = false;
					break;
				}
			}
			else if ("clear" == str || "cls" == str) {
				system("cls");
			}
		}
		cout << endl;
	}

	return EXIT_SUCCESS;
}