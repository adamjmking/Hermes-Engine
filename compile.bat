setlocal enabledelayedexpansion

set GLSLC="C:\VulkanSDK\1.3.290.0\Bin\glslc.exe"
set SHADER_DIR="F:\Special Projects\Repos\Hermes-Engine\src\graphics\shaders"

cd /d %SHADER_DIR%

:: echo Deleting existing .spv files...
:: del /q *.vert.spv *.frag.spv

for %%f in (*.vert *.frag) do (
    set "filename=%%~nf"
    if "%%~xf"==".vert" (
        echo Compiling vertext shader %%f...
        %GLSLC% "%%f" -o "!filename!.vert.spv"
    ) else (
        echo Compiling fragment shader %%f...
        %GLSLC% "%%f" -o "!filename!.frag.spv"
    )
)

pause
