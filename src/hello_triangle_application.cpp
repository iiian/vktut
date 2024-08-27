#include "hello_triangle_application.h"

#include <vector>
#include <stdexcept>
#include <iostream>
#include <fmt/format.h>
#include <map>

#define WIDTH 800
#define HEIGHT 600

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

void HelloTriangleApplication::initWindow()
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan()
{
  createInstance();
  // TODO: enable setupDebugMessenger();
  pickPhysicalDevice();
}

/* For now, a lazy implementation. If we find it, it's probably suitable. */
bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device)
{
  return findQueueFamilies(device).isComplete();
}

void HelloTriangleApplication::pickPhysicalDevice()
{
  VkPhysicalDevice phys_device = VK_NULL_HANDLE;
  uint32_t dev_count = 0;
  vkEnumeratePhysicalDevices(instance, &dev_count, nullptr);

  if (dev_count == 0)
  {
    throw std::runtime_error("no GPU detected");
  }

  std::vector<VkPhysicalDevice> devices(dev_count);
  vkEnumeratePhysicalDevices(instance, &dev_count, devices.data());

  for (const auto &device : devices)
  {
    if (isDeviceSuitable(device))
    {
      phys_device = device;
      break;
    }
  }

  if (phys_device == VK_NULL_HANDLE)
  {
    throw std::runtime_error("No registered GPU is Vulkan-compatible");
  }
  else
  {
    fmt::println("DEBUG: Found a suitable physical device");
  }
}

void HelloTriangleApplication::createInstance()
{
  if (enableValidationLayers && !checkValidationLayerSupport())
  {
    throw std::runtime_error("one or more validation layers that were requested are not available");
  }

  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Hello Triangle";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "No Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  uint32_t ext_count = 0;
  const char **exts = glfwGetRequiredInstanceExtensions(&ext_count);

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  create_info.enabledExtensionCount = ext_count;
  create_info.ppEnabledExtensionNames = exts;
  create_info.enabledLayerCount = 0;

  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

  fmt::println("DEBUG: available extensions:");

  for (const auto &extension : extensions)
  {
    fmt::println("\t\t{}", extension.extensionName);
  }

  if (enableValidationLayers)
  {
    create_info.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    create_info.ppEnabledLayerNames = validationLayers.data();

    fmt::println("DEBUG: Enabled {} validation layer(s)", create_info.enabledLayerCount);
  }
  else
  {
    create_info.enabledLayerCount = 0;
  }

  if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create vulkan instance!");
  }
}

QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(VkPhysicalDevice device)
{
  QueueFamilyIndices qfi;

  uint32_t family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, nullptr);
  std::vector<VkQueueFamilyProperties> qfams(family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &family_count, qfams.data());

  int i = 0;
  for (const auto &qf : qfams)
  {
    if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      qfi.graphicsFamily = i;
    }

    if (qfi.isComplete())
    {
      break;
    }

    i++;
  }

  return qfi;
}

bool HelloTriangleApplication::checkValidationLayerSupport()
{
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers)
  {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers)
    {
      if (strcmp(layerName, layerProperties.layerName) == 0)
      {
        layerFound = true;
        break;
      }
    }

    if (!layerFound)
    {
      return false;
    }
  }

  return true;
}

void HelloTriangleApplication::mainLoop()
{
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
  }
}

void HelloTriangleApplication::cleanup()
{
  vkDestroyInstance(instance, nullptr);

  glfwDestroyWindow(window);

  glfwTerminate();
}