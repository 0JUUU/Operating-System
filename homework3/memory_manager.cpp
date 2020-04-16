#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <queue>
#include <cstdlib>
#define PAGE_SIZE 256
using namespace std;

struct page_table {
	int page = -1;
	int offset = -1;
	int frame = -1;
} p[256];

double TLB_hit_ratio;
int TLB_hit_count = 0;
queue<int> TLB_count;

struct TLB {
	int page_num = -1;
	int frame_num = -1;
}T[32];

struct frame_table {
	int page_num = -1;
	bool v_i_flag = false;						// invaild = 0
} f[256];

int main() {
	FILE *fp;
	int count = 0;
	int a;											// 각 주소 받아들일 정수
	int addr_count = 0;											// 횟수
	fp = fopen("addresses.txt", "r");
	while (!feof(fp)) {
		fscanf(fp, "%d", &a);
		p[addr_count].page = a >> 8;
		p[addr_count].offset = (a << 24) >> 24;
		if (count == 0) {											// frame number 할당 과정
			p[addr_count].frame = 0;
			f[p[addr_count].frame].page_num = p[addr_count].page;
			f[p[addr_count].frame].v_i_flag = 1;
		}
		else {
			for (int j = 0; j < 256; j++) {
				if (addr_count != j && p[addr_count].page == p[j].page && p[addr_count].page != -1) {
					p[addr_count].frame = p[j].frame;
					count--;
					break;
				}
				else {
					if (j == 255) {
						p[addr_count].frame = count;
						f[p[addr_count].frame].v_i_flag = 1;
						f[p[addr_count].frame].page_num = p[addr_count].page;
					}
				}
			}
		}
		count++;
		addr_count++;
	}
	for (int i = 0; i < addr_count; i++) {
		if (i == 0) {							// 처음에는 TLB 무조건 miss -> 바로 삽입
			T[i].page_num = p[i].page;
			T[i].frame_num = 0;
			TLB_count.push(p[i].page);
		}
		else {
			int TLB_miss_count = 0;
			for (int j = 0; j < 32; j++) {
				if (p[i].page == T[j].page_num) {
					TLB_hit_count++;
					break;
				}
				else TLB_miss_count++;
			}
			if (TLB_miss_count == 32) {
				if (TLB_count.size() == 32) {
					int temp = TLB_count.front();
					TLB_count.pop();
					for (int j = 0; j < 32; j++) {
						if (T[j].page_num == temp) {
							T[j].page_num = p[i].page;
							T[j].frame_num = p[i].frame;
							TLB_count.push(p[i].page);
							break;
						}
					}
				}
				else {
					T[p[i].frame].page_num = p[i].page;
					T[p[i].frame].frame_num = p[i].frame;
					TLB_count.push(p[i].page);
				}
			}
		}
	}

	fp = fopen("Physical.txt", "w");
	for (int i = 0; i < 256; i++) {
		int physical = p[i].frame * PAGE_SIZE + p[i].offset;
		if (physical != -257) 	fprintf(fp, "%d ", physical);
	}
	fclose(fp);

	fp = fopen("Frame_table.txt", "w");
	for (int i = 0; i < 256; i++) {			
		fprintf(fp,"%d %d %d\n", i, f[i].v_i_flag, f[i].page_num);
	}
	fclose(fp);

	fp = fopen("TLB_hit_raito & final_TLB.txt", "w");
	TLB_hit_ratio = (double)TLB_hit_count / (double)addr_count;
	fprintf(fp, "TLB hit ratio : %f\n", TLB_hit_ratio);
	fprintf(fp, "Final TLB\n");
	for (int i = 0; i < 32; i++) {
		fprintf(fp, "%d %d\n", T[i].page_num, T[i].frame_num);
	}
	fclose(fp);

	return 0;
}
 
