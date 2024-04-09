#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

int main()
{
    long long n, temp10, answer = 0;
    string stringN;
    int m, size;
    cin >> n >> m;
    stringN = to_string(n);
    size = stringN.size(); 
    vector<long long> a(size);
    a[0] = stringN[0] - '0';
    for (int i = 1; i < size; i++){
        a[i] = a[i - 1] * 10 + (stringN[i] - '0');
    }
    temp10 = 1;
    for (int i = 0; i < size; i++){
        a[i] = (a[i]/m) - ((temp10 - 1)/m);
        temp10 *= 10;
    }
    for (int i = 0; i < size; i++){
        answer += a[i];
    }
    if (n%m == 0)
        answer--;
    cout << answer;
}

