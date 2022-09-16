/* include/rudiments/private/config.h.  Generated from config.h.in by configure.  */
/* include/rudiments/private/config.h.in.  Generated from configure.in by autoheader.  */

/* On some platforms */
/* #undef ADD_NEWLINE_AFTER_READ_FROM_STDIN */

/* Enable some hacks on Darwin with gcc 2.95.2 */
/* #undef DARWIN_GCC_2952_HACKS */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Platform requires default TLS ciper of PROFILE=SYSTEM */
#define RUDIMENTS_DEFAULT_CIPHER_PROFILE_SYSTEM 1

/* Enable runtime notification of methods that set ENOSYS */
/* #undef RUDIMENTS_ENOSYS_NOTIFY */

/* getsockopt has a void * on most systems */
#define RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE void *

/* SSL has ASN1_STRING_get0_data */
#define RUDIMENTS_HAS_ASN1_STRING_GET0_DATA 1

/* libcurl has CURLOPT_SSH_AUTH_TYPES */
#define RUDIMENTS_HAS_CURLOPT_SSH_AUTH_TYPES 1

/* libcurl has CURLOPT_USERNAME */
#define RUDIMENTS_HAS_CURLOPT_USERNAME 1

/* libcurl has CURLOPT_USERPWD */
#define RUDIMENTS_HAS_CURLOPT_USERPWD 1

/* libcurl has curl_multi_timeout */
#define RUDIMENTS_HAS_CURL_MULTI_TIMEOUT 1

/* SSL has EVP_CIPHER_CTX_NEW */
#define RUDIMENTS_HAS_EVP_CIPHER_CTX_NEW 1

/* SSL has EVP_PKEY_base_id */
#define RUDIMENTS_HAS_EVP_PKEY_BASE_ID 1

/* SSL has EVP_PKEY_get0 */
#define RUDIMENTS_HAS_EVP_PKEY_GET0 1

/* Rudiments supports GSS */
#define RUDIMENTS_HAS_GSS 1

/* GSS has gssapi/gssapi_ext.h */
#define RUDIMENTS_HAS_GSSAPI_GSSAPI_EXT_H 1

/* GSS has gssapi/gssapi_generic.h */
#define RUDIMENTS_HAS_GSSAPI_GSSAPI_GENERIC_H 1

/* GSS has gssapi/gssapi.h */
#define RUDIMENTS_HAS_GSSAPI_GSSAPI_H 1

/* GSS has gssapi.h */
#define RUDIMENTS_HAS_GSSAPI_H 1

/* GSS has GSS_C_NT_ANONYMOUS */
#define RUDIMENTS_HAS_GSS_C_NT_ANONYMOUS 1

/* GSS has gss name types */
#define RUDIMENTS_HAS_GSS_NAME_TYPES 1

/* GSS has gss_oid_to_str */
#define RUDIMENTS_HAS_GSS_OID_TO_STR 1

/* GSS has gss_release_oid */
#define RUDIMENTS_HAS_GSS_RELEASE_OID 1

/* GSS has gss_str_to_oid */
#define RUDIMENTS_HAS_GSS_STR_TO_OID 1

/* Rudiments supports libcurl */
#define RUDIMENTS_HAS_LIBCURL 1

/* SSL has MD5_Init/Update/Final returning int */
#define RUDIMENTS_HAS_MD5_INIT_RETURNING_INT 1

/* Rudiments supports PCRE */
#define RUDIMENTS_HAS_PCRE 1

/* SSL has SHA1_Init/Update/Final returning int */
#define RUDIMENTS_HAS_SHA1_INIT_RETURNING_INT 1

/* SSL has SHA256_CTX */
#define RUDIMENTS_HAS_SHA256_CTX 1

/* SSL has SHA256_Init/Update/Final returning int */
#define RUDIMENTS_HAS_SHA256_INIT_RETURNING_INT 1

/* Rudiments supports SSL */
#define RUDIMENTS_HAS_SSL 1

/* SSL has SSLv23_method */
#define RUDIMENTS_HAS_SSLV23_METHOD 1

/* SSL has SSLv2_method */
/* #undef RUDIMENTS_HAS_SSLV2_METHOD */

/* SSL has SSLv3_method */
/* #undef RUDIMENTS_HAS_SSLV3_METHOD */

/* SSL has dNSName */
#define RUDIMENTS_HAS_SSL_DNSNAME 1

/* Some systems have strftime */
#define RUDIMENTS_HAS_STRFTIME 1

/* Rudiments supports threads */
#define RUDIMENTS_HAS_THREADS 1

/* Some systems have timezone */
#define RUDIMENTS_HAS_TIMEZONE 1

/* SSL has TLSv1_1_method */
/* #undef RUDIMENTS_HAS_TLSV1_1_METHOD */

/* SSL has TLSv1_2_method */
/* #undef RUDIMENTS_HAS_TLSV1_2_METHOD */

/* SSL has TLSv1_method */
/* #undef RUDIMENTS_HAS_TLSV1_METHOD */

/* SSL has TLS_method */
#define RUDIMENTS_HAS_TLS_METHOD 1

/* Some systems define tm_gmtoff in their struct tm */
#define RUDIMENTS_HAS_TM_GMTOFF 1

/* Some systems define tm_name in their struct tm */
/* #undef RUDIMENTS_HAS_TM_NAME */

/* Some systems define tm_tzadj in their struct tm */
/* #undef RUDIMENTS_HAS_TM_TZADJ */

/* Some systems define tm_zone in their struct tm */
#define RUDIMENTS_HAS_TM_ZONE 1

/* Some systems have tzname */
#define RUDIMENTS_HAS_TZNAME 1

/* Some systems have tzset */
#define RUDIMENTS_HAS_TZSET 1

/* Some systems have tzset but dont define it */
/* #undef RUDIMENTS_HAS_UNDEFINED_TZSET */

/* SSL has X509_get_signature_nid */
#define RUDIMENTS_HAS_X509_GET_SIGNATURE_NID 1

/* Some systems have _get_timezone */
/* #undef RUDIMENTS_HAS__GET_TIMEZONE */

/* Some systems have _get_tzname */
/* #undef RUDIMENTS_HAS__GET_TZNAME */

/* Some systems have _timezone */
/* #undef RUDIMENTS_HAS__TIMEZONE */

/* Some systems have _tzname */
/* #undef RUDIMENTS_HAS__TZNAME */

/* Some systems have _tzset */
/* #undef RUDIMENTS_HAS__TZSET */

/* Some systems define __tm_gmtoff in their struct tm */
/* #undef RUDIMENTS_HAS___TM_GMTOFF */

/* Some systems define __tm_zone in their struct tm */
/* #undef RUDIMENTS_HAS___TM_ZONE */

/* Some systems have accctrl.h */
/* #undef RUDIMENTS_HAVE_ACCCTRL_H */

/* Most systems have access() */
#define RUDIMENTS_HAVE_ACCESS 1

/* Some systems have aclapi.h */
/* #undef RUDIMENTS_HAVE_ACLAPI_H */

/* Some systems have alarm */
#define RUDIMENTS_HAVE_ALARM 1

/* Some systems have arc4random */
/* #undef RUDIMENTS_HAVE_ARC4RANDOM */

/* Some systems have arpa/inet.h */
#define RUDIMENTS_HAVE_ARPA_INET_H 1

/* Some systems have atexit */
#define RUDIMENTS_HAVE_ATEXIT 1

/* Some systems have backtrace */
#define RUDIMENTS_HAVE_BACKTRACE 1

