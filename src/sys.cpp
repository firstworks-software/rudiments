// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/sys.h>
#include <rudiments/process.h>
#include <rudiments/error.h>
#include <rudiments/charstring.h>
#include <rudiments/device.h>
#include <rudiments/bytestring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>

#ifndef RUDIMENTS_HAVE_GETHOSTNAME
	#include <rudiments/linkedlist.h>
	#include <rudiments/directory.h>
	#include <rudiments/file.h>
#endif

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

#ifdef RUDIMENTS_HAVE_UNISTD_H
	// for sysconf, getpagesize, gethostname, sethostname
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_UTSNAME_H
	// for uname
	#include <sys/utsname.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	// for getloadavg on linux/bsd
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_LOADAVG_H
	// for getloadavg on solaris
	#include <sys/loadavg.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_REBOOT_H
	// for reboot
	#include <sys/reboot.h>
#endif

#ifdef RUDIMENTS_HAVE_LINUX_REBOOT_H
	// for linux reboot commands
	#include <linux/reboot.h>
#endif

#ifdef RUDIMENTS_HAVE_PROTOTYPES_H
	// for uadmin
	#include <prototypes.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_UADMIN_H
	// for uadmin
	#include <sys/uadmin.h>
#endif

#if defined(RUDIMENTS_HAVE_METREG_H) && \
	defined(RUDIMENTS_HAVE_MAS_H) && \
	defined(RUDIMENTS_HAVE_SYS_DL_H)
	#include <metreg.h>
	#include <mas.h>
	#include <sys/dl.h>
	#include <rudiments/datetime.h>
	#include <rudiments/snooze.h>
#endif

#ifdef RUDIMENTS_HAVE_GETVERSIONEX
	#ifdef RUDIMENTS_HAVE_TIME_H
		// for CLOCKS_PER_SEC
		#include <time.h>
	#endif
#endif

#ifdef RUDIMENTS_HAVE_ROSTER_H
	// for BRoster::_ShutDown on Haiku
	#include <Roster.h>
	extern "C" {
		extern	status_t	_kern_shutdown(bool reboot);
	}
#endif

#ifdef RUDIMENTS_HAVE_MISSING_SETHOSTNAME_DECLARATION
	extern "C" {
		extern	int	sethostname(char *name, int namelen);
	}
#endif

#include <stdio.h>
#ifdef RUDIMENTS_HAVE_SYS_MMAN_H
	#include <sys/mman.h>
#endif

// if SSIZE_MAX is undefined, choose a good safe value
// that should even work on 16-bit systems
#ifndef SSIZE_MAX
        #define SSIZE_MAX 16383
#endif

#ifdef RUDIMENTS_HAVE_MPROTECT_CADDR_T
	#define MPROTECT_ADDRCAST caddr_t
#else
	#define MPROTECT_ADDRCAST void *
#endif

#ifdef RUDIMENTS_HAVE_MINCORE_CADDR_T
	#define MINCORE_ADDRCAST caddr_t
#else
	#define MINCORE_ADDRCAST void *
#endif


#ifdef RUDIMENTS_HAVE_MLOCK_CADDR_T
	#define MLOCK_ADDRCAST caddr_t
#else
	#define MLOCK_ADDRCAST void *
#endif

#ifdef RUDIMENTS_HAVE_MUNLOCK_CADDR_T
	#define MUNLOCK_ADDRCAST caddr_t
#else
	#define MUNLOCK_ADDRCAST void *
#endif

#ifdef RUDIMENTS_HAVE_MADVISE_CADDR_T
	#define MADVISE_ADDRCAST caddr_t
#else
	#define MADVISE_ADDRCAST void *
#endif


char *sys::getOperatingSystemName() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		error::clearError();
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return (result==-1)?NULL:charstring::duplicate(u.sysname);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		return charstring::duplicate("Windows");
	#else
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

char *sys::getOperatingSystemRelease() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		error::clearError();
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return (result==-1)?NULL:charstring::duplicate(u.release);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		// for windows, just return the same as the version number
		return getOperatingSystemVersion();
	#else
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

char *sys::getOperatingSystemVersion() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		error::clearError();
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return (result==-1)?NULL:charstring::duplicate(u.version);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)

		// get the os version info
		// (yes, this craziness is how you have to do it)
		OSVERSIONINFOEX	info;
		bytestring::zero(&info,sizeof(info));
		info.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
		if (!GetVersionEx((LPOSVERSIONINFO)&info)) {
			info.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
			if (!GetVersionEx((LPOSVERSIONINFO)&info)) {
				return NULL;
			}
		}

		// build up the version number
		stringbuffer	retval;
		retval.append((uint64_t)info.dwMajorVersion)->append(".");
		retval.append((uint64_t)info.dwMinorVersion)->append(".");
		retval.append((uint64_t)info.dwBuildNumber);

		// return the version number
		return retval.detachString();
	#else
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

