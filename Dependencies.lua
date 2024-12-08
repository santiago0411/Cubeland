VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["entt"]			= "%{wks.location}/Cubeland/vendor/entt/include"
IncludeDir["spdlog"]		= "%{wks.location}/vendor/spdlog/include"
IncludeDir["stb_image"]		= "%{wks.location}/Cubeland/vendor/stb_image"
IncludeDir["GLFW"]			= "%{wks.location}/vendor/GLFW/include"
IncludeDir["Glad"]			= "%{wks.location}/vendor/Glad/include"
IncludeDir["glm"]			= "%{wks.location}/Cubeland/vendor/glm"
IncludeDir["ImGui"]			= "%{wks.location}/vendor/ImGui"
IncludeDir["VulkanSDK"]		= "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"]			= "%{VULKAN_SDK}/Lib"

Library = {}

Library["Vulkan"]		= "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"]	= "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"]			= "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"]		= "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"]	= "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"]		= "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"]			= "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"]		= "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
