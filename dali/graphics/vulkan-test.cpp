#include <X11/Xlib.h>
#include <unistd.h>
#include <cstdarg>
#include <iostream>
#include "vulkan/context.h"

using namespace std;
using namespace Dali::Graphics::Vulkan;

#define MAX_VKLOG_SIZE (1024 * 1024) // 1mb of log memory per thread
__thread char gLogBuffer[MAX_VKLOG_SIZE];

// something to replace in the future, maybe
void VkLog(const char* format, ...)
{
  std::va_list args;
  va_start(args, format);
  vsnprintf(gLogBuffer, MAX_VKLOG_SIZE - 4, format, args);
  puts(gLogBuffer);
  va_end(args);
}

enum class QueueType : uint8_t
{
  GRAPHICS,
  TRANSFER,
  COMPUTE,
  SPARSE_BINDING,
  PRESENT
};

// cast for enum classes
template< typename T >
inline int Eval(const T& type)
{
  return static_cast< int >(type);
}

// debug callbacks
VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(VkDebugReportFlagsEXT      flags,
                                                   VkDebugReportObjectTypeEXT objectType,
                                                   uint64_t                   object,
                                                   size_t                     location,
                                                   int32_t                    messageCode,
                                                   const char*                pLayerPrefix,
                                                   const char*                pMessage,
                                                   void*                      pUserData)
{
  VkLog("[VALIDATION:%s]: %s", pLayerPrefix, pMessage);
  return VK_FALSE;
}

// test app
class App
{
public:
  App()  = default;
  ~App() = default;

  enum class SurfaceType : uint8_t
  {
    XLIB, // linux
    XCB,  // linux
    WAYLAND,
    ANDROID,
    WIN32
  };

  void InitWindow(int width, int height)
  {
    BuildInstanceExtensionTable();

    // 1. Create Window ( done by DALI
    wnd.width         = width;
    wnd.height        = height;
    wnd.display       = XOpenDisplay(nullptr);
    wnd.defaultScreen = DefaultScreen(wnd.display);
    wnd.window = XCreateSimpleWindow(wnd.display, RootWindow(wnd.display, wnd.defaultScreen), 0, 0,
                                     wnd.width, wnd.height, 1, BlackPixel(wnd.display, wnd.defaultScreen),
                                     WhitePixel(wnd.display, wnd.defaultScreen));

    XSelectInput(wnd.display, wnd.window, ExposureMask | KeyPressMask);
    XMapWindow(wnd.display, wnd.window);
  }

  void EnableValidationLayers()
  {
    if(!mDebugEnabled)
    {
      return;
    }

    auto fn = reinterpret_cast< PFN_vkCreateDebugReportCallbackEXT >(
        mInstance.getProcAddr("vkCreateDebugReportCallbackEXT"));
    if(!fn)
    {
      cout << "[VALIDATION] Cannot find vkCreateDebugReportCallbackEXT()!" << endl;
      return;
    }

    vk::DebugReportCallbackCreateInfoEXT info;
    info.setPfnCallback(DebugReportCallback).setPUserData(this);
    info.setFlags(vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eWarning |
                  vk::DebugReportFlagBitsEXT::eInformation | vk::DebugReportFlagBitsEXT::eError |
                  vk::DebugReportFlagBitsEXT::ePerformanceWarning);
    if(fn(mInstance,
          reinterpret_cast< VkDebugReportCallbackCreateInfoEXT* >(&info),
          reinterpret_cast< VkAllocationCallbacks* >(mAllocatorCallbacks),
          &mDebugReportCallback) != VK_SUCCESS)
    {
      cout << "[VALIDATION] Cannot enable validation! mDebugEnabled = false" << endl;
      mDebugEnabled = false;
    }
    else
    {
      cout << "[VALIDATION] mDebugEnabled = true" << endl;
    }
  }

