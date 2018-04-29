#!/usr/bin/env python3
# coding: utf-8

if __name__ == "__main__":
    pid=-1
    system="unknown"

    first_row = False
    print("{")
    with open("profile.prof") as f:
        mode = 0
        for line in f:
            if mode is 0:
                if line.startswith("session events:"):
                    mode = 1
                elif line.startswith("pid:"):
                    pid = line[8:-1]
                elif line.startswith("system:"):
                    system = line[8:-1]
            elif mode is 1:
                print("\"traceEvents\": [")
                mode = 2
                first_row = True
            else:
                thread_id, time_point, type, name = line.split()
                thread_id = int(thread_id, 16)

                if not first_row:
                    print(",", end="")
                else:
                    first_row = False

                print("{ \"name\": \"" + name + "\"", end="")
                print(",\"pid\":" + pid, end='')
                print(",\"tid\":" + str(thread_id), end='')
                print(",\"ts\":" + time_point, end='')
                print(",\"ph\":", end='')
                if type == "S":
                    print("\"B\"", end='')
                elif type == "E":
                    print("\"E\"", end='')
                elif type == "I":
                    print("\"P\"", end='')
                print("}")

    # Ends the traceEvents block
    if mode == 2:
        print("],")

    # Write metadatas
    print("\"displayTimeUnit\":\"ns\",")
    print("\"meta_system\": \"" + system + "\"")
    print("}")