/* SCO OSR5 has an incorrect struct msghdr definition */
/* #undef RUDIMENTS_HAVE_BAD_SCO_MSGHDR */

/* Some systems dont have st_blocks in struct stat */
#define RUDIMENTS_HAVE_BLKCNT_T 1

/* Some systems dont have st_blksize in struct stat */
#define RUDIMENTS_HAVE_BLKSIZE_T 1

/* Most systems have bool */
#define RUDIMENTS_HAVE_BOOL 1

/* Some systems have broken towctrans */
/* #undef RUDIMENTS_HAVE_BROKEN_TOWCTRANS */

/* Some systems have BRoster::_ShutDown */
/* #undef RUDIMENTS_HAVE_BROSTER__SHUTDOWN */

/* Some systems have bswap64 */
/* #undef RUDIMENTS_HAVE_BSWAP64 */

/* Some systems have bswap_64 */
#define RUDIMENTS_HAVE_BSWAP_64 1

/* Some systems have byteswap.h */
#define RUDIMENTS_HAVE_BYTESWAP_H 1

/* Some systems have CaptureStackBackTrace */
/* #undef RUDIMENTS_HAVE_CAPTURESTACKBACKTRACE */

/* Some systems have char16_t */
#define RUDIMENTS_HAVE_CHAR16_T 1

/* Some systems have chmod */
#define RUDIMENTS_HAVE_CHMOD 1

/* Some systems have chroot */
#define RUDIMENTS_HAVE_CHROOT 1

/* Some systems have clearenv */
#define RUDIMENTS_HAVE_CLEARENV 1

/* Some systems have clock_nanosleep */
#define RUDIMENTS_HAVE_CLOCK_NANOSLEEP 1

/* Some systems have clock_settime */
#define RUDIMENTS_HAVE_CLOCK_SETTIME 1

/* Some systems have close */
#define RUDIMENTS_HAVE_CLOSE 1

/* some systems have closesocket */
/* #undef RUDIMENTS_HAVE_CLOSESOCKET */

/* Some systems have cmsghdr */
#define RUDIMENTS_HAVE_CMSGHDR 1

/* Some systems have _commit */
/* #undef RUDIMENTS_HAVE_COMMIT */

/* connect has a const sockaddr * on most systems */
/* #undef RUDIMENTS_HAVE_CONNECT_WITH_NON_CONST_SOCKADDR */

/* Most systems have ftok(const char *, int) */
#define RUDIMENTS_HAVE_CONST_CHAR_FTOK 1

/* Some systems have CreateDirectory */
/* #undef RUDIMENTS_HAVE_CREATEDIRECTORY */

/* Some systems have CreateFile */
/* #undef RUDIMENTS_HAVE_CREATEFILE */

/* Some systems have CreateNamedPipe */
/* #undef RUDIMENTS_HAVE_CREATENAMEDPIPE */

/* Some systems have CreateSemaphore */
/* #undef RUDIMENTS_HAVE_CREATESEMAPHORE */

/* CreateThread function exists */
/* #undef RUDIMENTS_HAVE_CREATETHREAD */

/* Some systems have CreateTimerQueueTimer */
/* #undef RUDIMENTS_HAVE_CREATETIMERQUEUETIMER */

/* Some systems have CreateFileMapping */
/* #undef RUDIMENTS_HAVE_CREATE_FILE_MAPPING */

/* Some systems have CreateIoCompletionPort */
/* #undef RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT */

/* CreateMutex function exists */
/* #undef RUDIMENTS_HAVE_CREATE_MUTEX */

/* Some systems have CreateProcess */
/* #undef RUDIMENTS_HAVE_CREATE_PROCESS */

/* Some systems have crypt */
#define RUDIMENTS_HAVE_CRYPT 1

/* Some systems have CryptGenRandom */
/* #undef RUDIMENTS_HAVE_CRYPTGENRANDOM */

/* Some systems have crypt.h */
#define RUDIMENTS_HAVE_CRYPT_H 1

/* Some systems have crypt_r */
#define RUDIMENTS_HAVE_CRYPT_R 1

/* Some systems have ctype.h */
#define RUDIMENTS_HAVE_CTYPE_H 1

/* Cygwin style statfs */
/* #undef RUDIMENTS_HAVE_CYGWIN_STATFS */

/* Darwin style statfs */
/* #undef RUDIMENTS_HAVE_DARWIN_STATFS */

/* Some systems have dbghelp.h */
/* #undef RUDIMENTS_HAVE_DBGHELP_H */

/* Some systems have direct.h */
/* #undef RUDIMENTS_HAVE_DIRECT_H */

/* Some systems have dirent.h */
#define RUDIMENTS_HAVE_DIRENT_H 1

/* Some systems have dirfd */
#define RUDIMENTS_HAVE_DIRFD 1

/* Some systems have DIR.dd_fd */
/* #undef RUDIMENTS_HAVE_DIR_DD_FD */

/* Some systems have DIR.d_fd */
/* #undef RUDIMENTS_HAVE_DIR_D_FD */

/* Some systems have dlfcn.h */
#define RUDIMENTS_HAVE_DLFCN_H 1

/* Some systems have dlopen */
#define RUDIMENTS_HAVE_DLOPEN 1

/* Some systems have dup */
#define RUDIMENTS_HAVE_DUP 1

/* Some systems have dup2 */
#define RUDIMENTS_HAVE_DUP2 1

/* Some systems have DuplicateHandle */
/* #undef RUDIMENTS_HAVE_DUPLICATEHANDLE */

/* Some systems define environ in unistd.h */
#define RUDIMENTS_HAVE_ENVIRON 1

/* Some systems have epoll */
#define RUDIMENTS_HAVE_EPOLL 1

/* Some systems have errno.h */
#define RUDIMENTS_HAVE_ERRNO_H 1

/* Some systems have execinfo.h */
#define RUDIMENTS_HAVE_EXECINFO_H 1

/* Some systems have execvp */
#define RUDIMENTS_HAVE_EXECVP 1

/* Some systems have ExitWindowsEx */
/* #undef RUDIMENTS_HAVE_EXITWINDOWSEX */

/* Some systems have fchmod */
#define RUDIMENTS_HAVE_FCHMOD 1

/* Some systems have fchown */
#define RUDIMENTS_HAVE_FCHOWN 1

/* Some systems have fcntl */
#define RUDIMENTS_HAVE_FCNTL 1

/* Some systems have fcntl.h */
#define RUDIMENTS_HAVE_FCNTL_H 1

/* Some systems have fdatasync */
#define RUDIMENTS_HAVE_FDATASYNC 1

/* Some systems have fdopen */
#define RUDIMENTS_HAVE_FDOPEN 1

/* Some systems have FD_CLOEXEC */
#define RUDIMENTS_HAVE_FD_CLOEXEC 1

/* Some systems have FILE->_file */
/* #undef RUDIMENTS_HAVE_FILE_FILE */

/* Some systems have FILE->__filedes */
/* #undef RUDIMENTS_HAVE_FILE_FILEDES */

/* Some systems have FILE->_fileno */
#define RUDIMENTS_HAVE_FILE_FILENO 1

/* Some systems have FILE->_fd */
/* #undef RUDIMENTS_HAVE_FILE__FD */

/* Some systems have FILE->__file */
/* #undef RUDIMENTS_HAVE_FILE__FILE */

/* Some systems have FindFirstFile */
/* #undef RUDIMENTS_HAVE_FINDFIRSTFILE */

/* Some systems have FlushFileBuffers */
/* #undef RUDIMENTS_HAVE_FLUSHFILEBUFFERS */

