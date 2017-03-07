/** Gotta have bubble sort right? */

#include <iostream>
#include <cstdlib>
#include <ctime>

#define arr_size 30

using namespace std;

/** Bubble sort
  * param: arr - the array to sort */
void sort(int* arr){
	for(bool check=1; check==1;){
		check=0;
		for(int i=0; i<arr_size-1; i++){
			if(arr[i]>arr[i+1]){
				int temp = arr[i];
				arr[i] = arr[i+1];
				arr[i+1] = temp;
				check=1;
			}
			cout << arr[i] << ' ';
		}
		cout << arr[arr_size-1] << endl;
	}
}

int main(){
	int arr[arr_size];
	srand(time(NULL));
	for(int i=0; i<arr_size; i++){
		arr[i]=rand()%arr_size;
		cout << arr[i] << ' ';
	}
	cout << endl;
	sort(arr);
	return 0;
}
