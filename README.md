# osview

What is "osview"? Back in the days of Silicon Graphics's supremacy over
the performance computing market, their operating system, IRIX, featured
a graphical system monitoring tool called "gr_osview". A screenshot of this
tool is below: <br/>
![Screenshot of gr_osview](screenshots/gr_osview.gif)
<br/>

## Modern Alternative

Since the days of IRIX being usable in the PC market are long gone, I have
sought a way to recreate this program for years. Often I have come close, but
before recently I was not able to recreate the proper look and feel of this tool.
Here are some screenshots of my tool as it stands: <br/>
![Screenshot of osview](screenshots/prev.png)
<br/>
![Screenshot of osview](screenshots/prev2.png)
<br/>
This tool uses the win32 api library in order to measure system resource
usage time over a period defined in the main program. Specifically, it measures the amount of time the system has been executing tasks in kernel mode, user mode, and the amount of time that it has spent idling. More information can be found in the Windows Dev Center; specifically, [here](https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getsystemtimes). All the rendering is handled using the SDL2 library.
Because this program uses the win32 api it is only compatible with Windows.

## Usage

This tool is not meant to be used as an actual resource monitoring tool. The information it uses more than likely will not line up with the usage percent shown in Task Manager or other applications like it due to the differences between their usage statistic/timing and mine.
