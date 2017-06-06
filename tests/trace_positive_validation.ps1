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

$skip = @( "LongFenceChain)" )

if ( -not($toolsroot) ) { Throw "You must supply a path to a VulkanTools build via -toolsroot" }

$env:VK_LAYER_PATH = "$toolsroot\build\layers\$build;$toolsroot\build\layersvt\$build"

$lines = Select-String -Path $toolsroot\tests\layer_validation_tests.cpp -Pattern "VkPositiveLayerTest,"
foreach ($line in $lines) {
    $left = $line -replace ".*, "
    $test = $left -replace "\) {"
    if ( -Not $skip.Contains($test)) {
        $command = "$toolsroot\build\vktrace\$build\vktrace -p $toolsroot\build\tests\$build\vk_layer_validation_tests.exe -o positive.vktrace -a ""--gtest_filter=VkPositiveLayerTest.$test"""
        echo $command
        Invoke-Expression $command
        $command = "$toolsroot\build\vktrace\$build\vkreplay -t positive.vktrace"
        echo $command
        Invoke-Expression $command
        rm positive.vktrace
    }
}
