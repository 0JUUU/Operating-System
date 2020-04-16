#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <list>
#include <fstream>
#define NICE_0_LOAD 1024
using namespace std;

struct Roundrobin {
	list<int> num;
	list<int> count;
	list<int> tmp;
};

struct Conventional {
	int num;
	int count;
	int time;
	int vruntime;
	int weight;
};

int main() {
	Roundrobin R[100];
	Conventional C[140];
	int a;
	int n = 0;
	int m = 0;
	int r[100], c[100];
	for(int i=0;i<100;i++) {
	r[i] = -1;
	c[i] = -1;
	}
	FILE *file = fopen("Input.txt", "r");
	for (int i = 0; i < 15; i++) {
		fscanf(file, "%d", &a);
		if (i % 3 == 0) {
			if (a < 100) {
				r[n] = a;
			}
			else c[m] = a;
		}

		else if (i % 3 == 1) {
			if (r[n] != -1) {
				R[r[n]].num.push_back(a);
			}
			if (c[m] != -1) {
				C[c[m]].num = a;
			}
		}

		else {
			if (r[n] != -1) {
				R[r[n]].count.push_back(a);
				n++;
			}
			if (c[m] != -1) {
				C[c[m]].count = a;
				m++;
			}
		}
	}
	fclose(file);
	for (int i = 0; i < n; i++) {
		int timeslot = 2;
		sort(r, r + n);
		int tmp;
		if (R[r[i]].num.empty()) continue;
		while (!R[r[i]].num.empty()) {
			if (R[r[i]].num.size() == R[r[i]].tmp.size() && !R[r[i]].tmp.empty()) {
				tmp = R[r[i]].tmp.front();
				R[r[i]].tmp.pop_front();
			} else tmp = R[r[i]].count.front();

			int num_s = R[r[i]].num.front();
			int count_s = R[r[i]].count.front();
			while (timeslot!=0) {
				cout << r[i] << " " << R[r[i]].num.front() << " ";
				timeslot--;
				tmp--;
				if (tmp == 0) {
					break;
				}
				if(timeslot == 0) R[r[i]].tmp.push_back(tmp);
			}
			timeslot = 2;
			R[r[i]].num.pop_front();
			R[r[i]].count.pop_front();
			if (tmp != 0) {
				R[r[i]].num.push_back(num_s);
				R[r[i]].count.push_back(count_s);
			}
		}
		cout << "\n";
	}
	
	sort(c, c + m);
	int sum = 0;
	for (int i = 0; i < m; i++) {
		sum += C[c[i]].count;
		C[c[i]].time = 0;
		if (100 <= c[i] && c[i] < 110) {
			C[c[i]].weight = 10;
		}
		else if (110 <= c[i] && c[i] < 120) {
			C[c[i]].weight = 8;
		}
		else if (120 <= c[i] && c[i] < 130) C[c[i]].weight = 6;
		else if (130 <= c[i] && c[i] < 140) C[c[i]].weight = 4;
	}

	cout << c[0] << " " << C[c[0]].num << endl;
	C[c[0]].count--;
	C[c[0]].time++;
	sum--;
	int min = C[c[0]].vruntime = (NICE_0_LOAD / C[c[0]].weight) * C[c[0]].time;
	while (sum != 0) {
		min = 1024;
		int a;
		for (int i = 0; i < m; i++) {	
			if (c[i] != 0) {
				C[c[i]].vruntime = (NICE_0_LOAD / C[c[i]].weight) * C[c[i]].time;
				if (min >= C[c[i]].vruntime && C[c[i]].count != 0) {
					min = C[c[i]].vruntime;
					a = i;
				}
			}
		}
		sum--;
		cout << c[a] << " " << C[c[a]].num << endl;
		if (C[c[a]].count != 0) {
			C[c[a]].count--;
			C[c[a]].time++;
		}
		else c[a] = 0;
	}

		
	return 0;
}
