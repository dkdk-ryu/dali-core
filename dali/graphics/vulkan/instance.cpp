//
// Created by adam.b on 23/03/17.
//

#include "instance.h"

#include <vulkan/vulkan.hpp>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

using ExtensionPropertiesTable = std::vector< vk::ExtensionProperties >;
using LayerPropertiesTable     = std::vector< vk::LayerProperties >;
using QueueFamilyPropTable     = std::vector< vk::QueueFamilyProperties >;
using CStringTable             = std::vector< const char* >;
struct InstanceImpl
{
  // enabled extensions
  static ExtensionPropertiesTable instanceExtensionTable;
  static LayerPropertiesTable     instanceLayerTable;

  vk::Instance             instance{};
  vk::AllocationCallbacks* allocatorCallbacks{ nullptr };

  // GPU
  vk::PhysicalDevice                 gpu{ nullptr };
  vk::PhysicalDeviceProperties       gpuProperties{};
  vk::PhysicalDeviceMemoryProperties gpuMemoryProperties{};
  vk::PhysicalDeviceFeatures         gpuFeatures{};

  // Device
  ExtensionPropertiesTable deviceExtensionProperties;

  QueueFamilyPropTable queueFamilyProperties;

  // Surface
  vk::SurfaceKHR surface;
  unsigned int   surfaceWidth, surfaceHeight;
};

ExtensionPropertiesTable InstanceImpl::instanceExtensionTable{};
LayerPropertiesTable     InstanceImpl::instanceLayerTable{};

Instance::Instance() : mImpl{ new InstanceImpl{} }
{
}

Instance::~Instance() = default;

InstancePtr Instance::New(const ExtensionNameList& extensions, ValidationFlags validationFlags)
{
  InstancePtr instance(new Instance{});
  if(instance->CreateInstance(extensions, validationFlags))
  {
    return instance;
  }
  return nullptr;
}

bool Instance::IsExtensionAvailable(const std::string& name)
{
  if(InstanceImpl::instanceExtensionTable.empty())
  {
    auto result = vk::enumerateInstanceExtensionProperties();
    if(result.result != vk::Result::eSuccess)
    {
      return false;
    }
    InstanceImpl::instanceExtensionTable = result.value;
  }

  for(auto& ext : InstanceImpl::instanceExtensionTable)
  {
    if(name == ext.extensionName)
    {
      return true;
    }
  }

  return false;
}

bool Instance::IsLayerAvailable(const std::string& name)
{
  if(InstanceImpl::instanceLayerTable.empty())
  {
    auto result = vk::enumerateInstanceLayerProperties();
    if(result.result != vk::Result::eSuccess)
    {
      return false;
    }
    InstanceImpl::instanceLayerTable = result.value;
  }

  for(auto& ext : InstanceImpl::instanceLayerTable)
  {
    if(name == ext.layerName)
    {
      return true;
    }
  }

  return false;
}

bool Instance::CreateInstance(const ExtensionNameList& extensions, ValidationFlags validationFlags)
{

  /*
  const auto& layers = vk::enumerateInstanceLayerProperties().value;
  for(const auto& layer : layers)
  {
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
   */
}

vk::Result Instance::InitialisePhysicalGPU(bool preferDescreeteNotIntegrated)
{

  const vk::PhysicalDeviceType preferredType = preferDescreeteNotIntegrated
                                                   ? vk::PhysicalDeviceType::eDiscreteGpu
                                                   : vk::PhysicalDeviceType::eIntegratedGpu;

  const vk::PhysicalDeviceType fallbackType = preferDescreeteNotIntegrated
                                                  ? vk::PhysicalDeviceType::eIntegratedGpu
                                                  : vk::PhysicalDeviceType::eDiscreteGpu;

  auto result = mImpl->instance.enumeratePhysicalDevices();
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
    if((currentGPUProps.deviceType != preferredType && gpuProperties.deviceType == preferredType) ||
       (currentGPUProps.deviceType != preferredType && gpuProperties.deviceType == fallbackType))
    {
      currentGPU      = gpu;
      currentGPUProps = gpuProperties;
    }
  }

  // make sure selected GPU is THE REAL GPU not a blanket or refrigerator
  if(currentGPUProps.deviceType != preferredType && currentGPUProps.deviceType != fallbackType)
  {
    // error
    return vk::Result::eErrorFeatureNotPresent;
  }

  // store GPU data
  mImpl->gpu                 = currentGPU;
  mImpl->gpuProperties       = currentGPUProps;
  mImpl->gpuMemoryProperties = currentGPU.getMemoryProperties();
  mImpl->gpuFeatures         = currentGPU.getFeatures();

  /*
  cout << "[GPU] Name     : " << mGPUProperties.deviceName << endl;
  cout << "[GPU] DeviceID : " << mGPUProperties.deviceID << endl;
  cout << "[GPU] VendorID : " << mGPUProperties.vendorID << endl;
  */

  mImpl->deviceExtensionProperties = mImpl->gpu.enumerateDeviceExtensionProperties().value;
  mImpl->queueFamilyProperties     = mImpl->gpu.getQueueFamilyProperties();

  //@fixme: need surface at this point
  //mSurfacePresentModes       = mImpl->gpu.getSurfacePresentModesKHR(mSurface).value;

  // as debug list all the extensions
  for(auto& ext : mImpl->queueFamilyProperties)
  {
    //cout << "[DEVICE] Ext: " << ext.extensionName << endl;
  }

  return result.result;
}

bool Instance::CreateSurface(const SurfaceInfo& info)
{
  if(info.mType == SurfaceInfo::Type::XLIB)
  {
    CreateXlibSurface(static_cast< const SurfaceXlibInfo& >(info));
  }
  return true;
}

bool Instance::CreateXlibSurface(const SurfaceXlibInfo& xlibInfo)
{
  XGetGeometry(xlibInfo.mDisplay, xlibInfo.mWindow, nullptr, nullptr, nullptr, &mImpl->surfaceWidth,
               &mImpl->surfaceHeight, nullptr, nullptr);
  vk::XlibSurfaceCreateInfoKHR info;
  info.setDpy(xlibInfo.mDisplay).setWindow(xlibInfo.mWindow);
  auto result    = mImpl->instance.createXlibSurfaceKHR(info);
  mImpl->surface = result.value;
  return result.result == vk::Result::eSuccess;
}
}
}
}