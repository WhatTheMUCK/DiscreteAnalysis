#include <iostream>

using namespace std;
 
class String{
public:
	char* clipboard;
	int size;
	int length;
 
	String(){
		size = 1;
		length = 0;
		clipboard = new char[size];
		clipboard[length] = '\0';
	}

	String(const String& string){
		size = string.size;
		length = string.length;
		clipboard = new char[size];
		Copy(clipboard, string.clipboard);
	}

	String(const char* string){
		length = Length(string);
		size = length+1;
		clipboard = new char[size];
		Copy(clipboard, string);
	}

	~String(){
		delete[] clipboard;
	}

	char* Copy(char* string_left, const char* string_right){
		while (*string_right)
			*string_left++ = *string_right++;
		++* string_left = '\0';
		return string_left;
	}
 
	int Length(const char* line){
		if (*line)
			return Length(++line)+1;
		return 0;
	}
 
	static int Lexis(const char* string_left, const char* string_right){
		while (*string_left && *string_right && *string_left == *string_right)
			++string_left, ++string_right;
		return *string_left-*string_right;
	}
 
	void Swap(String& string){
		char* temporary_storage = clipboard;
		clipboard = string.clipboard;
		string.clipboard = temporary_storage;
		int temporary = size;
		size = string.size;
		string.size = temporary;
		temporary = length;
		length = string.length;
		string.length = temporary;
	}
 
	int LengthString(){
		return length;
	}
 
	String& operator =(const String& string) {
		if (this != &string) {
			delete[] clipboard;
			clipboard = new char[string.size];
			Copy(clipboard, string.clipboard);
			size = string.size;
			length = string.length;
		}
		return *this;
	}

	char& operator [](int i) const{
		return clipboard[i];
	}
 
	friend bool operator ==(const String& string_left, const String& string_right){
		return Lexis(string_left.clipboard, string_right.clipboard) == 0;
	}
 
	friend bool operator <(const String& string_left, const String& string_right) {
		return Lexis(string_left.clipboard, string_right.clipboard) < 0;
	}

	friend bool operator >(const String& string_left, const String& string_right){
		return Lexis(string_left.clipboard, string_right.clipboard) > 0;
	}
 
	friend ostream& operator <<(ostream& Writeing, const String& string){
		for (int i = 0; i < string.length; i++){
			Writeing << string.clipboard[i];
		}
		return Writeing;
	}
 
	friend istream& operator >>(istream& reading, String& string){
		if (string.clipboard != NULL){
			delete[] string.clipboard;
		}
		string.size = 2<<3; // 2<<3 = 16
		string.length = 0;
		string.clipboard = new char[string.size];
		string.clipboard[string.length] = '\0';
		char icon;
		while (reading.get(icon)){
			if (icon == '\n' or icon == ' '){
				break;
			}
			else{
				if (string.length+1 == string.size){
					string.size *= 2;
					char* storage = new char[string.size];
					for (int i = 0; i < string.length; i++)
						storage[i] = string.clipboard[i];
					delete[] string.clipboard;
					string.clipboard = storage;
				}
			}
			if (icon >= 'A' and icon <= 'Z')
				icon += 'a'-'A';
			string.clipboard[string.length++] = icon;
		}
		string.clipboard[string.length] = '\0';
		return reading;
	}
};
 
struct Element{
	String word;
	uint64_t number;

	Element(){}
	Element& operator =(Element& string){
		word.Swap(string.word);
		number = string.number;
		return *this;
	}
};
 
class TreeNode{
public:
	int T;
	int data_count;
	bool is_leaf;
	Element* data;
	TreeNode** kids;
 
	TreeNode(int number, bool is_sheet){
		is_leaf = is_sheet;
		T = number;
		data = new Element[2*T-1];
		kids = new TreeNode * [2*T];
		for (int i = 0; i < 2*T; i++)
			kids[i] = NULL;
		data_count = 0;
	}
 
