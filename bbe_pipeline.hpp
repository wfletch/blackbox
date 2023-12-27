#pragma once

#include "bbe_device.hpp"
#include <string>
#include <vector>
namespace bbe 
{
    struct PipelineConfigInfo {};
   class BbePipeline {
    public:
    BbePipeline(
        BbeDevice& device, 
        const std::string& vertFilepath,
        const std::string& fragFilepath, 
        const PipelineConfigInfo& configInfo);
    ~BbePipeline() {}
    BbePipeline(const BbePipeline&) = delete;
    void operator=(const BbePipeline&) = delete;

    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
    private:

    // This is potentially unsafe. But a device is needed if we have a pipeline.
    BbeDevice& bbeDevice;
    static std::vector<char> readFile(const std::string& filepath);

    void createGraphicsPipeline(
        const std::string& vertFilepath, 
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo);
    void createShaderModule(const std::vector<char>& code, VkShaderModule* VkShaderModule);
//    This needs to exist above the pipeline.
   VkPipeline graphicsPipeline;
   VkShaderModule vertShaderModule;
   VkShaderModule fragShaderModule;

   };
} // namespace bbe 