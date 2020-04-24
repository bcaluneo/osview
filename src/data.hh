// Copyright (C) Brendan Caluneo

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "graph.hh"

#ifndef DATA_HH
#define DATA_HH

#define percent(a, b) a*100 / b

extern bool quit;
extern const size_t POLL_TIME;

void computeTime(FILETIME &result, const FILETIME &start, const FILETIME &end) {
	ULARGE_INTEGER largeStart = { 0 };
	largeStart.LowPart = start.dwLowDateTime;
	ULARGE_INTEGER largeEnd = { 0 };
	largeEnd.LowPart = end.dwLowDateTime;
	largeStart.QuadPart = largeEnd.QuadPart - largeStart.QuadPart;
	result.dwLowDateTime = largeStart.LowPart;
}

int getData(void *data) {

	std::ofstream log;
	log.open("logfile.txt", std::ios::trunc);
	log << "CPU:\tIdl:\tKer:\tUsr:\tMem %:" << "\n";
	log << std::fixed << std::setprecision(2);

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

		double band[3] = { percent(userTime, total), percent(actKernel, total),
											 percent(idleTime, total) };

		double band0[2] = { double(memStatus.dwMemoryLoad),
												100-double(memStatus.dwMemoryLoad) };

		g[0].updateSize(0, percent(userTime, total));
		g[0].updateSize(1, percent(actKernel, total));
		g[0].updateSize(2, percent(idleTime, total));
		g[0].insertBand(band);

		g[1].updateSize(0, double(memStatus.dwMemoryLoad));
		g[1].updateSize(1, 100-double(memStatus.dwMemoryLoad));
		g[1].insertBand(band0);

		log << cpu                            << "\t" <<
		       percent(idleTime, total)       << "\t" <<
					 percent(actKernel, total)      << "\t" <<
					 percent(userTime, total)       << "\t" <<
					 double(memStatus.dwMemoryLoad) << "\n";
	}

	log.close();

	return 0;
}

#endif
