#include <cstdint>
#include <cstdio>

#ifndef TIME_HH
#define TIME_HH

extern bool quit;
extern const int POLL_TIME;

void GetFileTime(FILETIME &result, const FILETIME &start, const FILETIME &end) {
	ULARGE_INTEGER largeStart = { 0 };
	largeStart.LowPart = start.dwLowDateTime;
	largeStart.HighPart = start.dwHighDateTime;
	ULARGE_INTEGER largeEnd = { 0 };
	largeEnd.LowPart = end.dwLowDateTime;
	largeEnd.HighPart = end.dwHighDateTime;
	largeStart.QuadPart = largeEnd.QuadPart - largeStart.QuadPart;
	result.dwLowDateTime = largeStart.LowPart;
	result.dwHighDateTime = largeStart.HighPart;
}

double percent(double a, double b) {
	return a*100 / b;
}

int getData(void *data) {
	printf("CPU:\tIdl:\tKer:\tUsr:\n");
	double *ret = static_cast<double*>(data);
	while (!quit) {
		FILETIME a0, a1, a2, b0, b1, b2;
		GetSystemTimes(&a0, &a1, &a2);
		SDL_Delay(POLL_TIME);
		GetSystemTimes(&b0, &b1, &b2);

		FILETIME finalIdle, finalKernel, finalUser;

		GetFileTime(finalIdle, a0, b0);
		GetFileTime(finalKernel, a1, b1);
		GetFileTime(finalUser, a2, b2);

		double idleTime, kernelTime, userTime;

		idleTime = finalIdle.dwLowDateTime;
		kernelTime = finalKernel.dwLowDateTime;
		userTime = finalUser.dwLowDateTime;

		double total = (kernelTime+userTime); // ker + idl + usr
		double cpu = (kernelTime+userTime-idleTime)*100 / total;

		ret[0] = percent(idleTime, total);
		ret[1] = percent(kernelTime - idleTime, total);
		ret[2] = percent(userTime, total);

		printf("%.2f\t%.2f\t%.2f\t%.2f\r", cpu, percent(idleTime, total),
																						percent(kernelTime-idleTime, total),
																						percent(userTime, total));
	}

	return 0;
}

#endif
