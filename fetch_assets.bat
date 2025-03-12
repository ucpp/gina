@echo off
setlocal

set "ASSETS_DIR=demo/assets"
set "TEMP_DIR=demo/assets_temp"
set "REPO_URL=https://github.com/ucpp/gina_assets.git"
set "BRANCH=master"

if exist "%ASSETS_DIR%" rmdir /s /q "%ASSETS_DIR%"
mkdir "%ASSETS_DIR%"

if exist "%TEMP_DIR%" rmdir /s /q "%TEMP_DIR%"

git clone --depth=1 --filter=blob:none --sparse --branch %BRANCH% %REPO_URL% "%TEMP_DIR%" || exit /b

pushd "%TEMP_DIR%"
git sparse-checkout init --cone
git sparse-checkout set textures models

for %%D in (textures models) do if exist "%%D" move "%%D" "..\assets" >nul

popd
rmdir /s /q "%TEMP_DIR%"

echo Done!
endlocal
