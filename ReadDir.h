/* 
 * ReadDir.h: header file for the ReadDir namespace
 *
 * Created: 13 Dicember 2016, Padova
 * Author: Luigi Pertoldi - luigi.pertoldi@pd.infn.it
 *
 * Namespace with functions that allow the user to search for files inside a directory
 *
 */

#include <vector>
#include <string>

#ifndef _READ_DIR_
#define _READ_DIR_

namespace ReadDir {

    namespace detail {
        extern std::vector<std::string> vDir;                           // vector to store found directories
        extern std::vector<std::string> vFiles;                         // vector to store found files
        extern bool firstInstance;                                      // to be sure to read only once
        extern bool verbose;                                            // verbose level (default false)
    }

    bool GetContent  (std::string foldName, std::string regex = "*");   // get the contents of 'foldName' directory and
                                                                        // store them into vDir and vFiles
    void GetContent_R(std::string foldName, std::string regex = "*");   // get the contents of 'foldName' directory
                                                                        // searching recursively and store them into
                                                                        // vDir and vFiles

    inline void Clear() { detail::vDir.clear(); detail::vFiles.clear(); }
    inline std::vector<std::string>& GetDirs () { return detail::vDir; }
    inline std::vector<std::string>& GetFiles() { return detail::vFiles; }
    inline void SetVerbose(bool stat) { detail::verbose = stat; }
}

#endif