	~TreeNode(){
		delete[] data;
		delete[] kids;
	}
 
	Element* Search(const String& string){
		int i = 0;
		while (i < data_count && data[i].word < string)
			i++;
		if (i < data_count && data[i].word == string)
			return &data[i];
		if (is_leaf)
			return NULL;
		return kids[i]->Search(string);
	}
 
	void DeleteNode(){
		if (this != NULL){
			for (int i = 0; i < data_count+1; i++){
				if (kids[i] != NULL)
					kids[i]->DeleteNode();
			}
			delete this;
		}
	}
 
	void
		InsertForNoFull(const Element& element){
		int i = data_count-1;
		if (is_leaf){
			while (i >= 0 && element.word < data[i].word){
				data[i+1] = data[i];
				--i;
			}
			data[i+1].word = element.word;
			data[i+1].number = element.number;
			++data_count;
		}
		else{
			while (i >= 0 && element.word < data[i].word)
				--i;
			i++;
			if (kids[i]->data_count == 2*T-1){
				SplitChild(i);
				if (data[i].word < element.word){
					i++;
				}
			}
			kids[i]->InsertForNoFull(element);
		}
	}
 
	void SplitChild(int i){
		TreeNode* left_child = kids[i];
		TreeNode* right_child = new TreeNode(T, left_child->is_leaf);
		right_child->data_count = T-1;
		int second_i;
		for (second_i = 0; second_i < T-1; second_i++)
			right_child->data[second_i] = left_child->data[second_i+T];
 
		if (!left_child->is_leaf)
			for (second_i = 0; second_i < T; second_i++)
				right_child->kids[second_i] = left_child->kids[second_i+T];
		left_child->data_count = T-1;
		for (second_i = data_count; second_i >= i+1; second_i--)
			kids[second_i+1] = kids[second_i];
		kids[i+1] = right_child;
		for (second_i = data_count-1; second_i >= i; second_i--)
			data[second_i+1] = data[second_i];
		data[i] = left_child->data[T-1];
		++data_count;
	}
 
	void Delete_leaf(int i){
		for (int second_i = i+1; second_i < data_count; second_i++)
			data[second_i-1] = data[second_i];
		--data_count;
		cout << "OK\n";
	}
 
	void DeleteInnerNode(int i){
		String storage;
		storage = data[i].word;
		if (kids[i]->data_count >= T){
			TreeNode* predecessor;
			predecessor = FindPredecessor(i);
			data[i].word = predecessor->data[predecessor->data_count-1].word;
			data[i].number = predecessor->data[predecessor->data_count-1].number;
			kids[i]->Delete(data[i].word);
		}
		else{
			if (kids[i+1]->data_count >= T){
				TreeNode* successor;
				successor = FindSuccessor(i);
				data[i].word = successor->data[0].word;
				data[i].number = successor->data[0].number;
				kids[i+1]->Delete(data[i].word);
			}
			else{
				Merge(i);
				kids[i]->Delete(storage);
			}
		}
	}
 
	void Balance(int i){
		if (i != 0 && kids[i-1]->data_count >= T){
			GetFromLeftChild(i);
		} else{
			if (i != data_count && kids[i+1]->data_count >= T){
				GetFromRightChild(i);
			} else{
				if (i != data_count){
					Merge(i);
				} else{
					Merge(i-1);
				}
			}
		}
	}
 
	void Delete(const String& string){
		int i = 0;
		while (i < data_count && data[i].word < string)
			i++;
		if (i < data_count && data[i].word == string){
			if (is_leaf){
				Delete_leaf(i);
			} else{
				DeleteInnerNode(i);
			}
		} else{
			if (is_leaf){
				cout << "NoSuchWord" << '\n';
			} else{
				bool last = (i == data_count);
				if (kids[i]->data_count < T)
					Balance(i);
				if (last && i > data_count){
					kids[i-1]->Delete(string);
				} else{
					kids[i]->Delete(string);
				}
			}
		}
	}
 
