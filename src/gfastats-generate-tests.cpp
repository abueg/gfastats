#include <gfastats-validate.h>
#include <map>
#include <cstdio>

int main(int argc, char **argv) {
    std::cout << "WARNING: only run this program if gfastats is in a working state" << std::endl;
    std::cout << "WARNING: previous validate files will be deleted" << std::endl;
    std::cout << "continue? (Y/N) ";
    std::string input;
    std::cin >> input;
    if(input != "Y" && input != "y") {
        std::cout << "validate generation cancelled" << std::endl;
        std::exit(0);
    }
    std::cout << "deleting old validate files..." << std::endl;

    for(auto &file : list_dir("validateFiles")) {
        if(getFileExt(file) != "tst") continue; // dont delete README
        file = "validateFiles/"+file;
        if(remove(file.c_str()) != 0) {
            std::cerr << "error deleting <" << file << ">" << std::endl;
            return -1;
        }
    }

    std::cout << "generating new validate files..." << std::endl;

    std::string exePath = getExePath(argv[0]);

    const std::map<std::set<std::string>, std::vector<std::string>> ext_args = {
        {{"fasta", "fasta.gz", "fastq", "fastq.gz"}, {"", "-b a", "-b c", "-b s", "--homopolymer-compress 5"}},
        {{"gfa", "gfa.gz", "gfa2"}, {"-o gfa2", "-o gfa", "-o fasta"}}
    //  {{set of test file extensions}, {list of command line args to run with}}
    };

    const std::map<std::set<std::string>, std::vector<std::string>> file_args = {
        {{"random1.fasta", "random1.fasta.gz", "random1.fastq", "random1.fastq.gz", "random1.gfa"}, {"-a testFiles/random1.agp --stats", "-a testFiles/random1.agp --stats -ofa"}},
        {{"random1.fasta"}, {"-k testFiles/random1.instructions.sak", "-ofa -k testFiles/random1.instructions.sak", "-ofa -k testFiles/random1.hc.sak", "-ofa -k testFiles/random1.hdc.sak"}},
        {{"random2.noseq.gfa"}, {""}},
        {{"random1.gfa2"}, {"-k testFiles/random1.gfa2.instructions.sak"}},
        {{"random1.fasta"}, {"Header2", "-r testFiles/random1.fastq"}},
        {{"random4.fasta"}, {""}}
    //  {{set of test file paths}, {list of command line args to run with}}
    };

    const std::set<std::string> exclude {"agp", "sak"};

    int i = 0;

    auto genTest = [&i, &exePath](const std::string &file, const std::string &args){
        std::string tstFile = "validateFiles/"+file+"."+std::to_string(i)+".tst";
        std::cout << "generating: " << tstFile << std::endl;
        std::ofstream ostream;
        ostream.open(tstFile);
        ostream << "testFiles/" << file << " " << args << "\nembedded" << std::endl;
        ostream.close();
#ifdef _WIN32
        std::string cmd = "\"\""+exePath+"\" "+args+" testFiles/"+file+" >> "+tstFile+"\"";
#else
        std::string cmd = "\""+exePath+"\" "+args+" testFiles/"+file+" >> "+tstFile;
#endif
        bool exit = system(cmd.c_str());
        if (exit == EXIT_SUCCESS) {
            ostream << cmd << std::endl;
            ostream << "Command executed.";
        }
        ++i;
    };

    for(const std::string &file : list_dir("testFiles")) {
        std::string ext = getFileExt(file);
        if(exclude.count(ext)) continue;
        for(auto pair : ext_args) {
            if(!pair.first.count(ext)) continue;
            for(auto args : pair.second) {
                genTest(file, args);
            }
        }
    }

    std::fstream fstream;
    for(const auto &pair : file_args) {
        for(const std::string &file : pair.first) {
            fstream.open("testFiles/"+file);
            if(!fstream) continue;
            fstream.close();
            for(const std::string &args : pair.second) {
                genTest(file, args);
            }
        }
    }

    std::exit(0);
}
