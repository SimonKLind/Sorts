/** This is an implementation of quick3 sort.
  * Quick3 uses 3 partitions:
  * One for elements less than pivot
  * One for elements equal to pivot
  * One for elements larger than pivot
  * This is to more efficiently handle cases where
  * there are many duplicates */

#include <iostream>
#include <random>
#include <ctime>

using namespace std;

const int length = 1000000; // Adjust size of array to sort

/** Just a helper function to print array */
void print(int *arr){
	for(int i=0; i<length; i++){
		cout << arr[i] << ' ';
		if((i+1)%10==0) cout << endl;
	}
	cout << endl;
}

/** Swaps to elements in array
 * param: arr - array to per form swap on
 * param: a - index to swap
 * param: b - other index to swap */
void swap(int *arr, int a, int b){
	int temp = arr[a];
	arr[a]=arr[b];
	arr[b]=temp;
}

/** Sorts using quick3
  * param: arr - array to sort
  * param: lo - low end of active range
  * param: hi - high end of active range */
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

/** Just a helper function to check if array is sorted */
bool check(int *arr){
	for(int i=0; i<length-1; i++) if(arr[i]>arr[i+1]) return false;
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
