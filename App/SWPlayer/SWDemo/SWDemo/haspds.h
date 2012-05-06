/*  $Id: haspds.h,v 1.44 2010-06-12 04:39:20 pwang Exp $
**
**  Copyright (C) 2010, SafeNet, Inc. All rights reserved.
**
**  haspds - API Interface for Sentinel HASP Run-time Environment Setup DLL
**
*/



#ifndef __HASPDS_H__
#define __HASPDS_H__

#pragma pack(push,_haspds_h_,1)

/*
 * data returned by the haspds_GetInfo function
 */
typedef struct Haspds_InfoItem {
    char  FileName[14];
    unsigned long InstalledVersion;
    unsigned long PackageVersion;
} HASPDS_INFO_ITEM, *PHASPDS_INFO_ITEM;

typedef struct HaspdsInfo {
    unsigned long ItemsNo;
    HASPDS_INFO_ITEM Items[1];
} HASPDS_INFO, *PHASPDS_INFO;

typedef struct HaspdsInfoEx {
    unsigned long DrvInstPkgVer;	// installed package driver version
    unsigned long DrvPkgVer;		// contained package driver version
    HASPDS_INFO Info;			// contains old structure 
} HASPDS_INFOEX, *PHASPDS_INFOEX;

/*
 * vendor data that is included in the Run-Time Environment installer,
 * and data that is already installed on the machine
 */
typedef struct Haspds_VendorInfoItem {
    char FileName[128];
    unsigned long PackageVersion;
} HASPDS_VENDOR_INFO_ITEM, *PHASPDS_VENDOR_INFO_ITEM;

typedef struct HaspdsVendorInfo {
    unsigned long ItemsNo;
    HASPDS_VENDOR_INFO_ITEM Items[1];
} HASPDS_VENDOR_INFO, *PHASPDS_VENDOR_INFO;


/*
 * used for haspds_GetClientProcess und haspds_KillClientProcess
 */
#define HASPDS_PROCNAME_LEN         128
typedef struct _HaspdsClientProcess {
    DWORD ProcessType;
    DWORD PID;
    CHAR  Name[HASPDS_PROCNAME_LEN];
    DWORD Reserved1;
    DWORD Reserved2;
} HASPDS_CLIENTPROCESS, *PHASPDS_CLIENTPROCESS;


/*
 * internal errors that can be obtained using haspds_GetLastError function.
 */
typedef enum haspds_error {
    HASPDS_ERR_OK = 0,                    /* Operation successfully completed */
    HASPDS_ERR_NO_ADMIN = 1,              /* Current user does not have administrator rights */
    HASPDS_ERR_INVALID_PARAM = 2,         /* Invalid parameter specified */
    HASPDS_ERR_OS_NOT_SUPPORTED = 3,      /* Current operating system not supported */
    HASPDS_ERR_CAB_PCD = 4,               /* Run-time environment package inconsistent with configuration file */
    HASPDS_ERR_LOAD_LIB = 5,              /* Unable to load required DLL */
    HASPDS_ERR_FCT_PTR = 6,               /* Failed to retrieve pointer to a function */
    HASPDS_ERR_WIN_ERR = 7,               /* Windows API function failed */
    HASPDS_ERR_NO_MEM = 8,                /* Memory allocation failed */
    HASPDS_ERR_MAX_PATH = 9,              /* Specified path exceeded 256 bytes */
    HASPDS_ERR_EOF = 10,                  /* Unexpected end of file reached */
    HASPDS_ERR_INVALID_CFGFILE = 11,      /* Invalid run-time environment package configuration file */
    HASPDS_ERR_FILE_ERROR = 12,           /* File processing error */
    HASPDS_ERR_DISK_SPACE = 13,           /* Insufficient free space on disk */
    HASPDS_ERR_SETUPAPI = 14,             /* Unexpected Windows setup API function failure */
    HASPDS_ERR_UNKNOWN = 15,              /* Unrecognized error */
    HASPDS_ERR_REGISTRY_ACCESS = 16,      /* Registry access error */
    HASPDS_ERR_NEED_REINSERT = 17,        /* To activate the new run-time environment, remove and reinsert HASP HL key */
    HASPDS_ERR_HLSERVER_RUNNING = 18,     /* Running HASP HL server detected. Stop service */
    HASPDS_ERR_HSSERVER_RUNNING = 19,     /* Running HASP License Manager detected. Stop service */
    HASPDS_ERR_STILL_DRV_PROC = 20,       /* Processes the access the run-time environment still running. Stop processes */
    HASPDS_ERR_ALREADY_RUNNING = 21,      /* Another installer instance already running */
    HASPDS_ERR_WIN_SETUP_RUNNING = 22,    /* Another Windows setup process already running */
    HASPDS_ERR_INSERT_REQUIRED = 23,      /* Access to HASP key required to activate the function. Insert HASP HL key */
    HASPDS_ERR_USEFR_REQUIRED = 24,       /* Older installation (HASP legacy) is present. Uninstall with -fr command */
    HASPDS_ERR_USEHINST_REQUIRED = 25,    /* Older installation (HASP legacy) is present. Use the legacy installer to completely uninstall old drivers */
    HASPDS_ERR_REBOOT_REQUIRED = 26,      /* Reboot machine to complete installation */
    HASPDS_SERVICE_NOT_INSTALLED = 27,    /* inconsistent name definition - remains for backward compatibility */
    HASPDS_ERR_SERVICE_NOT_INSTALLED = 27,/* Drivers not installed */
    HASPDS_UNKNOWN_PARAM = 28,            /* inconsistent name definition - remains for backward compatibility */
    HASPDS_ERR_UNKNOWN_PARAM = 28,        /* Unrecognized parameter specified for function */
    HASPDS_ERR_INSTALL_OLD = 29,          /* Current installer package is older than that already installed */
    HASPDS_ERR_SERVICE_NOT_STARTED = 30,  /* Unable to start a Windows service */
    HASPDS_ERR_SERVICE_NOT_STOPED = 31,   /* Unable to stop a Window service */
    HASPDS_ERR_REMOVE_REBOOT_REQ = 32,    /* Operation successful. Reboot machine to completely uninstall drivers */
    HASPDS_ERR_START_PROCESS = 33,        /* start hasplm9x failed */
    HASPDS_ERR_INSTALL_CAT = 34,          /* Unable to install a CAT file */
    HASPDS_ERR_ALREADY_INSTALLED = 35,    /* Drivers already installed */
    HASPDS_ERR_DRV_NEWER = 36,            /* Already-installed drivers are newer */
    HASPDS_ERR_V2C = 37,                  /* error in V@C processing */
    HASPDS_ERR_OPEN_SCMANAGER = 38,       /* Unable to open Service Manager */
    HASPDS_ERR_OPEN_SERVICE = 39,         /* Unable to open service */
    HASPDS_ERR_QUERY_STATUS = 40,         /* Error during service status query */
    HASPDS_ERR_DEL_SRV = 41,              /* Unable to install service */
    HASPDS_ERR_INSTALL_PNP = 42,          /* Unable to install INF file */
    HASPDS_ERR_SET_FW = 43,               /* Unable to open Port 1947 */
    HASPDS_ERR_REMOVE_REBOOT = 44,        /* Following uninstall procedure, reboot required */
    HASPDS_ERR_OLD_BRANDED_INST = 45,     /* Old branded installer detected */
    HASPDS_ERR_LLM_DIR = 46,	          /* Unable to delete a HASP License Manager-generated directory */
    HASPDS_ERR_SETUP_DIR = 47,             /* Unable to write to the windows\system32\setup\aladdin directory */
    HASPDS_ERR_SERVICE_NOT_STARTED_EX = 48,  /* Could not start a service */
    HASPDS_ERR_PURGE_DIRS = 49            /* purge LLM directories */
}haspds_error;

