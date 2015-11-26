
local projectname = 'SimpleVM'
solution(projectname)
    configurations{'Debug', 'Release'}
    project(projectname)
        language'C++'
        toolset'clang'
        kind'ConsoleApp'
        buildoptions{'-std=c++14'}
        files{'Source/**.cpp', 'Source/**.hpp'}
        defines{'SVM_OS_' .. _OS}
        location'Build'
        objdir'Build/Obj'
        debugdir'Bin'
        targetdir'Bin'
        filter{'configurations:Debug'}
            warnings'Extra'
            defines{'DEBUG','SVM_DEBUG'}
            flags{'Symbols'}
            targetsuffix'-d'
        filter{'configurations:Release'}
            optimize"Size"
