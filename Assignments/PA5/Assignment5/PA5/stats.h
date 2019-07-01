//
//  stats.h
//  Assignment5
//
//  Created by castle on 3/20/19.
//  Copyright © 2019 castle. All rights reserved.
//

#ifndef stats_h
#define stats_h

#include <time.h>
#include <mach/message.h>
#include <mach/mach.h>

static clock_t start_t, end_t;
static double turnaround_t;

void PrintStats(void);
void StartClock(void);
void StopClock(void);
void MacCPUTime(void);
void GetTurnaround(void);
void GetThroughput(void);

/*
 If you are not on a Mac, this is what you would see:
 Jobs:
 <0,11>    <2,20>    <5,34>     <5,40>      <9,31>
 <12,7>    <12,4>    <14,25>    <16,11>     <18,14>
 <18,11>
 First-Come First-Serve
 CPU usage..........: 89.573324%
 Throughput.........: 51401.869159 pps
 Turnaround.........: 0.000214 s
 
 Round Robin
 CPU usage..........: 89.573248%
 Throughput.........: 423076.923077 pps
 Turnaround.........: 0.000026 s
 */
void PrintStats()
{
    // (1) the CPU utilization
    MacCPUTime();
    // (2) the average job throughput per second
    GetThroughput();
    // (3) the average job turnaround time
    GetTurnaround();
    printf("\n");
}

void StartClock()
{
    start_t = clock();
}
void StopClock()
{
    end_t = clock();
    turnaround_t = ((double)(end_t - start_t)) / CLOCKS_PER_SEC;
}
void MacCPUTime()
{
    // Mac OS X Specific way to get CPU usage.
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    host_cpu_load_info_data_t r_load;
    kern_return_t kr;
    uint64_t totalSystemTime = 0, totalUserTime = 0, totalIdleTime = 0, total = 0;
    double onePercent;

    if ((kr = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (int *)&r_load, &count)) != KERN_SUCCESS)
        Error(mach_error_string(kr), 1);

    totalSystemTime = r_load.cpu_ticks[CPU_STATE_SYSTEM];
    totalUserTime = r_load.cpu_ticks[CPU_STATE_USER] + r_load.cpu_ticks[CPU_STATE_NICE];
    totalIdleTime = r_load.cpu_ticks[CPU_STATE_IDLE];
    total = totalSystemTime + totalUserTime + totalIdleTime;
    onePercent = total / 100.0f;

    printf("CPU usage..........: %f%%\n", (total - totalIdleTime) / onePercent);
}
// the number of processes that are completed per time unit
void GetThroughput()
{
    printf("Throughput.........: %f pps\n", ((double)queue.length / turnaround_t));
}
// The interval from the time of submission of a process to the time of completion
void GetTurnaround()
{
    printf("Turnaround.........: %f s\n", turnaround_t);
}
#endif /* stats_h */
