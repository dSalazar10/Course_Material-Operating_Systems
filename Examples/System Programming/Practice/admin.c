//
//  admin.c
//  programs
//
//  Created by DANIEL SALAZAR on 4/7/18.
//
// Check out /usr/include/sys/proc_info.h


#include <stdio.h>
#include <unistd.h>         /* read, close */
#include <fcntl.h>          /* open */
#include <arpa/inet.h>      /* inet_ntoa */
#include <stdlib.h>         /* malloc */
#include <string.h>         /* strlen */
#include <utmpx.h>          /* getutxent */
#include <sys/time.h>       /* gettimeofday */
#include <sys/utsname.h>    /* uname */
#include <termios.h>        /* termios */
#include <sys/sysctl.h>     /* sysctl */
#include <mach/mach_host.h> /* vm_stats */
#include <mach/mach_error.h>/* mach_error_string */
#include <sys/statvfs.h>    /* statvfs */
/* getpwnam_r */
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
/* getifaddrs */
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>

const unsigned int MB = 1024 * 1024, GB = MB * 1024;

/* User Count */
#define MAX_USRS 25
struct array
{
    char *users[MAX_USRS];
    int len;
};
// -1 = no insert
// 0 = insert
// 1 = duplicate
int insert(struct array *a, char source[])
{
    // empty array
    if(a->len == 0)
    {
        a->users[0] = source;
        a->len++;
        return 0;
    }
    // full array
    else if(a->len >= MAX_USRS)
    {
        return -1;
    }
    else
    {
        int i = 0, j;
        while(i < a->len)
        {
            if(strncmp(a->users[i], source, strlen(source)) == 0)
                return -1;
            i++;
        }
        // if at the end, just add
        if(i >= a->len)
        {
            a->users[i] = source;
            a->len++;
        }
        else  // otherwise, shift over everything to the right to make a hole
        {
            for(j = a->len; j > i; j--)
            {
                a->users[j] = a->users[j - 1];
            }
            a->users[j] = source;
            a->len++;
        }
    }
    return 0;
}
void print(struct array *a)
{
    int i;
    for(i = 0; i < a->len; i++)
        printf("%s ", a->users[i]);
    printf("\n");
}