  void BuildInstanceExtensionTable()
  {
    mInstanceExtensionProperties = vk::enumerateInstanceExtensionProperties().value;

    // extensions are sorted in order of being prioritized
    std::array< const char*, 2 > VK_KHR_SURFACE_EXTENSION_ARRAY = {
      VK_KHR_XLIB_SURFACE_EXTENSION_NAME, VK_KHR_XCB_SURFACE_EXTENSION_NAME,
    };

    // surfaces
    if(IsInstanceExtensionAvailable(VK_KHR_SURFACE_EXTENSION_NAME))
    {
      mInstanceExtensionTable.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
      int index = 0;
      for(auto& name : VK_KHR_SURFACE_EXTENSION_ARRAY)
      {
        if(IsInstanceExtensionAvailable(name))
        {
          mInstanceExtensionTable.emplace_back(name);
          mSurfaceType = static_cast< SurfaceType >(index);
          break;
        }
        ++index;
      }
    }

    // check if there's debug extension
    if(IsInstanceExtensionAvailable(VK_EXT_DEBUG_REPORT_EXTENSION_NAME))
    {
      mInstanceExtensionTable.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
      mDebugEnabled = true;
    }

    // print table
    for(auto& extname : mInstanceExtensionTable)
    {
      cout << "Ext: " << extname << endl;
    }
  }

  vk::Result CreateInstance()
  {
    std::vector< const char* > VALIDATION_LAYERS = {
      //"VK_LAYER_LUNARG_screenshot",
      "VK_LAYER_LUNARG_parameter_validation",
      //"VK_LAYER_LUNARG_vktrace",
      "VK_LAYER_LUNARG_monitor", "VK_LAYER_LUNARG_swapchain", "VK_LAYER_GOOGLE_threading",
      "VK_LAYER_LUNARG_api_dump", "VK_LAYER_LUNARG_object_tracker",
      "VK_LAYER_LUNARG_core_validation", "VK_LAYER_GOOGLE_unique_objects",
      "VK_LAYER_LUNARG_standard_validation",
    };

    const auto& layers = vk::enumerateInstanceLayerProperties().value;
    for(const auto& layer : layers)
    {
      cout << "[LAYER] " << layer.layerName << endl;
      mInstanceLayerTable.emplace_back(layer.layerName);
    }

    vk::InstanceCreateInfo info;
    info.setEnabledExtensionCount(mInstanceExtensionTable.size())
        .setPpEnabledExtensionNames(mInstanceExtensionTable.data())
        .setPApplicationInfo(nullptr)
        //        .setPpEnabledLayerNames(mInstanceLayerTable.empty() ? nullptr : mInstanceLayerTable.data())
        //      .setEnabledLayerCount(mInstanceLayerTable.size());
        .setPpEnabledLayerNames(VALIDATION_LAYERS.data())
        .setEnabledLayerCount(VALIDATION_LAYERS.size());
    auto result = vk::createInstance(info, mAllocatorCallbacks);
    if(result.result == vk::Result::eSuccess)
    {
      mInstance = result.value;
    }
    return result.result;
  }

  vk::Result CreateSurface()
  {
#ifdef __linux__
    switch(mSurfaceType)
    {
    case SurfaceType::XLIB:
    {
      vk::XlibSurfaceCreateInfoKHR info;
      info.setDpy(wnd.display).setWindow(wnd.window);
      auto result = mInstance.createXlibSurfaceKHR(info);
      mSurface    = result.value;
      return result.result;
    }
    case SurfaceType::XCB:
    {
      //@todo implement XCB surface
      return vk::Result::eIncomplete;
    }
    default:
      return vk::Result::eErrorInitializationFailed;
    }
#endif
  }

