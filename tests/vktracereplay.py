# Python script for running the regression test for both trace and replay. Python version 3 or greater required.
# NOTES - 
#   vkcube is expected to be in the PATH or working directory if the --legacy-vkcube switch
#   is passed in.
#   This script is normally invoked from _vktracereplay.ps1 or vktracereplay.sh.
#
# vkcube is traced and replayed with screenshot comparison, then again with trim.
# Also runs regression by iterating through old traces in a directory specified by the user, tracing a replay of the old trace, and replaying the new trace.
#
# To run this test:
#    cd <this-dir>
#    python ./vktracereplay.py <old-trace-directory> <vktrace-exe> <vktrace-layer-path> <vkreplay-exe>
#
#    <old-trace-directory> example: "C:\traces\" would result in the script testing against "C:\traces\trace1.vktrace", "C:\traces\trace2.vktrace", etc.

import os, sys, subprocess, time, argparse, filecmp, time, shutil



def findnth(haystack, needle, n):
    parts= haystack.split(needle, n+1)
    if len(parts)<=n+1:
        return -1
    return len(haystack)-len(parts[-1])-len(needle)



def HandleError(msg):
    # If msg is greater than 10 lines,
    # Replace 11th line and beyond with ellipses
    if msg.count('\n') > 10:
        msg[findnth(msg, '\n', 10):] = '...\n'
    print (msg)
    sys.exit(1)



def GetErrorMessage(out):
    matched_lines = [line for line in out.split('\n') if 'error' in line]
    return ''.join(matched_lines)




def TraceReplayProgramTest(testname, program, programArgs, args):
    print ('Beginning Trace/Replay Test: %s\n' % program)

    startTime = time.time()

    # Trace. Screenshot frame 1
    layerEnv = os.environ.copy()
    layerEnv['VK_LAYER_PATH'] = args.VkLayerPath
    try:
        out = subprocess.check_output([args.VkTracePath, '-o', '%s.vktrace' % testname, '-p', program, '-a', '%s' % programArgs, '-s', '1', '-w', '.'], env=layerEnv).decode('utf-8')
    except subprocess.CalledProcessError as e:
        HandleError('Error while tracing, return code %s:\n%s' % (e.returncode, e.output))

    if 'error' in out:
        err = GetErrorMessage(out)
        HandleError('Errors while tracing:\n%s' % err)

    # Rename 1.ppm to <testname>.trace.ppm
    if os.path.exists('1.ppm'):
        os.rename('1.ppm', '%s.trace.ppm' % testname)
    else:
        HandleError('Error: Screenshot not taken while tracing.')

    # Replay
    try:
        out = subprocess.check_output([args.VkReplayPath, '-o', '%s.vktrace' % testname, '-s', '1'], env=layerEnv).decode('utf-8')
    except subprocess.CalledProcessError as e:
        HandleError('Error while replaying, return code %s:\n%s' % (e.returncode, e.output))

    if 'error' in out:
        err = GetErrorMessage(out)
        HandleError('Error while replaying:\n%s' % err)

    # Rename 1.ppm to <testname>.replay.ppm
    if os.path.exists('1.ppm'):
        os.rename('1.ppm', '%s.replay.ppm'% testname)
    else:
        HandleError ('Error: Screenshot not taken while replaying.')

    # Compare screenshots
    if not filecmp.cmp('%s.trace.ppm' % testname, '%s.replay.ppm' % testname):
        HandleError ('Error: Trace/replay screenshots do not match.')

    elapsed = time.time() - startTime

    print ('Success')
    print ('Elapsed seconds: %s\n' % elapsed)




