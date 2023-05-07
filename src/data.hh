// Copyright (C) Brendan Caluneo

// Header Name:
// - Data
// Description:
// - Polls the system to retrieve data to feed on-screen bar graphs.

#include "graph.hh"
#include "util.hh"
#include <winternl.h>
#include <memory>
#include <cmath>
#include <array>

#ifndef DATA_HH
#define DATA_HH

extern bool quit;
extern std::vector<Graph> graphs;
extern std::atomic_int coreCount;

#define percent(a, b) (a*100.0/b)

template<size_t N>
void transformData(std::array<double, N> &input, signed value) {
	signed sum = 0;
	for (double &data : input) {
		data *= value;
		data /= 100;
		sum += signed(data);
	}

	for (double &data : input) {
		if (sum != value) {
			double dec = data - signed(data);
			signed toAdd = signed(std::ceil(dec));
			if (toAdd > 0) {
				data += toAdd;
				sum += toAdd;
			} 
		}
	}
}

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
				std::array<double, 2> horizontalDataArray { double(memStatus.dwMemoryLoad), 100-double(memStatus.dwMemoryLoad) };
				std::array<double, 2> verticalDataArray { double(memStatus.dwMemoryLoad), 100-double(memStatus.dwMemoryLoad) };
				transformData(horizontalDataArray, BAR_WIDTH);
				transformData(verticalDataArray, BAR_HEIGHT);

				g.setData(MEM_INUSE_STRING, signed(horizontalDataArray[0]));
				g.setData(MEM_FREE_STRING, signed(horizontalDataArray[1]));
				g.addBand({ 
					{ MEM_INUSE_STRING, signed(verticalDataArray[0]) },
					{ MEM_FREE_STRING, signed(verticalDataArray[1]) }
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

				std::array<double, 3> horizontalDataArray { percent(userTime, total), percent(actKernelTime, total), percent(idleTime, total) };
				std::array<double, 3> verticalDataArray { percent(userTime, total), percent(actKernelTime, total), percent(idleTime, total) };
				transformData(horizontalDataArray, BAR_WIDTH);
				transformData(verticalDataArray, BAR_HEIGHT);

				// PLOG_INFO << horizontalDataArray << ", " << verticalDataArray;

				// Calculate percentage of the bar width and bar height here
				// Then in the render function just use the data instead of converting it there.

				g.setData(CPU_USER_STRING, signed(horizontalDataArray[0]));
				g.setData(CPU_KERNEL_STRING, signed(horizontalDataArray[1]));
				g.setData(CPU_IDLE_STRING, signed(horizontalDataArray[2]));
				g.addBand({ 
					{ CPU_USER_STRING, signed(verticalDataArray[0]) },
					{ CPU_KERNEL_STRING, signed(verticalDataArray[1]) },
					{ CPU_IDLE_STRING, signed(verticalDataArray[2]) }
				});
			}
		}
	}

	return 0;
}

#endif