/* Some systems have fork */
#define RUDIMENTS_HAVE_FORK 1

/* Some systems have fpathconf */
#define RUDIMENTS_HAVE_FPATHCONF 1

/* FreeBSD style statfs */
/* #undef RUDIMENTS_HAVE_FREEBSD_STATFS */

/* Some systems have FreeConsole */
/* #undef RUDIMENTS_HAVE_FREECONSOLE */

/* Some systems have fsync */
#define RUDIMENTS_HAVE_FSYNC 1

/* Most systems have ftok() */
#define RUDIMENTS_HAVE_FTOK 1

/* Some systems have ftruncate */
#define RUDIMENTS_HAVE_FTRUNCATE 1

/* Some systems have GenerateConsoleCtrlEvent */
/* #undef RUDIMENTS_HAVE_GENERATECONSOLECTRLEVENT */

/* Some systems have GetAce */
/* #undef RUDIMENTS_HAVE_GETACE */

/* Some systems have getaddrinfo */
#define RUDIMENTS_HAVE_GETADDRINFO 1

/* Some systems have GetCurrentDirectory */
/* #undef RUDIMENTS_HAVE_GETCURRENTDIRECTORY */

/* Some systems have GetCurrentProcessId */
/* #undef RUDIMENTS_HAVE_GETCURRENTPROCESSID */

/* Some systems have getegid */
#define RUDIMENTS_HAVE_GETEGID 1

/* Some systems have getenv */
#define RUDIMENTS_HAVE_GETENV 1

/* Some systems have geteuid */
#define RUDIMENTS_HAVE_GETEUID 1

/* Some systems have GetFileInformationByHandle */
/* #undef RUDIMENTS_HAVE_GETFILEINFORMATIONBYHANDLE */

/* Some systems have GetFileType */
/* #undef RUDIMENTS_HAVE_GETFILETYPE */

/* Some systems have getgid */
#define RUDIMENTS_HAVE_GETGID 1

/* Some systems have getgrgid_r */
#define RUDIMENTS_HAVE_GETGRGID_R 1

/* Some systems have getgrgid_r */
/* #undef RUDIMENTS_HAVE_GETGRGID_R_4 */

/* Some systems have getgrgid_r */
#define RUDIMENTS_HAVE_GETGRGID_R_5 1

/* Some systems have getgrnam_r */
#define RUDIMENTS_HAVE_GETGRNAM_R 1

/* Some systems have getgrnam_r */
/* #undef RUDIMENTS_HAVE_GETGRNAM_R_4 */

/* Some systems have getgrnam_r */
#define RUDIMENTS_HAVE_GETGRNAM_R_5 1

/* Some systems have gethostbyaddr_r */
#define RUDIMENTS_HAVE_GETHOSTBYADDR_R 1

/* Some systems have gethostbyaddr_r */
/* #undef RUDIMENTS_HAVE_GETHOSTBYADDR_R_7 */

/* Some systems have gethostbyaddr_r */
#define RUDIMENTS_HAVE_GETHOSTBYADDR_R_8 1

/* Some systems have gethostbyname_r */
#define RUDIMENTS_HAVE_GETHOSTBYNAME_R 1

/* Some systems have gethostbyname_r */
/* #undef RUDIMENTS_HAVE_GETHOSTBYNAME_R_5 */

/* Some systems have gethostbyname_r */
#define RUDIMENTS_HAVE_GETHOSTBYNAME_R_6 1

/* Some systems have gethostname */
#define RUDIMENTS_HAVE_GETHOSTNAME 1

/* Some systems have GetLastError */
/* #undef RUDIMENTS_HAVE_GETLASTERROR */

/* Some systems have getloadavg */
#define RUDIMENTS_HAVE_GETLOADAVG 1

/* Some systems have GetLogicalProcessorInformation */
/* #undef RUDIMENTS_HAVE_GETLOGICALPROCESSORINFORMATION */

/* Some systems have getnameinfo */
#define RUDIMENTS_HAVE_GETNAMEINFO 1

/* Some systems have GetNativeSystemInfo */
/* #undef RUDIMENTS_HAVE_GETNATIVESYSTEMINFO */

/* Some systems have getpagesize */
#define RUDIMENTS_HAVE_GETPAGESIZE 1

/* Some systems have getpeername */
#define RUDIMENTS_HAVE_GETPEERNAME 1

/* Some systems have getpgid */
#define RUDIMENTS_HAVE_GETPGID 1

/* Some systems have getpid */
#define RUDIMENTS_HAVE_GETPID 1

/* Some systems have getppid */
#define RUDIMENTS_HAVE_GETPPID 1

/* Some systems have getprotobyname_r */
#define RUDIMENTS_HAVE_GETPROTOBYNAME_R 1

/* Some systems have getprotobyname_r */
/* #undef RUDIMENTS_HAVE_GETPROTOBYNAME_R_4 */

/* Some systems have getprotobyname_r */
#define RUDIMENTS_HAVE_GETPROTOBYNAME_R_5 1

/* Some systems have getprotobynumber_r */
#define RUDIMENTS_HAVE_GETPROTOBYNUMBER_R 1

/* Some systems have getprotobynumber_r */
/* #undef RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_4 */

/* Some systems have getprotobynumber_r */
#define RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_5 1

/* Some systems have getpwnam_r */
#define RUDIMENTS_HAVE_GETPWNAM_R 1

/* Some systems have getpwnam_r */
/* #undef RUDIMENTS_HAVE_GETPWNAM_R_4 */

/* Some systems have getpwnam_r */
#define RUDIMENTS_HAVE_GETPWNAM_R_5 1

/* Some systems have getpwuid_r */
#define RUDIMENTS_HAVE_GETPWUID_R 1

/* Some systems have getpwuid_r */
/* #undef RUDIMENTS_HAVE_GETPWUID_R_4 */

/* Some systems have getpwuid_r */
#define RUDIMENTS_HAVE_GETPWUID_R_5 1

/* Some systems have GetSecurityInfo */
/* #undef RUDIMENTS_HAVE_GETSECURITYINFO */

/* Some systems have getservbyname_r */
#define RUDIMENTS_HAVE_GETSERVBYNAME_R 1

/* Some systems have getservbyname_r */
/* #undef RUDIMENTS_HAVE_GETSERVBYNAME_R_5 */

/* Some systems have getservbyname_r */
#define RUDIMENTS_HAVE_GETSERVBYNAME_R_6 1

/* Some systems have getservbyport_r */
#define RUDIMENTS_HAVE_GETSERVBYPORT_R 1

/* Some systems have getservbyport_r */
/* #undef RUDIMENTS_HAVE_GETSERVBYPORT_R_5 */

/* Some systems have getservbyport_r */
#define RUDIMENTS_HAVE_GETSERVBYPORT_R_6 1

/* Some systems have getsid */
#define RUDIMENTS_HAVE_GETSID 1

/* Some systems have getsockopt */
#define RUDIMENTS_HAVE_GETSOCKOPT 1

/* Some systems have getspnam */
/* #undef RUDIMENTS_HAVE_GETSPNAM */

/* Some systems have getspnam_r */
#define RUDIMENTS_HAVE_GETSPNAM_R 1

/* Some systems have getspnam_r */
/* #undef RUDIMENTS_HAVE_GETSPNAM_R_4 */

/* Some systems have getspnam_r */
#define RUDIMENTS_HAVE_GETSPNAM_R_5 1

/* Some systems have GetSystemInfo */
/* #undef RUDIMENTS_HAVE_GETSYSTEMINFO */