char *sys::getOperatingSystemArchitecture() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		struct utsname	u;
		int32_t	result;
		error::clearError();
		do {
			result=uname(&u);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return (result==-1)?NULL:charstring::duplicate(u.machine);
	#elif defined(RUDIMENTS_HAVE_GETNATIVESYSTEMINFO) || \
			defined(RUDIMENTS_HAVE_GETSYSTEMINFO)

		SYSTEM_INFO	info;
        	#if _WIN32_WINNT>=0x0501
			GetNativeSystemInfo((LPSYSTEM_INFO)&info);
		#else
			GetSystemInfo(&info);
		#endif

		char	*arch=NULL;
		switch (info.wProcessorArchitecture) {
			case PROCESSOR_ARCHITECTURE_AMD64:
				arch=charstring::duplicate("amd64");
				break;
			case PROCESSOR_ARCHITECTURE_IA64:
				arch=charstring::duplicate("x86_64");
				break;
			case PROCESSOR_ARCHITECTURE_INTEL:
				arch=charstring::duplicate("x86");
				break;
			case PROCESSOR_ARCHITECTURE_UNKNOWN:
				arch=charstring::duplicate("Unknown");
				break;
		}
		return arch;
	#else
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

const char *sys::getRudimentsVersion() {
	return RUDIMENTS_VERSION;
}

char *sys::getHostName() {
	#if defined(RUDIMENTS_HAVE_GETHOSTNAME)

		// initialize winsock if necessary
		winsock::initWinsock();

		// initialize the result code
		int32_t		result;

		// start with a 64-byte buffer
		uint64_t	namelen=64;

		// try to get the name, retry if the buffer was too small...
		do {

			// create a buffer to store the name
			char	*name=new char[namelen];

			// try to get the name, retry if
			// the system call was interrupted
			error::clearError();
			do {
				result=gethostname(name,namelen);
			} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());

			// return the name on success
			if (!result) {
				return name;
			}

			// clean up,
			delete[] name;

			// resize the buffer
			namelen=namelen+64;

		} while (result==-1 && error::getErrorNumber()==ENAMETOOLONG);

		// for any other error, return null
		return NULL;

	#else

		// open /etc
		directory	d;
		if (!d.open("/etc")) {
			return NULL;
		}

		// get various file names into a list
		linkedlist< char * >	nodenames;
		for (;;) {
			char	*f=d.read();
			if (!charstring::compare(f,"hostname") ||
				!charstring::compare(f,"nodename") ||
				!charstring::compare(f,"nodename.")) {
				nodenames.append(f);
			} else if (!f) {
				break;
			} else {
				delete[] f;
			}
		}

		// bail if there weren't any
		if (!nodenames.getLength()) {
			return NULL;
		}

		// sort the list...
		// fortunately we care about hostname, nodename, then nodename.*
		// in that order, which also happens to be alphabetical order
		nodenames.heapSort();

		// get the contents of the first file
		stringbuffer	fname;
		fname.append("/etc/")->append(nodenames.getFirst()->getValue());
		char	*hostname=file::getContents(fname.getString());

		// clean up the list
		nodenames.clearAndArrayDelete();

		// return the hostname
		return (!charstring::isNullOrEmpty(hostname))?hostname:NULL;
	#endif
}

bool sys::setHostName(const char *hostname) {
	return setHostName(hostname,charstring::length(hostname));
}

