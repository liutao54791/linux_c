#ifndef SORT_ALGORITHM_H
#define SORT_ALGORITHM_H

#include "stdlib.h"
#include <stdio.h>
#include<malloc.h>
#include <iostream>

void swap(int &a,int &b);

void QuickSort(int a[],int low,int high);

void BubbleSort(int a[],int n);

//鸡尾酒排序(Cocktail sort，双向的冒泡排序) — O(n^2）
void CocktailSort(int a[],int n);

//直接插入排序
//将待比较的数值与它的前一个数值进行比较，所以外层循环是从第二个数值开始的。
//当前一数值比待比较数值大的情况下继续循环比较，直到找到比待比较数值小的并将待比较数值置入其后一位置，结束该次循环。
void InsertionSort(int a[],int n);

//折半插入排序(二分插入排序)
//在插入第i个元素时，对前面的0～i-1元素进行折半，先跟他们中间的那个元素比，如果小，则对前半再进行折半，否则对后半进行折半，直到left>right，
//然后再把第i个元素前1位与目标位置之间的所有元素后移，再把第i个元素放在目标位置上。
void BinaryInsertionSort(int a[],int n);

//桶排序 1,桶排序是稳定的 2,桶排序是常见排序里最快的一种,比快排还要快…大多数情况下 3,桶排序非常快,但是同时也非常耗空间,基本上是最耗空间的一种排序算法
void BucketSort(int a[],int n);

//选择排序 直接从待排序数组里选择一个最小(或最大)的数字,每次都拿一个最小数字出来,顺序放入新数组,直到全部拿完
void SelectionSort(int a[],int n);

//奇偶排序 奇数列排一趟序,偶数列排一趟序,再奇数排,再偶数排,直到全部有序
void OddEvenSort(int a[],int n);

//计数排序 这种排序算法，依靠一个辅助数组来实现，不基于比较，算法复杂度为 O(n) ，
//但由于要一个辅助数组C，所以空间复杂度要大一些，由于计算机的内存有限，这种算法不适合范围很大的数的排序。
//注：基于比较的排序算法的最佳平均时间复杂度为 O(nlogn)
void CountSort(int a[],int n);

//堆排序 ---调整堆  父节点：parent(i) = i/2; 孩子节点: 左节点：left(i) = 2*i 右节点：right(i) = 2*i + 1;
void HeapAdjust(int a[],int i,int n);

//堆排序  ---建立堆
void BuildHeap(int a[],int n);

//堆排序 把堆顶的最大数取出,将剩余的堆继续调整为最大堆,具体过程在第二块有介绍,以递归实现
//剩余部分调整为最大堆后,再次将堆顶的最大数取出,再将剩余部分调整为最大堆,这个过程持续到剩余数只有一个时结束
void HeapSort(int a[],int n);

//打印数组
void showArray(int a[],int len);

int sort_main(void);

#endif