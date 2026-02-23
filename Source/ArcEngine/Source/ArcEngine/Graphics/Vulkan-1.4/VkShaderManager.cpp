#include "VkShaderManager.h"


void VkShaderManager::Create()
{
    slang::createGlobalSession(mSlangGlobalSession.writeRef());
}

void VkShaderManager::Destroy()
{
    slang::shutdown();
}

void VkShaderManager::CreateShader(VulkanAPI& api)
{
    ShaderObject object;

    //Shader Creation

    mShaderList.push_back(object);
}

void VkShaderManager::DestroyShader(VulkanAPI& api)
{
    unsigned int listSize = mShaderList.size();

    for (unsigned int i = 0; i < listSize; i++)
    {
        api.vkDestroyPipelineLayout(api.device, mShaderList[i].pipelineLayout, nullptr);
        api.vkDestroyPipeline(api.device, mShaderList[i].pipeline, nullptr);
    }

    mShaderList.clear();
}