# Powershell script for running the Vulkan API_Dump Layer sanity test

# apidump_test.ps1
# This script will run the demo vulkaninfo with the api_dump_layer and capture the output.
# The script will search the output for certain APIs that are known to be used by this app.
# If the threshold is met, the test will indicate PASS, else FAILURE.
#
# To run this test:
#    Change to the repo build tests directory, run the script:
#         powershell apidump_test.ps1 [-Debug]
#
#    Specify -Debug for debug builds

Param(
    [switch]$Debug
)

if ($Debug) {
    $dPath = "Debug"
} else {
    $dPath = "Release"
}

$exitstatus = 0

write-host -background black -foreground green "[  RUN     ] " -nonewline
write-host "apidump_test.ps1: Vulkan API-Dump Layer Sanity Test"

# Save current directory
$current_directory = $pwd

# Set up some modified env vars
$Env:VK_LAYER_PATH = "$pwd\..\layersvt\$dPath"
cd "..\submodules\Vulkan-ValidationLayers\demos\$dPath"
$Env:VK_ICD_FILENAMES = "..\..\icd\$dPath\VkICD_mock_icd.json"
$Env:VK_INSTANCE_LAYERS = "VK_LAYER_LUNARG_api_dump"

# Run vulkaninfo with mock ICD and api_dump layer, capturing output
& .\vulkaninfo.exe > apidump_temp_output_file

# Fail if temp file is not present, or if results do not match expectations
if (!(Test-Path apidump_temp_output_file)) {
    echo 'Output file does not exist'
    write-host -background black -foreground red "[  FAILED  ] "  -nonewline;
    $exitstatus = 1
} else {
    # Look for sensible results in output file
    $GPDFP_count = (Select-String -Path .\apidump_temp_output_file -Pattern "vkGetPhysicalDeviceFormatProperties").length
    $pipelineCacheUUID_count = (Select-String -Path .\apidump_temp_output_file -Pattern "pipelineCacheUUID").length
    $vk_format_feature_count = (Select-String -Path .\apidump_temp_output_file -Pattern "VK_FORMAT_FEATURE").length
    if (($GPDFP_count -lt 50) -OR ($pipelineCacheUUID_count -lt 10) -OR ($vk_format_feature_count -lt 1000)) {
        $exitstatus = 1
    }
}

# Output pass/fail result
if ($exitstatus -eq 0) {
    write-host -background black -foreground green "[  PASSED  ] " -nonewline;
} else {
    write-host -background black -foreground red "[  FAILED  ] "
}

# Clean up env, files, path
Remove-Item Env:\VK_INSTANCE_LAYERS
Remove-Item Env:\VK_ICD_FILENAMES
Remove-Item Env:\VK_LAYER_PATH

Remove-Item -Path .\apidump_temp_output_file

cd $current_directory

exit $exitstatus