/* Banner */
// get interface addresses
int getIPaddr(char ipaddr[])
{
    struct ifaddrs *ifaddr, *ifa;
    int space = 0;
    // get interface addresses
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return -1;
    }
    
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
        {
            struct sockaddr_in *pAddr = (struct sockaddr_in *)ifa->ifa_addr;
            int skip = strncmp(ifa->ifa_name, "lo0", 3);
            if(skip != 0)
                strcat(ipaddr, inet_ntoa(pAddr->sin_addr));
            if(skip != 0 && ifa->ifa_next != NULL)
                strcat(ipaddr, " ");
        }
    }
    freeifaddrs(ifaddr);
    return 0;
}
void printUsers()
{
    struct utmpx* entry;
    struct array a;
    a.len = 0;
    // open database
    getutxent();
    // read entries
    int i = 1;
    while((entry = getutxent()))
    {
        if(insert(&a, entry->ut_user) == 0)
        {
            printf("\t User %d.............: %s ", i++, entry->ut_user);
        }
    }
    printf("\n");
    // close database
    endutxent();
}
int getUserCount()
{
    struct utmpx* entry;
    struct array a;
    a.len = 0;
    // open database
    getutxent();
    // read entries
    while((entry = getutxent()))
    {
        insert(&a, entry->ut_user);
    }
    // close database
    endutxent();
    return a.len;
}
void getTime(char t[])
{
    struct timeval ut_tv;
    time_t time;
    gettimeofday(&ut_tv, NULL);
    time = (time_t)ut_tv.tv_sec;
    strcpy(t, ctime(&time));
}
void getOS(char version[])
{
    struct utsname unameData;
    uname(&unameData);
    strcpy(version,unameData.release);
}
void printbanner()
{
    char hostname[1024], ipaddrs[1024], time[1024], version[1024];
    memset(hostname, 0, 1024);
    memset(ipaddrs, 0, 1024);
    memset(time, 0, 1024);
    memset(version, 0, 1024);
    
    int count;
    // get name of server
    gethostname(hostname, 1024);
    
    // get IP address of server
    getIPaddr(ipaddrs);
    
    // get number of users logged in
    count = getUserCount();
    
    // get the current date and time
    getTime(time);
    
    // get the OS version
    getOS(version);
    
    // Print Host
    printf("\t**********************************\n");
    printf("\t Host Name.........: %s\n", hostname);
    // Print IP Address
    printf("\t IP Address........: %s\n", ipaddrs);
    // Print Count of logged in users
    printf("\t Users Logged In...: %d\n", count);
    // Print current date/time
    printf("\t Date and Time.....: %12.12s\n", time+4);
    // Print OS version
    printf("\t OS Version........: %s\n", version);
    printf("\t**********************************\n");
}
void monitor()
{
    struct termios oldSettings, newSettings;
    struct timeval tv;
    struct statvfs buffer;
    int res, ret, mib[2];
    char c;
    long long free_memory, used_memory;
    double onePercent;
    
    int64_t physical_memory;
    uint64_t totalSystemTime = 0, totalUserTime = 0, totalIdleTime = 0, total = 0;
    size_t length;
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;
    kern_return_t kr;
    host_cpu_load_info_data_t r_load;
    fd_set set;
    
    system("clear");
    tcgetattr(fileno(stdin), &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &newSettings);
    tv.tv_sec = 0;
    tv.tv_usec = 10;
    
    while(1)
    {
        
        FD_ZERO(&set);
        FD_SET(fileno(stdin), &set);
        
        res = select(fileno(stdin)+1, &set, NULL, NULL, &tv);
        if(res > 0)
        {
            
            read(fileno(stdin), &c, 1);
            tcsetattr(fileno(stdin), TCSANOW, &oldSettings);
            return;
        }
        else if(res < 0)
        {
            perror("select error");
            tcsetattr(fileno(stdin), TCSANOW, &oldSettings);
            return;
        }
        else
        {
            system("clear");
            printf("  _      _             __  __             _ _\n");
            printf(" | |    (_)           |  \\/  |           (_) |\n");
            printf(" | |     ___   _____  | \\  / | ___  _ __  _| |_ ___  _ __\n");
            printf(" | |    | \\ \\ / / _ \\ | |\\/| |/ _ \\| '_ \\| | __/ _ \\| '__|\n");
            printf(" | |____| |\\ V /  __/ | |  | | (_) | | | | | || (_) | |\n");
            printf(" |______|_| \\_/ \\___| |_|  |_|\\___/|_| |_|_|\\__\\___/|_|\n");
            printf("\n");
            printf("\n");
            
            // print banner
            printbanner();
            fflush(stdout);
            
            // Print currently logged in users
            printUsers();
            
            printf("\t**********************************\n");
            fflush(stdout);
            
            // Total RAM available
            printf("\t RAM\n");
            mib[0] = CTL_HW;
            mib[1] = HW_MEMSIZE;
            length = sizeof(int64_t);
            sysctl(mib, 2, &physical_memory, &length, NULL, 0);
            printf("\t Available..........: %lld GB\n", physical_memory / GB);
            
            // RAM Currently Used
            mach_port = mach_host_self();
            count = sizeof(vm_stats) / sizeof(natural_t);
            if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
                KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                                  (host_info64_t)&vm_stats, &count))
            {
                free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;
                printf("\t Free...............: %lld MB\n", free_memory / MB);
                
                used_memory = ((int64_t)vm_stats.active_count +
                                         (int64_t)vm_stats.inactive_count +
                                         (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
                printf("\t Used...............: %lld MB\n", used_memory / MB);
            }
            
            printf("\t**********************************\n");
            fflush(stdout);
            
            // CPU usage
            printf("\t CPU\n");
            count = HOST_CPU_LOAD_INFO_COUNT;
            kr = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (int *)&r_load, &count);
            if (kr != KERN_SUCCESS) {
                printf("oops: %s\n", mach_error_string(kr));
                return;
            }
            totalSystemTime = r_load.cpu_ticks[CPU_STATE_SYSTEM];
            totalUserTime = r_load.cpu_ticks[CPU_STATE_USER] + r_load.cpu_ticks[CPU_STATE_NICE];
            totalIdleTime = r_load.cpu_ticks[CPU_STATE_IDLE];
            total = totalSystemTime + totalUserTime + totalIdleTime;
            onePercent = total/100.0f;
            printf("\t System usage.......: %.0f %%\n", totalSystemTime/onePercent);
            printf("\t User usage.........: %.0f %%\n", totalUserTime/onePercent);
            printf("\t Idle...............: %.0f %%\n", totalIdleTime/onePercent);
            
            printf("\t**********************************\n");
            fflush(stdout);
            
            // Disk Usage
            printf("\t Disk\n");
            
            ret = statvfs("/", &buffer);
            if (!ret)
            {
                const double total = (double)(buffer.f_blocks * buffer.f_frsize) / GB;
                const double available = (double)(buffer.f_bfree * buffer.f_frsize) / GB;
                const double used = total - available;
                const double usedPercentage = (double)(used / total) * (double)100;
                printf("\t Total..............: %.0f GB\n", total);
                printf("\t Available..........: %.0f GB\n", available);
                printf("\t Used...............: %.1f GB (%.0f %%)\n", used, usedPercentage);
            }
            
            printf("\t**********************************\n");
            printf("\n\t Press any key to exit.\n");
            fflush(stdout);
            
            // reset the timer to 5 seconds
            tv.tv_sec = 1;
        }
    }
    return;
}
void printMenu()
{
    printf("\n\t 1)List currently logged in users\n");
    printf("\t 2)List a user's active processes\n");
    printf("\t 3)Monitor a user's processes\n");
    printf("\t 4)Kill a process\n");
    printf("\t 5)Show the last 10 users who logged in\n");
    printf("\t 6)Show free disk space\n");
    printf("\t 7)Show disk space used by a user's home directory\n");
    printf("\t 8)Find large files (>100MB). bonus: make the size an option the user can enter\n");
    printf("\t 9)Live monitor:\n");
    printf("\t 10)Exit\n");
    return;
}
void menu()
{
    int n = -1, ret;
    struct statvfs buffer;
    char *user, home_dir[1024];
    
    while(n != 10)
    {
        system("clear");
        // print menu options
        
        printf("  __  __       _         __  __\n");
        printf(" |  \\/  |     (_)       |  \\/  |\n");
        printf(" | \\  / | __ _ _ _ __   | \\  / | ___ _ __  _   _\n");
        printf(" | |\\/| |/ _` | | '_ \\  | |\\/| |/ _ \\ '_ \\| | | |\n");
        printf(" | |  | | (_| | | | | | | |  | |  __/ | | | |_| |\n");
        printf(" |_|  |_|\\__,_|_|_| |_| |_|  |_|\\___|_| |_|\\__,_|\n");
        printf("\n");
        printf("\n");
        
        printbanner();
        printMenu();
        
        do
        {
            printf("\n\t Make a selection [1-10]:");
            scanf ("%d",&n);
        } while(n<1 || n>10);
        switch(n)
        {
            case 1: // List currently logged in users
                printUsers();
                break;
            case 2: // List a user's active processes
                // todo: use pid to get active processes
                user = getenv("USER");
                printf("\t %s's pid...: %i\n",user, getpwnam(user)->pw_uid);
                break;
            case 3: // Monitor a user's processes
                // todo: use pid to get all processes
                user = getenv("USER");
                printf("\t %s's pid...: %i\n",user, getpwnam(user)->pw_uid);
                break;
            case 4: // Kill a process
                // todo: use pid to get kill a process
                user = getenv("USER");
                printf("\t %s's pid...: %i\n",user, getpwnam(user)->pw_uid);
                break;
            case 5: // Show the last 10 users who logged in
                // todo: get last 10 logins
                break;
            case 6: // Show free disk space
                ret = statvfs("/", &buffer);
                if (!ret)
                {
                    printf("\t Available...: %.0f GB\n", (double)(buffer.f_bfree * buffer.f_frsize) / GB);
                }
                break;
            case 7: // Show disk space used by a user's home directory
                memset(home_dir, 0, 1024);
                strcpy(home_dir, "/Users/");
                strcat(home_dir, getenv("USER"));
                ret = statvfs(home_dir, &buffer);
                if (!ret)
                {
                    const double total = (double)(buffer.f_blocks * buffer.f_frsize) / GB;
                    const double available = (double)(buffer.f_bfree * buffer.f_frsize) / GB;
                    const double used = total - available;
                    const double usedPercentage = (double)(used / total) * (double)100;
                    printf("\t Used...: %.1f GB (%.0f%%)\n", used, usedPercentage);
                }
                break;
            case 8: // Find large files, make the size an option the user can enter
                // todo: find large files
                break;
            case 9: // Live monitor
                monitor();
                break;
            case 10: // Exit
                break;
            default:
                printf("This should not happen!");
        }
        if(n < 9)
            sleep(3);
    }
    return;
}
void test()
{
    
    //
    return;
}
int main(int argc, char *argv[])
{
    menu();
    return 0;
}






