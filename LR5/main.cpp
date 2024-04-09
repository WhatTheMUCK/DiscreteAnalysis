#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class SuffixTree;

class SuffixTreeNode{
    private:
        unordered_map<char, SuffixTreeNode*> children;
        SuffixTreeNode* suffixLink;
        int start;
        int *end;
        int size;
        int suffixIndex;
    public:
        friend SuffixTree;
    
        SuffixTreeNode(SuffixTreeNode* link, int start, int *end, int ind){
            suffixLink = link;
            this->start = start;
            this->end = end;
            suffixIndex = ind;
        }
};

class SuffixTree{
    private:
    SuffixTreeNode* root = new SuffixTreeNode(nullptr, -1, new int(-1), -1);
    SuffixTreeNode* lastCreatedInternalNode = nullptr;
    string text;
    
    SuffixTreeNode* activeNode = nullptr;
    int activeEdge = -1;
    int activeLength = 0;
    int remainingSuffixCount = 0;
    int leafEnd = -1;
    
    int EdgeLength(SuffixTreeNode* node){
        return *node->end - node->start + 1;
    }
    
    void ExtendSuffixTree(int pos){
        lastCreatedInternalNode = nullptr;
        leafEnd++;
        remainingSuffixCount++;
        while(remainingSuffixCount > 0){
            if (!activeLength)
                activeEdge = pos;
        
            auto find = activeNode->children.find(text[activeEdge]);
            
            if (find == activeNode->children.end()){
                activeNode->children.insert(make_pair(text[activeEdge], new SuffixTreeNode(root, pos, &leafEnd, pos - remainingSuffixCount + 1)));
                
                if (lastCreatedInternalNode != nullptr){
                    lastCreatedInternalNode->suffixLink = activeNode;
                    lastCreatedInternalNode = nullptr;
                }
            } else{
                SuffixTreeNode* next = find->second;
                int edge_length = EdgeLength(next);
                if (activeLength >= edge_length){
                    activeEdge += edge_length;
                    activeLength -= edge_length;
                    activeNode = next;
                    continue;
                }
                
                if (text[next->start + activeLength] == text[pos]){
                    if (lastCreatedInternalNode != nullptr && activeNode != root)
                        lastCreatedInternalNode->suffixLink = activeNode;
                    activeLength++;
                    break;
                }
                
                SuffixTreeNode* split = new SuffixTreeNode(root, next->start, new int(next->start + activeLength - 1), -1);
                activeNode->children[text[activeEdge]] = split;
                next->start += activeLength;
                split->children.insert(make_pair(text[pos], new SuffixTreeNode(root, pos, &leafEnd, pos - remainingSuffixCount + 1)));
                split->children.insert(make_pair(text[next->start], next));
                if (lastCreatedInternalNode != nullptr)
                    lastCreatedInternalNode->suffixLink = split;
                lastCreatedInternalNode = split;
            }
            remainingSuffixCount--;
            if (activeNode == root && activeLength > 0){
                activeLength--;
                activeEdge++;
            } else if (activeNode != root){
                activeNode = activeNode->suffixLink;
            }
        }   
    }
    
    void DeleteSuffixTree(SuffixTreeNode* node){
        for (auto it : node->children)
            DeleteSuffixTree(it.second);
        if (node->suffixIndex == -1)
            delete node->end;
        delete node;
    }
    
    void VisualizeHelper(SuffixTreeNode* node, int level) {
        if (node != nullptr) {
            for (int i = node->start; i <= *node->end; i++) {
                cout << text[i];
            }
            cout << " (Suffix Index: " << node->suffixIndex << ")\n";

            for (auto& pair : node->children) {
                VisualizeHelper(pair.second, level + 1);
            }
        }
    }
    
    void LeafsFinder(SuffixTreeNode* node, vector<int>* leafs){
        if (node != nullptr){
            if (node->suffixIndex != -1){
                leafs->push_back(node->suffixIndex + 1);
            } else {
                for (auto& pair : node->children){
                    LeafsFinder(pair.second, leafs);
                }
            }
        }
    }
    
    public:
    SuffixTree(string &text){
        this->text = text+"$";
        BuildSuffixTree();
        SizeSuffixTree(this->root);
    }
    
    void BuildSuffixTree(){
        activeNode = root;
        for (int i = 0; i < text.length(); i++)
            ExtendSuffixTree(i);
    }
    
    void SizeSuffixTree(SuffixTreeNode* node){
        if (node != nullptr){
            node->size = *node->end - node->start + 1;
            for (auto& pair: node->children){
                SizeSuffixTree(pair.second);
            }
        }  
    }
    
    void Visualize(){
       VisualizeHelper(root, 0);
    }
    
    void FindPatternOccurrences(const string &pattern, int order) {
        int patternLength = pattern.length();
        
        vector<int> occurrences(0);
        SuffixTreeNode* currentNode = root;
        int currentLength = 0;
        int patternIndex = 0;

        while (patternIndex < patternLength) {
            char currentChar = pattern[patternIndex];
            auto find = currentNode->children.find(currentChar);

            if (find == currentNode->children.end()) {
                // Образец не найден
                break;
            }

            SuffixTreeNode* nextNode = find->second;
            int edgeLength = nextNode->size;

            if (currentLength >= edgeLength) {
                // Переходим к следующему узлу
                currentLength -= edgeLength;
                patternIndex += edgeLength;
                currentNode = nextNode;
            } else {
                // Сравниваем символы на ребре
                char edgeChar = text[nextNode->start + currentLength];
                while (edgeChar == pattern[patternIndex] && currentLength < edgeLength) {
                    currentLength++;
                    patternIndex++;
                    edgeChar = text[nextNode->start + currentLength];
                }
                if (patternIndex == patternLength) {
                // Нашли образец, добавляем его позицию в список
                    LeafsFinder(nextNode, &occurrences);
                } else if (edgeChar != pattern[patternIndex] && edgeLength > currentLength) {
                    // Образец не совпадает с символами на ребре
                    break;
                }
            }

            if (currentLength == edgeLength) {
                // Достигли конца ребра, переходим к следующему узлу
                currentNode = nextNode;
                currentLength = 0;
            }
            
            if (text[nextNode->start + currentLength] == '$')
                break;
        }

        // Выводим результат
        if (!occurrences.empty()) {
            cout << order << ":";
            string output = "";
            sort(occurrences.begin(), occurrences.end());
            for (int position : occurrences) {
                output +=  " " + to_string(position) + ",";
            }
            //output.pop_back();
            output.pop_back();
            output += "\n";
            cout << output;
        }
    }
    
    ~SuffixTree(){
        DeleteSuffixTree(root);
    }
};

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);
    string text, pattern;
    cin >> text;
    SuffixTree suffixTree(text);
    int order = 1;
    while (cin >> pattern)
        suffixTree.FindPatternOccurrences(pattern, order++);
}

