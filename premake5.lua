workspace "Spark"
    architecture "x64"
    startproject "Sandbox"
    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

    -- 输出目录
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    -- 复制GLFW lua配置到本配置中
    IncludeDir = {}
    IncludeDir["GLFW"] = "Spark/vendor/GLFW/include"
    IncludeDir["Glad"] = "Spark/vendor/Glad/include"
    IncludeDir["ImGui"] = "Spark/vendor/imgui"

    -- 组依赖（include）
    -- include "Spark/vendor/GLFW"
    -- include "Spark/vendor/Glad"
    -- include "Spark/vendor/imgui"
    group "Dependencies"
        include "Spark/vendor/GLFW"
        include "Spark/vendor/Glad"
        include "Spark/vendor/imgui"
        

project "Spark"
    location "Spark"
    kind "SharedLib"
    language "C++"
    staticruntime "off"
    -- 设置编译输出目录
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("binInt/" .. outputdir .. "/%{prj.name}")

    -- 设置pch预编译头
    pchheader "spkpch.h"
    pchsource "Spark/src/spkpch.cpp"

    -- 归纳源代码文件
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    -- include文件
    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
    }
    -- link文件
    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    -- IDE编译器设置
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "SPK_PLATFORM_WINDOWS",
            "SPK_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        -- 自动拷贝连接dll命令
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

        -- 创建过滤器
        filter "configurations:Debug"
            defines "SPK_DEBUG"
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines "SPK_RELEASE"
            runtime "Release"
            optimize "On"

        filter "configurations:Distribution"
            defines "SPK_DISTRIBUTION"
            runtime "Release"
            optimize "On"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"
    -- 设置编译输出目录
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("binInt/" .. outputdir .. "/%{prj.name}")
    -- 归纳源代码文件
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    -- include文件
    includedirs
    {
        "Spark/vendor/spdlog/include",
        "Spark/src"
    }
    -- 连接Sandbox到Spark
    links
    {
        "Spark"
    }
    -- IDE编译器设置
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "SPK_PLATFORM_WINDOWS"
        }
        -- 创建过滤器
        filter "configurations:Debug"
            defines "SPK_DEBUG"
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines "SPK_RELEASE"
            runtime "Release"
            optimize "On"

        filter "configurations:Distribution"
            defines "SPK_DISTRIBUTION"
            runtime "Release"
            optimize "On"