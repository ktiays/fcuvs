//
//  Created by ktiays on 2024/10/18.
//  Copyright (c) 2024 ktiays. All rights reserved.
//

#include <Carbon/Carbon.h>
#include <chrono>
#include <iostream>
#include <libproc.h>
#include <thread>
#include <unistd.h>
#include <vector>

extern "C" int _CGSDefaultConnection(void);
extern "C" bool CGSEventIsAppUnresponsive(int connection, const ProcessSerialNumber *psn);

std::string proc_path(pid_t pid) {
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
    int ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
    if (ret <= 0) {
        return "";
    }

    return std::string(pathbuf);
}

std::vector<pid_t> get_pids_by_name(const std::string &name) {
    std::vector<pid_t> pids;
    int proc_count = proc_listpids(PROC_ALL_PIDS, 0, nullptr, 0);
    if (proc_count <= 0) {
        std::cerr << "Error getting process count" << std::endl;
        return pids;
    }

    std::vector<int> pids_buf(proc_count);
    proc_listpids(PROC_ALL_PIDS, 0, pids_buf.data(), proc_count * sizeof(int));

    for (int i = 0; i < proc_count; i++) {
        pid_t pid = pids_buf[i];
        auto path = proc_path(pid);
        if (path.empty()) {
            continue;
        }

        if (path.find(name) != std::string::npos) {
            pids.push_back(pid);
        }
    }

    return pids;
}

bool is_process_hanging(pid_t pid) {
    ProcessSerialNumber psn;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    if (GetProcessForPID(pid, &psn)) {
        return false;
    }
#pragma clang diagnostic pop
    return CGSEventIsAppUnresponsive(_CGSDefaultConnection(), &psn);
}

int main(int argc, char *argv[]) {
    const char *const target_name = "CursorUIViewService";
    pid_t pid = -1;
    while (true) {
        bool target_needs_update = false;
        if (pid == -1) {
            target_needs_update = true;
        } else {
            auto path = proc_path(pid);
            if (path.empty() || path.find(target_name) == std::string::npos) {
                target_needs_update = true;
            }
        }

        if (target_needs_update) {
            std::cout << "Searching for process " << target_name << std::endl;
            auto pids = get_pids_by_name(target_name);
            if (pids.empty()) {
                continue;
            }
            pid = pids.front();
            std::cout << "Found process " << pid << std::endl;
        }

        if (is_process_hanging(pid)) {
            std::cout << "Process is hanging, killing it" << std::endl;
            kill(pid, SIGKILL);
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}