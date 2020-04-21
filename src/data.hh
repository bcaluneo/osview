#include <cstdint>
#include <cstdio>
#include <tuple>
#include <vector>
#include "graph.hh"

#ifndef DATA_HH
#define DATA_HH

#define percent(a, b) a*100 / b

extern bool quit;
extern const int POLL_TIME, BAND_WIDTH, BAR_WIDTH;
extern std::vector<std::tuple<double,double,double>> bands;
size_t totalBands = BAR_WIDTH / BAND_WIDTH;

void computeTime(FILETIME &result, const FILETIME &start, const FILETIME &end) {
	ULARGE_INTEGER largeStart = { 0 };
	largeStart.LowPart = start.dwLowDateTime;
	ULARGE_INTEGER largeEnd = { 0 };
	largeEnd.LowPart = end.dwLowDateTime;
	largeStart.QuadPart = largeEnd.QuadPart - largeStart.QuadPart;
	result.dwLowDateTime = largeStart.LowPart;
}

int getData(void *data) {
	printf("CPU:\tIdl:\tKer:\tUsr:\tMem %%:\n");
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
		double cpu = (kernelTime+userTime-idleTime)*100 / total;

		double actKernel = kernelTime - idleTime;

		double band[3] = {percent(userTime, total), percent(actKernel, total),
											percent(idleTime, total)};

		g[0].updateSize(0, percent(userTime, total));
		g[0].updateSize(1, percent(actKernel, total));
		g[0].updateSize(2, percent(idleTime, total));
		g[0].insertBand(band);
		
		g[1].updateSize(0, double(memStatus.dwMemoryLoad));
		g[1].updateSize(1, 100-double(memStatus.dwMemoryLoad));

		// printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\r", cpu, percent(idleTime, total),
		// 																				percent(actKernel, total),
		// 																				percent(userTime, total),
		// 																				double(memStatus.dwMemoryLoad));
	}

	return 0;
}

#endif
