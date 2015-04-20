

solution "AssetTool"
    configurations { "Debug", "Release" }
        flags{ "Unicode", "NoPCH" }
        libdirs { "lib" }
        includedirs { "include"}
        platforms{"x64"}

        local location_path = "solution"

        if _ACTION then
	        defines { "_CRT_SECURE_NO_WARNINGS", "NOMINMAX" }

            location_path = location_path .. "/" .. _ACTION
            location ( location_path )
            location_path = location_path .. "/projects"
        end


    configuration { "Debug" }
        defines { "DEBUG" }
        flags { "Symbols" }
        

    configuration { "Release" }
        defines { "NDEBUG", "RELEASE" }
        flags { "Optimize", "FloatFast" }

    configuration { "Debug" }
        targetdir ( "bin/" .. "/debug" )

    configuration { "Release" }
        targetdir ( "bin/" .. "/release" )


	project "AssetTool"
		targetname "AssetTool"
		debugdir ""
		location ( location_path )
		language "C++"
		kind "ConsoleApp"
		files { "src/**" }
		includedirs { "include","src/shared", "src/core", "src/gfx"}
		links { "assimp", "glew32", "glfw3" , "opengl32"}

		configuration {"debug"}
			links { "glew32", "glfw3" , "opengl32"}

		configuration { "Release" }
			links { "glew32", "glfw3" , "opengl32"}
