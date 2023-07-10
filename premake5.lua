workspace "Spark"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

-- 输出目录
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Spark"
    location "Spark"
    kind "SharedLib"
    language "C++"
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
        "%{prj.name}/vendor/spdlog/include"
    }
    -- IDE编译器设置
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "SPK_PLATFORM_WINDOWS",
            "SPK_BUILD_DLL"
        }

        -- 自动拷贝连接dll命令
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

        -- 创建过滤器
        filter "configurations:Debug"
            defines "SPK_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "SPK_RELEASE"
            optimize "On"

        filter "configurations:Distribution"
            defines "SPK_DISTRIBUTION"
            optimize "On"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
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
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "SPK_PLATFORM_WINDOWS"
        }
        -- 创建过滤器
        filter "configurations:Debug"
            defines "SPK_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "SPK_RELEASE"
            optimize "On"

        filter "configurations:Distribution"
            defines "SPK_DISTRIBUTION"
            optimize "On"