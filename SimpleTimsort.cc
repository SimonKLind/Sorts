/** This is my take on a simpler variation of Timsort
  * it has most of the functionality of Timsort.
  * It counts runs and boosts them to at least minrun
  * and adds them to a merge stack that keeps track of 
  * when it's appropriate to merge.
  *
  * It does however not include the fancy merging features of Timsort,
  * i.e galloping, and the process of minimizing memory usage are not
  * implemented here. Mostly because i feel like a few extra bytes of 
  * memory might not be worth it, and because it feels like a lot of 
  * processing overhead in order to gain a little on partially sorted data. 
  * Might add this stuff in the future though. */

#include <iostream>
#include <ctime>
#include <random>

using namespace std;

const int SIZE = 100000000; // Size of array to sort

/** Just a struct to make run management less of a pain */
struct Run{
	int start;
	int length;
};

/** Just a helper function used for debugging
  * Checks if contents in two differently ordered arrays are the same */
int equals(int *a, int *b){
	int *aCount = new int[SIZE];
	int *bCount = new int[SIZE];
	for(int i=0; i<SIZE; ++i) aCount[i] = bCount[i] = 0;
	for(int i=0; i<SIZE; ++i){
		++aCount[a[i]];
		++bCount[b[i]];
	}
	bool same = true;
	for(int i=0; i<SIZE; ++i) if(aCount[i] != bCount[i]){
		cout << i << ": " << aCount[i] << " - " << bCount[i] << endl;
		same = false;
	}
	delete[] aCount;
	delete[] bCount;
	return same;
}

/** Just a little helper function for printing array */
void print(int *arr){
	for(int i=0; i<SIZE; i++){
		cout << arr[i] << ' ';
		if((i+1)%20==0) cout << endl;
	}
	cout << endl;
}

/** Just a little helper function to check if array is sorted */
bool check(int *arr){
	for(int i=0; i<SIZE-1; i++) if(arr[i]>arr[i+1]) return false;
	return true;
}

/** The merge stack as described in Tim's description 
  * Manages runs and merges when appropriate */
class MergeStack{
	int *arr; // The array being sorted
	Run *runs; // Current array of runs
	int capacity = 16;
	int size = 0;
	void expand(){
		Run *old = runs;
		capacity <<= 1;
		runs = new Run[capacity];
		for(int i=0; i<size; ++i) runs[i] = old[i];
		delete[] old;
	}
	
	/** Merges two runs, index a and index b */
	void merge(int a, int b){
		int *temp = new int[runs[a].length];
		for(int i=0; i<runs[a].length; ++i) temp[i] = arr[runs[a].start+i];
		int i=runs[a].start, j=0, k=runs[b].start;
		while(j<runs[a].length && k<runs[b].start+runs[b].length) arr[i++] = (temp[j]<=arr[k]) ? temp[j++] : arr[k++];
		while(j<runs[a].length) arr[i++] = temp[j++];
		delete[] temp;
		runs[a].length += runs[b].length;
		while(b<size-1) runs[b] = runs[++b];
		--size;
	}
public: 
	MergeStack(int *array): arr(array){
		runs = new Run[16];
	}
	
	/** Adds a run to stack 
	  * Checks to see if runs should be merged, as described by Tim */
	void add(Run r){
		runs[size++] = r;
		while(size >= 3 && runs[size-3].length <= runs[size-2].length + runs[size-1].length || 
				size >= 2 && runs[size-2].length <= runs[size-1].length){
			(size >= 3 && runs[size-3].length < runs[size-1].length) ? merge(size-3, size-2) : merge(size-2, size-1);
		}
		if(size >= capacity) expand();
	}
	
	/** Cleans up to make sure everything is merged */
	void mergeAll(){
		while(size>1) merge(size-2, size-1);
	}
	~MergeStack(){
		delete[] runs;
	}
};

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
void binSort(int *arr, int lo, int start, int hi){
	for(int i=start, ins, temp; i<hi; ++i){
		if(arr[i] < arr[i-1]){
			ins = search(arr, lo, i-1, arr[i]);
			temp = arr[i];
			for(int j=i; j>ins; --j) arr[j] = arr[j-1];
			arr[ins] = temp;
		}
	}
}

/** Calculates minrun, excactly as described by Tim */
int calcMinRun(int len){
	char r = 0;
	while(len > 64){
		r |= len&1;
		len>>=1;
	}
	return len+r;
}

/** Reverses section lo -> hi of array */
void reverse(int *arr, int lo, int hi){
	int temp;
	while(lo<hi){
		temp = arr[lo];
		arr[lo++] = arr[hi];
		arr[hi--] = temp;
	}
}

/** Counts run length at current position, reverses if descending */
int run_length(int *arr, int pos, int len){
	int i = pos+1;
	bool ascending = arr[i] >= arr[pos];
	while(i<len && (ascending && arr[i] >= arr[i-1] || !ascending && arr[i] < arr[i-1])) ++i;
	if(!ascending) reverse(arr, pos, i-1);
	return i-pos;
}

/** Main sort loop */
void sort(int *arr, int len){
	int minRun = calcMinRun(len);
	int pos = 0, runLen, hi;
	Run current;
	MergeStack stack(arr);
	while(pos<len){
		runLen = run_length(arr, pos, len); // Get run length
		if(runLen < minRun){ // If run length < minrun, boost to minrun (or end of array)
			hi = (pos+minRun<=len) ? pos+minRun : len; 
			binSort(arr, pos, pos+runLen, hi);
			runLen = hi-pos;
		}
		current.start = pos;
		current.length = runLen;
		stack.add(current); // add run to stack
		pos+=runLen;
	}
	stack.mergeAll(); // Clean up stack
}

int main(){
	random_device rd;
	mt19937 r(rd());
	double start, end;
	int *arr = new int[SIZE];
	// int *ref = new int[SIZE];
	for(int i=0; i<SIZE; ++i) arr[i] = r()%SIZE;
	// cout << equals(arr, ref) << endl;
	// print(arr);
	start = clock();
	sort(arr, SIZE);
	end = clock();
	cout << (end-start)/CLOCKS_PER_SEC << endl;
	// print(arr);
	cout << check(arr) << endl;
	// cout << equals(arr, ref) << endl;
	delete[] arr;
	// delete[] ref;
}
