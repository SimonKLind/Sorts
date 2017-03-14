#include <iostream>
#include <ctime>
#include <random>

using namespace std;

const int SIZE = 100000000;

struct Run{
	int start;
	int length;
};

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

void print(int *arr){
	for(int i=0; i<SIZE; i++){
		cout << arr[i] << ' ';
		if((i+1)%20==0) cout << endl;
	}
	cout << endl;
}

bool check(int *arr){
	for(int i=0; i<SIZE-1; i++) if(arr[i]>arr[i+1]) return false;
	return true;
}

class MergeStack{
	int *arr;
	Run *runs;
	int capacity = 16;
	int size = 0;
	void expand(){
		Run *old = runs;
		capacity <<= 1;
		runs = new Run[capacity];
		for(int i=0; i<size; ++i) runs[i] = old[i];
		delete[] old;
	}
	void merge(int a, int b){
		// print(arr);
		// cout << "Merging:\n" << runs[a].start << ", " << runs[a].start+runs[a].length-1 << '\n' 
		// 		<< runs[b].start << ", " << runs[b].start+runs[b].length-1 << '\n';
		int *temp = new int[runs[a].length];
		for(int i=0; i<runs[a].length; ++i) temp[i] = arr[runs[a].start+i];
		int i=runs[a].start, j=0, k=runs[b].start;
		while(j<runs[a].length && k<runs[b].start+runs[b].length) arr[i++] = (temp[j]<=arr[k]) ? temp[j++] : arr[k++];
		while(j<runs[a].length) arr[i++] = temp[j++];
		// cout << "Deleting temp\n";
		delete[] temp;
		runs[a].length += runs[b].length;
		while(b<size-1) runs[b] = runs[++b];
		--size;
	}
public: 
	MergeStack(int *array): arr(array){
		runs = new Run[16];
	}
	void add(Run r){
		runs[size++] = r;
		while(size >= 3 && runs[size-3].length <= runs[size-2].length + runs[size-1].length || 
				size >= 2 && runs[size-2].length <= runs[size-1].length){
			(size >= 3 && runs[size-3].length < runs[size-1].length) ? merge(size-3, size-2) : merge(size-2, size-1);
		}
		if(size >= capacity) expand();
	}
	void mergeAll(){
		while(size>1) merge(size-2, size-1);
	}
	~MergeStack(){
		delete[] runs;
	}
};

/** Search function for the binary sort and insert_lo */
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

int calcMinRun(int len){
	char r = 0;
	while(len > 64){
		r |= len&1;
		len>>=1;
	}
	return len+r;
}

void reverse(int *arr, int lo, int hi){
	int temp;
	while(lo<hi){
		temp = arr[lo];
		arr[lo++] = arr[hi];
		arr[hi--] = temp;
	}
}

int run_length(int *arr, int pos, int len){
	int i = pos+1;
	bool ascending = arr[i] >= arr[pos];
	while(i<len && (ascending && arr[i] >= arr[i-1] || !ascending && arr[i] < arr[i-1])) ++i;
	if(!ascending) reverse(arr, pos, i-1);
	return i-pos;
}

void sort(int *arr, int len){
	int minRun = calcMinRun(len);
	int pos = 0, runLen, hi;
	Run current;
	MergeStack stack(arr);
	while(pos<len){
		runLen = run_length(arr, pos, len);
		if(runLen < minRun){
			hi = (pos+minRun<=len) ? pos+minRun : len; 
			// cout << runLen << " " << hi << '\n';
			binSort(arr, pos, pos+runLen, hi);
			runLen = hi-pos;
		}
		current.start = pos;
		current.length = runLen;
		// cout << "Adding: " << current.start << " -> " << current.start+current.length-1 << '\n';
		stack.add(current);
		pos+=runLen;
	}
	stack.mergeAll();
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