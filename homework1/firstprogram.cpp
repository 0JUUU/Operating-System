#include <iostream>
#include <pthread.h>
#include <stdio.h>
using namespace std;

void* max(void* data) {
	int* arr = (int *)data;
	int maxv = arr[0];
	for (int i = 1; i < 9; i++) {
		if (maxv <= arr[i]) {
			maxv = arr[i];
		}
	}
	pthread_exit((void *)maxv);
}

int main() {
	FILE *fp;
	fp = fopen("highest_input.txt", "r");
	int max_final = 0;
	int input[9][9];
	int child_v[9];
	int parent_v[9];

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			fscanf(fp, "%d", &input[((i / 3) * 3) + (j / 3)][((i % 3) * 3) + (j % 3)]);
		}
	}
	pthread_t child[9];

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			child_v[j] = input[i][j];
		}
		pthread_create(&child[i], NULL, &max, (void *)child_v);
		pthread_join(child[i], (void **)&max_final);
		parent_v[j] = max_final;
	}

	pthread_t parent;
	pthread_create(&parent, NULL, &max, (void *)parent_v);
	pthread_join(parent, (void **)&max_final);
	cout << "Highest number: " << max_final << endl;
	fclose(fp);
}