  vk::Result GetGPU()
  {
    auto result = mInstance.enumeratePhysicalDevices();
    if(result.result != vk::Result::eSuccess)
    {
      // error, possibly no devices
      return result.result;
    }

    auto GPUs            = result.value;
    auto currentGPU      = GPUs.front();
    auto currentGPUProps = currentGPU.getProperties();

    // select discreet over integrated
    for(auto& gpu : GPUs)
    {
      auto gpuProperties = gpu.getProperties();
      if((currentGPUProps.deviceType != vk::PhysicalDeviceType::eDiscreteGpu &&
          gpuProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) ||
         (currentGPUProps.deviceType != vk::PhysicalDeviceType::eDiscreteGpu &&
          gpuProperties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu))
      {
        currentGPU      = gpu;
        currentGPUProps = gpuProperties;
      }
    }

    // make sure selected GPU is THE REAL GPU not a blanket or refrigerator
    if(currentGPUProps.deviceType != vk::PhysicalDeviceType::eDiscreteGpu &&
       currentGPUProps.deviceType != vk::PhysicalDeviceType::eIntegratedGpu)
    {
      // error
      return vk::Result::eErrorFeatureNotPresent;
    }

    // store GPU data
    mGPU                 = currentGPU;
    mGPUProperties       = currentGPUProps;
    mGPUMemoryProperties = currentGPU.getMemoryProperties();
    mGPUFeatures         = currentGPU.getFeatures();

    cout << "[GPU] Name     : " << mGPUProperties.deviceName << endl;
    cout << "[GPU] DeviceID : " << mGPUProperties.deviceID << endl;
    cout << "[GPU] VendorID : " << mGPUProperties.vendorID << endl;

    mDeviceExtensionProperties = mGPU.enumerateDeviceExtensionProperties().value;
    mSurfacePresentModes       = mGPU.getSurfacePresentModesKHR(mSurface).value;
    mQueueFamilyProperties     = mGPU.getQueueFamilyProperties();

    // as debug list all the extensions
    for(auto& ext : mDeviceExtensionProperties)
    {
      cout << "[DEVICE] Ext: " << ext.extensionName << endl;
    }

    return result.result;
  }

  bool QueryDeviceExtensionAvailable(std::string name)
  {
    if(mDeviceExtensionProperties.empty())
    {
      return false;
    }

    for(auto& ext : mDeviceExtensionProperties)
    {
      if(name == ext.extensionName)
      {
        return true;
      }
    }
    return false;
  }

  bool IsInstanceExtensionAvailable(const std::string& name)
  {
    if(mInstanceExtensionProperties.empty())
    {
      return false;
    }

    for(auto& ext : mInstanceExtensionProperties)
    {
      if(name == ext.extensionName)
      {
        return true;
      }
    }

    return false;
  }

  void PrepareQueueFamilies()
  {
    int& familyGraphics{ mQueueFamilyIndex[Eval(QueueType::GRAPHICS)] };
    int& familyTransfer{ mQueueFamilyIndex[Eval(QueueType::TRANSFER)] };
    int& familyCompute{ mQueueFamilyIndex[Eval(QueueType::COMPUTE)] };
    int& familyPresent{ mQueueFamilyIndex[Eval(QueueType::PRESENT)] };

    int queueIndex = 0;
    for(auto& queueFamily : mQueueFamilyProperties)
    {
      if(familyGraphics < 0 && (queueFamily.queueFlags | vk::QueueFlagBits::eGraphics))
      {
        familyGraphics = queueIndex;
      }
      if(familyTransfer < 0 && (queueFamily.queueFlags | vk::QueueFlagBits::eTransfer))
      {
        familyTransfer = queueIndex;
      }
      if(familyCompute < 0 && (queueFamily.queueFlags | vk::QueueFlagBits::eCompute))
      {
        familyCompute = queueIndex;
      }

      if(familyPresent < 0)
      {
        vk::Bool32 supported{ false };
        mGPU.getSurfaceSupportKHR(queueIndex, mSurface, &supported);
        if(supported)
        {
          familyPresent = queueIndex;
        }
      }
      ++queueIndex;
    }

    // test for single-queue-mode
    mQueueSingleMode = (familyGraphics == familyCompute == familyPresent == familyTransfer);

    cout << "[QUEUE] Single queue mode: " << mQueueSingleMode << endl;
    cout << "[QUEUE] Family GRAPHICS  : " << familyGraphics << endl;
    cout << "[QUEUE] Family TRANSFER  : " << familyTransfer << endl;
    cout << "[QUEUE] Family COMPUTE   : " << familyCompute << endl;
    cout << "[QUEUE] Family PRESENT   : " << familyPresent << endl;
  }

