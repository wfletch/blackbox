#include <stdio.h>
#include <vulkan/vulkan.h>
#include <stdlib.h>

int main()
{
    printf("Hello, Vulkan\n");

    VkInstance instance;
    VkApplicationInfo appInfo = {0};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "BlackBox";
    appInfo.pEngineName = "Arrow";
    appInfo.engineVersion = VK_MAKE_VERSION(0,0,1);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint8_t extensionCount = 3;
    const char** extensions = malloc(sizeof(const char*) * 3);
    extensions[0] = "VK_KHR_surface";
    extensions[1] = "VK_EXT_metal_surface";
    extensions[2] = "VK_KHR_portability_enumeration";

    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;

    VkResult result = vkCreateInstance(&createInfo, 0, &instance);
    if (result != VK_SUCCESS) {
        printf("Failed to create instance: %d\n", result);
        return -1;
    }
    return 0;
}