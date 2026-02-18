#include <stdio.h>

int binSearch(const int* arr, int n, int temp)
{
    if (n < 1) {
        return -1;
    }
    int l = 0;
    int r = n - 1;
    while (l <= r) {
        int p = (r + l) / 2;
        if (arr[p] == temp) {
            return p;
        } else if (arr[p] > temp) {
            r = p - 1;
        } else {
            l = p + 1;
        }
    }
    return -1;
}

void printArr(int* arr, int n)
{
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
int main(void)
{
    int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 22 };
    printf("%d\n", binSearch(arr, 10, 7));

    return 0;
}