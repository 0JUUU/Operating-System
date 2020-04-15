#include <iostream>
#include <pthread.h>
#include <stdio.h>
using namespace std;

int spinlock = 1;
int arr[10];
void* sort(void *data) {
	while (spinlock == 0) {};
	spinlock = 0;
	int lowest;
	if ((int)data == 0) {
		lowest = arr[0];
		for (int i = 1; i < 3; i++) {
			if (lowest > arr[i]) {
				lowest = arr[i];
			}
		}
	}
	else if ((int)data == 1) {
		lowest = arr[3];
		for (int i = 4; i < 6; i++) {
			if (lowest > arr[i]) {
				lowest = arr[i];
			}
		}
	}
	else if ((int)data == 2) {
		lowest = arr[6];
		for (int i = 7; i < 10; i++) {
			if (lowest > arr[i]) {
				lowest = arr[i];
			}
		}
	}
	else {
		lowest = arr[0];
		for (int i = 1; i < 3; i++) {
			if (lowest > arr[3 * i]) {
				lowest = arr[3 * i];
			}
		}
		pthread_exit((void *)lowest);
	}
	spinlock = 1;
}

int main() {
	int low;
	FILE *fp;
	fp = fopen("test.txt", "r");
	for (int i = 0; i < 10; i++) {
		fscanf(fp, "%d", &arr[i]);
	}
	pthread_t p[4];
	for (int i = 0; i < 4; i++) {
		pthread_create(&p[i], NULL, sort, (void *)i);
	}
	pthread_join(p[4], (void **)&low);
	cout << "Lowest: " << low << endl;
}