/* Some systems have GetSystemTimeAsFileTime */
/* #undef RUDIMENTS_HAVE_GETSYSTEMTIMEASFILETIME */

/* Some systems have gettimeofday */
#define RUDIMENTS_HAVE_GETTIMEOFDAY 1

/* Some systems have getuid */
#define RUDIMENTS_HAVE_GETUID 1

/* Some systems have GetUserName */
/* #undef RUDIMENTS_HAVE_GETUSERNAME */

/* Some systems have GetVersionEx */
/* #undef RUDIMENTS_HAVE_GETVERSIONEX */

/* Some systems have glob */
#define RUDIMENTS_HAVE_GLOB 1

/* Some systems have GlobalMemoryStatusEx */
/* #undef RUDIMENTS_HAVE_GLOBALMEMORYSTATUSEX */

/* Some systems have glob.h */
#define RUDIMENTS_HAVE_GLOB_H 1

/* Some systems have GNU strerror_r */
#define RUDIMENTS_HAVE_GNU_STRERROR_R 1

/* Some systems have grp.h */
#define RUDIMENTS_HAVE_GRP_H 1

/* Some systems have _G_config.h */
/* #undef RUDIMENTS_HAVE_G_CONFIG_H */

/* Some systems have HANDLE_FLAG_INHERIT */
/* #undef RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT */

/* Some systems have htonll */
/* #undef RUDIMENTS_HAVE_HTONLL */

/* Some systems have inet_addr */
#define RUDIMENTS_HAVE_INET_ADDR 1

/* Some systems have inet_aton */
#define RUDIMENTS_HAVE_INET_ATON 1

/* Some systems have InitiateSystemShutdownEx */
/* #undef RUDIMENTS_HAVE_INITIATESYSTEMSHUTDOWNEX */

/* Some systems have int16_t */
#define RUDIMENTS_HAVE_INT16_T 1

/* Some systems have int32_t */
#define RUDIMENTS_HAVE_INT32_T 1

/* Some systems have int64_t */
#define RUDIMENTS_HAVE_INT64_T 1

/* Some systems have int8_t */
#define RUDIMENTS_HAVE_INT8_T 1

/* Some systems have inttypes.h */
#define RUDIMENTS_HAVE_INTTYPES_H 1

/* Some systems have in_addr_t type */
#define RUDIMENTS_HAVE_IN_ADDR_T 1

/* Some systems have ioctl */
#define RUDIMENTS_HAVE_IOCTL 1

/* Some systems have ioctlsocket */
/* #undef RUDIMENTS_HAVE_IOCTLSOCKET */

/* Some systems have io.h */
/* #undef RUDIMENTS_HAVE_IO_H */

/* Some systems have iswascii */
/* #undef RUDIMENTS_HAVE_ISWASCII */

/* Some systems have iswblank */
/* #undef RUDIMENTS_HAVE_ISWBLANK */

/* Some systems have kill */
#define RUDIMENTS_HAVE_KILL 1

/* Some systems have kqueue */
/* #undef RUDIMENTS_HAVE_KQUEUE */

/* Some systems have void * kqueue user data */
/* #undef RUDIMENTS_HAVE_KQUEUE_VOID_UDATA */

/* Do we have libedit */
#define RUDIMENTS_HAVE_LIBEDIT 1

/* Some systems have limits.h */
#define RUDIMENTS_HAVE_LIMITS_H 1

/* Some systems have link */
#define RUDIMENTS_HAVE_LINK 1

/* Linux libc4 style statfs */
/* #undef RUDIMENTS_HAVE_LINUX_LIBC4_STATFS */

/* Some systems have linux/reboot.h */
#define RUDIMENTS_HAVE_LINUX_REBOOT_H 1

/* Some systems have linux/rtc.h */
#define RUDIMENTS_HAVE_LINUX_RTC_H 1

/* Linux style statfs */
#define RUDIMENTS_HAVE_LINUX_STATFS 1

/* Some systems have llabs */
#define RUDIMENTS_HAVE_LLABS 1

/* Some systems have lm.h */
/* #undef RUDIMENTS_HAVE_LM_H */

/* Some systems have LoadLibrary */
/* #undef RUDIMENTS_HAVE_LOADLIBRARY */

/* Some systems have LoadLibraryEx */
/* #undef RUDIMENTS_HAVE_LOADLIBRARYEX */

/* Some systems have locale.h */
#define RUDIMENTS_HAVE_LOCALE_H 1

/* Some systems have localtime_r */
#define RUDIMENTS_HAVE_LOCALTIME_R 1

/* Some systems have localtime_s */
/* #undef RUDIMENTS_HAVE_LOCALTIME_S */

/* Some systems have LockFileEx */
/* #undef RUDIMENTS_HAVE_LOCKFILEEX */

/* Most systems have long long */
#define RUDIMENTS_HAVE_LONG_LONG 1

/* Some systems have lrand48 */
#define RUDIMENTS_HAVE_LRAND48 1

/* Some systems have lrand48_r */
#define RUDIMENTS_HAVE_LRAND48_R 1

/* Some systems have lseek */
#define RUDIMENTS_HAVE_LSEEK 1

/* Some systems have machine/endian.h */
/* #undef RUDIMENTS_HAVE_MACHINE_ENDIAN_H */

/* Some systems have mach-o/dyld.h */
/* #undef RUDIMENTS_HAVE_MACH_O_DYLD_H */

/* Some systems have madvise */
#define RUDIMENTS_HAVE_MADVISE 1

/* Some systems use caddr_t argument for madvise */
/* #undef RUDIMENTS_HAVE_MADVISE_CADDR_T */

/* Some systems have mas.h */
/* #undef RUDIMENTS_HAVE_MAS_H */

/* Some systems have mas_open */
/* #undef RUDIMENTS_HAVE_MAS_OPEN */

/* Some systems have mbrtowc */
#define RUDIMENTS_HAVE_MBRTOWC 1

/* Some systems have mbtowc */
#define RUDIMENTS_HAVE_MBTOWC 1

/* Some systems have memccpy with char argument */
#define RUDIMENTS_HAVE_MEMCCPY_CHAR 1

/* Some systems have memmem */
#define RUDIMENTS_HAVE_MEMMEM 1

/* Some systems have memrchr */
#define RUDIMENTS_HAVE_MEMRCHR 1

/* Some systems have metreg.h */
/* #undef RUDIMENTS_HAVE_METREG_H */

/* Some systems have mincore */
#define RUDIMENTS_HAVE_MINCORE 1

/* Some systems use caddr_t argument for mincore */
/* #undef RUDIMENTS_HAVE_MINCORE_CADDR_T */

/* Some systems have mincore with char argument */
/* #undef RUDIMENTS_HAVE_MINCORE_CHAR */

/* Some systems have mincore with unsigned char argument */
#define RUDIMENTS_HAVE_MINCORE_UCHAR 1

/* statvfs */
/* #undef RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS */

/* Some systems have sethostname but forgot to declare it */
/* #undef RUDIMENTS_HAVE_MISSING_SETHOSTNAME_DECLARATION */

/* Some systems have mkdir */
/* #undef RUDIMENTS_HAVE_MKDIR_1 */

/* Some systems have mkdir */
#define RUDIMENTS_HAVE_MKDIR_2 1

/* Some systems have mkdtemp */
#define RUDIMENTS_HAVE_MKDTEMP 1

/* Some systems have mkfifo */
#define RUDIMENTS_HAVE_MKFIFO 1

