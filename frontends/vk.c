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

#include "common.h"
#include "vk.h"
#include "../game.h"

static const int *board = NULL;
static int size = 0;

static int wWidth, wHeight;
static GLFWwindow* g_window;
static VkInstance instance;


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

VkPhysicalDevice
pickDevice() {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    if (deviceCount == 0)
        printf("Failed to find GPUs with Vulkan support");

    VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    /* Find dGPU */
    physicalDevice = findDGPU(devices, deviceCount);
    if (physicalDevice == VK_NULL_HANDLE)
        physicalDevice = devices[0];

    VkPhysicalDeviceProperties dp;
    vkGetPhysicalDeviceProperties(physicalDevice, &dp);
    printf("Using GPU %s\n", dp.deviceName);

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

    
    /* Create window */
    if ((g_window = glfwCreateWindow(wWidth, wHeight, TXT_TITLE, NULL, NULL))
        == NULL)
    {
        printf("Error creating window\n");
        return -1;
    }
    
    /* Create vulkan instance */
    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = TXT_TITLE;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = { 0 };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
    if (result != VK_SUCCESS) {
        printf("Failed to create VK instance\n");
        return 0;
    }

    VkPhysicalDevice physicalDevice = pickDevice();

    
}


void
vkDestroy() {
    vkDestroyInstance(instance, NULL);
    glfwDestroyWindow(g_window);
    glfwTerminate();
}

