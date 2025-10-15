#!/bin/python3
import subprocess
import sys
import os
import datetime
import signal
import time

# Config
TEST_DIR = "test"
LOG_DIR = "logs"
END_PATTERN = "Returned from app_main()"

def run_qemu_tests():
    os.makedirs(LOG_DIR, exist_ok=True)
    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    log_file_path = os.path.join(LOG_DIR, f"runlog_{timestamp}.txt")

    if not os.path.isdir(TEST_DIR):
        print(f"Directory '{TEST_DIR}' not found.")
        return 1

    cmd = ["idf.py", "qemu"]
    print(f" Running QEMU Unity tests in: {TEST_DIR}")
    print(f" Logging output to: {log_file_path}")

    with open(log_file_path, "w") as log_file:
        proc = subprocess.Popen(
            cmd,
            cwd=TEST_DIR,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            bufsize=1,
            universal_newlines=True,
            preexec_fn=os.setsid  # So we can kill the whole group
        )

        exit_code = None
        try:
            for line in proc.stdout:
                print(line, end="")
                log_file.write(line)
                log_file.flush()

                if "OK" in line:
                    exit_code = 0
                
                if "FAIL" in line:
                    exit_code = 1

                if END_PATTERN in line:
                    print(f"Detected test end pattern: '{END_PATTERN}'. Terminating QEMU.")
                    os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
                    break

            proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            print(" Timeout waiting for QEMU to close. Forcing shutdown.")
            os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
            exit_code = 1
        except Exception as e:
            print(f"Exception occurred: {e}")
            os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
            exit_code = 1

    print(f"\n Script exiting with status: {exit_code}")
    return exit_code

if __name__ == "__main__":
    sys.exit(run_qemu_tests())
