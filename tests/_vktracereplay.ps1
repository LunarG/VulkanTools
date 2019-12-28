# Powershell script for running the vktrace trace/replay auto test
# To run this test:
#    cd <this-dir>
#    powershell C:\src\ValidationLayers\vktracereplay.ps1 [-Replay <tracepath>] [-Debug]
#
#    <tracepath> example: "C:\trace" would result in the script testing against "C:\trace.vktrace" and "C:\trace.ppm"
param (
    [switch]$Debug,
    [string]$Replay
)

$exitstatus = 0

if ($Debug) {
    $dPath = "Debug"
    $tplarg = "-debug"
} else {
    $dPath = "Release"
}

write-host -background black -foreground green "[  RUN     ] " -nonewline
write-host "vktracereplay.ps1: Vktrace trace/replay"

# Create a temp directory to run the test in

if (Test-Path .\vktracereplay_tmp) {
    rm -recurse -force .\vktracereplay_tmp  > $null 2> $null
}
new-item vktracereplay_tmp -itemtype directory > $null 2> $null

$VT_DIR = "Vulkan-Tools\"

# Copy everything we need into the temp directory, so we
# can make sure we are using the correct dll and exe files
cd vktracereplay_tmp
cp ..\..\vktrace\$dPath\vkreplay.exe .
cp ..\..\vktrace\$dPath\vktrace.exe .
cp ..\..\layersvt\$dPath\VkLayer_screenshot.dll .
cp ..\..\layersvt\$dPath\VkLayer_screenshot.json .
cp ..\..\layersvt\$dPath\VkLayer_vktrace_layer.dll .
cp ..\..\layersvt\$dPath\VkLayer_vktrace_layer.json .

# Replay old trace if specified.
if ($Replay) {
    # Run trace/replay test on any .vktrace in this folder
    $tracedir = Join-Path -Path $PWD -ChildPath '..\vktracereplay.py'
    & python $tracedir $Replay $PWD\vktrace.exe $PWD $PWD\vkreplay.exe

    if (!($LastExitCode -eq 0)) {
        echo 'Trace/replay regression tests failed.'
        write-host -background black -foreground red "[  FAILED  ] "  -nonewline;
        $exitstatus = 1
    }
}

# if we passed all the checks, the test is good
if ($exitstatus -eq 0) {
   write-host -background black -foreground green "[  PASSED  ] " -nonewline;

    # cleanup
    cd ..
    rm -recurse -force vktracereplay_tmp  > $null 2> $null
}

#if ($exitstatus -eq 0) {
#    $scriptpath = (Split-Path -parent $PSCommandPath)
#    $toolsroot = (Get-Item $scriptpath).parent.parent.FullName
#    $command = ".\trace_positive_validation.ps1 -toolsroot $toolsroot $tplarg"
#    Invoke-Expression $command
#    # If test didn't crash, it passed
#}

write-host "vktracereplay.ps1: Vktrace trace/replay"
write-host
if ($exitstatus) {
    echo '1 FAILED TEST'
}

Remove-Item Env:\VK_LAYER_PATH
exit $exitstatus
