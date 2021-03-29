// Copyright (C) Brendan Caluneo

// Header Name:
// - Data
// Description:
// - Polls the system to retrieve data to feed on-screen bar graphs.

#include <cstdint>
#include "graph.hh"
#include "util.hh"

#ifndef DATA_HH
#define DATA_HH

extern bool quit;

#define percent(a, b) (a*100/b)

void computeTime(FILETIME &result, const FILETIME &start, const FILETIME &end) {
	ULARGE_INTEGER largeStart = { 0 };
	largeStart.LowPart = start.dwLowDateTime;
	ULARGE_INTEGER largeEnd = { 0 };
	largeEnd.LowPart = end.dwLowDateTime;
	largeStart.QuadPart = largeEnd.QuadPart - largeStart.QuadPart;
	result.dwLowDateTime = largeStart.LowPart;
}

int getData(void *data) {
	Graph *g = static_cast<Graph*>(data);

	while (!quit) {
		FILETIME a0, a1, a2, b0, b1, b2;
		GetSystemTimes(&a0, &a1, &a2);
		SDL_Delay(POLL_TIME);
		GetSystemTimes(&b0, &b1, &b2);

		MEMORYSTATUSEX memStatus;
		memStatus.dwLength = sizeof (memStatus);
		GlobalMemoryStatusEx(&memStatus);

		FILETIME finalIdle, finalKernel, finalUser;

		computeTime(finalIdle, a0, b0);
		computeTime(finalKernel, a1, b1);
		computeTime(finalUser, a2, b2);

		double idleTime, kernelTime, userTime;

		idleTime = finalIdle.dwLowDateTime;
		kernelTime = finalKernel.dwLowDateTime;
		userTime = finalUser.dwLowDateTime;

		double total = (kernelTime+userTime); // ker + idl + usr
		// double cpu = (kernelTime+userTime-idleTime)*100 / total;
		double actKernel = kernelTime - idleTime;

		g[0].setData(0, percent(userTime, total));
		g[0].setData(1, percent(actKernel, total));
		g[0].setData(2, percent(idleTime, total));
		g[0].insertBand({ percent(userTime, total),
			                percent(actKernel, total),
									    percent(idleTime, total)
										});

		g[1].setData(0, double(memStatus.dwMemoryLoad));
		g[1].setData(1, 100-double(memStatus.dwMemoryLoad));
		g[1].insertBand({ double(memStatus.dwMemoryLoad),
										  100-double(memStatus.dwMemoryLoad)
										});
	}

	return 0;
}

#endif