/* Some systems have mknod */
#define RUDIMENTS_HAVE_MKNOD 1

/* Some systems have mkstemp */
#define RUDIMENTS_HAVE_MKSTEMP 1

/* Some systems have mlock */
#define RUDIMENTS_HAVE_MLOCK 1

/* Some systems have mlockall */
#define RUDIMENTS_HAVE_MLOCKALL 1

/* Some systems use caddr_t argument for mlock */
/* #undef RUDIMENTS_HAVE_MLOCK_CADDR_T */

/* Some systems have mmap */
#define RUDIMENTS_HAVE_MMAP 1

/* Some systems have mprotect */
#define RUDIMENTS_HAVE_MPROTECT 1

/* Some systems use caddr_t argument for mprotect */
/* #undef RUDIMENTS_HAVE_MPROTECT_CADDR_T */

/* Some msghdr structs have a msg_accrights member */
/* #undef RUDIMENTS_HAVE_MSGHDR_MSG_ACCRIGHTS */

/* Some msghdr structs have a msg_controllen member */
#define RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN 1

/* Some msghdr structs have a msg_flags member */
#define RUDIMENTS_HAVE_MSGHDR_MSG_FLAGS 1

/* Some systems have msync */
#define RUDIMENTS_HAVE_MSYNC 1

/* Some systems use caddr_t argument for msync */
/* #undef RUDIMENTS_HAVE_MSYNC_CADDR_T */

/* Some systems have munlock */
#define RUDIMENTS_HAVE_MUNLOCK 1

/* Some systems have munlockall */
#define RUDIMENTS_HAVE_MUNLOCKALL 1

/* Some systems use caddr_t argument for munlock */
/* #undef RUDIMENTS_HAVE_MUNLOCK_CADDR_T */

/* Some systems use caddr_t argument for munmap */
/* #undef RUDIMENTS_HAVE_MUNMAP_CADDR_T */

/* Some systems have nanosleep */
#define RUDIMENTS_HAVE_NANOSLEEP 1

/* NetBSD style statfs */
/* #undef RUDIMENTS_HAVE_NETBSD_STATFS */

/* NetBSD-3.0 style statvfs */
/* #undef RUDIMENTS_HAVE_NETBSD_STATVFS */

/* Some systems have netdb.h */
#define RUDIMENTS_HAVE_NETDB_H 1

/* Some systems have NetGroupGetInfo */
/* #undef RUDIMENTS_HAVE_NETGROUPGETINFO */

/* Some systems have netinet/in.h */
#define RUDIMENTS_HAVE_NETINET_IN_H 1

/* Some systems have netinet/tcp.h */
#define RUDIMENTS_HAVE_NETINET_TCP_H 1

/* Some systems have NetUserGetInfo */
/* #undef RUDIMENTS_HAVE_NETUSERGETINFO */

/* Some systems have new */
#define RUDIMENTS_HAVE_NEW 1

/* Some systems have new.h */
/* #undef RUDIMENTS_HAVE_NEW_H */

/* Some systems define _NSGetEnviron in crt_externs.h */
/* #undef RUDIMENTS_HAVE_NSGETENVIRON */

/* Some systems have NSLinkModule */
/* #undef RUDIMENTS_HAVE_NSLINKMODULE */

/* Some systems have ntohll */
/* #undef RUDIMENTS_HAVE_NTOHLL */

/* Some systems have open */
#define RUDIMENTS_HAVE_OPEN 1

/* OpenBSD style statfs */
/* #undef RUDIMENTS_HAVE_OPENBSD_STATFS */

/* Some systems have OSSwapHostToLittleInt64 */
/* #undef RUDIMENTS_HAVE_OSSWAPHOSTTOLITTLEINT64 */

/* Some systems have OSSwapLittleToHostInt64 */
/* #undef RUDIMENTS_HAVE_OSSWAPLITTLETOHOSTINT64 */

/* Some systems have os/kernel/OS.h */
/* #undef RUDIMENTS_HAVE_OS_KERNEL_OS_H */

/* Some systems have os/support/ByteOrder.h */
/* #undef RUDIMENTS_HAVE_OS_SUPPORT_BYTEORDER_H */

/* Some systems have pathconf */
#define RUDIMENTS_HAVE_PATHCONF 1

/* Some systems have pipe */
#define RUDIMENTS_HAVE_PIPE 1

/* Some systems have poll */
#define RUDIMENTS_HAVE_POLL 1

/* Some systems have poll.h */
#define RUDIMENTS_HAVE_POLL_H 1

/* Some systems have port_create */
/* #undef RUDIMENTS_HAVE_PORT_CREATE */

/* Some systems have port.h */
/* #undef RUDIMENTS_HAVE_PORT_H */

/* Some systems have posix_fadvise */
#define RUDIMENTS_HAVE_POSIX_FADVISE 1

/* Some systems have posix_fallocate */
#define RUDIMENTS_HAVE_POSIX_FALLOCATE 1

/* Some systems have Process32First */
/* #undef RUDIMENTS_HAVE_PROCESS32FIRST */

/* Some systems have process.h */
/* #undef RUDIMENTS_HAVE_PROCESS_H */

/* Some systems have prototypes.h */
/* #undef RUDIMENTS_HAVE_PROTOTYPES_H */

/* Some systems have psapi.h */
/* #undef RUDIMENTS_HAVE_PSAPI_H */

/* Some systems have pselect */
#define RUDIMENTS_HAVE_PSELECT 1

/* Some systems have pthread_attr_setstacksize */
#define RUDIMENTS_HAVE_PTHREAD_ATTR_SETSTACKSIZE 1

/* Some systems have pthread_kill */
#define RUDIMENTS_HAVE_PTHREAD_KILL 1

/* pthread_mutex_t type exists */
#define RUDIMENTS_HAVE_PTHREAD_MUTEX_T 1

/* pthread_sigmask exists */
#define RUDIMENTS_HAVE_PTHREAD_SIGMASK 1

/* pthread_t type exists */
#define RUDIMENTS_HAVE_PTHREAD_T 1

/* Some systems have putenv */
#define RUDIMENTS_HAVE_PUTENV 1

/* Some systems have raise */
#define RUDIMENTS_HAVE_RAISE 1

/* Some systems have rand */
#define RUDIMENTS_HAVE_RAND 1

/* Some systems have random */
#define RUDIMENTS_HAVE_RANDOM 1

/* Some systems have random_r */
#define RUDIMENTS_HAVE_RANDOM_R 1

/* Some systems have rand_r */
#define RUDIMENTS_HAVE_RAND_R 1

/* Some systems have RB_AUTOBOOT */
#define RUDIMENTS_HAVE_RB_AUTOBOOT 1

/* Some systems have RB_HALT */
/* #undef RUDIMENTS_HAVE_RB_HALT */

/* Some systems have read */
#define RUDIMENTS_HAVE_READ 1

/* Some systems have readlink */
#define RUDIMENTS_HAVE_READLINK 1

/* Some systems have reboot with 1 argument */
#define RUDIMENTS_HAVE_REBOOT_1 1

/* Some systems have reboot with 2 argument */
/* #undef RUDIMENTS_HAVE_REBOOT_2 */

/* Some systems have recvmsg */
#define RUDIMENTS_HAVE_RECVMSG 1

/* recv has a void * on most systems */
#define RUDIMENTS_HAVE_RECV_WITH_VOID 1

/* some systems have regcomp */
/* #undef RUDIMENTS_HAVE_REGCOMP */

/* Some systems have RemoveDirectory */
/* #undef RUDIMENTS_HAVE_REMOVEDIRECTORY */

