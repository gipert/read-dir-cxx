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

    bool GetContent( std::string foldName ) {

        auto p = std::unique_ptr<DIR,std::function<int(DIR*)>>{opendir(foldName.c_str()), &closedir};
        dirent entry;
        bool foundDir = false;
        for (auto* r = &entry; readdir_r(p.get(), &entry, &r) == 0 && r; ) {
            if ( entry.d_type == 4 ) {
                if ( std::string(entry.d_name) != "." and std::string(entry.d_name) != ".." ) {
                    detail::vDir.push_back(foldName + "/" + std::string(entry.d_name));
                    foundDir = true;
                }
            }
            if ( entry.d_type == 8 ) {
                detail::vFiles.push_back(foldName + "/" + std::string(entry.d_name));
                if (detail::verbose) std::cout << "  " << std::string(entry.d_name) << std::endl;
            }
        }
        return foundDir;
    }

    void GetContent_R( std::string foldName ) {

        if ( !detail::firstInstance ) {
            std::cout << "Warning: you called ReadDir::GetContent more than "
                      << "one time, you will have duplicate files..." << std::endl;
        }

        // fill vector with first directory
        detail::vDir.push_back(foldName);

        for ( int i = 0; i < detail::vDir.size(); i++ ) {
            if (detail::verbose) std::cout << detail::vDir[i] << "/" << std::endl;
            GetContent( detail::vDir[i] );
            if (detail::verbose) std::cout << std::endl;
        }
        detail::firstInstance = false;
        return;
    }
}
