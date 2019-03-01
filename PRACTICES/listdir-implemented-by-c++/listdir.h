#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>


std::vector<std::string> ListDir(const std::string dirName)
{
  std::vector<std::string> fileList;

  struct stat status;
  lstat(dirName.c_str(), &status);
  if (!S_ISDIR(status.st_mode))
  {
    std::cerr << "The dir name [" << dirName << "] is not valid!" << std::endl;
    return fileList;
  }

  struct dirent *filename;
  DIR *dir;
  dir = opendir(dirName.c_str());
  if (NULL == dir)
  {
    std::cerr << "Open dir [" << dirName << "] failed!" << std::endl;
    return fileList;
  }

  while (NULL != (filename = readdir(dir)))
  {
    fileList.push_back(filename->d_name);
  }

  closedir(dir);

  std::sort(fileList.begin(), fileList.end());
  return fileList;
}


std::vector<std::string> ListJpgImages(const std::string dirName)
{
  std::vector<std::string> fileList;

  struct stat status;
  lstat(dirName.c_str(), &status);
  if (!S_ISDIR(status.st_mode))
  {
    std::cerr << "The dir name [" << dirName << "] is not valid!" << std::endl;
    return fileList;
  }

  struct dirent *filename;
  DIR *dir;
  dir = opendir(dirName.c_str());
  if (NULL == dir)
  {
    std::cerr << "Open dir [" << dirName << "] failed!" << std::endl;
    return fileList;
  }

  while (NULL != (filename = readdir(dir)))
  {
    std::string fileName(filename->d_name);
    std::string::size_type find_pos1, find_pos2;

    find_pos1 = fileName.find(".jpg");
    find_pos2 = fileName.find(".JPG");

    if (std::string::npos != find_pos1 || std::string::npos != find_pos2)
    {
      fileList.push_back(filename->d_name);
    }
  }

  closedir(dir);

  std::sort(fileList.begin(), fileList.end());
  return fileList;
}


std::string PathJoin(const std::string dirName, const std::string fileName)
{
  std::string::size_type fslash_pos, bslash_pos;

  fslash_pos = dirName.find("/");
  bslash_pos = dirName.find("\\");

  std::string fileSeparator = (std::string::npos == fslash_pos) ? "\\" : "/";

  std::string filePath;
  if (dirName[dirName.size()-1] == '\\' || dirName[dirName.size()-1] == '/')
  {
    filePath = dirName + fileName;
  }
  else
  {
    filePath = dirName + fileSeparator + fileName;
  }
  return filePath;
}


std::vector<std::string> ListJpgImagesWithPath(const std::string dirName)
{
  std::vector<std::string> fileList;

  struct stat status;
  lstat(dirName.c_str(), &status);
  if (!S_ISDIR(status.st_mode))
  {
    std::cerr << "The dir name [" << dirName << "] is not valid!" << std::endl;
    return fileList;
  }

  struct dirent *filename;
  DIR *dir;
  dir = opendir(dirName.c_str());
  if (NULL == dir)
  {
    std::cerr << "Open dir [" << dirName << "] failed!" << std::endl;
    return fileList;
  }

  while (NULL != (filename = readdir(dir)))
  {
    std::string fileName(filename->d_name);
    std::string::size_type find_pos1, find_pos2;

    find_pos1 = fileName.find(".jpg");
    find_pos2 = fileName.find(".JPG");

    if (std::string::npos != find_pos1 || std::string::npos != find_pos2)
    {
      std::string dirStr(dirName);
      std::string fileStr(filename->d_name);
      std::string filePath = PathJoin(dirStr, fileStr);
      fileList.push_back(filePath);
    }
  }

  closedir(dir);

  std::sort(fileList.begin(), fileList.end());
  return fileList;
}
