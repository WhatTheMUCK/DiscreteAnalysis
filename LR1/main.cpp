#include <iostream>
#include <cstring>
using namespace std;

const int MAX_VALUE = 1000000;
const int MAX_LENGTH = 2048;

typedef struct {
    char **messages;
    int size;
    int place;
} own_vector;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int key;
    char *value = new char[MAX_LENGTH];
    own_vector *storage = new own_vector[MAX_VALUE];
    for (int i = 0; i < MAX_VALUE; ++i){
        storage[i].size = 0; 
        storage[i].place = 1;
    }
    int maxI = -1;
    int minI = MAX_VALUE;
    while ((cin >> key) and (cin >> value)){
        if (storage[key].size == 0) {
            storage[key].messages = new char*[1];
            storage[key].messages[0] = new char[strlen(value) + 1];
        } else if (storage[key].size >= storage[key].place) {
            storage[key].place *= 2;
            char **newStorage = new char*[storage[key].place];
            memcpy(newStorage, storage[key].messages, storage[key].size * sizeof(char*));
            storage[key].messages = newStorage;
            storage[key].messages[storage[key].size] = new char[strlen(value) + 1];
        } else {
            storage[key].messages[storage[key].size] = new char[strlen(value) + 1];
        }
        strcpy(storage[key].messages[storage[key].size], value);
        ++storage[key].size;
    	if(key < minI) {
    	    minI = key;
    	}
    	if(key > maxI) {
    	    maxI = key;
    	}
    }
    //cout << "\n";
    char *buffer = new char[6];
    for (int i = minI; i <= maxI; ++i) {
        sprintf(buffer, "%06d", i);
        for (int j = 0; j < storage[i].size; ++j)
            cout << buffer << "\t" << storage[i].messages[j] << "\n";
    }
}
