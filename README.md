# read-dir-cxx

Get contents of a directory tree.

```cpp
#include "../ReadDir.h"
ReadDir::GetContent_R("/home/pippofranco");
std::vector<std::string> vDir  (ReadDir::GetDirs());
std::vector<std::string> vFiles(ReadDir::GetFiles());
```
