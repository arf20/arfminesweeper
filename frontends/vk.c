/*

    arfminesweeper: Cross-plataform multi-frontend game
    Copyright (C) 2023 arf20 (Ángel Ruiz Fernandez)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    vk.c: Vulkan/GLFW frontend

*/

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "vk.h"
#include "../game.h"

static const int *board = NULL;
static int size = 0;

static int wWidth, wHeight;
static GLFWwindow* window;
static VkInstance instance;
static VkDevice device;
static VkSurfaceKHR surface;



#define VALIDATION_LAYER "VK_LAYER_KHRONOS_validation"
char *validationLayer = NULL;

int
checkValidationLayerSupport() {
    /* get validation layers */
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);
    VkLayerProperties* availableLayers = malloc(sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    /* search layer */
    printf("Available Layers:\n");
    int found = 0;
    for (int i = 0; i < layerCount; i++) {
        printf("\t%s\n", availableLayers[i].layerName);
	    if (strcmp(availableLayers[i].layerName, VALIDATION_LAYER) == 0) {
            found = 1;
	    }
    }

    validationLayer = malloc(strlen(VALIDATION_LAYER) + 1);
    strcpy(validationLayer, VALIDATION_LAYER);

    return found;
}


VkPhysicalDevice
findDGPU(VkPhysicalDevice *devices, int n) {
    for (int i = 0; i < n; i++) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
        vkGetPhysicalDeviceFeatures(devices[i], &deviceFeatures);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
            && deviceFeatures.geometryShader)
        {
            return devices[i];
        }
    }
    return VK_NULL_HANDLE;
}

int
findGraphicQueueFamilies(VkPhysicalDevice device) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    VkQueueFamilyProperties* queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    int graphicQueueFamilyIdx = -1;
    for (int i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicQueueFamilyIdx = i;
            break;
        }
    }

    return graphicQueueFamilyIdx;
}

int
findPresentationQueueFamilies(VkPhysicalDevice device) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    VkQueueFamilyProperties* queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    VkBool32 presentSupport = 0;
    int presentationQueueFamilyIdx = -1;
    for (int i = 0; i < queueFamilyCount; i++) {
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);  
        if (presentSupport) {
            presentationQueueFamilyIdx = i;
            break;
        }
    }

    return presentationQueueFamilyIdx;
}

VkPhysicalDevice
pickDevice() {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    if (deviceCount == 0)
        printf("Failed to find GPUs with Vulkan support\n");

    VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    VkPhysicalDeviceProperties dp;
    printf("Available Physical Devices:\n");
    for (int i = 0; i < deviceCount; i++) {
        vkGetPhysicalDeviceProperties(devices[i], &dp);
        printf("\t%s\n", dp.deviceName);
    }

    /* Find dGPU */
    int discrete = 1;
    physicalDevice = findDGPU(devices, deviceCount);
    if (physicalDevice == VK_NULL_HANDLE) {
        physicalDevice = devices[0];
        discrete = 0;
    }

    vkGetPhysicalDeviceProperties(physicalDevice, &dp);
    printf("Using ");
    discrete ? printf("d") : printf("i");
    printf("GPU %s\n", dp.deviceName);

    return physicalDevice;
}


int
vkStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);

    /* Init glfw */
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    /* Create window */
    if ((window = glfwCreateWindow(wWidth, wHeight, TXT_TITLE, NULL, NULL))
        == NULL)
    {
        printf("Error creating window\n");
        return -1;
    }

    /* ==================== GLFW & WINDOW INITIALISED ==================== */
    
    /* Create vulkan instance */
    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = TXT_TITLE;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Fuck Engines";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceCreateInfo = { 0 };
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    instanceCreateInfo.enabledExtensionCount = glfwExtensionCount;
    instanceCreateInfo.ppEnabledExtensionNames = glfwExtensions;


    if (!checkValidationLayerSupport()) {
        printf("Validation layer " VALIDATION_LAYER " unavailable\n");
        return 0;
    }

    instanceCreateInfo.enabledLayerCount = 1;
    instanceCreateInfo.ppEnabledLayerNames = (const char* const*)&validationLayer;

    VkResult result = vkCreateInstance(&instanceCreateInfo, NULL, &instance);
    if (result != VK_SUCCESS) {
        printf("Failed to create VK instance\n");
        return 0;
    }

    /* Create surface */
    if (glfwCreateWindowSurface(instance, window, NULL, &surface) != VK_SUCCESS) {
        printf("Failed to create window surface\n");
    }

    /* Pick physical device */
    VkPhysicalDevice physicalDevice = pickDevice();
    if (physicalDevice == VK_NULL_HANDLE) {
        return 0;
    }

    /* Create Logical Device */
    VkPhysicalDeviceFeatures deviceFeatures = { 0 };

    VkDeviceCreateInfo deviceCreateInfo = { 0 };
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    deviceCreateInfo.enabledExtensionCount = 0;

    deviceCreateInfo.enabledLayerCount = 1;
    deviceCreateInfo.ppEnabledLayerNames = (const char* const*)&validationLayer;

    int graphicQueueFamilyIdx = findGraphicQueueFamilies(physicalDevice);
    if (graphicQueueFamilyIdx < 0) {
        printf("Graphic Queue Family not supported on this device\n");
        return 0;
    }

    int presentationQueueFamilyIdx = findPresentationQueueFamilies(physicalDevice);
    if (presentationQueueFamilyIdx < 0) {
        printf("Presentation Queue Family not supported on this device\n");
        return 0;
    }

    VkDeviceQueueCreateInfo* queueCreateInfos = malloc(sizeof(VkDeviceQueueCreateInfo) * 2);
    memset(queueCreateInfos, 0, sizeof(VkDeviceQueueCreateInfo) * 2);
    uint32_t uniqueQueueFamilies[] = { graphicQueueFamilyIdx, presentationQueueFamilyIdx };

    queueCreateInfos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[0].queueFamilyIndex = uniqueQueueFamilies[0];
    queueCreateInfos[0].queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfos[0].pQueuePriorities = &queuePriority;

    queueCreateInfos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[1].queueFamilyIndex = uniqueQueueFamilies[1];
    queueCreateInfos[1].queueCount = 1;
    queueCreateInfos[1].pQueuePriorities = &queuePriority;

    deviceCreateInfo.queueCreateInfoCount = 2;
    // HACK: if not unique lol
    if (uniqueQueueFamilies[0] == uniqueQueueFamilies[1])
        deviceCreateInfo.queueCreateInfoCount = 1;

    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos;


    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device) != VK_SUCCESS) {
        printf("Failed to create logical device\n");
        return 0;
    }

    /* Get Graphics Queue Handler */
    VkQueue graphicsQueue;
    vkGetDeviceQueue(device, graphicQueueFamilyIdx, 0, &graphicsQueue);
    VkQueue presentQueue;
    vkGetDeviceQueue(device, presentationQueueFamilyIdx, 0, &presentQueue);

    /* ==================== VULKAN INITIALISED ==================== */

    

    
    
}


void
vkDestroy() {
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyDevice(device, NULL);
    vkDestroyInstance(instance, NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
}

