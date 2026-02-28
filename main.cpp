#include <iostream>
#include <filesystem>
#include <fstream>
#include <functional>

std::unordered_map<size_t, std::vector<std::string>> buildSizeMap(const std::string& directory){
    std::unordered_map<size_t, std::vector<std::string>> sizeMap;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)){
        if(entry.is_regular_file()){
            std::string filePath = entry.path().string();
            size_t fileSize = std::filesystem::file_size(filePath);
            sizeMap[fileSize].push_back(filePath);
        }
    };

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
        const std::string& hash = pair.first;
        const std::vector<std::string>& paths = pair.second;

        if (paths.size() > 1){
            std::cout << "Duplicate files found with hash: " << hash << std::endl;

            for (const auto& path : paths){
                std::cout << " - " << path << std::endl;
            }

        }
    }
}


int main(){

    std::cout << "This is a c++ learning project for finding duplicate files" << std::endl;
    std::string directory = "/Users/harry_chung/Downloads/"; 

    std::unordered_map<size_t, std::vector<std::string>> sizeMap = buildSizeMap(directory);
    std::unordered_map<std::string, std::vector<std::string>> fileMap = findDuplicates(sizeMap);
    printDuplicates(fileMap);

    return 0;
}