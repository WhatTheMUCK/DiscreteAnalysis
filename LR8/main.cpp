#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int main() {
    int N;
    cin >> N;

    vector<int> lengths(N);

    for (int i = 0; i < N; i++) {
        cin >> lengths[i];
    }

    sort(lengths.begin(), lengths.end(), greater<int>());

    double max_area = 0.0;
    vector<int> max_lengths(3, 0);

    for (int i = 0; i < N - 2; i++) {
        int a = lengths[i];
        int b = lengths[i + 1];
        int c = lengths[i + 2];

        if (a < b + c) { // Проверка на возможность образования треугольника
            double s = 0.0;

            // Используем формулу Герона для вычисления площади треугольника
            double p = (a + b + c) / 2.0;
            s = sqrt(p * (p - a) * (p - b) * (p - c));

            if (s > max_area) {
                max_area = s;
                max_lengths[0] = a;
                max_lengths[1] = b;
                max_lengths[2] = c;
            }
        }
    }
    cout.precision(3);
    if (max_area > 0.0) {
        cout << fixed << max_area << "\n";
        cout << max_lengths[2] << " " << max_lengths[1] << " " << max_lengths[0] << "\n";
    } else {
        cout << "0\n";
    }

    return 0;
}
