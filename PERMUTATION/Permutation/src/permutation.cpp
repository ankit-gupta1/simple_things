/*
 * permutation.cpp
 *
 *  Created on: 19-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>

using namespace std;

void swap(char &x, char &y)
{
    char temp;
    temp = x;
    x = y;
    y = temp;
}

void permute(char *a, int i, int n)
{
   int j;
   if (i == n)
     cout<<a<<endl;
   else {
        for (j = i; j <= n; j++) {
          swap(a[i], a[j]);
          permute(a, i+1, n);
          swap(a[i], a[j]); //backtrack
       }
   }
}

int main()
{
       char a[] = "ABC";
       permute(a, 0, (sizeof(a)/sizeof(a[0])) - 2);
       return 0;
}

