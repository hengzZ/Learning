#include<iostream>
#include<vector>
#include<string>
#include<io.h>

using namespace std;


std::vector<std::string> ListDir(const std::string dirName)
{
  std::vector<std::string> fileList;

  long handle;
  struct _finddata_t fileinfo;
  handle = _findfirst(dirName.c_str(), &fileinfo);

  if (-1 == handle) {
    return fileList;
  }

  do {
    fileList.push_back(fileinfo.name);
  } while(!_findnext(handle, &fileinfo));

  _findclose(handle);
  return fileList;
}

int main(int argc, char** argv)
{
  std::string dirName;
  std::cout << "Please enter [wildmode]: (such as: C:\\Users\\*)" << std::endl;
  std::cin >> dirName;

  std::vector<std::string> fileList = ListDir(dirName);

  for (int i=0; i<fileList.size(); i++) {
    std::cout << fileList[i] << std::endl;
  }

  system("pause");
  return 0;
}