
New-Item -Path "$($PSScriptRoot)\..\Plugins\pluginInstalled.txt" -ItemType "File"

# Plugins URLs

# exemple
$ueImGuiDlUrl = "https://github.com/IDI-Systems/UnrealImGui/archive/refs/heads/master.tar.gz"


# Download Plugins

# exemple
Write-Output "[openGL-portable-setup] Downloading UnrealImGUI..."
curl.exe -L "$ueImGuiDlUrl" -o "$($PSScriptRoot)\..\Plugins\imgui.tar.gz" --progress-bar


# Extract Plugins

# exemple
Write-Output "[openGL-portable-setup] Extracting UnrealImGUI..."
tar -xzf "$($PSScriptRoot)\..\Plugins\imgui.tar.gz" -C "$($PSScriptRoot)\..\Plugins"

# Extra steps

# exemple (renaming the plugin)
Rename-Item -Path "$($PSScriptRoot)\..\Plugins\UnrealImGui-master" -NewName "UnrealImGui"

return 0