/* Some systems have Roster.h */
/* #undef RUDIMENTS_HAVE_ROSTER_H */

/* Do we have real-time-clock RTC_RD_TIME support */
#define RUDIMENTS_HAVE_RTC_RD_TIME 1

/* Do we have real-time-clock RTC_SET_TIME support */
#define RUDIMENTS_HAVE_RTC_SET_TIME 1

/* On some systems you cant include byteswap.h after netinet/in.h */
#define RUDIMENTS_HAVE_SAFE_BYTESWAP_H_AFTER_NETINET_IN_H 1

/* SCO has /dev/table/avenrun instead of getloadavg */
/* #undef RUDIMENTS_HAVE_SCO_AVENRUN */

/* Some systems have sddl.h */
/* #undef RUDIMENTS_HAVE_SDDL_H */

/* Some systems have semaphore.h */
#define RUDIMENTS_HAVE_SEMAPHORE_H 1

/* Some systems have semget */
#define RUDIMENTS_HAVE_SEMGET 1

/* Some systems have semtimedop */
#define RUDIMENTS_HAVE_SEMTIMEDOP 1

/* Some systems define union semun */
/* #undef RUDIMENTS_HAVE_SEMUN */

/* Some systems have sem_init */
#define RUDIMENTS_HAVE_SEM_INIT 1

/* Some systems have sendmsg */
#define RUDIMENTS_HAVE_SENDMSG 1

/* send has a const void * on most systems */
#define RUDIMENTS_HAVE_SEND_WITH_VOID 1

/* Some systems have SetConsoleCtrlHandler */
/* #undef RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER */

/* Some systems have SetCurrentDirectory */
/* #undef RUDIMENTS_HAVE_SETCURRENTDIRECTORY */

/* Some systems have setegid */
#define RUDIMENTS_HAVE_SETEGID 1

/* Some systems have setegid but dont define it in a header file */
/* #undef RUDIMENTS_HAVE_SETEGID_BUT_NOT_DEFINED */

/* Some systems have SetEndOfFile */
/* #undef RUDIMENTS_HAVE_SETENDOFFILE */

/* Some systems have SetEntriesInAcl */
/* #undef RUDIMENTS_HAVE_SETENTRIESINACL */

/* Some systems have setenv */
#define RUDIMENTS_HAVE_SETENV 1

/* Some systems have seteuid */
#define RUDIMENTS_HAVE_SETEUID 1

/* Some systems have seteuid but dont define it in a header file */
/* #undef RUDIMENTS_HAVE_SETEUID_BUT_NOT_DEFINED */

/* Some systems have SetFileTime */
/* #undef RUDIMENTS_HAVE_SETFILETIME */

/* Some systems have setgid */
#define RUDIMENTS_HAVE_SETGID 1

/* Some systems have setgroups */
#define RUDIMENTS_HAVE_SETGROUPS 1

/* Some systems have sethostname */
#define RUDIMENTS_HAVE_SETHOSTNAME 1

/* Some systems have setitimer */
/* #undef RUDIMENTS_HAVE_SETITIMER */

/* Some systems have setpgid */
#define RUDIMENTS_HAVE_SETPGID 1

/* Some systems have setregid */
#define RUDIMENTS_HAVE_SETREGID 1

/* Some systems have setreuid */
#define RUDIMENTS_HAVE_SETREUID 1

/* Some systems have SetSecurityInfo */
/* #undef RUDIMENTS_HAVE_SETSECURITYINFO */

/* Some systems have setsid */
#define RUDIMENTS_HAVE_SETSID 1

/* Some systems have setsockopt */
#define RUDIMENTS_HAVE_SETSOCKOPT 1

/* Some systems have SetSystemTime */
/* #undef RUDIMENTS_HAVE_SETSYSTEMTIME */

/* Some systems have settimeofday */
#define RUDIMENTS_HAVE_SETTIMEOFDAY 1

/* Some systems have setuid */
#define RUDIMENTS_HAVE_SETUID 1

/* Some systems have SetUnhandledExceptionFilter */
/* #undef RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER */

/* Some systems have set_real_time_clock */
/* #undef RUDIMENTS_HAVE_SET_REAL_TIME_CLOCK */

/* Some systems have shmget */
#define RUDIMENTS_HAVE_SHMGET 1

/* Some systems have sigaction */
#define RUDIMENTS_HAVE_SIGACTION 1

/* Some systems have signal */
#define RUDIMENTS_HAVE_SIGNAL 1

/* Some systems have sigpending */
#define RUDIMENTS_HAVE_SIGPENDING 1

/* Some systems have sigsuspend */
#define RUDIMENTS_HAVE_SIGSUSPEND 1

/* Some systems have sockaddr_un */
#define RUDIMENTS_HAVE_SOCKADDR_UN 1

/* Some systems have socket */
#define RUDIMENTS_HAVE_SOCKET 1

/* some type of statfs */
#define RUDIMENTS_HAVE_SOME_KIND_OF_STATFS 1

/* some type of statvfs */
/* #undef RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS */

/* struct spwd has sp_expire */
#define RUDIMENTS_HAVE_SP_EXPIRE 1

/* struct spwd has sp_flag */
#define RUDIMENTS_HAVE_SP_FLAG 1

/* struct spwd has sp_inact */
#define RUDIMENTS_HAVE_SP_INACT 1

/* struct spwd has sp_warn */
#define RUDIMENTS_HAVE_SP_WARN 1

/* statvfs */
/* #undef RUDIMENTS_HAVE_STATVFS */

/* statvfs has f_b/fresvd */
/* #undef RUDIMENTS_HAVE_STATVFS_RESVD */

/* Some systems have stdarg.h */
#define RUDIMENTS_HAVE_STDARG_H 1

/* Some systems have stdint.h */
#define RUDIMENTS_HAVE_STDINT_H 1

/* Some systems have stdlib.h */
#define RUDIMENTS_HAVE_STDLIB_H 1

/* Some systems have strcasecmp */
#define RUDIMENTS_HAVE_STRCASECMP 1

/* Some systems have strchrnul */
#define RUDIMENTS_HAVE_STRCHRNUL 1

/* Some systems have strcspn */
#define RUDIMENTS_HAVE_STRCSPN 1

/* Some systems have strerror */
#define RUDIMENTS_HAVE_STRERROR 1

/* Some systems have strerror_s */
/* #undef RUDIMENTS_HAVE_STRERROR_S */

/* Some systems have strings.h */
#define RUDIMENTS_HAVE_STRINGS_H 1

/* Some systems have string.h */
#define RUDIMENTS_HAVE_STRING_H 1

/* Some systems have strncasecmp */
#define RUDIMENTS_HAVE_STRNCASECMP 1

/* Some systems have strpbrk */
#define RUDIMENTS_HAVE_STRPBRK 1

/* Some systems have strspn */
#define RUDIMENTS_HAVE_STRSPN 1

/* Some systems have strtold */
#define RUDIMENTS_HAVE_STRTOLD 1

/* Some systems have strtoll */
#define RUDIMENTS_HAVE_STRTOLL 1

/* Some systems have strtoull */
#define RUDIMENTS_HAVE_STRTOULL 1

/* Some systems have struct itimerval */
/* #undef RUDIMENTS_HAVE_STRUCT_ITIMERVAL */

/* Some systems have struct timespec in pthread.h */
#define RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_PTHREAD 1

/* Some systems have struct timespec in sys/siginfo.h */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SIGINFO */