	void Merge(int i){
		TreeNode* child = kids[i];
		TreeNode* brother_for_child = kids[i+1];
		child->data[T-1] = data[i];
		for (int second_i = 0; second_i < brother_for_child->data_count; second_i++)
			child->data[second_i+T] = brother_for_child->data[second_i];
		if (!child->is_leaf)
			for (int second_i = 0; second_i <= brother_for_child->data_count; second_i++)
				child->kids[second_i+T] = brother_for_child->kids[second_i];
		for (int second_i = i+1; second_i < data_count; second_i++)
			data[second_i-1] = data[second_i];
		for (int second_i = i+2; second_i <= data_count; second_i++)
			kids[second_i-1] = kids[second_i];
		child->data_count += brother_for_child->data_count+1;
		data_count--;
		delete brother_for_child;
	}
 
	void GetFromLeftChild(int i){
		TreeNode* left_child = kids[i-1];
		TreeNode* right_child = kids[i];
		for (int second_i = right_child->data_count-1; second_i >= 0; second_i--)
			right_child->data[second_i+1] = right_child->data[second_i];
		if (!right_child->is_leaf){
			for (int second_i = right_child->data_count; second_i >= 0; second_i--)
				right_child->kids[second_i+1] = right_child->kids[second_i];
			right_child->kids[0] = left_child->kids[left_child->data_count];
		}
		right_child->data[0] = data[i-1];
		data[i-1] = left_child->data[left_child->data_count-1];
		++right_child->data_count;
		--left_child->data_count;
	}
 
	void GetFromRightChild(int i){
		TreeNode* left_child = kids[i];
		TreeNode* right_child = kids[i+1];
		left_child->data[left_child->data_count] = data[i];
		if (!left_child->is_leaf)
			left_child->kids[left_child->data_count+1] = right_child->kids[0];
		data[i] = right_child->data[0];
		for (int second_i = 1; second_i < right_child->data_count; second_i++)
			right_child->data[second_i-1] = right_child->data[second_i];
		if (!right_child->is_leaf)
			for (int second_i = 1; second_i <= right_child->data_count; second_i++)
				right_child->kids[second_i-1] = right_child->kids[second_i];
		++left_child->data_count;
		--right_child->data_count;
	}
 
	TreeNode* FindSuccessor(int i){
		TreeNode* node = kids[i+1];
		while (!node->is_leaf)
			node = node->kids[0];
		return node;
	}
 
	TreeNode* FindPredecessor(int i){
		TreeNode* node = kids[i];
		while (!node->is_leaf)
			node = node->kids[node->data_count];
		return node;
	}
 
	bool Read(FILE* file){
		if (fread(&is_leaf, sizeof(is_leaf), 1, file) != 1)
			return false;
		if (fread(&data_count, sizeof(data_count), 1, file) != 1)
			return false;
		for (int i = 0; i < data_count; i++){
			if (fread(&data[i].word.length, sizeof(data[i].word.length), 1, file) != 1)
				return false;
			delete[] data[i].word.clipboard;
			data[i].word.size = data[i].word.length+1;
			data[i].word.clipboard = new char[data[i].word.size];
			if (fread(data[i].word.clipboard, sizeof(char), data[i].word.length, file) != data[i].word.length)
				return false;
			data[i].word.clipboard[data[i].word.length] = '\0';
			if (fread(&data[i].number, sizeof(data[i].number), 1, file) != 1)
				return false;
		}
		if (!is_leaf){
			for (int i = 0; i < data_count+1; i++){
				kids[i] = new TreeNode(T, false);
				if (!kids[i]->Read(file))
					return false;
			}
		}
		return true;
	}
 
