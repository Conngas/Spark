project "Spark-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    -- ���ñ������Ŀ¼
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/binInt/" .. outputdir .. "/%{prj.name}")
    -- ����Դ�����ļ�
    
	files
    {
		"src/**.h",
		"src/**.cpp",
		"Assets/**.png",
		"Assets/**.jpg"
    }
    -- include�ļ�
    includedirs
    {
        "%{wks.location}/Spark/vendor/spdlog/include",
        "%{wks.location}/Spark/src",
        "%{wks.location}/Spark/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}"
    }
    -- ����Sandbox��Spark
    links
    {
        "Spark"
    }
    -- IDE����������
    filter "system:windows"
        systemversion "latest"

        defines
        {
            "SPK_PLATFORM_WINDOWS"
        }
        -- ����������
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
