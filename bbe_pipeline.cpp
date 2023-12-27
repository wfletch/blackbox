#include "bbe_pipeline.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>
namespace bbe {
    
    std::vector<char> BbePipeline::readFile(const std::string& filepath) {
        
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
                throw std::runtime_error("Failed to open file: " + filepath);
        }
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char>buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }
    void BbePipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath) {
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout << "Size" << vertCode.size() << "\n";   
        std::cout << "Size" << fragCode.size() << "\n";   
    }
    BbePipeline::BbePipeline(const std::string& vertFilepath, const std::string& fragFilepath){
        createGraphicsPipeline(vertFilepath, fragFilepath);
    }
}