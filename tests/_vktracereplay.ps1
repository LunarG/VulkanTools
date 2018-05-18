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

$LVL_DIR = "submodules\Vulkan-ValidationLayers\"

# Copy everything we need into the temp directory, so we
# can make sure we are using the correct dll and exe files
cd vktracereplay_tmp
cp ..\..\vktrace\$dPath\vkreplay.exe .
cp ..\..\vktrace\$dPath\vktrace.exe .
cp ..\..\$LVL_DIR\demos\$dPath\cube.exe .
cp ..\..\$LVL_DIR\demos\$dPath\smoketest.exe .
cp ..\..\$LVL_DIR\demos\*.ppm .
cp ..\..\$LVL_DIR\demos\*.spv .
cp ..\..\$LVL_DIR\loader\$dPath\vulkan-1.dll .
cp ..\..\layersvt\$dPath\VkLayer_screenshot.dll .
cp ..\..\layersvt\$dPath\VkLayer_screenshot.json .
cp ..\..\layersvt\$dPath\VkLayer_vktrace_layer.dll .
cp ..\..\layersvt\$dPath\VkLayer_vktrace_layer.json .

# Change PATH to the temp directory
$oldpath = $Env:PATH
$Env:PATH = $pwd

# Set up some modified env vars
$Env:VK_LAYER_PATH = $pwd

# Do a trace and replay for cube
& vktrace -o c01.vktrace -s 1 -p cube -a "--c 10" --PMB true > trace.sout 2> trace.serr
rename-item -path 1.ppm -newname 1-cubetrace.ppm
& vkreplay  -s 1 -o  c01.vktrace > replay.sout 2> replay.serr
rename-item -path 1.ppm -newname 1-cubereplay.ppm

# Do a trace and replay for smoketest
& vktrace -o s01.vktrace -s 1 -p smoketest -a "--c 10" --PMB true > trace.sout 2> trace.serr
rename-item -path 1.ppm -newname 1-smoketrace.ppm
& vkreplay  -s 1 -o  s01.vktrace > replay.sout 2> replay.serr
rename-item -path 1.ppm -newname 1-smokereplay.ppm

# Do a trace and replay for smoketest with flush
& vktrace -o s02.vktrace -s 1 -p smoketest -a "--c 10 --flush" --PMB false > trace.sout 2> trace.serr
rename-item -path 1.ppm -newname 2-smoketrace.ppm
& vkreplay  -s 1 -o  s02.vktrace > replay.sout 2> replay.serr
rename-item -path 1.ppm -newname 2-smokereplay.ppm

# Do a trace and replay for smoketest with push constants
& vktrace -o s03.vktrace -s 1 -p smoketest -a "--c 10 -p" --PMB false > trace.sout 2> trace.serr
rename-item -path 1.ppm -newname 3-smoketrace.ppm
& vkreplay  -s 1 -o  s03.vktrace > replay.sout 2> replay.serr
rename-item -path 1.ppm -newname 3-smokereplay.ppm

# Replay old trace if specified.
if ($Replay) {
    if (Test-Path $Replay/cubeold.vktrace) {
        & vkreplay -s 1 -o "$Replay/cubeold.vktrace" > replayold.sout 2> replayold.serr
        rename-item -path 1.ppm -newname 1-replayold.ppm
    }

    # Restore PATH
    $Env:PATH = $oldpath
    # Run trace/replay test on any .vktrace in this folder
    $tracedir = Join-Path -Path $PWD -ChildPath '..\vktracereplay.py'
    & python $tracedir $Replay $PWD\vktrace.exe $PWD $PWD\vkreplay.exe

    if (!($LastExitCode -eq 0)) {
        echo 'Trace/replay regression tests failed.'
        write-host -background black -foreground red "[  FAILED  ] "  -nonewline;
        $exitstatus = 1
    }
}
else {
    # Restore PATH
    $Env:PATH = $oldpath
}

# Force a failure - for testing this script
#cp vulkan.dll 1-cubereplay.ppm
#rm 1-cubetrace.ppm
#rm 1-cubereplay.ppm