/* Some systems have struct timespec in sys/time.h */
#define RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SYS_TIME_H 1

/* Some systems have struct timespec in time.h */
#define RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_TIME_H 1

/* Some systems have struct timeval */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMEVAL */

/* Some systems have swab with a char * argument */
#define RUDIMENTS_HAVE_SWAB_CHAR 1

/* Some systems have swab with a const char * argument */
#define RUDIMENTS_HAVE_SWAB_CONST_CHAR 1

/* Some systems have swab with a const void * argument */
#define RUDIMENTS_HAVE_SWAB_CONST_VOID 1

/* Some systems have swap64 */
/* #undef RUDIMENTS_HAVE_SWAP64 */

/* Some systems have __swap_int64 */
/* #undef RUDIMENTS_HAVE_SWAP_INT64 */

/* Some systems have swprintf */
#define RUDIMENTS_HAVE_SWPRINTF 1

/* Some systems have symlink */
#define RUDIMENTS_HAVE_SYMLINK 1

/* Some systems have sync */
#define RUDIMENTS_HAVE_SYNC 1

/* Some systems have sysconf */
#define RUDIMENTS_HAVE_SYSCONF 1

/* Some systems have syslog.h */
#define RUDIMENTS_HAVE_SYSLOG_H 1

/* Some systems have sys/bitypes.h */
#define RUDIMENTS_HAVE_SYS_BITYPES_H 1

/* Some systems have sys/byteorder.h */
/* #undef RUDIMENTS_HAVE_SYS_BYTEORDER_H */

/* Some systems have sys/devpoll.h */
/* #undef RUDIMENTS_HAVE_SYS_DEVPOLL_H */

/* Some systems have sys/dl.h */
/* #undef RUDIMENTS_HAVE_SYS_DL_H */

/* Some systems have sys/epoll.h */
#define RUDIMENTS_HAVE_SYS_EPOLL_H 1

/* Some systems have sys/event.h */
/* #undef RUDIMENTS_HAVE_SYS_EVENT_H */

/* Some systems have sys/fcntl.h */
#define RUDIMENTS_HAVE_SYS_FCNTL_H 1

/* Some systems have sys/ioctl.h */
#define RUDIMENTS_HAVE_SYS_IOCTL_H 1

/* Some systems have sys/ipc.h */
#define RUDIMENTS_HAVE_SYS_IPC_H 1

/* Some systems have sys/loadavg.h */
/* #undef RUDIMENTS_HAVE_SYS_LOADAVG_H */

/* Some systems have sys/mman.h */
#define RUDIMENTS_HAVE_SYS_MMAN_H 1

/* Some systems have sys/param.h */
#define RUDIMENTS_HAVE_SYS_PARAM_H 1

/* Some systems have sys/poll.h */
#define RUDIMENTS_HAVE_SYS_POLL_H 1

/* Some systems have sys/reboot.h */
#define RUDIMENTS_HAVE_SYS_REBOOT_H 1

/* Some systems have sys/select.h */
#define RUDIMENTS_HAVE_SYS_SELECT_H 1

/* Some systems have sys/sem.h */
#define RUDIMENTS_HAVE_SYS_SEM_H 1

/* Some systems have sys/signal.h */
#define RUDIMENTS_HAVE_SYS_SIGNAL_H 1

/* Some systems have sys/socket.h */
#define RUDIMENTS_HAVE_SYS_SOCKET_H 1

/* Some systems have sys/stat.h */
#define RUDIMENTS_HAVE_SYS_STAT_H 1

/* Some systems have sys/time.h */
#define RUDIMENTS_HAVE_SYS_TIME_H 1

/* Some systems have sys/types.h */
#define RUDIMENTS_HAVE_SYS_TYPES_H 1

/* Some systems have sys/uadmin.h */
/* #undef RUDIMENTS_HAVE_SYS_UADMIN_H */

/* Some systems have sys/uio.h */
#define RUDIMENTS_HAVE_SYS_UIO_H 1

/* Some systems have sys/un.h */
#define RUDIMENTS_HAVE_SYS_UN_H 1

/* Some systems have sys/utsname.h */
#define RUDIMENTS_HAVE_SYS_UTSNAME_H 1

/* Some systems have sys/wait.h */
#define RUDIMENTS_HAVE_SYS_WAIT_H 1

/* Some systems dont have S_ISBLK */
#define RUDIMENTS_HAVE_S_ISBLK 1

/* Some systems dont have S_ISLNK */
#define RUDIMENTS_HAVE_S_ISLNK 1

/* Some systems dont have S_ISSOCK */
#define RUDIMENTS_HAVE_S_ISSOCK 1

/* Some systems have tcsetattr */
/* #undef RUDIMENTS_HAVE_TCSETATTR */

/* Some systems have termios.h */
#define RUDIMENTS_HAVE_TERMIOS_H 1

/* Some systems have time.h */
#define RUDIMENTS_HAVE_TIME_H 1

/* Some systems have tlhelp32.h */
/* #undef RUDIMENTS_HAVE_TLHELP32_H */

/* Most systems have true/false */
#define RUDIMENTS_HAVE_TRUE_FALSE 1

/* Some systems have truncate */
#define RUDIMENTS_HAVE_TRUNCATE 1

/* Some systems have uadmin with a char * argument */
/* #undef RUDIMENTS_HAVE_UADMIN_CHAR */

/* Some systems have uadmin with a integer argument */
/* #undef RUDIMENTS_HAVE_UADMIN_INT */

/* Some systems have uchar.h */
#define RUDIMENTS_HAVE_UCHAR_H 1

/* Some systems have uint16_t */
#define RUDIMENTS_HAVE_UINT16_T 1

/* Some systems have uint32_t */
#define RUDIMENTS_HAVE_UINT32_T 1

/* Some systems have uint64_t */
#define RUDIMENTS_HAVE_UINT64_T 1

/* Some systems have uint8_t */
#define RUDIMENTS_HAVE_UINT8_T 1

/* Ultrix style statfs */
/* #undef RUDIMENTS_HAVE_ULTRIX_STATFS */

/* Some systems have umask */
#define RUDIMENTS_HAVE_UMASK 1

/* Some systems have uname */
#define RUDIMENTS_HAVE_UNAME 1

/* Some systems have fsync but dont define it */
/* #undef RUDIMENTS_HAVE_UNDEFINED_FSYNC */

/* Some systems have ftruncate but dont define it */
/* #undef RUDIMENTS_HAVE_UNDEFINED_FTRUNCATE */

/* Some systems have getpeername but dont define it */
/* #undef RUDIMENTS_HAVE_UNDEFINED_GETPEERNAME */

/* Some systems have getsockopt but dont define it */
/* #undef RUDIMENTS_HAVE_UNDEFINED_GETSOCKOPT */

/* Some systems have recvmsg but dont define it */
/* #undef RUDIMENTS_HAVE_UNDEFINED_RECVMSG */

/* Some systems have sendmsg but dont define it */
/* #undef RUDIMENTS_HAVE_UNDEFINED_SENDMSG */

/* Some systems have setsockopt but dont define it */
/* #undef RUDIMENTS_HAVE_UNDEFINED_SETSOCKOPT */

/* Some systems have socket but dont define it */
/* #undef RUDIMENTS_HAVE_UNDEFINED_SOCKET */

/* Some systems have __vsnprintf but its not defined */
#define RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF 1

/* Some systems have unistd.h */
#define RUDIMENTS_HAVE_UNISTD_H 1

/* Some systems have unlink */
#define RUDIMENTS_HAVE_UNLINK 1

