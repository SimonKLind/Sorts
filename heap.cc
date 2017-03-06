#include <iostream>
#include <random>
#include <ctime>

using namespace std;

const int length = 100000000;

bool check(int *arr){
	for(int i=0; i<length-1; i++) if(arr[i]>arr[i+1]) return false;
	return true;
}

void print(int *arr){
	for(int i=0; i<length; i++){
		cout << arr[i] << ' ';
		if((i+1)%10==0) cout << endl;
	}
	cout << endl;
}

void swap(int *arr, int a, int b){
	int temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

void sink(int *arr, int i, int n){
	int swapIndex;
	while(2*i<=n){
		swapIndex=2*i;
		if(swapIndex<n && arr[swapIndex-1]<arr[swapIndex]) swapIndex++;
		if(arr[i-1]>=arr[swapIndex-1]) break;
		swap(arr, i-1, swapIndex-1);
		i = swapIndex;
	}
}

void sort(int *arr){
	int n=length;
	for(int i=(int)(length/2); i>0; i--) sink(arr, i, n);
	while(n>1){
		swap(arr, --n, 0);
		sink(arr, 1, n);
	}
}

int main(){
	random_device rd;
	mt19937 mersenne(rd());
	int *array = new int[length];
	double start, end, time;
	for(int i=0; i<length; i++) array[i]=mersenne()%length;

	//print(array);
	start = clock();
	sort(array);
	end = clock();
	time = (end-start)/CLOCKS_PER_SEC;
	cout << time << endl;

	//print(array);
	cout << check(array) << endl;

	delete[] array;

	return 0;
}