# Python script for running the regression test for both trace and replay. Python version 3 or greater required.
# This script operates by iterating through old traces in a directory specified by the user, tracing a replay of the old trace, and replaying the new trace.
# NOTE - This is normally invoked from _vktracereplay.ps1 or vktracereplay.sh.
# To run this test:
#    cd <this-dir>
#    python ./vktracereplay.py <old-trace-directory> <vktrace-exe> <vktrace-layer-path> <vkreplay-exe>
#
#    <old-trace-directory> example: "C:\traces\" would result in the script testing against "C:\traces\trace1.vktrace", "C:\traces\trace2.vktrace", etc.

import os, sys, subprocess, time, argparse

def GetErrorMessage(out):
    matched_lines = [line for line in out.split('\n') if 'error' in line]
    return '\n'.join(matched_lines)

def Replay(testname, traceFile, args):
    replayEnv = os.environ.copy()
    replayEnv['VK_INSTANCE_LAYERS'] = 'VK_LAYER_LUNARG_vktrace'
    replayEnv['VK_LAYER_PATH'] = args.VkLayerPath

    out = subprocess.check_output([args.VkReplayPath, '-o', traceFile], env=replayEnv).decode('utf-8')
    return out

def TraceReplayTest(testname, filename, args):
    print ('Beginning Test: %s\n' % testname)
    
    p = subprocess.Popen([args.VkTracePath, '-o', '%s.vktrace' % testname], stdout=subprocess.PIPE)
    time.sleep(1)

    out = Replay(testname, filename, args)

    if 'error' in out:
        err = GetErrorMessage(out)
        print ('Errors while replaying original trace:\n')
        print ('%s\n' % err)
        p.kill()
        sys.exit(1)

    # Wait for vktrace to process and finish it
    time.sleep(3)
    p.kill()
    out = p.communicate()[0].decode('utf-8')
    time.sleep(1)

    # Check stdout for errors
    if 'error' in out:
        err = GetErrorMessage(out)
        print ('Error while tracing:\n')
        print ('%s\n' % err)
        sys.exit(1)

    out = subprocess.check_output([args.VkReplayPath, '-o', '%s.vktrace' % testname]).decode('utf-8')

    if 'error' in out:
        err = GetErrorMessage(out)
        print ('Error while replaying:\n')
        print ('%s\n' % err)
        sys.exit(1)

    # Remove trace file
    try:
        os.remove('./%s.vktrace' % testname)
    except OSError:
        pass

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