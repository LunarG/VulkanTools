# Python script for running the regression test for both trace and replay. Python version 3 or greater required.
# This script operates by iterating through old traces in a directory specified by the user, tracing a replay of the old trace, and replaying the new trace.
# NOTE - This is normally invoked from _vktracereplay.ps1 or vktracereplay.sh.
# To run this test:
#    cd <this-dir>
#    python ./vktracereplay.py <old-trace-directory> <vktrace-exe> <vktrace-layer-path> <vkreplay-exe>
#
#    <old-trace-directory> example: "C:\traces\" would result in the script testing against "C:\traces\trace1.vktrace", "C:\traces\trace2.vktrace", etc.

import os, sys, subprocess, time, argparse, filecmp

def HandleError(msg):
    print (msg)
    sys.exit(1)

def GetErrorMessage(out):
    matched_lines = [line for line in out.split('\n') if 'error' in line]
    return '\n'.join(matched_lines)

def TraceReplayTest(testname, traceFile, args):
    print ('Beginning Test: %s\n' % testname)

    # Parse frame
    if not os.path.exists('%s.config' % traceFile):
        frame = '1'
    else:
        with open('%s.config' % traceFile) as configFile:
            frame = configFile.read().strip()
    
    # Trace replay of <traceFile>
    traceEnv = os.environ.copy()
    traceEnv['VK_LAYER_PATH'] = args.VkLayerPath
    out = subprocess.check_output([args.VkTracePath, '-o', '%s.vktrace' % testname, '-p', args.VkReplayPath, '-a', '-o %s' % traceFile, '-s', frame, '-w', '.'], env=traceEnv).decode('utf-8')

    if 'error' in out:
        p.kill()
        err = GetErrorMessage(out)
        HandleError('Errors while tracing replay of original trace:\n%s' % err)

    # Rename 1.ppm to <testname>.trace.ppm
    if os.path.exists('%s.ppm' % frame):
        os.rename('%s.ppm' % frame, '%s.trace.ppm' % testname)
    else:
        HandleError('Error: Screenshot not taken while tracing.')

    out = subprocess.check_output([args.VkReplayPath, '-o', '%s.vktrace' % testname, '-s', frame]).decode('utf-8')

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

    print ('Success\n')

# Load settings from command-line
parser = argparse.ArgumentParser(description='Test vktrace and vkreplay.')
parser.add_argument('OldTracesPath', help='directory of old traces to replay')
parser.add_argument('VkTracePath', help='directory containing vktrace')
parser.add_argument('VkLayerPath', help='directory containing vktrace layer')
parser.add_argument('VkReplayPath', help='directory containing vkreplay')
args = parser.parse_args()

# Run Trace/Replay on old trace files
directory = args.OldTracesPath
for filename in os.listdir(directory):
    if filename.endswith(".vktrace"):
        TraceReplayTest(filename, os.path.join(directory, filename), args)

sys.exit(0);