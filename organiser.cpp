#include <iostream>
#include <filesystem>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;
namespace fs = std::filesystem;

string folderNames[] = {"images", "videos", "documents", "audios", "misc", "code"};
map<string, vector<string>> folderVsExtensions = {
  {"images", {"jpg", "jpeg", "png"}},
  {"videos", {"mp4"}},
  {"documents", {"pdf", "log"}},
  {"audios", {"mp3"}},
  {"code", {"py", "cpp", "java", "go", "rs", "class", "sh", "json"}}
};

bool list_files_and_folders(string dir) {
  fs::path dirpath = dir;
  if(fs::exists(dirpath) && fs::is_directory(dirpath)) {
    for (const auto& entry : fs::directory_iterator(dirpath))
      if(fs::is_directory(entry.path()))
        cout << "Folder : " << entry.path() << "\n";
      else 
        cout << "File : " << entry.path() << "\n";
    return true;
  }
  cout << "Directory not found\n";
  return false;
}  

void create_folders(string entry) {
  cout << "Creating folders...\n";
  for(string folderName : folderNames) {
    string possible_foldername = entry + "/" + folderName;
    cout << possible_foldername << endl;
    if(fs::exists(possible_foldername)) 
      continue;
    else
     fs::create_directory(possible_foldername);
  }
  cout << "Done with creating folders\n";
}

void move_file(string entry) {
  map<string, vector<string>>::iterator it;
  for(it = folderVsExtensions.begin(); it != folderVsExtensions.end(); ++it) {
    for(string filetype : it->second) {
      if(entry.size() < filetype.size())
        continue;

      if(entry.compare(entry.size() - filetype.size(), filetype.size(), filetype) == 0) {
        const char *src = entry.c_str();
        size_t pos = entry.rfind("/");
        string dest = entry.substr(0, pos) + "/" + it->first + entry.substr(pos, entry.size());
        cout << "Printing dest : " << dest << endl;
        const char *dst = dest.c_str();
        rename(src, dst);
      } 
    }
  }
}

void move_files_to_folders(string dir) {
  fs::path dirpath = dir;
  if(fs::exists(dirpath) && fs::is_directory(dirpath)) {
    for(const auto& entry : fs::directory_iterator(dirpath)) {
      if(fs::is_directory(entry.path())) 
        continue; 
      
      move_file(entry.path().string());
    }
  }
}

void move_to_misc(string entry) {
  const char *src = entry.c_str();
  size_t pos = entry.rfind(".");
  string dest = entry.substr(0, pos) + "/misc" + entry.substr(pos, entry.size());
  const char *dst = dest.c_str();
  rename(src, dst);
}

void move_files_to_misc(string dir) {
  fs::path dirpath = dir;
  if(fs::exists(dirpath) && fs::is_directory(dirpath)) {
    for(const auto& entry : fs::directory_iterator(dirpath)) {
      if(fs::is_directory(entry.path()))
        continue;
      move_to_misc(entry.path().string());
    }
  }
}

int main() {
  string dir;
  cout << "Enter the relative directory path from current directory : ";
  cin >> dir;
  cout << "\n";
  if(list_files_and_folders(dir)) {
    create_folders(dir);
    move_files_to_folders(dir);
    move_files_to_misc(dir);
  }
 return 0;
}