  vk::Result CreateDevice()
  {
    // At the moment we are interested mostly with obtaining the swapchain
    // extension from the device
    if(QueryDeviceExtensionAvailable(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
    {
      mDeviceExtensionEnabledTable.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }
    else
    {
      // no swapchain, no-can-do!!!
      return vk::Result::eErrorExtensionNotPresent;
    }

    vk::DeviceQueueCreateInfo queueCreateInfo;

    // for now allocate max from each family
    auto queueInfos = std::vector< vk::DeviceQueueCreateInfo >{};
    queueInfos.resize(mQueueFamilyProperties.size());
    int                  index = 0;
    std::vector< float > priority;
    for(int index = 0; index < queueInfos.size(); ++index)
    {
      int count = mQueueFamilyProperties[index].queueCount;

      priority.resize(count);
      std::fill(priority.begin(), priority.end(), 1.0f);
      queueInfos[index].setPQueuePriorities(priority.data()).setQueueCount(count).setQueueFamilyIndex(index);
    }

    vk::DeviceCreateInfo deviceInfo;
    deviceInfo.setPpEnabledExtensionNames(mDeviceExtensionEnabledTable.data())
        .setEnabledExtensionCount(mDeviceExtensionEnabledTable.size())
        .setPEnabledFeatures(&mGPUFeatures) // set all the features for now
        .setPQueueCreateInfos(queueInfos.data())
        .setQueueCreateInfoCount(queueInfos.size());

    auto result = mGPU.createDevice(deviceInfo, mAllocatorCallbacks);
    if(result.result == vk::Result::eSuccess)
    {
      cout << "[DEVICE] Device created successfully!\n";
    }

    return result.result;
  }

  vk::Result GetQueues()
  {
  }

  void RunLoop()
  {
    XEvent e;
    while(true)
    {
      XNextEvent(wnd.display, &e);
      if(e.type == Expose)
      {
      }
      if(e.type == KeyPress)
      {
        // exit
        break;
      }
      usleep(16000);
    }

    XCloseDisplay(wnd.display);
  }

  struct WindowX11
  {
    int      defaultScreen;
    Display* display;
    Window   window;
    int      width, height;
  } wnd{};

  SurfaceType mSurfaceType;
  bool        mDebugEnabled{ false };

  std::vector< vk::ExtensionProperties > mInstanceExtensionProperties;
  std::vector< const char* >             mInstanceExtensionTable;
  std::vector< const char* >             mInstanceLayerTable;

  std::vector< vk::ExtensionProperties > mDeviceExtensionProperties;
  std::vector< const char* >             mDeviceExtensionEnabledTable;

  // Validation layers
  VkDebugReportCallbackEXT mDebugReportCallback{ nullptr };

  // vulkan objects
  vk::AllocationCallbacks* mAllocatorCallbacks{ nullptr }; // to be replaced by custom allocation in future
  vk::Instance             mInstance{};
  vk::SurfaceKHR           mSurface{};
  std::vector< vk::PresentModeKHR > mSurfacePresentModes;
  vk::PresentModeKHR mSurfacePresentMode{ vk::PresentModeKHR::eFifo }; // fallback by default

  // GPU
  vk::PhysicalDevice                 mGPU{ nullptr };
  vk::PhysicalDeviceProperties       mGPUProperties{};
  vk::PhysicalDeviceMemoryProperties mGPUMemoryProperties{};
  vk::PhysicalDeviceFeatures         mGPUFeatures{};

  // Queue
  std::vector< vk::QueueFamilyProperties > mQueueFamilyProperties;

  // we are going to use single queue per family, however
  // all queues will be stored as vectors in case of change of hearts and brains
  std::vector< vk::Queue > mQueueGraphics;
  std::vector< vk::Queue > mQueueTransfer;
  vk::Queue                mQueuePresent;
  vk::Queue mQueueSparseBinding; // not in use for now, but don't want to forget it exists

  // queue family indices stored in order:
  // GRAPHICS, TRANSFER, COMPUTE, SPARSE, PRESENT
  std::array< int, 5 > mQueueFamilyIndex{ { -1, -1, -1, -1, -1 } };
  bool mQueueSingleMode{ false };
};

int main(int argc, char** argv)
{
  cout << "Vulkan TEST" << endl;
  App app;
  app.InitWindow(720, 360);
  app.CreateInstance();
  app.EnableValidationLayers();
  app.CreateSurface();
  app.GetGPU();
  app.PrepareQueueFamilies();
  app.CreateDevice();
  app.RunLoop();
  /*
  Context context;
  if( !context.IsInstanceExtensionAvailable( VK_KHR_SURFACE_EXTENSION_NAME ) )
  {
    cout << "No surface extension, die now\n";
    return 0;
  }
  else
  {
    cout << "Found surface\n";
  }
  */
  return 0;
}
