project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    -- 设置编译输出目录
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/binInt/" .. outputdir .. "/%{prj.name}")
    -- 归纳源代码文件
    files
    {
        "src/**.h",
        "src/**.cpp"
    }
    -- include文件
    includedirs
    {
        "&{wks.location}/Spark/vendor/spdlog/include",
        "%{wks.location}/Spark/src",
        "%{wks.location}/Spark/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}"
    }
    -- 连接Sandbox到Spark
    links
    {
        "Spark"
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