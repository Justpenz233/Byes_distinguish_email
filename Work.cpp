#define _CRT_SECURE_NO_WARNINGS 1
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <io.h>
#include <vector>
#include <map>
#include <algorithm>
#include <time.h>
#include <hash_map>
#include <set>
using namespace std;
hash_map<string, int> ham;
hash_map<string, int> spam;
hash_map<string, int> sum;
long long Sspam = 0, Sham = 0, Sum = 0;
string change_path(string path) {
	for (int i = 0; i < path.length(); i++) {
		if (path[i] != '/') continue;
		path[i] = '\\';
		path.insert(i, 1, '\\');
	}
	return path;
}
bool is_word(string word) {
	if (word == "Subject:") return false;
	if (word[0] >= 'a' && word[0] <= 'z') return true;
	if (word[0] >= 'A' && word[0] <= 'Z') return true;
	if (word[0] >= '0' && word[0] <= '9') return true;
	return false;
}
string Sturn(string t) {
	for (int i = 0; i < t.length(); i++) {
		if (t[i] >= 'A' && t[i] <= 'Z')
			t[i] += 32;
	}
	return t;
}
int Train_ham(string path) {
	int count = 0;
	_finddata_t file_info;
	ifstream file;
	string Findname = path + "/*.txt";
	int handle = _findfirst(Findname.c_str(), &file_info);
	do
	{
		string temp;
		string current_file_name = change_path(path + "/" + file_info.name);
		file.open(current_file_name);
		count++;
		while (!file.eof()) {
			file >> temp;
			if (is_word(temp)) {
				temp = Sturn(temp);
				ham[temp] ++;
				sum[temp] ++;
				Sham++;
				Sum++;
			}
		}
		file.close();
	} while (!_findnext(handle, &file_info));
	_findclose(handle);
	return count;
}
int Train_spam(string path) {
	int count = 0;
	_finddata_t file_info;
	ifstream file;
	string Findname = path + "/*.txt";
	int handle = _findfirst(Findname.c_str(), &file_info);
	do
	{
		string temp;
		count++;
		string current_file_name = change_path(path + "/" + file_info.name);
		file.open(current_file_name);
		while (!file.eof()) {
			file >> temp;
			if (is_word(temp)) {
				temp = Sturn(temp);
				spam[temp] ++;
				sum[temp] ++;
				Sspam++;
				Sum++;
			}
		}
		file.close();
	} while (!_findnext(handle, &file_info));
	_findclose(handle);
	return count;
}
bool Judge(string path)
{
	set<string> k;
	ifstream f;
	f.open(change_path(path));
	string tstring;
	while (!f.eof()) {
		f >> tstring;
		if (!is_word(tstring))
			continue;
		tstring = Sturn(tstring);
		int st = spam[tstring];
		if (st == 0) continue;
		k.insert(tstring);
	}
	f.close();
	long double Pup = 1.0 * Sspam / Sum;
	long double Pdown = 1.0;
	for (set<string>::iterator i = k.begin(); i != k.end(); i++) {
		Pdown *= (long double)sum[*i] / (long double)spam[*i];
		Pdown *= Pup;
	}
	long double Pmail = (long double)1.0 / ((long double)1.0 + Pdown);
	//cout << Pmail << endl;
	if (Pmail >= 0.85) return 1;
	return 0;
}
double Test_spam(string path) {
	int a = 0, b = 0;
	_finddata_t file_info;
	ifstream file;
	string Findname = path + "/*.txt";
	int handle = _findfirst(Findname.c_str(), &file_info);
	do
	{
		string temp;
		string current_file_name = change_path(path + "/" + file_info.name);
		if (Judge(current_file_name)) {
			a++;
		}
		b++;
	} while (!_findnext(handle, &file_info));
	_findclose(handle);
	double c = double(a) / double(b);
	printf("Spam test situation:\n");
	printf("%d was tested,%d was right,the rate is %lf\n", b, a, c);
	return c;
}
double Test_ham(string path) {
	int a = 0, b = 0;
	_finddata_t file_info;
	ifstream file;
	string Findname = path + "/*.txt";
	int handle = _findfirst(Findname.c_str(), &file_info);
	do
	{
		string temp;
		string current_file_name = change_path(path + "/" + file_info.name);
		if (!Judge(current_file_name)) {
			a++;
		}
		b++;
	} while (!_findnext(handle, &file_info));
	_findclose(handle);
	double c = double(a) / double(b);
	printf("Ham test situation:\n");
	printf("%d was tested,%d was right,the rate is %lf\n", b, a, c);
	return c;
}
int main()
{
	clock_t tb, te, st, des;
	st = clock();
	tb = clock();
	int t = Train_ham("C:/Users/Penz/Desktop/Byes/train/ham");
	te = clock();
	printf("%d Ham Email Training Had Done  Use time %ld ms\n", t, te - tb);
	
	tb = clock();
	t = Train_spam("C:/Users/Penz/Desktop/Byes/train/spam");
	te = clock();
	printf("%d Spam Email Training Had Done  Use time %ld ms\n", t, te - tb);

	double a = Test_spam("C:/Users/Penz/Desktop/Byes/test/spam");
	double b = Test_ham("C:/Users/Penz/Desktop/Byes/test/ham");
	printf("The average rate is %lf\n", (a + b) / 2.0);
	des = clock();
	printf("\nUse time:%d ms", des - st);
	system("pause");
}