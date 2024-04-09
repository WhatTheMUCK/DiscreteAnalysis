#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <iterator>

using namespace std;

//Определяем для каждой позиции паттерна (с конца) позицию правого конца крайней правой копии P[i...n] 
// (P - паттерн, n - длина паттерна, P[i...n] - суффикс паттерна начиная с iой позиции), которая сама не является суффиксом P
void CalculateSuffixes(const vector<unsigned long long int> &pattern, vector<int> *result) {
    const int patternSize = pattern.size();

    vector<int> suffixes(patternSize);
    suffixes[patternSize - 1] = patternSize;

    int previousIndex = 0; 
    int g = patternSize - 1;

    for (int i = patternSize - 2; i >= 0; --i) {
        if (i > g && suffixes[i + patternSize - 1 - previousIndex] < i - g) {
            suffixes[i] = suffixes[i + patternSize - 1 - previousIndex];
        } else {
            g = i;
            previousIndex = i;
            while (g >= 0 && pattern[g] == pattern[g + patternSize - 1 - i])
                --g;
            suffixes[i] = i - g;
        }
    }
    *result = suffixes;
}

//Правило хорошего суффикса 
void CalculateGoodSuffixShiftFunction(const vector<unsigned long long int> &pattern, const vector<int> &suffixes, vector<int> *result) {
    const int patternSize = pattern.size();
    int j = 0;

    vector<int> goodSuffixShiftFunction(patternSize);

    for (int i = patternSize - 1; i >= -1; --i) {
        if (i == -1 || suffixes[i] == i + 1) {
            while (j < patternSize - 1 - i) {
                goodSuffixShiftFunction[j] = patternSize - 1 - i;
                ++j;
            }
        }
    }
    for (int i = 0; i <= patternSize - 2; ++i)
        goodSuffixShiftFunction[patternSize - 1 - suffixes[i]] = patternSize - 1 - i;

    *result = goodSuffixShiftFunction;
}

// Сдвиг вхождения (Правило плохого символа)
void CalculateoccurrenceShiftFunctionFunction(const vector<unsigned long long int> &pattern, map<unsigned long long int, int> *result) {
    
    const int patternSize = pattern.size();
    map<unsigned long long int, int> occurrenceShiftFunction;

    for (const unsigned long long int element: pattern) {
        for (int i = 1; i <= patternSize - 1; ++i)
            if (pattern[patternSize - 1 - i] == element) {
                occurrenceShiftFunction[element] = i;
                break;
            }
        }
    *result = occurrenceShiftFunction;
}

// Алгоритм Апостолико-Джанкарло
void ApostolicoGiancarlo(const vector<unsigned long long int> &pattern, const vector<vector<unsigned long long int>> &text, vector<vector<unsigned long long int>> *result) {
    const int patternSize = pattern.size(); 
    const int textSize = text.size(); 

    vector<int> suffixes;
    CalculateSuffixes(pattern, &suffixes); 
    
    vector<int> goodSuffixShiftFunctiones;
    CalculateGoodSuffixShiftFunction(pattern, suffixes, &goodSuffixShiftFunctiones); 
    
    map<unsigned long long int, int> occurrenceShiftFunction; 
    CalculateoccurrenceShiftFunctionFunction(pattern, &occurrenceShiftFunction); 

    vector<int> skip(textSize); 
    vector<vector<unsigned long long int>> positions;

    int j = 0;
    while (j <= textSize - patternSize) {
        int i = patternSize - 1;
        while (i >= 0) {
            if (skip[i + j] == 0) {
                if (pattern[i] == text[i+j][0]) 
                    --i;
                else 
                    break;
            }
            else if (skip[i + j] > suffixes[i]) {
                i -= suffixes[i];
                break;
            }
            else if (skip[i + j] < suffixes[i]) {
                i -= skip[i + j];
                break;
            }
            else {
                i -= suffixes[i];
            }
        }
        if (i < 0) {
            positions.push_back({text[j][1], text[j][2]});
            j += goodSuffixShiftFunctiones[0];
        } else {
            const map<unsigned long long int, int>::const_iterator search = occurrenceShiftFunction.find(text[i + j][0]);
            const int value = (search == occurrenceShiftFunction.cend()) ? patternSize : search->second; //Если символа в паттерне просто нет, то стоит паттерн сместить полностью за отсутствующий в паттерне символ, иначе на высчитанный сдвиг по правилу плохого символа
            j += max(goodSuffixShiftFunctiones[i], value - patternSize + i + 1);
        }
    }

    *result = positions;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    vector<unsigned long long int> pattern;
    vector<vector<unsigned long long int>> text;
    unsigned long long int number;
    string s;
    
    getline(cin, s);
    istringstream input(s);
    while (input >> number)
        pattern.push_back(number);
        
    unsigned long long int stringNumber = 0; 
    unsigned long long index;
    while (getline(cin, s)){
    	index = 0;
        stringNumber++;
        istringstream input(s);
        while (input >> number){
            text.push_back({number, stringNumber, index});
        	index++;
        }
    }
    
    vector<vector<unsigned long long int>> result;
    ApostolicoGiancarlo(pattern, text, &result);
    
    for (int i = 0; i < result.size(); i++)
        cout << result[i][0] << ", " << result[i][1] + 1 << endl;
}

    
    
