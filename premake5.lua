workspace "Spark"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

-- ���Ŀ¼
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Spark"
    location "Spark"
    kind "SharedLib"
    language "C++"
    -- ���ñ������Ŀ¼
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("binInt/" .. outputdir .. "/%{prj.name}")
    -- ����Դ�����ļ�
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    -- include�ļ�
    includedirs
    {
        "%{prj.name}/vendor/spdlog/include"
    }
    -- IDE����������
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "SPK_PLATFORM_WINDOWS",
            "SPK_BUILD_DLL"
        }

        -- �Զ���������dll����
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

        -- ����������
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
    -- ���ñ������Ŀ¼
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("binInt/" .. outputdir .. "/%{prj.name}")
    -- ����Դ�����ļ�
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    -- include�ļ�
    includedirs
    {
        "Spark/vendor/spdlog/include",
        "Spark/src"
    }
    -- ����Sandbox��Spark
    links
    {
        "Spark"
    }
    -- IDE����������
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "SPK_PLATFORM_WINDOWS"
        }
        -- ����������
        filter "configurations:Debug"
            defines "SPK_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "SPK_RELEASE"
            optimize "On"

        filter "configurations:Distribution"
            defines "SPK_DISTRIBUTION"
            optimize "On"