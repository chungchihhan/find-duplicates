#include <iostream>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>

std::vector<std::string> getAllDirectories(const std::string& rootPath){
    std::vector<std::string> directories;

    directories.push_back(rootPath);
    try{
        for (const auto& entry : std::filesystem::recursive_directory_iterator(rootPath, std::filesystem::directory_options::skip_permission_denied)){
            if (entry.is_directory()){
                directories.push_back(entry.path().string());
            }
        }
    } catch (const std::filesystem::filesystem_error& e){
        if (e.path1().string() == rootPath) {
          std::cerr << "⚠️ Warning: Cannot access root directory: " << rootPath << std::endl;
        }
    }
    
    return directories;
}

std::unordered_map<size_t, std::vector<std::string>> buildSizeMap(const std::string& directory, bool includeHidden = false){
    std::unordered_map<size_t, std::vector<std::string>> sizeMap;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory, std::filesystem::directory_options::skip_permission_denied)){
            if(entry.is_regular_file()){
                std::string filePath = entry.path().string();
                std::string fileName = entry.path().filename().string();
                
                if (!includeHidden && !fileName.empty() && fileName[0] == '.'){
                    continue;
                }
                
                size_t fileSize = std::filesystem::file_size(filePath);
                sizeMap[fileSize].push_back(filePath);
            }
        }
    } catch (const std::filesystem::filesystem_error& e){
        std::cerr << "⚠️ Skipped (no permission): " << directory << std::endl;
    } 

    return sizeMap;
}

std::unordered_map<std::string, std::vector<std::string>> findDuplicates(const std::unordered_map<size_t, std::vector<std::string>>& sizeMap){
    std::unordered_map<std::string, std::vector<std::string>> fileMap;
        
    for(const auto& entry: sizeMap){
        const size_t fileSize = entry.first;
        const std::vector<std::string>& paths = entry.second;

        if (paths.size() == 1){
            continue;
        }

        for (const auto& filePath : paths){
            std::ifstream file(filePath, std::ios::binary);
            if (file.is_open()){
                std::string content(fileSize, '\0');
                file.read(&content[0], fileSize);
                std::hash<std::string> hasher;
                size_t hashResult = hasher(content);
                std::string hashKey = std::to_string(hashResult);

                fileMap[hashKey].push_back(filePath);
            } else {
                std::cerr << "Failed to open file: " << filePath << std::endl;
            };
        };
    };

    return fileMap;
}


void printDuplicates(const std::unordered_map<std::string, std::vector<std::string>>& fileMap){
    for (const auto& pair: fileMap){
        // const std::string& hash = pair.first;
        const std::vector<std::string>& paths = pair.second;

        if (paths.size() <= 1 ){
            continue;
        }

        for (const auto& path : paths){
            std::cout << " - " << path << std::endl;
        }
    }
}

void interactiveDelete(const std::unordered_map<std::string, std::vector<std::string>>& fileMap ){
    for (const auto& pair: fileMap){
        // const std::string& hash = pair.first;
        const std::vector<std::string>& paths = pair.second;

        if (paths.size() <= 1 ){
            continue;
        }

        int choice;
        int index = 1;
        std::cout << "Which one do you wanna keep?" << std::endl;
        for (const auto& path : paths){
            std::cout << " [" << index << "] " << path << std::endl;
            index++;
        }
        
        std::cin >> choice;
        if (!std::cin){
            std::cerr << "Invalid input" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            continue;
        }

        if (choice < 1 || choice > paths.size()){
            std::cerr << "Invalid choice. You must select a number between 1 and " << paths.size() << std::endl;
            continue;
        }

        int currentIndex = 1;
        for (const auto& path : paths){
            if (currentIndex == choice){
                std::cout << "Keeping:" << path << std::endl;
            } else{
                if(std::filesystem::remove(path)){
                    std::cout << "Deleted" << path << std::endl;
                }
                else {
                    std::cerr << "Failed to delete: " << path << std::endl;
                }
            }
            currentIndex++;
        }
    }
}


int main(int argc, char* argv[]){
    bool recursive = false;
    bool deleteFiles = false;
    bool includeHidden = false;
    std::string directory;

    for (int i = 1; i< argc; i++){
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help"){
            std::cout << "Usage: " << argv[0] << " [options] <directory_path>" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -h, --help        Show this help message and exit" << std::endl;
            std::cout << "  -r, --recursive   Search for duplicates in subdirectories" << std::endl;
            std::cout << "  -d, --delete      Interactively delete duplicates" << std::endl;
            std::cout << "  -a, --all         Include hidden files in the search" << std::endl;
            return 0;
        } else if (arg == "-r" || arg == "--recursive"){
            recursive = true;
        } else if (arg == "-d" || arg == "--delete") {
            deleteFiles = true;
        } else if (arg == "-a" || arg == "--all") {
            includeHidden = true;
        } else {
            directory = arg;
        }
    };

    if (directory.empty()){
        std::cerr << "Error: Directory path is required." << std::endl;
        std::cout << "Usage: " << argv[0] << " <directory_path>" << std::endl;
        return 1;
    };

    if (recursive){
        std::vector<std::string> alldirs = getAllDirectories(directory);

        for (const auto& dir : alldirs){
            std::unordered_map<size_t, std::vector<std::string>> sizeMap = buildSizeMap(dir, includeHidden);
            std::unordered_map<std::string, std::vector<std::string>> fileMap = findDuplicates(sizeMap);

            if (!fileMap.empty()){
                std::cout << "\n=== Processing directory: " << dir << " ===" << std::endl;
                if (deleteFiles){
                    interactiveDelete(fileMap);
                } else {
                    printDuplicates(fileMap);
                }
            }
            
        }
    } else {
        std::unordered_map<size_t, std::vector<std::string>> sizeMap = buildSizeMap(directory, includeHidden);
        std::unordered_map<std::string, std::vector<std::string>> fileMap = findDuplicates(sizeMap);

        if (!fileMap.empty()){
            std::cout << "\n=== Processing directory: " << directory << " ===" << std::endl;
            if (deleteFiles){
                interactiveDelete(fileMap);
            } else {
                printDuplicates(fileMap);
            }
        }

    }

    return 0;
}