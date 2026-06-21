#include"BufferedReader.h"
#include"Tokenizer.h"
#include"VersionedIndex.h"
#include"QueryProcessor.h"
#include"Utils.h"
#include<stdexcept>
#include<chrono>
#include<iostream>

struct Config {
    std::string file;
    std::string file1;
    std::string file2;

    std::string version;
    std::string version1;
    std::string version2;

    size_t bufferKB = 0;

    std::string querytype;
    std::string word;

    size_t topk = 0;
};

Config parseArguments(int argc, char* argv[]) {
    Config config;
    for(int i=1;i<argc;i++) {
        std::string arg = argv[i];
        if(arg=="--file") {
            if(i+1>=argc) {
                throw std::runtime_error("Missing value for --file");
            }
            config.file = argv[++i];
        }
        else if(arg=="--file1") {
            if(i+1>=argc) {
                throw std::runtime_error("Missing value for --file1");
            }
            config.file1 = argv[++i];
        }
        else if(arg=="--file2") {
            if(i+1>=argc) {
                throw std::runtime_error("Missing value for --file2");
            }
            config.file2 = argv[++i];
        }
        else if(arg=="--version") {
            if(i+1>=argc) {
                throw std::runtime_error("Missing value for --version");
            }
            config.version = argv[++i];
        }
        else if(arg=="--version1") {
            if(i+1>=argc) {
                throw std::runtime_error("Missing value for --version1");
            }
            config.version1 = argv[++i];
        }
        else if(arg=="--version2") {
            if(i+1>=argc) {
                throw std::runtime_error("Missing value for --version2");
            }
            config.version2 = argv[++i];
        }
        else if(arg=="--buffer") {
            if(i+1>=argc) {
                throw std::runtime_error("Missing value for --buffer");
            }
            config.bufferKB = std::stoul(argv[++i]);
        }
        else if(arg=="--query") {
            if(i+1>=argc) {
                throw std::runtime_error("Missing value for --query");
            }
            config.querytype = argv[++i];
        }
        else if(arg=="--word") {
            if(i+1>=argc) {
                throw std::runtime_error("Missing value for --word");
            }
            config.word = argv[++i];
        }
        else if(arg=="--top") {
            if(i+1>=argc) {
                throw std::runtime_error("Missing value for --top");
            }
            config.topk = std::stoul(argv[++i]);
        }
        else {
            throw std::runtime_error("Unknown argument: " + arg);
        }
    }
    return config;
}

void validateConfig(const Config& config) {
    if(config.bufferKB<256 || config.bufferKB>1024) {
        throw std::runtime_error("Buffer size invalid");
    }
    if(config.querytype!="word" && config.querytype!="diff" && config.querytype!="top") {
        throw std::runtime_error("Invalid query type");
    }
    bool isdiffmode = (config.querytype=="diff");
    if(isdiffmode) {
        if(config.file1.empty() || config.file2.empty()) {
            throw std::runtime_error("Diff query requires --file1 and --file2");
        }
        if(config.version1.empty() || config.version2.empty()) {
            throw std::runtime_error("Diff query requires --version1 and --version2");
        }
        if(config.word.empty()) {
            throw std::runtime_error("Diff query requires --word");
        }
    }
    else {
        if(config.file.empty()) throw std::runtime_error("Single query requires --file");
        if(config.version.empty()) throw std::runtime_error("Single query requires --version");
        if(config.querytype=="word" && config.word.empty()) throw std::runtime_error("Word query requires --word");
        if(config.querytype=="top" && config.topk==0) throw std::runtime_error("Top query requires --top with positive value");
    }
}

VersionedIndex buildIndex(IReader* reader)
{
    Tokenizer tokenizer;
    VersionedIndex index;

    std::string chunk;
    std::string token;

    while (reader->readChunk(chunk)) {
        tokenizer.processChunk(chunk);
    }

    tokenizer.flush();

    while (tokenizer.getNextToken(token)) {
        index.addToken(token);
    }
    return index;
}

int main(int argc, char* argv[]) {
    try {
        Config config = parseArguments(argc, argv);
        validateConfig(config);

        QueryProcessor qp;

        auto start = std::chrono::high_resolution_clock::now();

        if (config.querytype == "diff") {
            IReader* reader1 = new BufferedReader(config.file1, config.bufferKB);
            IReader* reader2 = new BufferedReader(config.file2, config.bufferKB);

            VersionedIndex index1 = buildIndex(reader1);
            VersionedIndex index2 = buildIndex(reader2);

            delete reader1;
            delete reader2;

            int64_t diff = qp.diffQuery(index1, index2, config.word);
            std::cout << "Difference (v2-v1): " << diff << "\n";
        }
        else {
            IReader* reader = new BufferedReader(config.file, config.bufferKB);

            VersionedIndex index = buildIndex(reader);

            delete reader;

            if (config.querytype == "word") {
                uint64_t freq = qp.wordQuery(index, config.word);
                std::cout << "Version: " << config.version << "\n";
                std::cout << "Count: " << freq << "\n";
            }
            else if (config.querytype == "top") {
                auto top = qp.topKQuery(index, config.topk);
                std::cout << "Top " << config.topk << " words in version " << config.version << ":\n";
                printpairs(top);
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        double seconds = std::chrono::duration<double>(end - start).count();

        std::cout << "Buffer size (KB): " << config.bufferKB << "\n";
        std::cout << "Execution time (s): " << seconds << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}