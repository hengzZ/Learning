#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>

#include<iostream>
#include<vector>
#include<string>

using namespace std;


std::vector<std::string> ListDir(const std::string dirName)
{
  std::vector<std::string> fileList;
  struct stat status;

  lstat(dirName.c_str(), &status);
  if (!S_ISDIR(status.st_mode))
  {
    std::cout << "The dir name is not valid!" << std::endl;
    return fileList;
  }

  struct dirent *filename;
  DIR *dir;
  dir = opendir(dirName.c_str());
  if (NULL == dir)
  {
    std::cout << "Open dir failed!" << std::endl;
    return fileList;
  }

  while (NULL != (filename = readdir(dir)))
  {
    fileList.push_back(filename->d_name);
  }
  return fileList;
}


int main(int argc, char** argv)
{
  std::string dirName;
  std::cout << "Please enter a dir name: " << std::endl;
  std::cin >> dirName;


  std::vector<std::string> fileList = ListDir(dirName);

  for (int i=0; i<(int)fileList.size(); i++)
  {
    std::cout << fileList[i] << std::endl;
  }


  return 0;
}
