#include <iostream>
#include <random>
#include <ctime>

using namespace std;

const int size = 100000000;

int equals(int *a, int *b){
	int *aCount = new int[size];
	int *bCount = new int[size];
	for(int i=0; i<size; ++i) aCount[i] = bCount[i] = 0;
	for(int i=0; i<size; ++i){
		++aCount[a[i]];
		++bCount[b[i]];
	}
	bool same = true;
	for(int i=0; i<size; ++i) if(aCount[i] != bCount[i]){
		cout << i << ": " << aCount[i] << " - " << bCount[i] << endl;
		same = false;
	}
	delete[] aCount;
	delete[] bCount;
	return same;
}

void print(int *arr){
	for(int i=0; i<size; i++){
		cout << arr[i] << ' ';
		if((i+1)%20==0) cout << endl;
	}
	cout << endl;
}

bool check(int *arr){
	for(int i=0; i<size-1; i++) if(arr[i]>arr[i+1]) return false;
	return true;
}

/** Search function for the binary sort */
int search(int *arr, int lo, int hi, int val){
	for(int i=(lo>>1)+(hi>>1); lo<hi; i=(lo>>1)+(hi>>1)){
		if(val > arr[i]) lo = i+1;
		else if(val < arr[i]) hi = i-1;
		else{
			while(arr[++i] == val);
			return i;
		}
	}
	return (val >= arr[lo]) ? lo+1 : lo;
}

/** Binary insertion sort */
void binSort(int *arr, int lo, int hi){
	for(int i=lo+1, ins, temp; i<=hi; ++i){
		if(arr[i] < arr[i-1]){
			ins = search(arr, lo, i-1, arr[i]);
			temp = arr[i];
			for(int j=i; j>ins; --j) arr[j] = arr[j-1];
			arr[ins] = temp;
		}
	}
}

int calcMinRun(int len){
	char r = 0;
	while(len > 64){
		r |= len&1;
		len>>=1;
	}
	return len+r;
}

void sort(int *arr, int lo, int hi, int minrun){
	// cout << hi-lo+1 << ", " << minrun << endl;
	/*if(hi-lo+1 <= minrun) binSort(arr, lo, hi);
	else{*/
		int mid = (lo>>1)+(hi>>1), midLen = mid-lo+1;
		(midLen <= minrun) ? binSort(arr, lo, mid) : sort(arr, lo, mid, minrun);
		(hi-mid <= minrun) ? binSort(arr, mid+1, hi) : sort(arr, mid+1, hi, minrun);
		int *temp = new int[midLen];
		for(int i=0; i<midLen; ++i) temp[i] = arr[lo+i];
		int i=lo, j=0, k=mid+1;
		while(j<midLen && k<=hi) arr[i++] = (temp[j]<=arr[k]) ? temp[j++] : arr[k++];
		while(j<midLen) arr[i++] = temp[j++];
		delete[] temp;
	// }
}

int main(){
	random_device rd;
	mt19937 r(rd());
	double start, end;
	int *arr = new int[size];
	// int *ref = new int[size];
	for(int i=0; i<size; ++i) arr[i] = r()%size;
	// cout << equals(arr, ref) << endl;
	// print(arr);
	start = clock();
	sort(arr, 0, size-1, calcMinRun(size));
	end = clock();
	cout << (end-start)/CLOCKS_PER_SEC << endl;
	// print(arr);
	cout << check(arr) << endl;
	// cout << equals(arr, ref) << endl;
	delete[] arr;
	// delete[] ref;
}