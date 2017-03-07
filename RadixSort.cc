/** This is an implementation of radix sort */

#include <iostream>
#include <random>
#include <ctime>
#include <vector>

using namespace std;

const int length = 100000000;

/** Just a helper function to check if array is sorted */
bool check(int *arr){
	for(int i=0; i<length-1; i++) if(arr[i]>arr[i+1]) return false;
	return true;
}

/** Just a helper function to print array */
void print(int *arr){
	for(int i=0; i<length; i++){
		cout << arr[i] << ' ';
		if((i+1)%10==0) cout << endl;
	}
	cout << endl;
}

/** A basic bucket class designed to be fast
  * especially with radix sort */
class bucket{
	int allocated;
public:
	int *arr;
	int size=0;

	bucket(){
		arr = new int[1];
		allocated=1;
	}

	void reserve(int capacity){
		delete[] arr;
		arr = new int[capacity];
		allocated=capacity;
	}

	void expand(){
		int *temp = arr;
		arr = new int[allocated*2];
		for(int i=0; i<allocated; i++) arr[i]=temp[i];
		allocated*=2;
		delete[] temp;
	}

	void push(int e){
		if(size >= allocated) expand();
		arr[size++]=e;
	}

	~bucket(){
		delete[] arr;
	}
};

/** Sorts array using radix sort
  * param: arr - array to sort */
void sort(int *arr){
	int div=1, largest=0, divMax=0, it[10], x;
	bucket b[10];
	for(int i=0; i<10; i++) b[i].reserve((int)(length/10));
	for(int i=0; i<length; i++) if(arr[i]>largest) largest=arr[i];
	while(largest>0){
		divMax++;
		largest/=10;
	}
	for(int i=0; i<divMax; i++){
		for(int j=0; j<10; j++) b[j].size = 0;
		for(int j=0; j<length; j++) b[(arr[j]/div)%10].push(arr[j]);
		largest=0;
		for(int k=0; k<10; k++){
			for(int j=0; j<b[k].size; j++) arr[largest++]=b[k].arr[j];
		}
		div*=10;
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
