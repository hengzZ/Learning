#!/bin/python3

import os
import sys
import argparse


def usage():
    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--core", help="set specific frequency")
    parser.add_argument("-m", "--min", help="set min frequency")
    parser.add_argument("-a", "--max", help="set max frequency")
    parser.add_argument("-f", "--freq", help="set specific frequency")
    parser.add_argument("-r", "--reset", help="reset all core frequency to default setting", action="store_true")
    args = parser.parse_args()
    return args


def check_kernel_user():
    uname_r_cmd = "uname -r"
    kernel_version = os.popen(uname_r_cmd).read()
    if kernel_version is None:
        print("Run `%s` command failed.")
        exit(-1)
    print("kernel Version: %s" % kernel_version)
    kernel_version_list = kernel_version.split(".")
    Mnum = kernel_version_list[0]
    Snum = kernel_version_list[1]
    if (Mnum > 4) or (Mnum == 4 and Snum < 11):
        print("Check Kernel version: OK!")
    else:
        print("Check Kernel Version: too low, can't be configured! Configure is aborted...")
        exit(-1)

    username = os.popen("whoami").read()
    if username == "root":
        print("Check Permission: root user, OK!")
    else:
        print("Check Permission: not root user, permission denied.")
        sys.exit(-1)


def reset():
    hd_min_freq_cmd = "cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq"
    hd_min_freq = convert_to_hex(os.popen(hd_min_freq_cmd).read())
    hd_max_freq_cmd = "cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"
    hd_max_freq = convert_to_hex(os.popen(hd_max_freq_cmd).read())
    
    print("Rest all cores to max %s min: %s" %(hd_max_freq,hd_min_freq))
    write_core_freq_script = "./wrmsr -a  0x774 " + "0x8000%s%s" % (str(hd_max_freq), str(hd_min_freq))
    os.popen(write_core_freq_script)

def disable_C1E_C3_C6():
    """disable C1E, C3, C6"""
    os.popen("cpupower idle-set -e 0")
    os.popen("cpupower idle-set -e 1")
    os.popen("cpupower idle-set -e 2")
    os.popen("cpupower idle-set -e 3")


def get_cores(cores):
    """get user specific core"""
    cores_list = []
    if not cores:
        # 0-95
        # print("set all cores.")
        lscpu_cmd = "lscpu | grep 'On-line CPU(s) list:'"
        online_cores = os.popen(lscpu_cmd).read().split(":")[-1]
        # print(online_cores)
        online_cores_list = online_cores.split("-")
        for i in range(int(online_cores_list[0]), int(online_cores_list[-1])+1):
            cores_list.append(str(i))
        return cores_list
    if "," not in cores and "-" not in cores:
        # 0 1 2 3
        cores_list = cores.split()
        return cores_list

    if "," in cores and "-" not in cores:
        # 0,1,2,3,4
        cores_list = cores.split(",")
    if "-" in cores and "," not in cores:
        # 0-23
        tmp_cores = cores.split("-")
        for i in range(int(tmp_cores[0]), int(tmp_cores[-1])+1):
            cores_list.append(str(i))

    if "," in cores and "-" in cores:
        # 0-23,48-71
        tmp_cores = cores.split(",")
        cores_part1 = tmp_cores[0]
        cores_part2 = tmp_cores[-1]
        cores_part1_list = cores_part1.split("-")
        for i in range(int(cores_part1_list[0]), int(cores_part1_list[-1])+1):
            cores_list.append(str(i))
        cores_part2_list = cores_part2.split("-")
        for i in range(int(cores_part2_list[0]), int(cores_part2_list[-1])+1):
            cores_list.append(str(i))
    # print(cores_list)
    return cores_list


