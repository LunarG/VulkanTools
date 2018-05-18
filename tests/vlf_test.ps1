# Powershell script for running the Vulkan Layer Factory sanity test

# vlf_test.ps1
# This script will run the demo vulkaninfo with the demo_layer and capture the output.
# This layer will output all of the API names used by the application, and this script
# will search the output for a certain amount of APIs that are known to be used by this
# demo.  If the threshold is met, the test will indicate PASS, else FAILURE.
#
# To run this test:
#    Change to the repo build tests directory, run the script:
#         powershell vlf_test.ps1 [-Debug]
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
write-host "vlf_test.ps1: Vulkan Layer Factory Sanity Test"

# Save current directory
$current_directory = $pwd

# Set up some modified env vars
$Env:VK_LAYER_PATH = "$pwd\..\layers\$dPath"
cd "..\submodules\Vulkan-ValidationLayers\demos\$dPath"
$Env:VK_ICD_FILENAMES = "..\..\icd\$dPath\VkICD_mock_icd.json"
$Env:VK_INSTANCE_LAYERS = "VK_LAYER_LUNARG_demo_layer"

# Run vulkaninfo with mock ICD and demo layer, capturing output
& .\vulkaninfo.exe > temp_output_file

# Fail if temp file is not present, or if results do not match expectations
if (!(Test-Path temp_output_file)) {
    echo 'Output file does not exist'
    write-host -background black -foreground red "[  FAILED  ] "  -nonewline;
    $exitstatus = 1
} else {
    # Look for sensible results in output file
    $count = (Select-String -Path .\temp_output_file -Pattern "vkGetPhysicalDeviceFormatProperties").length
    if ($count -lt 50) {
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

Remove-Item -Path .\temp_output_file

cd $current_directory

exit $exitstatus
