#include "helpers.h"

vector<uint32_t> recoverPath(uint32_t from, uint32_t goal, vector<uint32_t>& ids, vector<uint32_t>& parent); 

double AStar(uint32_t start, uint32_t goal, vector<uint32_t>& ids, 
             vector<uint32_t>& offsets, FILE* inputFile, uint32_t infoStart, uint32_t adjStart, 
             vector<double>& g, vector<double>& f, vector<uint32_t>& parent);

void readGraph(FILE*& inputFile, vector<uint32_t>& ids, vector<uint32_t>& offsets);

void search(FILE* graphFile, FILE* inputFile, FILE* outputFile, bool fullOutput);