def TrimTest(testname, program, programArgs, args):
    print ('Beginning Trim Test: %s\n' % program)

    startTime = time.time()

    # Trace with trim frames 100-200. Screenshot frame 101
    layerEnv = os.environ.copy()
    layerEnv['VK_LAYER_PATH'] = args.VkLayerPath
    try:
        out = subprocess.check_output([args.VkTracePath, '-o', '%s.vktrace' % testname, '-p', program, '-a', '%s' % programArgs, '-tr', 'frames-100-200', '-s', '101', '-w', '.'], env=layerEnv).decode('utf-8')
    except subprocess.CalledProcessError as e:
        HandleError('Error while tracing with trim, return code %s:\n%s' % (e.returncode, e.output))

    if 'error' in out:
        err = GetErrorMessage(out)
        HandleError('Errors while tracing with trim:\n%s' % err)

    # Rename 101.ppm to <testname>.trace.ppm
    if os.path.exists('101.ppm'):
        os.rename('101.ppm', '%s.trace.ppm' % testname)
    else:
        HandleError('Error: Screenshot not taken while tracing.')

    # Replay
    try:
        out = subprocess.check_output([args.VkReplayPath, '-o', '%s.vktrace' % testname, '-s', '2'], env=layerEnv).decode('utf-8')
    except subprocess.CalledProcessError as e:
        HandleError('Error while replaying, return code %s:\n%s' % (e.returncode, e.output))

    if 'error' in out:
        err = GetErrorMessage(out)
        HandleError('Error while replaying:\n%s' % err)

    # Rename 1.ppm to <testname>.replay.ppm
    if os.path.exists('2.ppm'):
        os.rename('2.ppm', '%s.replay.ppm' % testname)
    else:
        HandleError ('Error: Screenshot not taken while replaying.')

    # Compare screenshots
    if not filecmp.cmp('%s.trace.ppm' % testname, '%s.replay.ppm' % testname):
        HandleError ('Error: Trim Trace/replay screenshots do not match.')

    elapsed = time.time() - startTime

    print ('Success')
    print ('Elapsed seconds: %s\n' % elapsed)




def LoopTest(testname, program, programArgs, args):
    """ Runs a test on replay loop functionality """

    print ('Beginning Loop Test: %s\n' % program)

    startTime = time.time()

    # Trace and screenshot frame 1
    layerEnv = os.environ.copy()
    layerEnv['VK_LAYER_PATH'] = args.VkLayerPath
    try:
        out = subprocess.check_output([args.VkTracePath, '-o', '%s.vktrace' % testname, '-p', program, '-a', '%s' % programArgs, '-s', '1', '-w', '.'], env=layerEnv).decode('utf-8')
    except subprocess.CalledProcessError as e:
        HandleError('Error while tracing, return code %s:\n%s' % (e.returncode, e.output))

    if 'error' in out:
        err = GetErrorMessage(out)
        HandleError('Errors while tracing:\n%s' % err)

    # Rename 1.ppm to <testname>.trace.ppm
    if os.path.exists('1.ppm'):
        os.rename('1.ppm', '%s.trace.ppm' % testname)
    else:
        HandleError('Error: Screenshot not taken while tracing.')

    # Test against 2nd loop and 3rd loop. Screenshot will always be from the last loop
    for loopCount in [2, 3]:
        # Replay
        try:
            out = subprocess.check_output([args.VkReplayPath, '-o', '%s.vktrace' % testname, '-s', '1', '-l', str(loopCount)], env=layerEnv).decode('utf-8')
        except subprocess.CalledProcessError as e:
            HandleError('Error while replaying, return code %s:\n%s' % (e.returncode, e.output))

        if 'error' in out:
            err = GetErrorMessage(out)
            HandleError('Error while replaying:\n%s' % err)

        # Rename 1.ppm to <testname>.replay.ppm
        if os.path.exists('1.ppm'):
            os.rename('1.ppm', '%s.%s.replay.ppm' % (testname, str(loopCount)))
        else:
            HandleError ('Error: Screenshot not taken while replaying.')

        # Compare screenshots
        if not filecmp.cmp('%s.trace.ppm' % testname, '%s.%s.replay.ppm' % (testname, str(loopCount))):
            HandleError ('Error: Loop Trace/replay screenshots do not match.')

    elapsed = time.time() - startTime

    print ('Success')
    print ('Elapsed seconds: %s\n' % elapsed)




