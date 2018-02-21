/* 
 * ReadDir.cc: implementation file for objects in DirRead namespace
 *
 * Created: 13 Dicember 2016
 * Author: Luigi Pertoldi - luigi.pertoldi@pd.infn.it
 *
 */

/*
from /usr/include/sys/dirent.h
struct dirent {
    ino_t d_ino;                            // file number of entry
    __uint16_t d_reclen;                    // length of this record
    __uint8_t  d_type;                      // file type, see below
    __uint8_t  d_namlen;                    // length of string in d_name
    char d_name[__DARWIN_MAXNAMLEN + 1];    // name must be no longer than this: __DARWIN_MAXNAMELEN = 255
};

// file types

#define DT_UNKNOWN       0
#define DT_FIFO          1
#define DT_CHR           2
#define DT_DIR           4
#define DT_BLK           6
#define DT_REG           8
#define DT_LNK          10  <-- symbolic link
#define DT_SOCK         12
#define DT_WHT          14
*/

#include "ReadDir.h"

#include <iostream>
#include <dirent.h>
#include <memory>

namespace ReadDir {

    std::vector<std::string> detail::vDir;
    std::vector<std::string> detail::vFiles;
    bool detail::firstInstance = true;
    bool detail::verbose = false;

    bool GetContent(std::string foldName, std::string regex) {
        if (std::count(regex.begin(), regex.end(), '*') > 1) {
            std::cout << "WARNING: regex with multiple '*' is unsupported! Setting regex = \"*\"\n";
            regex = "*";
        }
        if (regex.empty()) {
            std::cout << "WARNING: empty regex! Setting regex = \"*\"\n";
            regex = "*";
        }
        if (detail::verbose) std::cout << "Regex: " << regex << std::endl;
        auto starpos = regex.find('*');
        std::string prefix, postfix;
        if (starpos != std::string::npos) {
            prefix  = regex.substr(0, starpos);
            postfix = regex.substr(starpos+1, regex.size());
        }
        auto p = std::unique_ptr<DIR,std::function<int(DIR*)>>{opendir(foldName.c_str()), &closedir};
        dirent entry;
        bool foundDir = false;
        for (auto* r = &entry; readdir_r(p.get(), &entry, &r) == 0 && r;) {
            auto e = std::string(entry.d_name);
            if (e == "." or e == "..") continue;
            if (entry.d_type == 4) {
                detail::vDir.push_back(foldName + "/" + e);
                if (detail::verbose) std::cout << e << std::endl;
            }
            if (entry.d_type == 8) {
                if (prefix.empty() and !postfix.empty()){
                    if (!e.compare(e.size() - postfix.size(), postfix.size(), postfix)) {
                        detail::vFiles.push_back(foldName + "/" + e);
                        if (detail::verbose) std::cout << "  " << e << std::endl;
                        foundDir = true;
                    }
                }
                else if (!prefix.empty() and postfix.empty()){
                    auto e = std::string(entry.d_name);
                    if (!e.compare(0, prefix.size(), prefix)) {
                        detail::vFiles.push_back(foldName + "/" + e);
                        if (detail::verbose) std::cout << "  " << e << std::endl;
                        foundDir = true;
                    }
                }
                else if (!prefix.empty() and !postfix.empty()){
                    auto e = std::string(entry.d_name);
                    if (!e.compare(0, prefix.size(), prefix) and !e.compare(e.size() - postfix.size(), postfix.size(), postfix)) {
                        detail::vFiles.push_back(foldName + "/" + e);
                        if (detail::verbose) std::cout << "  " << e << std::endl;
                        foundDir = true;
                    }
                }
                else if (prefix.empty() and postfix.empty() and regex == "*"){
                        detail::vFiles.push_back(foldName + "/" + e);
                        if (detail::verbose) std::cout << "  " << e << std::endl;
                        foundDir = true;
                }
                else {
                    if (e == regex) {
                        detail::vFiles.push_back(foldName + "/" + e);
                        if (detail::verbose) std::cout << "  " << e << std::endl;
                        foundDir = true;
                    }
                }
            }
        }
        return foundDir;
    }

    void GetContent_R(std::string foldName, std::string regex) {

        if (!detail::firstInstance and (!detail::vDir.empty() or !detail::vFiles.empty())) {
            std::cout << "Warning: you called ReadDir::GetContent more than "
                      << "once, you may will have duplicate files..." << std::endl;
        }

        // fill vector with first directory
        detail::vDir.push_back(foldName);

        for (int i = 0; i < detail::vDir.size(); i++) {
            if (detail::verbose) std::cout << detail::vDir[i] << "/" << std::endl;
            GetContent(detail::vDir[i], regex);
            if (detail::verbose) std::cout << std::endl;
        }
        detail::firstInstance = false;
        return;
    }
}