bool sys::setHostName(const char *hostname, uint64_t hostnamelen) {
	#if defined(RUDIMENTS_HAVE_SETHOSTNAME) || \
		defined(RUDIMENTS_HAVE_MISSING_SETHOSTNAME_DECLARATION)
		winsock::initWinsock();
		int32_t	result;
		error::clearError();
		do {
			// Some systems (Solaris) have a char *, rather than
			// const char * for the first parameter.  Nothing will
			// be written back to that parameter so it's safe to
			// cast it to a char * and this appears to work on all
			// platforms.
			result=sethostname((char *)hostname,hostnamelen);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

#ifndef LOADAVG_1MIN
	#define LOADAVG_1MIN 0
#endif

#ifndef LOADAVG_5MIN
	#define LOADAVG_5MIN 1
#endif

#ifndef LOADAVG_15MIN
	#define LOADAVG_15MIN 2
#endif

bool sys::getLoadAverages(double *oneminuteaverage,
				double *fiveminuteaverage,
				double *fifteenminuteaverage) {
	*oneminuteaverage=0.0;
	*fiveminuteaverage=0.0;
	*fifteenminuteaverage=0.0;
	#if defined(RUDIMENTS_HAVE_GETLOADAVG)
		// use getloadavg if the system has it
		double	averages[3]={0.0,0.0,0.0};
		bool	retval=!getloadavg(averages,3);
		*oneminuteaverage=averages[LOADAVG_1MIN];
		*fiveminuteaverage=averages[LOADAVG_5MIN];
		*fifteenminuteaverage=averages[LOADAVG_15MIN];
		return retval;
	#elif defined(RUDIMENTS_HAVE_SCO_AVENRUN)
		// use avenrun if the system has it
		device	avenrun;
		if (avenrun.open("/dev/table/avenrun",O_RDONLY)) {
			uint16_t	avgs[3];
			if (avenrun.read((void *)avgs,sizeof(avgs))==
							sizeof(avgs)) {
				*oneminuteaverage=((double)avgs[0])/256.0;
				*fiveminuteaverage=((double)avgs[1])/256.0;
				*fifteenminuteaverage=((double)avgs[2])/256.0;
				return true;
			}
		}
		return false;
	#else
		// try /proc/loadavg as a last-ditch effort
		device	loadavg;
		if (!loadavg.open("/proc/loadavg",O_RDONLY)) {
			RUDIMENTS_SET_ENOSYS
			return false;
		}

		char		*buffer=NULL;
		if (loadavg.read(&buffer," ")==RESULT_ERROR) {
			return false;
		}
		*oneminuteaverage=(double)charstring::toFloat(buffer);
		delete[] buffer;

		if (loadavg.read(&buffer," ")==RESULT_ERROR) {
			return false;
		}
		*fiveminuteaverage=(double)charstring::toFloat(buffer);
		delete[] buffer;
		
		if (loadavg.read(&buffer," ")==RESULT_ERROR) {
			return false;
		}
		*fifteenminuteaverage=(double)charstring::toFloat(buffer);
		delete[] buffer;

		return true;
	#endif
}

void sys::sync() {
	#if defined(RUDIMENTS_HAVE_SYNC)
		::sync();
	#elif defined(RUDIMENTS_HAVE_FLUSHFILEBUFFERS)

		// FIXME: for some reason GetLogicalDrives doesn't return file
		// systems mounted by users when run as the Adminstrator but
		// when run as a user, CreateFile can't open any volume

		// get the volume bitmap
		DWORD	volumes=GetLogicalDrives();

		// for each volume...
		for (char driveletter='A'; driveletter<='Z'; driveletter++) {

			// ignore volumes that don't exist
			DWORD	exists=volumes&0x0001;
			volumes=volumes>>1;
			if (!exists) {
				continue;
			}

			// create the volume name, eg: \\.\C:
			char	volume[8];
			charstring::copy(volume,"\\\\.\\x:");
			volume[4]=driveletter;

			// open the volume
			HANDLE	vh=CreateFile(volume,
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_WRITE|
						FILE_SHARE_READ,NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,NULL);
			if (vh==INVALID_HANDLE_VALUE) {
				continue;
			}

			// flush the volume
			FlushFileBuffers(vh);

			// close the volume
			CloseHandle(vh);
		}
	#endif
}

#ifdef _WIN32
static bool shutDownWindows(bool reboot) {

	// make sure the curernt thread has permissions
	// to shut down the system...

	// open the current thread
	HANDLE	ph;
	if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&ph)) {
		return false;
	}
	if (ph==INVALID_HANDLE_VALUE) {
		return false;
	}

	// lookup the SE_SHUTDOWN_NAME privilege
	LUID	luid;
	if (!LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&luid)) {
		return false;
	}

	// enable the privilege
	TOKEN_PRIVILEGES	tp;
	tp.PrivilegeCount=1;
	tp.Privileges[0].Luid=luid;
	tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(ph,FALSE,
				&tp,sizeof(TOKEN_PRIVILEGES),
				NULL,NULL)) {
		return false;
	}

	#if defined(RUDIMENTS_HAVE_INITIATESYSTEMSHUTDOWNEX) || \
				defined(RUDIMENTS_HAVE_EXITWINDOWSEX)

		#if _WIN32_WINNT>=0x0500
		// FIXME: Arguably this should check to see if the calling user
		// is the interactive user and just run ExitWindowsEx instead of
		// InitiateSystemShutdownEx if it is.
		return InitiateSystemShutdownEx(
				NULL,NULL,0,FALSE,
				(reboot)?TRUE:FALSE,
				SHTDN_REASON_MAJOR_OTHER|
				SHTDN_REASON_MINOR_OTHER|
				SHTDN_REASON_FLAG_PLANNED)!=0;
		#else
		return ExitWindowsEx(
				(reboot)?EWX_REBOOT:EWX_POWEROFF,
				SHTDN_REASON_MAJOR_OTHER|
				SHTDN_REASON_MINOR_OTHER|
				SHTDN_REASON_FLAG_PLANNED)==TRUE;
		#endif
	#endif
}
#endif

