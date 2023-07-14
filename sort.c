#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated

//swap funtion taken from code written for bubble sort for lab assignment 7.
void swap(int* xp, int* yp){
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}
void heapify(int arr[], int n, int i){
	int biggest = i;
	//this equation was taken from the powerpoint to represent the left child
	int left = 2 * i + 1;
	//this one represents right child taken from the same powerpoint
	int right = 2 * i + 2;

	//this is for when left child is larger than its parent 
	if (left < n && arr[left] > arr[biggest])
		biggest = left;

	//this is for when right child is larger than its parent
	if ( right < n && arr[right] > arr[biggest])
		biggest = right; 

	//if the biggest number is not the root it will swap it with whatever child was fount to be bigger in the above if statements
	if (biggest != i){

		//calls the swap function to swap the numbers
		swap(&arr[i], &arr[biggest]);

		//calls heapifiy again to continue sorting until the biggest number is in the root node
		heapify ( arr, n, biggest);
	}
}
void heapSort(int arr[], int n)
{
	extraMemoryAllocated = 0;
	 // this is to create the max heap
    for (int i = n / 2 - 1; i >= 0; i--)
 
        heapify(arr, n, i);
 
    // this loop sorts the heap 
    for (int i = n - 1; i >= 0; i--) {

		//swaps ints so largest is on top to eventually get the biggest to the root
        swap(&arr[0], &arr[i]);
 
        // keep calling heapify untill the biggest is the root
        heapify(arr, i, 0);

		extraMemoryAllocated += sizeof(int);
    }

}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated


//merge function taken from what I turned in on the last assignment
void merge(int pData[], int l, int m, int r)
{
	int i;
	int j;
	int k;
	int n1 = m - l + 1;
	int n2 = r - m;

	//creates temporary arrays
	int *L = (int*) malloc(n1*sizeof(int));
	int *R = (int*) malloc(n2*sizeof(int));


	//copy the data over to the temporary arrays
	for (i = 0; i < n1; i++)
		L[i] = pData[l + i];
	for (j = 0; j < n2; j++)
		R[j] = pData[m + 1 + j];


	//the next section merges the temporry arrays back into pData array
	i = 0;
	j = 0;
	k = l;

	while (i < n1 && j < n2){
		if (L[i] <= R[j]){
			pData[k] = L[i];
			i++;
		}
		else{
			pData[k] = R[j];
			j++;
		}
		k++;
	}
	while (i < n1){
		pData[k] = L[i];
		i++;
		k++;
	}
	while (j < n2){
		pData[k] = R[j];
		j++;
		k++;
	}
	//frees the temporary arrays
	free(L);
	free(R);
}

//mergeSort function also taken from what I turned in on the last assignment. (assignmnet 4)
void mergeSort(int pData[], int l, int r)
{


int *tempArray;

if (l < r)
{
int m = (l+r)/2;

// Only allocate a temporary array for the first recursive call
if (l == 0)
{
tempArray = (int*) malloc(sizeof(int)*(r - l + 1));
}

// Sort first and second halves
mergeSort(pData, l, m);
mergeSort(pData, m+1, r);

merge(pData, l, m, r);

// Free the temporary array
if (l == 0)
{
free(tempArray);
}

// Update the extra memory allocated
if (l == 0)
{
extraMemoryAllocated += sizeof(int)*(r - l + 1);
}
return;
}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");

		
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

	
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}