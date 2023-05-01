// Copyright (C) Brendan Caluneo

// Header Name:
// - Data
// Description:
// - Polls the system to retrieve data to feed on-screen bar graphs.

#include <cstdint>
#include <memory>
#include "graph.hh"
#include "util.hh"
#include "sysinfoapi.h"
#include <winternl.h>
#include <chrono>

#ifndef DATA_HH
#define DATA_HH

extern bool quit;
extern std::vector<Graph> graphs;
extern std::atomic_int coreCount;

#define percent(a, b) (a*100/b)

int getData(void *data) {
	while (!quit) {
		MEMORYSTATUSEX memStatus;
		memStatus.dwLength = sizeof (memStatus);
		GlobalMemoryStatusEx(&memStatus);

		std::unique_ptr<SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION> processorDataThen(new SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION[coreCount.load()]);
		std::unique_ptr<SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION> processorDataNow(new SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION[coreCount.load()]);

		NtQuerySystemInformation(SystemProcessorPerformanceInformation, processorDataThen.get(), sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * coreCount.load(), NULL);
		SDL_Delay(POLL_TIME);
		NtQuerySystemInformation(SystemProcessorPerformanceInformation, processorDataNow.get(), sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * coreCount.load(), NULL);

		for (size_t i = 0; i < graphs.size(); ++i) {
			Graph &g = graphs[i];
			if ("Memory Usage" == g.getTitle()) {
				g.setData("inuse", double(memStatus.dwMemoryLoad));
				g.setData("free", 100-double(memStatus.dwMemoryLoad));
				g.pushBand({ double(memStatus.dwMemoryLoad),
												100-double(memStatus.dwMemoryLoad)
												});
			} else {
				// This assumes all the graphs are in the correct order.
				auto processorInformationThen = processorDataThen.get()[i];
				auto processorInformationNow = processorDataNow.get()[i];

				auto idleTime = processorInformationNow.IdleTime.QuadPart - processorInformationThen.IdleTime.QuadPart;
				auto kernelTime = processorInformationNow.KernelTime.QuadPart - processorInformationThen.KernelTime.QuadPart;
				auto userTime = processorInformationNow.UserTime.QuadPart - processorInformationThen.UserTime.QuadPart;
				auto actKernelTime = kernelTime - idleTime;
				double total = (kernelTime+userTime); // ker + idl + usr

				g.setData("usr", percent(userTime, total));
				g.setData("sys", percent(actKernelTime, total));
				g.setData("idl", percent(idleTime, total));
				g.pushBand({ 
					percent(userTime, total),
					percent(actKernelTime, total),
					percent(idleTime, total)
				});
			}
		}
	}

	return 0;
}

#endif