bool sys::halt() {
	#if defined(RUDIMENTS_HAVE_REBOOT_1)
		int32_t	cmd;
		#if defined(LINUX_REBOOT_CMD_HALT)
			cmd=LINUX_REBOOT_CMD_HALT;
		#elif defined(RB_HALT) || defined(RUDIMENTS_HAVE_RB_HALT)
			cmd=RB_HALT;
		#elif defined(RB_HALT_SYSTEM)
			cmd=RB_HALT_SYSTEM;
		#elif defined(RBT_HALT)
			cmd=RBT_HALT;
		#else
			#error no RB_HALT or anything like it
		#endif
		return (::reboot(cmd)!=-1);
	#elif defined(RUDIMENTS_HAVE_REBOOT_2)
		int32_t	cmd;
		#if defined(RB_HALT) || defined(RUDIMENTS_HAVE_RB_HALT)
			cmd=RB_HALT;
		#else
			#error no RB_HALT or anything like it
		#endif
		// bootstr is const char * on some platforms and char * on
		// others.  This works for both.
		char	*bootstr=(char *)"";
		return (::reboot(cmd,bootstr)==-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_CHAR)
		return (uadmin(A_SHUTDOWN,AD_HALT,NULL)!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_INT)
		return (uadmin(A_SHUTDOWN,AD_HALT,0)!=-1);
	#elif defined(RUDIMENTS_HAVE_BROSTER__SHUTDOWN)
		_kern_shutdown(false);
		return true;
	#elif defined(_WIN32)
		return shutDownWindows(false);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

#ifdef RUDIMENTS_HAVE_BROSTER__SHUTDOWN
class BRoster::Private {
	public:
		Private(BRoster *r) : roster(r) {
		};
		status_t	ShutDown(bool reboot, bool confirm, bool sync) {
			return roster->_ShutDown(reboot,confirm,sync);
		};
	private:
		BRoster	*roster;
};
#endif

bool sys::shutDown() {
	#if defined(RUDIMENTS_HAVE_REBOOT_1)
		int32_t	cmd;
		#if defined(LINUX_REBOOT_CMD_POWER_OFF)
			cmd=LINUX_REBOOT_CMD_POWER_OFF;
		#elif defined(RB_POWEROFF)
			cmd=RB_POWEROFF;
		#elif defined(RB_HALT) || defined(RUDIMENTS_HAVE_RB_HALT)
			// OS X doesn't appear to have a power-down option,
			// just use halt instead
			cmd=RB_HALT;
		#elif defined(RB_HALT_SYSTEM)
			// Older linux doesn't have a power-down option
			// just use halt instead
			cmd=RB_HALT_SYSTEM;
		#elif defined(RBT_HALT)
			// Minix doesn't have a power-down option
			// just use halt instead
			cmd=RBT_HALT;
		#else
			#error no RB_POWEROFF or anything like it
		#endif
		return (::reboot(cmd)!=-1);
	#elif defined(RUDIMENTS_HAVE_REBOOT_2)
		int32_t	cmd;
		#if defined(RB_POWERDOWN)
			cmd=RB_POWERDOWN;
		#elif defined(RB_HALT) || defined(RUDIMENTS_HAVE_RB_HALT)
			// Solaris doesn't appear to have a power-down option,
			// just use halt instead
			cmd=RB_HALT;
		#else
			#error no RB_POWERDOWN or anything like it
		#endif
		// bootstr is const char * on some platforms and char * on
		// others.  This works for both.
		char	*bootstr=(char *)"";
		return (::reboot(cmd,bootstr)!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_CHAR)
		return (uadmin(A_SHUTDOWN,AD_PWRDOWN,NULL)!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_INT)
		// OSR6 doesn't appear to define AD_PWRDOWN,
		// just use halt instead.
		return (uadmin(A_SHUTDOWN,AD_HALT,0)!=-1);
	#elif defined(RUDIMENTS_HAVE_BROSTER__SHUTDOWN)
		BRoster			roster;
		BRoster::Private	rosterprivate(&roster);
		return (rosterprivate.ShutDown(false,false,true)==B_OK);
	#elif defined(_WIN32)
		return shutDownWindows(false);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif

}

bool sys::reboot() {
	#if defined(RUDIMENTS_HAVE_REBOOT_1)
		int32_t	cmd;
		#if defined(LINUX_REBOOT_CMD_RESTART)
			cmd=LINUX_REBOOT_CMD_RESTART;
		#elif defined(RB_AUTOBOOT) || \
			defined(RUDIMENTS_HAVE_RB_AUTOBOOT)
			cmd=RB_AUTOBOOT;
		#elif defined(RBT_REBOOT)
			cmd=RBT_REBOOT;
		#else
			#error no RB_AUTOBOOT or anything like it
		#endif
		return (::reboot(cmd)!=-1);
	#elif defined(RUDIMENTS_HAVE_REBOOT_2)
		int32_t	cmd;
		#if defined(RB_AUTOBOOT) || \
			defined(RUDIMENTS_HAVE_RB_AUTOBOOT)
			cmd=RB_AUTOBOOT;
		#else
			#error no RB_AUTOBOOT or anything like it
		#endif
		// bootstr is const char * on some platforms and char * on
		// others.  This works for both.
		char	*bootstr=(char *)"";
		return (::reboot(cmd,bootstr)!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_CHAR)
		return (uadmin(A_SHUTDOWN,AD_BOOT,NULL)!=-1);
	#elif defined(RUDIMENTS_HAVE_UADMIN_INT)
		return (uadmin(A_SHUTDOWN,AD_BOOT,0)!=-1);
	#elif defined(RUDIMENTS_HAVE_BROSTER__SHUTDOWN)
		BRoster			roster;
		BRoster::Private	rosterprivate(&roster);
		return (rosterprivate.ShutDown(true,false,true)==B_OK);
	#elif defined(_WIN32)
		return shutDownWindows(true);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

int64_t sys::getMaxCommandLineArgumentLength() {
	#if defined(_SC_ARG_MAX)
		return sysConf(_SC_ARG_MAX);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)

		// Lower then XP, it's 2047
		// On XP or higher, it's 8191

		// get the os version info
		// (yes, this craziness is how you have to do it)
		OSVERSIONINFOEX	info;
		bytestring::zero(&info,sizeof(info));
		info.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
		if (!GetVersionEx((LPOSVERSIONINFO)&info)) {
			info.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
			if (!GetVersionEx((LPOSVERSIONINFO)&info)) {
				return -1;
			}
		}

		// XP is 5.1
		return (info.dwMajorVersion>=5 &&
				info.dwMinorVersion>=1)?8191:2047;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxProcessesPerUser() {
	#if defined(_SC_CHILD_MAX)
		return sysConf(_SC_CHILD_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxHostNameLength() {
	#if defined(_SC_HOST_NAME_MAX)
		return sysConf(_SC_HOST_NAME_MAX);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		// 15 characters on windows for the NETBIOS name (Computer Name)
		// windows requires that the DNS hostname be the same as the
		// NETBIOS name
		return 15;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxLoginNameLength() {
	#if defined(_SC_LOGIN_NAME_MAX)
		return sysConf(_SC_LOGIN_NAME_MAX);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		// 20 characters on windows (pre-dns suffix)
		return 20;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getClockTicksPerSecond() {
	#if defined(_SC_CLK_TCK)
		return sysConf(_SC_CLK_TCK);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		// this is a macro on windows
		return CLOCKS_PER_SEC;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxOpenFilesPerProcess() {
	#if defined(_SC_OPEN_MAX)
		return sysConf(_SC_OPEN_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int32_t sys::getPageSize() {
	#if defined(RUDIMENTS_HAVE_GETPAGESIZE)
		return getpagesize();
	#elif defined(RUDIMENTS_HAVE_SYSCONF) && defined(_SC_PAGESIZE)
		return sysConf(_SC_PAGESIZE);
	#elif defined(RUDIMENTS_HAVE_SYSCONF) && defined(_SC_PAGE_SIZE)
		return sysConf(_SC_PAGE_SIZE);
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMINFO)
		SYSTEM_INFO	systeminfo;
		GetSystemInfo(&systeminfo);
		return systeminfo.dwPageSize;
	#else
		#error no getpagesize or anything like it
	#endif
}

int32_t sys::getAllocationGranularity() {
	#if defined(RUDIMENTS_HAVE_GETPAGESIZE)
		return getpagesize();
	#elif defined(RUDIMENTS_HAVE_SYSCONF) && defined(_SC_PAGESIZE)
		return sysConf(_SC_PAGESIZE);
	#elif defined(RUDIMENTS_HAVE_SYSCONF) && defined(_SC_PAGE_SIZE)
		return sysConf(_SC_PAGE_SIZE);
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMINFO)
		SYSTEM_INFO	systeminfo;
		GetSystemInfo(&systeminfo);
		return systeminfo.dwAllocationGranularity;
	#else
		#error no getpagesize or anything like it
	#endif
}

int64_t sys::getMaxOpenStreamsPerProcess() {
	#if defined(_SC_STREAM_MAX)
		return sysConf(_SC_STREAM_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxSymlinkLoops() {
	#if defined(_SC_SYMLOOP_MAX)
		return sysConf(_SC_SYMLOOP_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxTerminalDeviceNameLength() {
	#if defined(_SC_TTY_NAME_MAX)
		return sysConf(_SC_TTY_NAME_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxTimezoneNameLength() {
	#if defined(_SC_TZNAME_MAX)
		return sysConf(_SC_TZNAME_MAX);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		// 256 on windows
		return 256;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxLineLength() {
	#if defined(_SC_LINE_MAX)
		return sysConf(_SC_LINE_MAX);
	#else
		return SSIZE_MAX;
	#endif
}

int64_t sys::getPhysicalPageCount() {
	#if defined(_SC_PHYS_PAGES)
		return sysConf(_SC_PHYS_PAGES);
	#elif defined(_SC_TOTAL_MEMORY)
		return sysConf(_SC_TOTAL_MEMORY);
	#elif defined(RUDIMENTS_HAVE_GLOBALMEMORYSTATUSEX) && \
						_WIN32_WINNT>=0x0500
		MEMORYSTATUSEX	ms;
		ms.dwLength=sizeof(ms);
		return (GlobalMemoryStatusEx(&ms)==TRUE)?
				ms.ullTotalPhys/getPageSize():-1;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getAvailablePhysicalPageCount() {
	#if defined(_SC_AVPHYS_PAGES)
		return sysConf(_SC_AVPHYS_PAGES);
	#elif defined(RUDIMENTS_HAVE_MAS_OPEN)

		// The available memory metric isn't just a page count.  Rather
		// the available page count is added to the metric every second.
		//
		// To get the currently available page count, you have to
		// sample it twice, one second apart, then subtract the samples.
		//
		// Really, you have to do (sample2-sample2)/(time2-time1) to get
		// the page count, but since (time2-time1) is always 1 when we
		// wait 1 second, then we can get away with just doing
		// (sample2-sample1).

		// open /var/adm/metreg.data (or whatever...)
		// Even though this file typically has 644 perms, it can't be
		// mas_open()ed by non-root users (at least on UnixWare 7.0.1).
		int	mas=mas_open(MAS_FILE,MAS_MMAP_ACCESS);
		if (mas==-1) {
			// NOTE: when this fails, the error is available via
			// mas_error() and mas_errstr(), but errno appears to
			// also get set appropriately (at least on
			// UnixWare 7.0.1) so for now, we won't worry about
			// doing anything with mas_error()/mas_errstr().
			return -1;
		}

		// get the first sample
		uint64_t	*met1p=(uint64_t *)mas_get_met(mas,FREEMEM,0);
		if (!met1p) {
			// see note above re. mas_error()/mas_errstr()
			mas_close(mas);
			return -1;
		}
		uint64_t	met1=*met1p;

		// get the second sample, handling overflows...
		uint64_t	met2=0;
		for (;;) {

			// wait 1 second
			snooze::macrosnooze(1);

			// get the second sample
			uint64_t	*met2p=
					(uint64_t *)mas_get_met(mas,FREEMEM,0);
			if (!met2p) {
				// see note above re. mas_error()/mas_errstr()
				mas_close(mas);
				return -1;
			}
			met2=*met2p;

			// handle overflows...
			// if the second sample was greater than the first, then
			// we're good, otherwise we need to loop back and try
			// again
			if (met2>met1) {
				break;
			}
			met1=met2;
		}

		// close /var/adm/metreg.data (or whatever...)
		mas_close(mas);

		// return the result
		return met2-met1;

	#elif defined(RUDIMENTS_HAVE_GLOBALMEMORYSTATUSEX) && \
						_WIN32_WINNT>=0x0500
		MEMORYSTATUSEX	ms;
		ms.dwLength=sizeof(ms);
		return (GlobalMemoryStatusEx(&ms)==TRUE)?
				ms.ullAvailPhys/getPageSize():-1;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getUsedPhysicalPageCount() {
	return getPhysicalPageCount()-getAvailablePhysicalPageCount();
}

int64_t sys::getPhysicalMemorySize() {
	return getPhysicalPageCount()*getPageSize();
}

int64_t sys::getAvailablePhysicalMemorySize() {
	return getAvailablePhysicalPageCount()*getPageSize();
}

int64_t sys::getUsedPhysicalMemorySize() {
	return getUsedPhysicalPageCount()*getPageSize();
}

int64_t sys::getProcessorCount() {
	#if defined(_SC_NPROCESSORS_CONF)
		return sysConf(_SC_NPROCESSORS_CONF);
	#elif defined(RUDIMENTS_HAVE_GETLOGICALPROCESSORINFORMATION) || \
			defined(RUDIMENTS_HAVE_GETSYSTEMINFO)

		// It turns out that GetLogicalProcessorInformation isn't
		// actually available until WinXP, even though it's declared
		// in the headers for all platforms...

        	#if _WIN32_WINNT>=0x0501

		// get the array of info about the processors
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION	buffer=NULL;
		DWORD					bufferlen=0;
		GetLogicalProcessorInformation(buffer,&bufferlen);
		buffer=(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(bufferlen);
		if (GetLogicalProcessorInformation(buffer,&bufferlen)==FALSE) {
			return -1;
		}

		// step through the array, counting cores, and hyperthreads
		int64_t	count=0;
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION	ptr=buffer;
		for (DWORD offset=0; offset<bufferlen;
				offset=offset+sizeof(
					SYSTEM_LOGICAL_PROCESSOR_INFORMATION)) {
			if (ptr->Relationship==RelationProcessorCore) {
				// count the bits in the processor mask,
				// a hyperthreaded processor will have more
				// than one bit set to 1
				uint64_t	bits=ptr->ProcessorMask;
				for (uint16_t i=0; i<64; i++) {
					if (bits&0x0001) {
						count++;
					}
					bits=bits>>1;
				}
			}
			ptr++;
		}

		// clean up
		free(buffer);

		return count;

		#else

		// I'm not sure if this reports cores and hyperthreads as
		// processors, but it's the best we can do.
		SYSTEM_INFO	systeminfo;
		GetSystemInfo(&systeminfo);
		return systeminfo.dwNumberOfProcessors;

		#endif
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxProcessorCount() {
	#if defined(_SC_NPROCESSORS_MAX)
		return sysConf(_SC_NPROCESSORS_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getProcessorsOnline() {
	#if defined(_SC_NPROCESSORS_ONLN)
		return sysConf(_SC_NPROCESSORS_ONLN);
	#else
		return getProcessorCount();
	#endif
}

int64_t sys::getMaxSupplementalGroupsPerUser() {
	#if defined(_SC_NGROUPS_MAX)
		return sysConf(_SC_NGROUPS_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxDelayTimerExpirations() {
	#if defined(_SC_DELAYTIMER_MAX)
		return sysConf(_SC_DELAYTIMER_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxRealtimeSignals() {
	#if defined(_SC_RTSIG_MAX)
		return sysConf(_SC_RTSIG_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxSemaphoresPerProcess() {
	#if defined(_SC_SEM_NSEMS_MAX)
		return sysConf(_SC_SEM_NSEMS_MAX);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		// on windows it's 2^24 (the maximum number of kernel handles
		// per-process) though available memory limits it too
		return 16777216;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxSemaphoreValue() {
	#if defined(_SC_SEM_VALUE_MAX)
		return sysConf(_SC_SEM_VALUE_MAX);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		// on windows it's 2^31-1
		return 2147483647;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxSignalQueueLength() {
	#if defined(_SC_SIGQUEUE_MAX)
		return sysConf(_SC_SIGQUEUE_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxTimersPerProcess() {
	#if defined(_SC_TIMER_MAX)
		return sysConf(_SC_TIMER_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getSuggestedGroupEntryBufferSize() {
	#if defined(_SC_GETGR_R_SIZE_MAX)
		int64_t	size=sysConf(_SC_GETGR_R_SIZE_MAX);
		return (size>0)?size:1024;
	#else
		return 1024;
	#endif
}

int64_t sys::getSuggestedPasswordEntryBufferSize() {
	#if defined(_SC_GETPW_R_SIZE_MAX)
		int64_t	size=sysConf(_SC_GETPW_R_SIZE_MAX);
		return (size>0)?size:1024;
	#else
		return 1024;
	#endif
}

int64_t sys::getMinThreadStackSize() {
	#if defined(_SC_THREAD_STACK_MIN)
		return sysConf(_SC_THREAD_STACK_MIN);
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		// On windows, the stack size of a thread is defined when
		// the thread is created and rounded up to the nearest
		// page.  The minimum is actually defined in the header of
		// the executable though and rounded up to the nearest multiple
		// of the system's allocation granularity which is usually 64K.
		// It can be specified by the /STACK argument to the linker or
		// in the STACKSIZE variable in the .def file.  There's no
		// obvious way to get that value but the default is 1MB so I
		// guess we'll return 1MB for now.
		return 1048576;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxThreadsPerProcess() {
	#if defined(_SC_THREAD_THREADS_MAX)
		return sysConf(_SC_THREAD_THREADS_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getThreadDestructorIterations() {
	#if defined(_SC_THREAD_DESTRUCTOR_ITERATIONS)
		return sysConf(_SC_THREAD_DESTRUCTOR_ITERATIONS);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxThreadKeys() {
	#if defined(_SC_THREAD_KEYS_MAX)
		return sysConf(_SC_THREAD_KEYS_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxAtExitFunctions() {
	#if defined(_SC_ATEXIT_MAX)
		return sysConf(_SC_ATEXIT_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getCpuSetSize() {
	#if defined(_SC_CPUSET_SIZE)
		return sysConf(_SC_CPUSET_SIZE);
	#elif defined(_WIN32)

		// 64 for 64-bit Windows 7/Server 2008 R2,
		// 1 for other platforms...

        	#if _WIN32_WINNT>=0x0601
			SYSTEM_INFO	sysinfo;
			GetNativeSystemInfo((LPSYSTEM_INFO)&sysinfo);
			return (sysinfo.wProcessorArchitecture==
					PROCESSOR_ARCHITECTURE_AMD64 ||
				sysinfo.wProcessorArchitecture==
					PROCESSOR_ARCHITECTURE_IA64)?64:1;
		#else
			return 1;
		#endif
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxPasswordLength() {
	#if defined(_SC_PASS_MAX)
		return sysConf(_SC_PASS_MAX);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)

		// lower than 2000 - 14 characters
		// 2000/XP/Server 2003 - 127 technically but password-change
		//			dialogs limit it to 32 characters
		// Vista and higher - 127 characters

		// get the os version info
		// (yes, this craziness is how you have to do it)
		OSVERSIONINFOEX	info;
		bytestring::zero(&info,sizeof(info));
		info.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
		if (!GetVersionEx((LPOSVERSIONINFO)&info)) {
			info.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
			if (!GetVersionEx((LPOSVERSIONINFO)&info)) {
				return -1;
			}
		}

		// 2000 is 5.0
		if (info.dwMajorVersion<5) {
			return 14;
		}

		// 2000, XP and Server 2003 are all 5.x
		if (info.dwMajorVersion==5) {
			return 32;
		}

		// Vista is 6.0
		return 127;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxLogNameLength() {
	#if defined(_SC_LOGNAME_MAX)
		return sysConf(_SC_LOGNAME_MAX);
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::getMaxProcessId() {
	#if defined(_SC_MAXPID)
		return sysConf(_SC_MAXPID);
	#elif defined(RUDIMENTS_HAVE_GETVERSIONEX)
		// On windows, there doesn't appear to be a definitive word on
		// this.  It's stored as a dword and must be aligned, so the
		// maximum PID ought to be 0xFFFFFFFC
		return 0xFFFFFFFC;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

int64_t sys::sysConf(int32_t name) {
	#if defined(RUDIMENTS_HAVE_SYSCONF)
		int64_t	result;
		error::clearError();
		do {
			result=sysconf(name);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return result;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

bool sys::signalsInterruptSystemCalls() {
	#ifdef _WIN32
		return false;
	#else
		return true;
	#endif
}

char sys::getDirectorySeparator() {
	#if defined(_WIN32)
		return '\\';
	#elif defined(__VMS)
		return '.';
	#else
		return '/';
	#endif
}

bool sys::setProtection(unsigned char *ptr, size_t size, int32_t protection) {
	#ifdef RUDIMENTS_HAVE_MPROTECT
		int32_t	result;
		error::clearError();
		do {
			result=mprotect(
				reinterpret_cast<MPROTECT_ADDRCAST>(ptr),
				size,protection);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool sys::sequentialAccess(unsigned char *ptr, size_t size) {
	#if defined(RUDIMENTS_HAVE_MADVISE) && defined(MADV_SEQUENTIAL)
		return mAdvise(ptr,size,MADV_SEQUENTIAL);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool sys::randomAccess(unsigned char *ptr, size_t size) {
	#if defined(RUDIMENTS_HAVE_MADVISE) && defined(MADV_RANDOM)
		return mAdvise(ptr,size,MADV_RANDOM);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool sys::willNeed(unsigned char *ptr, size_t size) {
	#if defined(RUDIMENTS_HAVE_MADVISE) && defined(MADV_WILLNEED)
		return mAdvise(ptr,size,MADV_WILLNEED);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool sys::wontNeed(unsigned char *ptr, size_t size) {
	#if defined(RUDIMENTS_HAVE_MADVISE) && defined(MADV_DONTNEED)
		return mAdvise(ptr,size,MADV_DONTNEED);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool sys::normalAccess(unsigned char *ptr, size_t size) {
	#if defined(RUDIMENTS_HAVE_MADVISE) && defined(MADV_NORMAL)
		return mAdvise(ptr,size,MADV_NORMAL);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool sys::lock(unsigned char *ptr, size_t size) {
	#ifdef RUDIMENTS_HAVE_MLOCK
		int32_t	result;
		error::clearError();
		do {
			result=mlock(
				reinterpret_cast<MLOCK_ADDRCAST>(ptr),
				size);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool sys::unlock(unsigned char *ptr, size_t size) {
	#ifdef RUDIMENTS_HAVE_MUNLOCK
		int32_t	result;
		error::clearError();
		do {
			result=munlock(
				reinterpret_cast<MUNLOCK_ADDRCAST>(ptr),
				size);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool sys::notPagedOut(unsigned char *ptr, size_t size) {

	#ifdef RUDIMENTS_HAVE_MINCORE
		// create an array of char's, 1 for each page
		int32_t		pagesize=sys::getAllocationGranularity();
		int32_t		tmpsize=(size+pagesize-1)/pagesize;
		#ifdef RUDIMENTS_HAVE_MINCORE_CHAR
			char		*tmp=new char[tmpsize];
		#else
			unsigned char	*tmp=new unsigned char[tmpsize];
		#endif

		// call mincore to fill the array
		int32_t		result;
		error::clearError();
		do {
			result=mincore(
				reinterpret_cast<MINCORE_ADDRCAST>(ptr),
				size,tmp);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		if (result) {
			delete[] tmp;
			return false;
		}

		// look through the array, if any of the
		// pages aren't in memory, return false
		for (int32_t i=0; i<tmpsize; i++) {
			if (tmp[i]) {
				delete[] tmp;
				return false;
			}
		}
		delete[] tmp;
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool sys::disablePaging() {
	#if defined(MCL_CURRENT) && defined(MCL_FUTURE)
		return mLockAll(MCL_CURRENT|MCL_FUTURE);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool sys::disablePagingOfCurrent() {
	#if defined(MCL_CURRENT)
		return mLockAll(MCL_CURRENT);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool sys::disablePagingOfNew() {
	#if defined(MCL_FUTURE)
		return mLockAll(MCL_FUTURE);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool sys::enablePaging() {
	#ifdef RUDIMENTS_HAVE_MUNLOCKALL
		int32_t	result;
		error::clearError();
		do {
			result=munlockall();
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool sys::mAdvise(unsigned char *start, size_t size, int32_t advice) {
	#if defined(RUDIMENTS_HAVE_MADVISE)
		int32_t	result;
		error::clearError();
		do {
			result=madvise(
				reinterpret_cast<MADVISE_ADDRCAST>(start),
				size,advice);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool sys::mLockAll(int32_t flags) {
	#ifdef RUDIMENTS_HAVE_MLOCKALL
		int32_t	result;
		error::clearError();
		do {
			result=mlockall(flags);
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}
