#include <iostream>
#include <random>
#include <ctime>

using namespace std;

const int length = 1000000;

void print(int *arr){
	for(int i=0; i<length; i++){
		cout << arr[i] << ' ';
		if((i+1)%10==0) cout << endl;
	}
	cout << endl;
}

void swap(int *arr, int a, int b){
	int temp = arr[a];
	arr[a]=arr[b];
	arr[b]=temp;
}

void sort(int *arr, int lo, int hi){
	int OH = hi;
	int OL = lo;
	int pivot = lo++;
	while(lo<=hi){
		if(arr[pivot]>arr[lo]) swap(arr, pivot++, lo++);
		else if(arr[pivot]<arr[lo]){
			while(arr[hi]>arr[lo])hi--;
			swap(arr, lo, hi--);
		}
		else if(arr[pivot]==arr[lo]) lo++;
	}
	if(OL < --pivot) sort(arr, OL, pivot);
	if(OH > ++hi) sort(arr, hi, OH);
}

void pSwap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void pSort(int *lo, int *hi){
	int *OH = hi;
	int *OL = lo;
	int *pivot = lo++;
	while(lo<=hi){
		if(*pivot>*lo) pSwap(pivot++, lo++);
		else if(*pivot<*lo){
			while(*hi>*lo) hi--;
			pSwap(lo, hi--);
		}
		else if(*pivot==*lo) lo++;
	}
	if(OL < --pivot) pSort(OL, pivot);
	if(OH > ++hi) pSort(hi, OH);
}

bool check(int *arr){
	for(int i=0; i<length-1; i++) if(arr[i]>arr[i+1]) return false;
	return true;
}

bool equal(int *a, int *b){
	for(int i=0; i<length; i++) if(a[i]!=b[i]) return false;
	return true;
}

int main(){
	random_device rd;
	mt19937 mersenne(rd());
	int *array1 = new int[length];
	int *array2 = new int[length];
	double start, end, time;
	for(int i=0; i<length; i++) array1[i]=array2[i]=mersenne()%length;
	cout << equal(array1, array2) << endl;

	start = clock();
 	pSort(array1, &array1[length-1]);
 	end = clock();
 	time = (end-start)/CLOCKS_PER_SEC;
 	cout << time << endl;

 	start = clock();
 	sort(array2, 0, length-1);
 	end = clock();
 	time = (end-start)/CLOCKS_PER_SEC;
 	cout << time << endl;

 	cout << check(array1) << endl;
 	cout << equal(array1, array2) << endl;

 	delete[] array1;
 	delete[] array2;
 	
 	return 0;
}