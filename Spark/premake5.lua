project "Spark"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    -- 设置编译输出目录
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/binInt/" .. outputdir .. "/%{prj.name}")

    -- 设置pch预编译头
    pchheader "spkpch.h"
    pchsource "src/spkpch.cpp"

    -- 归纳源代码文件
    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/stb_image/**.h",
        "vendor/stb_image/**.cpp",
        "vendor/glm/gml/**.hpp",
        "vendor/glm/gml/**.inl"
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
        "src",
		"Panel",
        "vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
    }

    -- link文件
    links
    {
        "GLFW",
        "Glad",
        "ImGui",
		"yaml-cpp",
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