if ($exitstatus -eq 0) {
   # Check that two screenshots were created, and the third if replaying an old trace
   if (!(Test-Path 1-cubetrace.ppm) -or !(Test-Path 1-cubereplay.ppm) -or
       #!(Test-Path 1-smoketrace.ppm) -or !(Test-Path 1-smokereplay.ppm) -or
        ($Replay -and (Test-Path $Replay/cubeold.vktrace) -and !(Test-Path 1-replayold.ppm))) {
           echo 'Trace file does not exist'
           write-host -background black -foreground red "[  FAILED  ] "  -nonewline;
           $exitstatus = 1
   }
}

if ($exitstatus -eq 0) {
    # ensure the trace and replay snapshots are identical
    fc.exe /b 1-cubetrace.ppm 1-cubereplay.ppm > $null
    if (!(Test-Path 1-cubetrace.ppm) -or !(Test-Path 1-cubereplay.ppm) -or $LastExitCode -eq 1) {
        echo 'Cube trace files do not match'
        write-host -background black -foreground red "[  FAILED  ] "  -nonewline;
        $exitstatus = 1
    }
    fc.exe /b 1-smoketrace.ppm 1-smokereplay.ppm > $null
    if (!(Test-Path 1-smoketrace.ppm) -or !(Test-Path 1-smokereplay.ppm) -or $LastExitCode -eq 1) {
        echo 'Smoke trace files do not match'
        write-host -background black -foreground red "[  FAILED  ] "  -nonewline;
        $exitstatus = 1
    }
    if ($Replay -and (Test-Path $Replay/cubeold.vktrace)) {
        # check old trace
        fc.exe /b "$Replay\cubeold.ppm" 1-replayold.ppm > $null
        if (!(Test-Path "$Replay/cubeold.ppm") -or !(Test-Path 1-replayold.ppm) -or $LastExitCode -eq 1) {
            echo 'Old trace does not match'
            write-host -background black -foreground red "[  FAILED  ] "  -nonewline;
            $exitstatus = 1
        }
    }
}

# check the average pixel value of each screenshot to ensure something plausible was written
#if ($exitstatus -eq 0) {
#    $trace_mean = (convert 1-cubetrace.ppm -format "%[mean]" info:)
#    $replay_mean = (convert 1-cubereplay.ppm -format "%[mean]" info:)
#    $version = (identify -version)
#
#    # normalize the values so we can support Q8 and Q16 imagemagick installations
#    if ($version -match "Q8") {
#        $trace_mean = $trace_mean   / 255 # 2^8-1
#        $replay_mean = $replay_mean / 255 # 2^8-1
#    } else {
#        $trace_mean = $trace_mean   / 65535 # 2^16-1
#        $replay_mean = $replay_mean / 65535 # 2^16-1
#    }
#
#    # if either screenshot is too bright or too dark, it either failed, or is a bad test
#    if (($trace_mean -lt 0.10) -or ($trace_mean -gt 0.90)){
#        echo ''
#        echo 'Trace screenshot failed mean check, must be in range [0.1, 0.9]'
#        write-host 'Detected mean:' $trace_mean
#        echo ''
#        write-host -background black -foreground red "[  FAILED  ] "  -nonewline;
#        $exitstatus = 1
#    }
#    if (($replay_mean -lt 0.10) -or ($replay_mean -gt 0.90)){
#        echo ''
#        echo 'Replay screenshot failed mean check, must be in range [0.1, 0.9]'
#        write-host 'Detected mean:' $replay_mean
#        echo ''
#        write-host -background black -foreground red "[  FAILED  ] "  -nonewline;
#        $exitstatus = 1
#    }
#}

# if we passed all the checks, the test is good
if ($exitstatus -eq 0) {
   write-host -background black -foreground green "[  PASSED  ] " -nonewline;

    # cleanup
    cd ..
    rm -recurse -force vktracereplay_tmp  > $null 2> $null
}

if ($exitstatus -eq 0) {
    $scriptpath = (Split-Path -parent $PSCommandPath)
    $toolsroot = (Get-Item $scriptpath).parent.parent.FullName
    $command = ".\trace_positive_validation.ps1 -toolsroot $toolsroot $tplarg"
    Invoke-Expression $command
    # If test didn't crash, it passed
}

write-host "vktracereplay.ps1: Vktrace trace/replay"
write-host
if ($exitstatus) {
    echo '1 FAILED TEST'
}

Remove-Item Env:\VK_LAYER_PATH
exit $exitstatus
