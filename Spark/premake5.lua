project "Spark"
    location "Spark"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
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
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/gml/**.hpp",
        "%{prj.name}/vendor/glm/gml/**.inl"
    }

    -- 未来会删除
    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    -- include文件
    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}"
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
        systemversion "latest"

        defines
        {
            "SPK_PLATFORM_WINDOWS"
        }
        -- 创建过滤器
        filter "configurations:Debug"
            defines "SPK_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "SPK_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Distribution"
            defines "SPK_DISTRIBUTION"
            runtime "Release"
            optimize "on"