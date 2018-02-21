/*
 * tagToName.cc
 *
 * Created: Thursday 3 November 2016, Padova
 * Author: Luigi Pertoldi - luigi.pertoldi@pd.infn.it
 *
 */ 

#include <iostream>
#include <string>
#include <vector>

#include "../ReadDir.h"

// define the role of the << operator with vectors and standard output
template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& c) {
    std::copy(std::begin(c), std::end(c), std::ostream_iterator<T>(os,"\n"));
    return os;
}

int main( int argc, char** argv ) {

    if (argc < 3) {
        std::cout << "USAGE: " << std::string(argv[0]) << " <dir> <regex>\n";
        return 1;
    }
    std::string initName = argv[1];

    // set verbose
    ReadDir::SetVerbose(true);

    // get directory contents recursively
    ReadDir::GetContent_R(initName, std::string(argv[2]));

    // get results
    std::vector<std::string> vDir  (ReadDir::GetDirs());
    std::vector<std::string> vFiles(ReadDir::GetFiles());

    std::cout << "########## DIRECTORIES ##########" << std::endl
              << vDir << std::endl
              << "############# FILES #############" << std::endl
              << vFiles;

    return 0;
}
