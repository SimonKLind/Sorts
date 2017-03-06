/** This is an implementation of merge sort.
  * It will default to a binary insetion sort 
  * when the active array is short enough */

#include <iostream>
#include <ctime>
#include <random>

using namespace std;

const int size = 100000000; // Adjust size of array to sort

/** Just a helper function to print array
  * param: arr - array to print */
void print(int *arr){
	for(int i=0; i<size; i++){
		cout << arr[i] << ' ';
		if((i+1)%10==0) cout << endl;
	}
	cout << endl;
}

/** Just a helper function to check if array is sorted
  * param: arr - array to check
  * returns: true if sorted, otherwise false */
bool check(int *arr){
	for(int i=0; i<size-1; i++) if(arr[i]>arr[i+1]) return false;
	return true;
}

/** Loop version of binary search */
int search(int *arr, int lo, int hi, int val){
	for(int i=hi>>1; lo<hi; i=(lo>>1)+(hi>>1)){
		if(val > arr[i]) lo = i+1;
		else if(val < arr[i]) hi = i-1;
		else{
			while(arr[++i] == val);
			return i;
		}
	}
	return (val >= arr[lo]) ? lo+1 : lo;
}

/** Binary insertion sort, same as in the separate file */
void binSort(int *arr, int len){
	for(int i=1, ins, temp; i<len; ++i){
		if(arr[i] < arr[i-1]){
			ins = search(arr, 0, i-1, arr[i]);
			temp = arr[i];
			for(int j=i; j>ins; --j) arr[j] = arr[j-1];
			arr[ins] = temp;
		}
	}
}

/** Sorts using heap sort
  * defaults to binary insetion sort when length is <= 32
  * param: arr - array to sort
  * param: len - length of array */
void sort(int *arr, int len){
	if(len <= 32) binSort(arr, len);
	else{
		int mid = len>>1;
		int *sub1 = new int[mid];
		int *sub2 = new int[len-mid];
		for(int i=0, j=0, k=0; i<len; ++i) (i<mid) ? sub1[j++] = arr[i] : sub2[k++] = arr[i];
		sort(sub1, mid);
		sort(sub2, len-mid);
		int i=0, j=0, k=0;
		while(j<mid && k<len-mid) (sub1[j] <= sub2[k]) ? arr[i++] = sub1[j++] : arr[i++] = sub2[k++];
		while(j<mid) arr[i++] = sub1[j++];
		while(k<len-mid) arr[i++] = sub2[k++];
		delete[] sub1;
		delete[] sub2;
	}
}

int main(){
	random_device rd;
	mt19937 r(rd());
	double start, end;
	int *arr = new int[size];
	for(int i=0; i<size; ++i) arr[i] = r()%size;
	//print(arr);
	start = clock();
	sort(arr, size);
	end = clock();
	cout << (end-start)/CLOCKS_PER_SEC << endl;
	//print(arr);
	cout << check(arr) << endl;
	delete[] arr;
}
