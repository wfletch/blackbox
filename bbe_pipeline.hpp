#pragma once

#include "bbe_device.hpp"
#include <string>
#include <vector>
namespace bbe 
{
    struct PipelineConfigInfo {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
        // VkViewport viewport;
        // VkRect2D scissor;
        

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicsStateInfo;

        // What are these?
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;


        
    };
   class BbePipeline {
    public:
    BbePipeline(
        BbeDevice& device, 
        const std::string& vertFilepath,
        const std::string& fragFilepath, 
        const PipelineConfigInfo& configInfo);
    ~BbePipeline();

    BbePipeline(const BbePipeline&) = delete;
    BbePipeline operator=(const BbePipeline&) = delete;

    void bind(VkCommandBuffer VkCommandBuffer);
    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
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