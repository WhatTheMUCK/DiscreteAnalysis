#include "preprocess.h"
#include "search.h"

const int PREPROCESS_ARGS_COUNT = 3;
const int SEARCH_ARGS_COUNT = 3;

inline int fileCheck(FILE*& file, string&& fileName, string&& curParam, bool forWrite = false, bool asBinary = false){
    if(file != NULL){
        throw logic_error("Filename already set for " + curParam + " file");
    }

    string args = "";
    if(forWrite){
        args += "w";
    } else{
        args += "r";
    }
    if(asBinary){
        args += "b";
    }
    file = fopen(fileName.c_str(), args.c_str());

    if(file == NULL){
        throw logic_error("Unable to open " + curParam + " file");
    }
    return 0;
}

int main(int argc, char* argv[]){
    cerr.tie(nullptr);

    if(!strcmp(argv[1], "preprocess")){
        try{
            if(argc < 2 + 2 * PREPROCESS_ARGS_COUNT){
                throw logic_error("Wrong number of arguments");
            }
        } catch(exception& ex){
            cerr << ex.what() << '\n';
            return -1;
        }

        FILE* nodesFile = NULL, *edgesFile = NULL;
        FILE* outputFile = NULL;
        for(size_t i = 2; i < 2 * PREPROCESS_ARGS_COUNT + 2; i += 2){
            if(!strcmp(argv[i], "--nodes")){
                fileCheck(nodesFile, argv[i + 1], argv[i] + 2);
            } else if(!strcmp(argv[i], "--edges")) {
                fileCheck(edgesFile, argv[i + 1], argv[i] + 2);
            } else if(!strcmp(argv[i], "--output")){
                fileCheck(outputFile, argv[i + 1], argv[i] + 2, true, true);
            } else{
                throw logic_error("Undefined parameter: " + string(argv[i]));
            }
        }

        vector<uint32_t> ids;
        preprocessNodes(nodesFile, outputFile, ids);
        
        fclose(nodesFile);

        preprocessEdges(edgesFile, outputFile, ids);

        fclose(edgesFile);
        fclose(outputFile);
    } 
    else if(!strcmp(argv[1], "search")){
        try{
            if(argc < 2 + 2 * SEARCH_ARGS_COUNT || argc > 2 + 2 * SEARCH_ARGS_COUNT + 1){
                throw logic_error("Wrong number of arguments");
            }
        } catch(exception& ex){
            cerr << ex.what() << '\n';
            return -1;
        }

        FILE* graphFile = NULL, *inputFile = NULL;
        FILE* outputFile = NULL;
        try{
            bool fullOutput = false;
            for(size_t i = 2; i < (2 * PREPROCESS_ARGS_COUNT + 2 + 1) && ((int)i < argc); i += 2){
                if(!strcmp(argv[i], "--graph")){
                    if(int(i + 1) >= argc)
                        throw logic_error("Missing graph file name");
                    fileCheck(graphFile, argv[i + 1], argv[i] + 2, false, true);
                } else if(!strcmp(argv[i], "--input")) {
                    if(int(i + 1) >= argc)
                        throw logic_error("Missing input file name");
                    fileCheck(inputFile, argv[i + 1], argv[i] + 2);
                } else if(!strcmp(argv[i], "--output")){
                    if(int(i + 1) >= argc)
                        throw logic_error("Missing output file name");
                    fileCheck(outputFile, argv[i + 1], argv[i] + 2, true);
                } else if(!strcmp(argv[i], "--full-output")){
                    fullOutput = true;
                    i -= 1;
                }else{
                    throw logic_error("Undefined parameter: " + string(argv[i]));
                }
            }       

            search(graphFile, inputFile, outputFile, fullOutput);
        } catch(exception& ex){
            cerr << ex.what() << '\n';
        } 
        
        fclose(graphFile);
        fclose(inputFile);
        fclose(outputFile);
    } else{
        cerr << "Unknown operation name provided" << '\n';
        return -1;
    }

    return 0;
}