typedef enum haspds_status_t{
    HASPDS_STATUS_SUCCESS = 0,            /* success */
    HASPDS_STATUS_FAILED = 1,             /* function failed */
    HASPDS_STATUS_REBOOT_REQUIRED = 2,    /* reboot required */
    HASPDS_STATUS_SMALL_BUFFER = 3,       /* buffer too small */
    HASPDS_STATUS_REINSERT_REQUIRED = 4,  /* a device reinsertion is required */
    HASPDS_STATUS_USEFR_REQUIRED = 5,     /* older installation present to uninstall the -fr option is required */
    HASPDS_STATUS_USEHINST_REQUIRED = 6,  /* older installation present - use of hinstall is required */
    HASPDS_STATUS_INSERT_REQUIRED = 7,    /* a device insertion is required */
    HASPDS_STATUS_NEED_RESCAN = 8         /* need to ask a DevMgrrescan - internal only */
} haspds_status_t;


/*
 * Parameters for haspds_Install & haspds_Uninstall
 */
#define HASPDS_PARAM_KILLPROC    0x00000001  /* killproc argument present         */
#define HASPDS_PARAM_FREMOVE     0x00000002  /* fremove argument present          */
#define HASPDS_PARAM_FINSTALL    0x00000004  /* fi argument present - workaround  */
                                             /* for W2K3 issues.                  */
#define HASPDS_PARAM_REPAIR      0x00000010  /* repair an old installation        */

/*						
 * valid bits as parameters - add below with logical or (|) every new flag you use	       
 */                                                                                            
#define HASPDS_VALID_FLAGS HASPDS_PARAM_KILLPROC | \
                           HASPDS_PARAM_FREMOVE  | \
                           HASPDS_PARAM_VERBOSE  | \
                           HASPDS_PARAM_FINSTALL | \
                           HASPDS_PARAM_NORESCAN | \
			   HASPDS_PARAM_PURGE    | \
			   HASPDS_PARAM_REPAIR      

/*
 * exported functions
 */
#ifdef __cplusplus
extern "C" {
#endif
haspds_status_t  __stdcall haspds_Install(unsigned long Param);
haspds_status_t  __stdcall haspds_UnInstall(unsigned long Param);
haspds_status_t  __stdcall haspds_GetInfo(PHASPDS_INFO pInfo,unsigned long* pSize);
haspds_status_t  __stdcall haspds_GetInfoEx(PHASPDS_INFOEX pInfo,unsigned long* pSize);
haspds_status_t  __stdcall haspds_GetLastErrorMessage(char* pBuffer,unsigned long* pSize);
haspds_error     __stdcall haspds_GetLastError(void);
haspds_status_t  __stdcall haspds_GetClientProcess(PHASPDS_CLIENTPROCESS pClientList, unsigned long* pSize);
haspds_status_t  __stdcall haspds_KillClientProcess(PHASPDS_CLIENTPROCESS pClientList, unsigned long* pSize);
unsigned long    __stdcall haspds_GetVersion(void);
haspds_status_t  __stdcall haspds_GetVendorInfo(HASPDS_VENDOR_INFO* Buffer,unsigned long* size);
#ifdef __cplusplus
}
#endif

#pragma pack(pop,_haspds_h_)
#endif __HASPDS_H__
