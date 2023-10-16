include "./vendor/premake/solution_items.lua"


workspace "Spark"
    architecture "x86_64"
    startproject "Spark-Editor"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

    solution_items
    {
        ".editorconfig"
    }

    flags
    {
        "MultiProcessorCompile"
    }
    -- 输出目录
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    -- 复制GLFW lua配置到本配置中
    IncludeDir = {}
    IncludeDir["GLFW"] = "%{wks.loaction}/vendor/GLFW/include"
    IncludeDir["stb_image"] = "%{wks.loaction}/vendor/stb_image"
    
    IncludeDir["Glad"] = "%{wks.location}/Spark/vendor/Glad/include"
    IncludeDir["ImGui"] = "%{wks.location}/Spark/vendor/imgui"
    IncludeDir["glm"] = "%{wks.location}/Spark/vendor/glm"
    IncludeDir["entt"] = "%{wks.location}/Spark/vendor/entt/include"

    -- 组依赖（include）
    -- include "Spark/vendor/GLFW"
    -- include "Spark/vendor/Glad"
    -- include "Spark/vendor/imgui"
    group "Dependencies"
        include "vendor/premake"
        include "Spark/vendor/GLFW"
        include "Spark/vendor/Glad"
        include "Spark/vendor/imgui"
        
    group ""

include "Spark"
include "Sandbox"
include "Spark-Editor"