def convert_to_hex(freq):
    base = 100000
    freq = int(freq)
    ret = str(hex(freq//base)).split("x")[-1]

    if len(ret) == 1:
        return ret.zfill(2)
    return ret


#  -a|--max
def set_core_max_freq(allCores, maxFreq):
    # maxFreq = "100"
    maxFreq = convert_to_hex(maxFreq)
    # print("maxFreq : ", maxFreq)
    for core in allCores:
        read_core_freq_script = "./rdmsr -p " + str(core) + " 0x770 -f 0:0"
        status = os.popen(read_core_freq_script).read().strip()
        # print("status: ", status)
        if status == "1":
            print("Core", core, ":HWP is enabled")
            print("Set max: ", maxFreq)

            orimsr_script = "./rdmsr -p " + str(core) + " 0x774"
            orimsr = os.popen(orimsr_script).read()
            # print("orimsr: ",orimsr)
            desmsr_script = "printf \"%#x\n\" $(( 0x" + str(orimsr) + " & 0xffff00ff ))"
            desmsr = os.popen(desmsr_script).read()
            desmsr_script = "printf \"%#x\n\" $(( " + str(desmsr) + " | 0x0000" + str(maxFreq) + "00 ))"
            desmsr = os.popen(desmsr_script).read()
            # print("desmsr: ",desmsr)

            write_core_freq_script = "./wrmsr -p " + str(core) + " 0x774 " + str(desmsr)
            os.popen(write_core_freq_script).read()

            final_script = "./rdmsr -p " + str(core) + " 0x770 -f 0:0"
            final = os.popen(final_script).read()
            print("Core ", core, " max feequency is set to",maxFreq, "KHZ, msr : ", final)

        else:
            print("Core[", core, "]:HWP is Disabled")


# -m|--min
def set_cores_min_freq(all_cores, min_freq):
    min_freq = convert_to_hex(min_freq)
    # print("min_freq : ", min_freq)
    for core in all_cores:
        read_core_freq_script = "./rdmsr -p " + str(core) + " 0x770 -f 0:0"
        status = os.popen(read_core_freq_script).read().strip()
        # print("status:  ", status)
        if status == "1":
            print("Core[", core, "]:HWP is enabled")
            print("Set min:", min_freq)
            orimsr_script = "./rdmsr -p " + str(core) + " 0x774"
            orimsr = os.popen(orimsr_script).read()

            desmsr_script = "printf \"%#x\n\" $(( 0x" + str(orimsr) + " & 0xffffff00 ))"
            desmsr = os.popen(desmsr_script).read()
            desmsr_script = "printf \"%#x\n\" $(( " + str(desmsr) + " | 0x000000" + str(min_freq) + "))"
            desmsr = os.popen(desmsr_script).read()
            # print(desmsr)

            write_core_freq_script = "./wrmsr -p " + str(core) + " 0x774 " + str(desmsr)
            os.popen(write_core_freq_script).read()

            final_script = "./rdmsr -p " + str(core) + " 0x774"
            final = os.popen(final_script).read()
            print("Core ", core, " mix frequency is set to", min_freq, "KHZ, msr : ", final)

        else:
            print("Core[", core, "]:HWP is Disabled")


def set_cores_desire_freq(all_cores, desire_freq):
    hd_min_freq_cmd = "cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq"
    hd_min_freq = convert_to_hex(os.popen(hd_min_freq_cmd).read())
    hd_max_freq_cmd = "cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"
    hd_max_freq = convert_to_hex(os.popen(hd_max_freq_cmd).read())
    desire_freq = convert_to_hex(desire_freq)
    # print("desire_freq : ", desire_freq)
    for core in all_cores:
        read_core_freq_script = "./rdmsr -p " + str(core) + " 0x770 -f 0:0"
        status = os.popen(read_core_freq_script).read().strip()
        # print("status:  ", status)
        if status == "1":
            print("Core[", core, "]:HWP is enabled")
            print("high: %s min: %s desire: %s" %(hd_max_freq, hd_min_freq, desire_freq))

            write_core_freq_script = "./wrmsr -p " + str(core) + " 0x774 " + "0x80%s%s%s" % (str(desire_freq), str(hd_max_freq), str(hd_min_freq))
            os.popen(write_core_freq_script).read()

            final_script = "./rdmsr -p " + str(core) + " 0x774"
            final = os.popen(final_script).read()
            print("Core ", core, " are be  set to", desire_freq, "KHZ, msr : ", final)

        else:
            print("Core[", core, "]:HWP is Disabled")

def fetch_vender():
    log_file = "temp.info"
    cpu_cmd = "lscpu > {}".format(log_file)
    os.system(cpu_cmd)
    with open(log_file, 'r') as f:
        for line in f:
            if "Model name:".upper() in line.strip().upper():
                vender = "unknow"
                if "Intel(R)".upper() in line.strip().upper():
                    vender = "Intel"
                elif "AMD".upper() in line.strip().upper():
                    vender = "AMD"
        os.remove(log_file)
    return vender 


def run_intel(args):
    r = args.reset
    # print(r)
    if r:
        reset()
        disable_C1E_C3_C6()
        sys.exit(0)

    core = args.core
    min_freq = args.min
    max_freq = args.max
    freq = args.freq
    print("min_freq: %s, max_freq: %s, desire_freq: %s" %(min_freq, max_freq, freq))
    cores = get_cores(core)
    print("changed cores:%s" % cores)
    if max_freq:
        set_core_max_freq(cores, max_freq)
    elif min_freq:
        set_cores_min_freq(cores, min_freq)
    elif freq:
        set_cores_desire_freq(cores, freq)
    else:
        print("please input the user specific frequency in KHZ!")

def run_amd(args):
    cores = ",".join(args.core)
    print("Changed cores :%s" % cores)
    min_freq = str(args.min)
    max_freq = str(args.max)
    if max_freq and min_freq:
        cpupower_cmd = "cpupower -c %s frequency-set -d %s -u %s -g performance" % (cores, max_freq, min_freq)
    elif min_freq:
        cpupower_cmd = "cpupower -c %s frequency-set -u %s -g performance" % (cores, min_freq)
    elif max_freq:
        cpupower_cmd = "cpupower -c %s frequency-set -d %s -g performance" % (cores, max_freq)
    else:
        print("please input the user specific frequency in KHZ!")
        sys.exit(-1)
    ret = os.popen(cpupower_cmd).read()


def main():
    vender = fetch_vender()
    args = usage()
    if vender.lower() == "intel":
        run_intel(args)
    elif vender.lower() == "amd":
        run_amd(args)
    else:
        print("unkonw platform.")
        sys.exit(-1)


if __name__ == "__main__":
    main()
