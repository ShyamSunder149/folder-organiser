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
  {"documents", {"pdf"}},
  {"audios", {"mp3"}},
  {"code", {"py", "cpp", "java", "go", "rs", "class"}}
};

void list_files_and_folders(string dir) {
  fs::path dirpath = dir;
  if(fs::exists(dirpath) && fs::is_directory(dirpath)) {
    for (const auto& entry : fs::directory_iterator(dirpath))
      if(fs::is_directory(entry.path()))
        cout << "Folder : " << entry.path() << "\n";
      else 
        cout << "File : " << entry.path() << "\n";
  } else cout << "Directory not found\n";
}  

void create_folders() {
  cout << "Creating folders...\n";
  for(string folderName : folderNames) {
    if(fs::exists(folderName)) 
      continue;
    else 
      fs::create_directory(folderName);
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
        string dest = it->first + entry.substr(1);
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
  string dest = "misc" + entry.substr(1);
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
  list_files_and_folders(dir);
  create_folders();
  move_files_to_folders(dir);
  move_files_to_misc(dir);
  return 0;
}