	bool Write(FILE* file){
		if (fwrite(&is_leaf, sizeof(is_leaf), 1, file) != 1)
			return false;
		if (fwrite(&data_count, sizeof(data_count), 1, file) != 1)
			return false;
		for (int i = 0; i < data_count; i++){
			int length = data[i].word.LengthString();
			char* line = data[i].word.clipboard;
			if (fwrite(&length, sizeof(length), 1, file) != 1)
				return false;
			if (fwrite(line, sizeof(char), length, file) != length)
				return false;
			if (fwrite(&data[i].number, sizeof(data[i].number), 1, file) != 1)
				return false;
		}
		if (!is_leaf){
			for (int i = 0; i < data_count+1; i++)
				if (!kids[i]->Write(file))
					return false;
		}
		return true;
	}
};
 
class Tree{
private:
	int T;
	TreeNode* root;
 
public:
	Tree(int number){
		T = number;
		root = new TreeNode(number, true);
	}
 
	~Tree(){
		if (root != NULL)
			root->DeleteNode();
	}
 
	Element* Search(const String& string){
		if (root == NULL){
			return NULL;
		} else{
			return root->Search(string);
		}
	}
 
	void Delete(const String& string){
		if (root != NULL){
			root->Delete(string);
		} else{
			return;
		}
		if (!root->is_leaf && root->data_count == 0){
			TreeNode* temporary = root;
			root = root->kids[0];
			temporary->kids[0] = NULL;
			delete temporary;
		}
	}
 
	void Insert(Element& element){
		if (root->data_count == 2*T-1){
			TreeNode* old_root = root;
			TreeNode* new_root = new TreeNode(T, false);
			root = new_root;
			new_root->kids[0] = old_root;
			new_root->SplitChild(0);
			if (new_root->data[0].word > element.word){
				new_root->kids[0]->InsertForNoFull(element);
			} else{
				new_root->kids[1]->InsertForNoFull(element);
			}
		} else{
			root->InsertForNoFull(element);
		}
	}
 
	bool Read(const char* path){
		FILE* file = fopen(path, "rb");
		if (file == NULL)
			return false;
		if (fread(&T, sizeof(T), 1, file) != 1)
			return false;
		TreeNode* new_root = new TreeNode(T, true);
		bool success = new_root->Read(file);
		fclose(file);
		if (success){
			if (root != NULL)
				delete root;
			root = new_root;
		} else{
			delete new_root;
		}
		return success;
	}
 
	bool Write(const char* path){
		FILE* file = fopen(path, "wb");
		if (file == NULL)
			return false;
		if (fwrite(&T, sizeof(T), 1, file) != 1)
			return false;
		bool success = root->Write(file);
		fclose(file);
		return success;
	}
 
	void DeleteTree(){
		if (root == nullptr)
			return;
		root->DeleteNode();
		root = nullptr;
	}
};
 
int main(){
	Tree tree(3);
	char icon;
	Element* res;
	cin.tie(NULL);
	ios_base::sync_with_stdio(false);
	String path;
	while (!cin.eof()){
		Element element;
		cin.get(icon);
		if (cin.eof())
			break;
		switch (icon){
		case '\n':
			break;
		case '+':
			cin.get();
			cin >> element.word;
			cin >> element.number;
			res = tree.Search(element.word);
			if (res != NULL) {
				cout << "Exist\n";
			}
			else {
				tree.Insert(element);
				cout << "OK\n";
			}
			break;
		case '-':
			cin.get();
			cin >> element.word;
			tree.Delete(element.word);
			break;
		case '!':
			cin.get();
			cin >> element.word >> path;
			if (element.word == "save"){
				if (tree.Write(path.clipboard)){
					cout << "OK\n";
				} else{
					cout << "OK\n";
				}
			}
			else{
				if (tree.Read(path.clipboard)){
					cout << "OK\n";
				}
				else{
					tree.DeleteTree();
					cout << "OK\n";
				}
			}
			break;
		default:
			cin.unget();
			cin >> element.word;
			res = tree.Search(element.word);
			if (res == NULL){
				cout << "NoSuchWord\n";
			} else{
				cout << "OK: " << res->number << "\n";
			}
			break;
		}
	}
	return 0;
}
