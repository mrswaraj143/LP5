#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;

class Parallel_bubble_merge
{
public:
    void parallel_bubble(vector<int> &arr)
    {
        int n = arr.size();

        for (int i = 0; i < n - 1; i++)
        {
#pragma omp parallel for
            for (int j = 0; j < n - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    void merge(vector<int> &arr, int low, int mid, int high)
    {
        vector<int> temp;

        int i = low;
        int j = mid + 1;

        while (i <= mid && j <= high)
        {
            if (arr[i] < arr[j])
            {
                temp.push_back(arr[i]);
                i++;
            }
            else
            {
                temp.push_back(arr[j]);
                j++;
            }
        }

        while (i <= mid)
        {
            temp.push_back(arr[i]);
            i++;
        }

        while (j <= high)
        {
            temp.push_back(arr[j]);
            j++;
        }

        for (int i = low; i <= high; i++)
        {
            arr[i] = temp[i - low];
        }
    }

    void parallel_merge(vector<int> &arr, int low, int high)
    {
        if (low < high)
        {
            int mid = (low + high) / 2;

#pragma omp parallel sections
            {

#pragma omp section
                parallel_merge(arr, low, mid);

#pragma omp section
                parallel_merge(arr, mid + 1, high);
            }
            merge(arr, low, mid, high);
        }
    }

    void print_array_parallel(vector<int> &arr)
    {
        for (int n : arr)
        {
            cout << n << " ";
        }
        cout << endl;
    }
};

class sequential_bubble_merge
{

public:
    void sequential_bubble(vector<int> &arr)
    {
        int n = arr.size();

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    void merge(vector<int> &arr, int low, int mid, int high)
    {
        vector<int> temp;
        int i = low;
        int j = mid + 1;

        while (i <= mid && j <= high)
        {
            if (arr[i] < arr[j])
            {
                temp.push_back(arr[i]);
                i++;
            }
            else
            {
                temp.push_back(arr[j]);
                j++;
            }
        }

        while (i <= mid)
        {
            temp.push_back(arr[i]);
            i++;
        }

        while (j <= high)
        {
            temp.push_back(arr[j]);
            j++;
        }

        for (int i = low; i <= high; i++)
        {
            arr[i] = temp[i - low];
        }
    }

    void sequential_merge(vector<int> &arr, int low, int high)
    {
        if (low < high)
        {
            int mid = (low + high) / 2;

#pragma omp parallel sections
            {
#pragma omp section
                sequential_merge(arr, low, mid);

#pragma omp section
                sequential_merge(arr, mid + 1, high);
            }

            merge(arr, low, mid, high);
        }
    }

    void print_array_sequential(vector<int> &arr)
    {
        for (int n : arr)
        {
            cout << n << " ";
        }
        cout << endl;
    }
};

int main()
{
    Parallel_bubble_merge p;
    sequential_bubble_merge s;

    vector<int> arr1;

    for (int i = 500; i >= 1; i--)
    {
        arr1.push_back(i);
    }

    vector<int> arr2 = arr1;
    vector<int> arr3 = arr2;
    vector<int> arr4 = arr3;

    auto start = high_resolution_clock::now();
    s.sequential_bubble(arr1);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "sequential buuble sort" << endl;
    s.print_array_sequential(arr1);
    cout << "The duration is " << duration.count() << " ms" << endl;


    auto start1 = high_resolution_clock::now();
    p.parallel_bubble(arr2);
    auto end1 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(end1 - start1);

    cout << "parallel bubble sort" << endl;
    p.print_array_parallel(arr2);
    cout << "The time required for parallel " << duration2.count() << " ms" << endl;


    // auto start2 = high_resolution_clock::now();
    // p.parallel_merge(arr3,0,500);
    // auto end2 = high_resolution_clock::now();
    // auto duration3 = duration_cast<microseconds>(end1 - start1);

    // cout << "parallel merge sort" << endl;
    // p.print_array_parallel(arr3);
    // cout << "The time required for parallel " << duration3.count() << " ms" << endl;


    // auto start3 = high_resolution_clock::now();
    // s.sequential_merge(arr4,0,500);
    // auto end3 = high_resolution_clock::now();
    // auto duration4 = duration_cast<microseconds>(end1 - start1);

    // cout << "sequntial merge sort" << endl;
    // s.print_array_sequential(arr4);
    // cout << "The time required for parallel " << duration4.count() << " ms" << endl;
}