def TraceReplayTraceTest(testname, traceFile, args):
    print ('Beginning Trace/Replay Test: %s\n' % testname)

    startTime = time.time()

    # Parse frame
    if not os.path.exists('%s.config' % traceFile):
        frame = '1'
    else:
        with open('%s.config' % traceFile) as configFile:
            frame = configFile.read().strip()
    
    # Trace replay of <traceFile>
    layerEnv = os.environ.copy()
    layerEnv['VK_LAYER_PATH'] = args.VkLayerPath
    try:
        out = subprocess.check_output([args.VkTracePath, '-o', '%s.vktrace' % testname, '-p', args.VkReplayPath, '-a', '-o %s' % traceFile, '-s', frame, '-w', '.'], env=layerEnv).decode('utf-8')
    except subprocess.CalledProcessError as e:
        HandleError('Error while tracing replay of original trace, return code %s:\n%s' % (e.returncode, e.output))

    if 'error' in out:
        err = GetErrorMessage(out)
        HandleError('Errors while tracing replay of original trace:\n%s' % err)

    # Rename 1.ppm to <testname>.trace.ppm
    if os.path.exists('%s.ppm' % frame):
        os.rename('%s.ppm' % frame, '%s.trace.ppm' % testname)
    else:
        HandleError('Error: Screenshot not taken while tracing.')

    try:
        out = subprocess.check_output([args.VkReplayPath, '-o', '%s.vktrace' % testname, '-s', frame], env=layerEnv).decode('utf-8')
    except subprocess.CalledProcessError as e:
        HandleError('Error while replaying, return code %s:\n%s' % (e.returncode, e.output))

    if 'error' in out:
        err = GetErrorMessage(out)
        HandleError('Error while replaying:\n%s' % err)

    # Rename 1.ppm to <testname>.replay.ppm
    if os.path.exists('%s.ppm' % frame):
        os.rename('%s.ppm' % frame, '%s.replay.ppm' % testname)
    else:
        HandleError ('Error: Screenshot not taken while replaying.')

    # Compare screenshots
    if not filecmp.cmp('%s.trace.ppm' % testname, '%s.replay.ppm' % testname):
        HandleError ('Error: Trace/replay screenshots do not match.')

    elapsed = time.time() - startTime

    print ('Success')
    print ('Elapsed seconds: %s\n' % elapsed)



if __name__ == '__main__':

    # Load settings from command-line
    parser = argparse.ArgumentParser(description='Test vktrace and vkreplay.')
    parser.add_argument('--legacy-vkcube', help='run the legacy vkcube tests', action='store_true')
    parser.add_argument('OldTracesPath', help='directory of old traces to replay')
    parser.add_argument('VkTracePath', help='directory containing vktrace')
    parser.add_argument('VkLayerPath', help='directory containing vktrace layer')
    parser.add_argument('VkReplayPath', help='directory containing vkreplay')
    args = parser.parse_args()

    # vkcube tests were removed by default after commit b270c0024bafbc91e47d81b4bde2ddc38f391ea5
    # in LunarG/VulkanTools, which removed the glslang build as well as the vkcube
    # build for the project.  This means that vkcube is no longer guaranteed to be on
    # the system.  We reviewed several options for fixing this, but ulimately decided
    # to just remove the vkcube testing, because we assert the other replay testing
    # is sufficient, and that vktrace is likely to be deprecated soon in favor of
    # gfxreconstruct anyway.
    if args.legacy_vkcube:

        # Get vkcube executable path from PATH
        cubePath = shutil.which('vkcube')
        if (cubePath is None):
            HandleError('Error: vkcube executable not found')

        # Trace/replay test on vkcube
        TraceReplayProgramTest('vkcube', cubePath, '--c 50', args)

        # Run trim test on vkcube
        # Removing until github issue #646 is resolved
        # TrimTest('cube-trim', cubePath, '--c 250', args)

        # Run loop test on cube
        LoopTest('cube-loop', cubePath, '--c 50', args)

    # Run Trace/Replay on old trace files if directory specified
    directory = args.OldTracesPath
    if os.path.isdir(directory):
        for filename in os.listdir(directory):
            if filename.endswith(".vktrace"):
                TraceReplayTraceTest(filename, os.path.join(directory, filename), args)


    sys.exit(0)
