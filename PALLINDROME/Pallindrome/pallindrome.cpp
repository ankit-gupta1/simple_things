/*
 * pallindrome.cpp
 *
 *  Created on: 20-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

void print_sub_string(char* str, int low, int high)
{
    for(int i = low; i < high - 1; i++)
        cout<<str[i];
}

void longest_pallindrome(char *str)
{
    int i, j, k;
    int n = 0; /*Assuming NULL terminated string*/
    bool **sub_str_pal_table = NULL;

    int start = 0;
    int maxlength = 1;

    while (str[n])
        n++;

    /*Dynamically create n_X_n size table. */
    sub_str_pal_table = new bool*[n];

    for (i = 0; i < n; i++)
        sub_str_pal_table[i] = new bool[n];

    /*Initialise truth table to false. */
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            sub_str_pal_table[i][j] = false;

    // All substrings of length 1 are palindrome.
    for (i = 0; i < n; i++)
        sub_str_pal_table[i][i] = true;

    // check for sub-string of length 2.
    for (i = 0; i < (n - 1); i++) {
        if(str[i] == str[i+1])
        {
            sub_str_pal_table[i][i+1] = true;
            start = i;
            maxlength = 2;
        }
    }

    for(k = 3; k <= n; k++) {
        for(i = 0; i < (n - k + 1); i++) {
            int j = i + k - 1;
            if (sub_str_pal_table[i+1][j-1] && str[i] == str[j]) {
                sub_str_pal_table[i][j] = true;
                if(k > maxlength) {
                    start = i;
                    maxlength = k;
                }
            }
        }
    }

    cout<<"\nLongest palindrome substring is: ";
    print_sub_string(str, start, start + maxlength + 1);
}

int main()
{
    char str[] = "fsgeekskeegfs";
    longest_pallindrome(str);
    return 0;
}
