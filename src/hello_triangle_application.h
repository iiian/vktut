#ifndef HELLO_TRIANGLE_APPLICATION_H
#define HELLO_TRIANGLE_APPLICATION_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <optional>

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;

  bool isComplete()
  {
    return graphicsFamily.has_value();
  }
};

class HelloTriangleApplication
{
public:
  void run()
  {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  VkInstance instance;

  void pickPhysicalDevice();
  void initWindow();
  void initVulkan();
  void createInstance();
  bool checkValidationLayerSupport();
  void mainLoop();
  void cleanup();
  bool isDeviceSuitable(VkPhysicalDevice);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);
};

#endif // HELLO_TRIANGLE_APPLICATION_H