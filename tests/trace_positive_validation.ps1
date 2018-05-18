param (
    [switch]$debug,
    [string]$toolsroot
)

if ($debug) {
    $build = "Debug"
} else {
    $build = "Release"
}

trap {
    Write-Error $_
    exit 1
}

$skip = @( "LongFenceChain)", "CreatePipelineCheckShaderCapabilityExtension1of2",
    "CreatePipelineCheckShaderCapabilityExtension2of2", "ExternalFence", "ExternalMemory" )
$lvlbinpath = "$toolsroot\build\submodules\Vulkan-ValidationLayers"
$lvlcodepath = "$toolsroot\submodules\Vulkan-ValidationLayers"

if ( -not($toolsroot) ) { Throw "You must supply a path to a VulkanTools build via -toolsroot" }

$env:VK_LAYER_PATH = "$lvlbinpath\layers\$build;$toolsroot\build\layersvt\$build"

$lines = Select-String -Path $lvlcodepath\tests\layer_validation_tests.cpp -Pattern "VkPositiveLayerTest,"
foreach ($line in $lines) {
    $left = $line -replace ".*, "
    $test = $left -replace "\) {"
    if ( -Not $skip.Contains($test)) {
        $command = "$toolsroot\build\vktrace\$build\vktrace -p $lvlbinpath\tests\$build\vk_layer_validation_tests.exe -o positive.vktrace -a ""--gtest_filter=VkPositiveLayerTest.$test"""
        echo $command
        Invoke-Expression $command
        #if ($lastExitCode) {
        #    Write-Error $lastExitCode
        #    exit 1
        #}
        $command = "$toolsroot\build\vktrace\$build\vkreplay -t positive.vktrace"
        echo $command
        Invoke-Expression $command
        #if ($lastExitCode) {
        #    Write-Error $lastExitCode
        #    exit 1
        #}
        rm positive.vktrace
    }
}

# Make it to here and we pass
exit 0