/* Some systems have unsetenv */
#define RUDIMENTS_HAVE_UNSETENV 1

/* Some systems have utime */
#define RUDIMENTS_HAVE_UTIME 1

/* Some systems have utimes with char * argument */
#define RUDIMENTS_HAVE_UTIMES_CHAR 1

/* Some systems have utimes with const char * argument */
#define RUDIMENTS_HAVE_UTIMES_CONST_CHAR 1

/* Some systems have utime.h */
#define RUDIMENTS_HAVE_UTIME_H 1

/* Some systems have vasprintf */
#define RUDIMENTS_HAVE_VASPRINTF 1

/* Some systems have va_copy */
#define RUDIMENTS_HAVE_VA_COPY 1

/* Some systems have vdprintf */
#define RUDIMENTS_HAVE_VDPRINTF 1

/* Some systems have vfwprintf */
#define RUDIMENTS_HAVE_VFWPRINTF 1

/* Most systems have vsnprintf */
#define RUDIMENTS_HAVE_VSNPRINTF 1

/* Some systems have vsnprintf_s */
/* #undef RUDIMENTS_HAVE_VSNPRINTF_S */

/* Some systems have vswprintf */
#define RUDIMENTS_HAVE_VSWPRINTF 1

/* Some systems have wchar.h */
#define RUDIMENTS_HAVE_WCHAR_H 1

/* Some systems have wcrtomb */
#define RUDIMENTS_HAVE_WCRTOMB 1

/* Some systems have wcscasecmp */
#define RUDIMENTS_HAVE_WCSCASECMP 1

/* Some systems have wcschrnul */
#define RUDIMENTS_HAVE_WCSCHRNUL 1

/* Some systems have wcscspn */
#define RUDIMENTS_HAVE_WCSCSPN 1

/* Some systems have wcsncasecmp */
#define RUDIMENTS_HAVE_WCSNCASECMP 1

/* Some systems have wcspbrk */
#define RUDIMENTS_HAVE_WCSPBRK 1

/* Some systems have wcsspn */
#define RUDIMENTS_HAVE_WCSSPN 1

/* Some systems have wcsstr */
/* #undef RUDIMENTS_HAVE_WCSSTR */

/* Some systems have wcstold */
#define RUDIMENTS_HAVE_WCSTOLD 1

/* Some systems have wcstoll */
#define RUDIMENTS_HAVE_WCSTOLL 1

/* Some systems have wcstoull */
#define RUDIMENTS_HAVE_WCSTOULL 1

/* Some systems have wctomb */
#define RUDIMENTS_HAVE_WCTOMB 1

/* Some systems have wctype.h */
#define RUDIMENTS_HAVE_WCTYPE_H 1

/* GetDiskFreeSpace */
/* #undef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE */

/* Some systems have windows.h */
/* #undef RUDIMENTS_HAVE_WINDOWS_H */

/* Some systems have Sleep */
/* #undef RUDIMENTS_HAVE_WINDOWS_SLEEP */

/* Some systems have winsock2.h */
/* #undef RUDIMENTS_HAVE_WINSOCK2_H */

/* Some systems have write */
#define RUDIMENTS_HAVE_WRITE 1

/* Some systems have WSAConnect */
/* #undef RUDIMENTS_HAVE_WSACONNECT */

/* Some systems have WSADuplicateSocket */
/* #undef RUDIMENTS_HAVE_WSADUPLICATESOCKET */

/* Some systems have XSI strerror_r */
#define RUDIMENTS_HAVE_XSI_STRERROR_R 1

/* Some systems have _access_s() */
/* #undef RUDIMENTS_HAVE__ACCESS_S */

/* Some systems have _chsize_s */
/* #undef RUDIMENTS_HAVE__CHSIZE_S */

/* Some systems have _close */
/* #undef RUDIMENTS_HAVE__CLOSE */

/* Some systems have _dup */
/* #undef RUDIMENTS_HAVE__DUP */

/* Some systems have _dup2 */
/* #undef RUDIMENTS_HAVE__DUP2 */

/* Some systems have _dupenv_s */
/* #undef RUDIMENTS_HAVE__DUPENV_S */

/* Some systems have _lseek */
/* #undef RUDIMENTS_HAVE__LSEEK */

/* Some systems have _memccpy */
/* #undef RUDIMENTS_HAVE__MEMCCPY */

/* Some systems have _open */
/* #undef RUDIMENTS_HAVE__OPEN */

/* Some systems have _pipe */
/* #undef RUDIMENTS_HAVE__PIPE */

/* Some systems have _putenv */
/* #undef RUDIMENTS_HAVE__PUTENV */

/* Some systems have _read */
/* #undef RUDIMENTS_HAVE__READ */

/* Some systems have _swab */
/* #undef RUDIMENTS_HAVE__SWAB */

/* Some systems have _unlink */
/* #undef RUDIMENTS_HAVE__UNLINK */

/* Some systems have _vsnprintf */
/* #undef RUDIMENTS_HAVE__VSNPRINTF */

/* Some systems have _write */
/* #undef RUDIMENTS_HAVE__WRITE */

/* Some systems have __bswap64 */
/* #undef RUDIMENTS_HAVE___BSWAP64 */

/* Some systems have __vsnprintf */
/* #undef RUDIMENTS_HAVE___VSNPRINTF */

/* iov_base is char * */
#define RUDIMENTS_IOV_BASE_TYPE void *

/* Modern versions of libedit provide history_truncate_file */
#define RUDIMENTS_LIBEDIT_HAS_HISTORY_TRUNCATE_FILE 1

/* Some systems have swab but dont define it */
/* #undef RUDIMENTS_MUST_DEFINE_SWAB */

/* Solaris 2.6 has a few missing function prototypes */
/* #undef RUDIMENTS_NEED_XNET_PROTOTYPES */

/* Redfine NULL as 0 */
/* #undef RUDIMENTS_REDEFINE_NULL */

/* setsockopt has a const void * on most systems */
#define RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE const void *

/* Most systems define signal handlers with an integer parameter */
#define RUDIMENTS_SIGNAL_HANDLER_INT 1

/* Use small rather than fast code */
/* #undef RUDIMENTS_SMALL_CODE */

/* getsockopt has a socklen_t * on most systems */
#define RUDIMENTS_SOCKLEN_OR_SIZE_T socklen_t

/* SSL_read/write can use a void * parameter instead of char * */
#define RUDIMENTS_SSL_VOID_PTR 1

/* Version */
#define RUDIMENTS_VERSION "2.0.0"

/* Version */
/* #undef SONAME_VERSION_INFO */

/* Define to 1 if you have the ANSI C header files. */
/* #undef STDC_HEADERS */

/* Hack for gcc on SCO OSR6 */
/* #undef _CFRONT_3_0 */

/* Darwin */
/* #undef _DARWIN */

/* Haiku OS */
/* #undef _HAIKU */

/* Irix OS */
/* #undef _IRIX */

/* Minix */
/* #undef _MINIX */

/* SCO OSR5 OS */
/* #undef _SCO_OSR5 */

/* SCO OSR6 OS */
/* #undef _SCO_OSR6 */

/* SCO UnixWare OS */
/* #undef _SCO_UW */

/* Syllable OS */
/* #undef _SYLLABLE */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef gid_t */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Some systems dont have key_t */
/* #undef key_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Some systems dont have nlink_t */
/* #undef nlink_t */

/* Some systems dont have off64_t */
/* #undef off64_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Some systems dont define socklen_t */
/* #undef socklen_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef ssize_t */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef uid_t */
