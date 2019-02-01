/*
 ============================================================================
 Name        : QuickSort.c
 Author      : YF
 Version     :
 Copyright   : Your copyright notice
 Description : QuickSort in C, Ansi-style
 ============================================================================
 */


#include <iostream>
#include <fstream>
using namespace std;

void showArray(int A[],int sz)
{
	for(int i = 0; i < sz; ++i)
		printf("%d\n",A[i]);
	printf("\n");
}

void quickSort(int A[],int s_idx, int l_idx);

int main(void) {

	int A[100000] = { -1 };
	int index = 0;

	std::fstream in_file("./src/input_arrays/quick_sort_data.txt", std::ios_base::in);

	while(in_file >> A[index])
	{
		++index;
	}
	cout << "The read size is = " << index << endl;

	quickSort(A,0,index - 1);
	showArray(A,index);

	return EXIT_SUCCESS;
}

int choose_pivot(int A[],int s_idx, int l_idx)
{
	return s_idx;
}

void swap(int *p1, int *p2)
{
	int tmp  = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

int partition_array(int A[],int s_idx, int l_idx,int p_idx)
{
	int pivot_element = A[p_idx];
	int i,j;
	i  = s_idx + 1;
	for(j = s_idx + 1; j <= l_idx; ++j)
	{
		if(A[j] < pivot_element)
		{
			swap(&A[j],&A[i]);
			++i;
		}
	}
	swap(&A[i - 1],&A[p_idx]);
	return i - 1;
}

void quickSort(int A[],int s_idx, int l_idx)
{
	if(l_idx == s_idx) // size of 1
		return;
	int pivot_idx = choose_pivot(A,s_idx,l_idx);
	pivot_idx = partition_array(A,s_idx,l_idx,pivot_idx);
	if(pivot_idx == s_idx)
		quickSort(A,s_idx,pivot_idx);
	else
		quickSort(A,s_idx,pivot_idx - 1);

	if(pivot_idx !=  l_idx)
		quickSort(A,pivot_idx + 1,l_idx);
}