/**
 * \mainpage Sentinel HASP Run-time API
 * \file hasp_api.h Sentinel HASP Run-time API declarations
 *
 * Copyright (C) 2010, SafeNet, Inc. All rights reserved.
 *
 * $Id: hasp_api.h,v 1.74 2010-06-02 07:38:19 pwang Exp $
 */


#ifndef __HASP_API_H__
#define __HASP_API_H__

#if !defined(WITH_AKSTYPES) && !defined(WITH_OEFTYPES)
#  if defined(_MSC_VER) || defined(__WATCOMC__) || defined(__BORLANDC__)
typedef unsigned __int64 hasp_u64_t;
typedef signed __int64 hasp_s64_t;
#  else
typedef unsigned long long hasp_u64_t;
typedef signed long long hasp_s64_t;
#  endif
#  if defined(_MSC_VER)
typedef unsigned long hasp_u32_t;
typedef signed long hasp_s32_t;
#  else
typedef unsigned int hasp_u32_t;
typedef signed int hasp_s32_t;
#  endif
typedef unsigned short hasp_u16_t;
typedef signed short hasp_s16_t;
typedef signed char hasp_s8_t;
typedef unsigned char hasp_u8_t;
#endif

#if defined(_WIN32) || defined(WIN32) || defined(_MSC_VER) || defined(__BORLANDC__)
#  define HASP_CALLCONV __stdcall
#else
#  define HASP_CALLCONV
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup hasp_feature_ids Legacy HASP HL Run-time API: Feature ID defines
 *
 * @{
 */

/**
 * \brief Legacy HASP HL Run-time API: "Feature Type" mask
 *
 * AND-mask used to identify Feature type.
 */
#define HASP_FEATURETYPE_MASK 0xffff0000

/**
 * \brief Legacy HASP HL Run-time API: "Program Number Feature" type
 *
 * After AND-ing with HASP_FEATURETYPE_MASK the Feature type contain this value.
 */
#define HASP_PROGNUM_FEATURETYPE 0xffff0000

/**
 * \brief Legacy HASP HL Run-time API: "Program Number Mask"
 *
 * AND-mask used to extract the Program Number from a
 * "prognum" Feature ID.
 */
#define HASP_PROGNUM_MASK 0x000000ff

/**
 * \brief Legacy HASP HL Run-time API: "Program Number Options" mask
 *
 * AND-mask used to identify Program Number options:
 *   <ul>
 *     <li>HASP_PROGNUM_OPT_NO_LOCAL</li>
 *     <li>HASP_PROGNUM_OPT_NO_REMOTE</li>
 *     <li>HASP_PROGNUM_OPT_PROCESS</li>
 *     <li>HASP_PROGNUM_OPT_CLASSIC</li>
 *     <li>HASP_PROGNUM_OPT_TS</li>
 *   </ul>
 *
 * 3 bits of the mask are reserved for future extensions and currently unused.
 * Initialize them with zero.
 */
#define HASP_PROGNUM_OPT_MASK 0x0000ff00

/**
 * \brief Legacy HASP HL Run-time API: "Program Number" option
 *
 * Disables the search for local licenses.
 */
#define HASP_PROGNUM_OPT_NO_LOCAL 0x00008000

/**
 * \brief Legacy HASP HL Run-time API: "Program Number" option
 *
 * Disables the search for network licenses.
 */
#define HASP_PROGNUM_OPT_NO_REMOTE 0x00004000

/**
 * \brief Legacy HASP HL Run-time API: "Program Number" option
 *
 * Sets session count of network licenses to "per-process".
 */
#define HASP_PROGNUM_OPT_PROCESS 0x00002000

/**
 * \brief Legacy HASP HL Run-time API: "Program Number" option
 *
 * Enables the API to access "classic" (HASP4 or earlier) keys.
 */
#define HASP_PROGNUM_OPT_CLASSIC 0x00001000

/**
 * \brief Legacy HASP HL Run-time API: "Program Number" option
 *
 * Ignores the presence of terminal servers.
 */
#define HASP_PROGNUM_OPT_TS 0x00000800

/**
 * \brief The HASP default Feature ID
 *
 * Available in every HASP key.
 */
#define HASP_DEFAULT_FID 0

/**
 * \brief Legacy HASP HL Run-time API: The HASP default Feature ID
 *
 * Available in every legacy HASP hardware key.
 */
#define HASP_PROGNUM_DEFAULT_FID (HASP_DEFAULT_FID | HASP_PROGNUM_FEATURETYPE)

/**
 * @}
 */



/**
 * @defgroup hasp_file_ids Memory File ID defines
 *
 * @{
 */

/**
 * \brief Legacy HASP HL Run-time API: HASP4 memory file
 *
 * File ID for HASP4-compatible memory contents w/o FAS.
 */
#define HASP_FILEID_MAIN 0xfff0

/**
 * \brief Legacy HASP HL Run-time API: HASP4 FAS memory file
 *
 * (Dummy) File ID for the license data area of memory contents.
 */
#define HASP_FILEID_LICENSE 0xfff2

/**
 * \brief Sentinel HASP secure writable memory file
 *
 * File ID for Sentinel HASP secure writable memory.
 */
#define HASP_FILEID_RW 0xfff4

/**
 * \brief Sentinel HASP secure read only memory file
 *
 * File ID for HASP Sentinel HASP read only memory.
 */
#define HASP_FILEID_RO 0xfff5

/**
 * @}
 */



/**
 * @defgroup hasp_error_codes Run-time API Status Codes
 *
 * @{
 */

enum hasp_error_codes
{
    /** Request successfully completed */
    HASP_STATUS_OK = 0,

    /** Request exceeds memory range of a HASP file */
    HASP_MEM_RANGE = 1,

    /** Legacy HASP HL Run-time API: Unknown/Invalid Feature ID option */
    HASP_INV_PROGNUM_OPT = 2,

    /** System is out of memory */
    HASP_INSUF_MEM = 3,

    /** Too many open Features/login sessions */
    HASP_TMOF = 4,

    /** Access to Feature, HASP protection key or functionality denied */
    HASP_ACCESS_DENIED = 5,

    /** Legacy decryption function cannot work on Feature */
    HASP_INCOMPAT_FEATURE = 6,

    /** Sentinel HASP protection key not available */
    HASP_HASP_NOT_FOUND = 7,

    /* Deprecated - use HASP_HASP_NOT_FOUND */
    HASP_CONTAINER_NOT_FOUND = 7,

    /** Encrypted/decrypted data length too short to execute function call */
    HASP_TOO_SHORT = 8,

    /** Invalid login handle passed to function */
    HASP_INV_HND = 9,

    /** Specified File ID not recognized by API */
    HASP_INV_FILEID = 10,

    /** Installed driver or daemon too old to execute function */
    HASP_OLD_DRIVER = 11,

    /** Real-time clock (rtc) not available */
    HASP_NO_TIME = 12,

    /** Generic error from host system call */
    HASP_SYS_ERR = 13,

    /** Required driver not installed */
    HASP_NO_DRIVER = 14,

    /** Invalid XML format */
    HASP_INV_FORMAT = 15,

    /** Unable to execute function in this context; the requested
     * functionality is not implemented */
    HASP_REQ_NOT_SUPP = 16,

    /** Binary data passed to function does not contain valid update */
    HASP_INV_UPDATE_OBJ = 17,

    /** HASP protection key not found */
    HASP_KEYID_NOT_FOUND = 18,

    /** Required XML tags not found; Contents in binary data are missing
     * or invalid */
    HASP_INV_UPDATE_DATA = 19,

    /** Update request not supported by Sentinel HASP protection key */
    HASP_INV_UPDATE_NOTSUPP = 20,

    /** Update counter set incorrectly */
    HASP_INV_UPDATE_CNTR = 21,

    /** Invalid Vendor Code passed */
    HASP_INV_VCODE = 22,

    /** Sentinel HASP protection key does not support encryption type */
    HASP_ENC_NOT_SUPP = 23,

    /** Passed time value outside supported value range */
    HASP_INV_TIME = 24,

    /** Real-time clock battery out of power */
    HASP_NO_BATTERY_POWER = 25,

    /** Acknowledge data requested by update, but ack_data parameter
     * is NULL */
    HASP_NO_ACK_SPACE = 26,

    /** Program running on a terminal server */
    HASP_TS_DETECTED = 27,

    /** Requested Feature type not implemented */
    HASP_FEATURE_TYPE_NOT_IMPL = 28,

    /** Unknown algorithm used in H2R/V2C file */
    HASP_UNKNOWN_ALG = 29,

    /** Signature verification operation failed */
    HASP_INV_SIG = 30,

    /** Requested Feature not available */
    HASP_FEATURE_NOT_FOUND = 31,

    /** Access log not enabled */
    HASP_NO_LOG = 32,

    /** Communication error between API and local Sentinel HASP License Manager */
    HASP_LOCAL_COMM_ERR = 33,

    /** Vendor Code not recognized by API */
    HASP_UNKNOWN_VCODE = 34,

    /** Invalid XML specification */
    HASP_INV_SPEC = 35,

    /** Invalid XML scope */
    HASP_INV_SCOPE = 36,

    /** Too many Sentinel HASP protection keys currently connected */
    HASP_TOO_MANY_KEYS = 37,

    /** Too many concurrent user sessions currently connected */
    HASP_TOO_MANY_USERS = 38,

    /** Session been interrupted */
    HASP_BROKEN_SESSION = 39,

    /** Communication error between local and remote Sentinel HASP License Managers */
    HASP_REMOTE_COMM_ERR = 40,

    /** Feature expired */
    HASP_FEATURE_EXPIRED = 41,

    /** Sentinel HASP License Manager version too old */
    HASP_OLD_LM = 42,

    /** Input/Output error occurred in secure storage area of HASP SL key OR
     * a USB error occurred when communicating with a HASP HL key */
    HASP_DEVICE_ERR = 43,

    /** Update installation not permitted; This update was already applied */
    HASP_UPDATE_BLOCKED = 44,

    /** System time has been tampered with */
    HASP_TIME_ERR = 45,

    /** Communication error occurred in secure channel */
    HASP_SCHAN_ERR = 46,

    /** Corrupt data exists in secure storage area of HASP SL protection key */
    HASP_STORAGE_CORRUPT = 47,

    /** Unable to find Vendor library */
    HASP_NO_VLIB = 48,

    /** Unable to load Vendor library */
    HASP_INV_VLIB = 49,

    /** Unable to locate any Feature matching scope */
    HASP_SCOPE_RESULTS_EMPTY = 50,

    /** Program running on a virtual machine */
    HASP_VM_DETECTED = 51,

    /** HASP SL key incompatible with machine hardware; HASP SL key is locked
     * to different hardware. OR:
     * In the case of a V2C file, conflict between HASP SL key data and machine
     * hardware data; HASP SL key locked to different hardware */
    HASP_HARDWARE_MODIFIED = 52,

    /** Login denied because of user restrictions */
    HASP_USER_DENIED = 53,

    /** Trying to install a V2C file with an update counter that is out of
     * sequence with the update counter on the Sentinel HASP protection key.
     * The update counter value in the V2C file is lower than the value in
     * sentinel HASP protection key. */
    HASP_UPDATE_TOO_OLD = 54,

    /** Trying to install a V2C file with an update counter that is out of
     * sequence with update counter in the Sentinel HASP protection key. The
     * first value in the V2C file is greater than the value in the
     * Sentinel HASP protection key. */
    HASP_UPDATE_TOO_NEW = 55,

    /** Vendor library version too old */
    HASP_OLD_VLIB = 56,

    /** Upload via ACC failed, e.g. because of illegal format */
    HASP_UPLOAD_ERROR = 57,

    /** Invalid XML "recipient" parameter */
    HASP_INV_RECIPIENT = 58,

    /** Invalid XML "action" parameter */
    HASP_INV_DETACH_ACTION = 59,

    /** scope does not specify a unique Product */
    HASP_TOO_MANY_PRODUCTS = 60,

    /** Invalid Product information */
    HASP_INV_PRODUCT = 61,

    /** Unknown Recipient; update can only be applied to the 
     * Recipient specified in hasp_detach(), and not to this computer */
    HASP_UNKNOWN_RECIPIENT = 62,

    /** Invalid duration */
    HASP_INV_DURATION = 63,

    /** Cloned HASP SL secure storage detected */
    HASP_CLONE_DETECTED = 64,

    /** API dispatcher: API for this Vendor Code was not found */
    HASP_NO_API_DYLIB = 400,

    /** API dispatcher: Unable to load API; DLL possibly corrupt? */
    HASP_INV_API_DYLIB = 401,

    /** Invalid function parameter */
    HASP_INVALID_PARAMETER = 501,

    /** C++ API: Object incorrectly initialized */
    HASP_INVALID_OBJECT = 500,

    /** C++ API: Logging in twice to the same object */
    HASP_ALREADY_LOGGED_IN = 502,

    /** C++ API: Logging out twice of the same object */
    HASP_ALREADY_LOGGED_OUT = 503,

    /** .NET API: Incorrect use of system or platform */
    HASP_OPERATION_FAILED = 525,

    /* Internal use: no classic memory extension block available */
    HASP_NO_EXTBLOCK = 600,

    /* Internal use: invalid port type */
    HASP_INV_PORT_TYPE = 650,

    /* Internal use: invalid port value */
    HASP_INV_PORT = 651,

    /** Requested function not implemented */
    HASP_NOT_IMPL = 698,

    /** Internal error occurred in API */
    HASP_INT_ERR = 699,

    /* Reserved for HASP helper libraries */
    HASP_FIRST_HELPER = 2001,

    /* Reserved for HASP Activation API */
    HASP_FIRST_HASP_ACT = 3001,

    HASP_NEXT_FREE_VALUES = 5001,
};

/**
 * @}
 */



/**
 * @defgroup hasp_general HASP typedefs and macros
 *
 * @{
 */

/** A HASP status code */
typedef enum hasp_error_codes hasp_status_t;

/** HASP size type */
typedef hasp_u32_t hasp_size_t;

/** HASP connection handle */
typedef hasp_u32_t hasp_handle_t;

/** HASP Feature ID */
typedef hasp_u32_t hasp_feature_t;

/** HASP File ID */
typedef hasp_u32_t hasp_fileid_t;

/** HASP timestamp, representing elapsed seconds since 
 * Jan-01-1970 0:00:00 GMT */
typedef hasp_u64_t hasp_time_t;

/** HASP Vendor Code buffer */
typedef const void *hasp_vendor_code_t;

/** format to retrieve update info (C2V) */
#define HASP_UPDATEINFO     "<haspformat format=\"updateinfo\"/>"

/* format to retrieve a small update info (C2V) */
#define HASP_FASTUPDATEINFO "<haspformat format=\"fastupdateinfo\"/>"

/** format to retrieve session info */
#define HASP_SESSIONINFO    "<haspformat format=\"sessioninfo\"/>"

/** format to retrieve key/hardware info */
#define HASP_KEYINFO        "<haspformat format=\"keyinfo\"/>"

/** format to retrieve recipient parameter for hasp_detach */
#define HASP_RECIPIENT      "<haspformat root=\"location\">"             \
                            "  <license_manager>"                        \
                            "    <attribute name=\"id\" />"              \
                            "    <attribute name=\"time\" />"            \
                            "    <element name=\"hostname\" />"          \
                            "    <element name=\"version\" />"           \
                            "    <element name=\"host_fingerprint\" />"  \
                            "  </license_manager>"                       \
                            "</haspformat>"

/*
 * Invalid handle value for hasp_login() and hasp_login_scope() functions.
 */
#define HASP_INVALID_HANDLE_VALUE 0

/**
 * \brief Minimum block size for hasp_encrypt() and hasp_decrypt() functions.
 */
#define HASP_MIN_BLOCK_SIZE 16

/**
 * \brief Minimum block size for hasp_legacy_encrypt()
 *        and hasp_legacy_decrypt() legacy functions.
 */
#define HASP_MIN_BLOCK_SIZE_LEGACY 8

/**
 * @}
 */



/**
 * @defgroup hasp_basic The Basic Sentinel HASP Run-time API
 *
 * @{
 */

/**
 * \brief Logs into a Feature and thereby establishes a session context.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * This function establishes a context to a Sentinel HASP protection key
 * containing a license for the requested Feature ID.
 *
 * The requisite Vendor Codes are stored in a VendorCodes folder in your
 * system. Without the correct Vendor Code, the function call cannot succeed.
 *
 * You can open up to 512 simultaneous login sessions.
 *
 * <b>Legacy HASP Remarks</b><br>
 * For local prognum Features, concurrency is not handled and each login
 * performs a decrement if it is a counting license.
 *
 * Network "prognum" features continue to use the old HASP LM login logic, 
 * with its inherent limitations.
 *
 * There is only support for concurrent usage of <b>one</b> server (global
 * server address).
 *
 * With "Program Number" features (see \ref HASP_FEATURETYPE_MASK), 8
 * bits are reserved for legacy options (see \ref HASP_PROGNUM_OPT_MASK,
 * currently 5 bits are used):
 * <ul>
 *   <li>only local</li>
 *   <li>only remote</li>
 *   <li>login is counted per process ID</li>
 *   <li>disable terminal server check</li>
 *   <li>enable access to old (HASP3/HASP4) keys</li>
 * </ul>
 *
 * \param feature_id  Unique identifier for a specific Feature stored
 *                    in a Sentinel HASP protection key
 * \param vendor_code Pointer to the Vendor Code
 * \param handle      Pointer to the resulting session handle
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_HASP_NOT_FOUND</li> Required Sentinel HASP
 *                        protection key not found
 *                      <li>HASP_FEATURE_NOT_FOUND</li> Cannot find requested
 *                        Feature
 *                      <li>HASP_FEATURE_TYPE_NOT_IMPL</li> Requested Feature
 *                        type not available
 *                      <li>HASP_TMOF</li> Too many open login sessions
 *                      <li>HASP_INSUF_MEM</li> Out of memory
 *                      <li>HASP_INV_VCODE</li> Invalid Vendor Code
 *                      <li>HASP_NO_DRIVER</li> Driver not installed
 *                      <li>HASP_NO_VLIB</li> Vendor library cannot be found
 *                      <li>HASP_INV_VLIB</li> Vendor library cannot be loaded
 *                      <li>HASP_OLD_DRIVER</li> Driver too old
 *                      <li>HASP_UNKNOWN_VCODE</li> Vendor Code not recognized
 *                      <li>HASP_FEATURE_EXPIRED</li> Feature has expired
 *                      <li>HASP_TOO_MANY_USERS</li> Too many users currently
 *                        connected
 *                      <li>HASP_OLD_LM</li> HASP License Manager version
 *                        too old
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_TIME_ERR</li> System time has been tampered
 *                        with
 *                      <li>HASP_HARDWARE_MODIFIED</li> HASP SL key
 *                        incompatible with machine hardware; HASP SL key is
 *                        locked to different hardware
 *                      <li>HASP_TS_DETECTED</li> Program is running on a
 *                        Terminal Server
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                      <li>HASP_OLD_VLIB</li> Vendor Library version too old
 *                      <li>HASP_CLONE_DETECTED</li> Cloned HASP SL storage
 *                        detected. Feature unavailable
 *                    </ul>
 *
 * \sa hasp_login_scope()
 * \sa hasp_logout()
 *
 */
hasp_status_t HASP_CALLCONV hasp_login(hasp_feature_t feature_id, 
                hasp_vendor_code_t vendor_code,
                hasp_handle_t *handle);


/**
 * \brief Logs into a Feature to establish a session, according to
 * predefined search parameters.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * This function is used to specify conditions that describe where login
 * information is to searched for.
 *
 * The requisite Vendor Codes are stored in a VendorCodes folder in your
 * system. Without the correct Vendor Code, the function call cannot succeed.
 *
 * You can open up to 512 simultaneous login sessions.
 *
 * This function does not work with legacy HASP Features.
 *
 * \param feature_id  Unique identifier for a specific Feature stored in
 *                      a Sentinel HASP protection key
 * \param scope       Definition of the search parameters for
 *                      this Feature ID. See the additional HASP API
 *                      Reference documentation for more information
 *                      about Scope XML Tags
 * \param vendor_code Pointer to the vendor code
 * \param handle      Pointer to the resulting session handle
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_HASP_NOT_FOUND</li> Required Sentinel HASP
 *                        protection key not found
 *                      <li>HASP_FEATURE_NOT_FOUND</li> Cannot find requested
 *                        Feature
 *                      <li>HASP_SCOPE_RESULTS_EMPTY</li> Unable to locate
 *                        a Feature matching the scope
 *                      <li>HASP_TMOF</li> Too many open login sessions
 *                      <li>HASP_INSUF_MEM</li> Out of memory
 *                      <li>HASP_INV_VCODE</li> Invalid Vendor Code
 *                      <li>HASP_NO_DRIVER</li> Driver not installed
 *                      <li>HASP_NO_VLIB</li> Vendor library cannot be found
 *                      <li>HASP_INV_VLIB</li> Vendor library cannot be loaded
 *                      <li>HASP_OLD_DRIVER</li> Driver too old
 *                      <li>HASP_UNKNOWN_VCODE</li> Vendor Code not recognized
 *                      <li>HASP_INVALID_PARAMETER</li> Scope string too
 *                        long (max. length 32 kb)
 *                      <li>HASP_FEATURE_EXPIRED</li> Feature has expired
 *                      <li>HASP_TOO_MANY_USERS</li> Too many users currently
 *                        connected
 *                      <li>HASP_OLD_LM</li> HASP License Manager version
 *                        too old
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_TIME_ERR</li> System time has been tampered
 *                        with
 *                      <li>HASP_TS_DETECTED</li> Program is running on a
 *                        Terminal Server
 *                      <li>HASP_HARDWARE_MODIFIED</li> HASP SL key
 *                        incompatible with machine hardware; HASP SL key is
 *                        locked to different hardware
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                      <li>HASP_OLD_VLIB</li> Vendor Library version too old
 *                      <li>HASP_CLONE_DETECTED</li> Cloned HASP SL storage
 *                        detected. Feature unavailable
 *                    </ul>
 *
 * \sa hasp_login()
 * \sa hasp_logout()
 * \sa hasp_get_info()
 *
 */
hasp_status_t HASP_CALLCONV hasp_login_scope(hasp_feature_t feature_id, 
                const char *scope,
                hasp_vendor_code_t vendor_code,
                hasp_handle_t *handle);


/**
 * \brief Logs out from a context or session.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * Use this function to end a connection to an API session object. Once
 * logged out from a session, all memory allocated for the session is released.
 *
 * \param handle      Handle for the session being terminated
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                    </ul>
 *
 * \sa hasp_login()
 * \sa hasp_login_scope()
 *
 */
hasp_status_t HASP_CALLCONV hasp_logout(hasp_handle_t handle);


/**
 * \brief Encrypts a buffer.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * Encrypts data using the encryption engine in the Sentinel HASP protection 
 * key.
 * The specific session handle determines which Sentinel HASP protection key and
 * which Feature ID encrypts the data buffer. The encryption key remains
 * in the Sentinel HASP protection key. If the encryption fails, the buffer
 * is not modified. To decrypt the data buffer, use the \ref hasp_decrypt
 * function.
 *
 * \param handle      Handle for the session
 * \param buffer      Pointer to the buffer to be encrypted
 * \param length      Size (in bytes) of the buffer to be encrypted
 *                      (16 bytes minimum)
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_TIME_ERR</li> System time has been tampered
 *                        with
 *                      <li>HASP_TOO_SHORT</li> Encryption data length is
 *                        too short
 *                      <li>HASP_ENC_NOT_SUPP</li> Sentinel HASP protection key
 *                        does not support encryption type
 *                      <li>HASP_SCHAN_ERR</li> Communication error occurred
 *                        in secure channel OR HASP HL Firmware too old (update
 *                        to 3.25 or later)
 *                      <li>HASP_BROKEN_SESSION</li> Session has been
 *                        interrupted
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                    </ul>
 *
 * \sa hasp_decrypt()
 *
 */
hasp_status_t HASP_CALLCONV hasp_encrypt(hasp_handle_t handle, 
                void *buffer,
                hasp_size_t length);

/**
 * \brief Decrypts a buffer.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * Decrypts data using the decryption engine in the Sentinel HASP protection 
 * key.
 * The specific session handle determines which Sentinel HASP protection key and
 * which Feature ID decrypts the data buffer. The decryption key remains
 * in the Sentinel HASP protection key. If the decryption fails, the buffer
 * is not modified. To encrypt the data buffer, use the \ref hasp_encrypt
 * function.
 *
 * \param handle      Handle for the session
 * \param buffer      Pointer to the buffer to be decrypted
 * \param length      Size (in bytes) of the buffer to be decrypted
 *                      (16 bytes minimum)
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_TIME_ERR</li> System time has been tampered
 *                        with
 *                      <li>HASP_TOO_SHORT</li> Decryption data length is
 *                        too short
 *                      <li>HASP_SCHAN_ERR</li> Communication error occurred
 *                        in secure channel OR HASP HL Firmware too old (update
 *                        to 3.25 or later)
 *                      <li>HASP_ENC_NOT_SUPP</li> Sentinel HASP protection key
 *                        does not support encryption type
 *                      <li>HASP_BROKEN_SESSION</li> Session has been
 *                        interrupted
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                    </ul>
 *
 * \sa hasp_encrypt()
 *
 */
hasp_status_t HASP_CALLCONV hasp_decrypt(hasp_handle_t handle, 
                void *buffer,
                hasp_size_t length);

/**
 * \brief Reads the memory of a Sentinel HASP protection key.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * Valid File IDs are \ref HASP_FILEID_RW and \ref HASP_FILEID_RO.
 *
 * <b>Legacy HASP Remarks</b><br>
 * Valid File IDs are \ref HASP_FILEID_LICENSE and \ref HASP_FILEID_MAIN.
 *
 * \param handle      Handle for the session
 * \param fileid      Identifier for the file that is to be read
 * \param offset      Byte offset in the file
 * \param length      Number of bytes to be read from the file
 * \param buffer      Pointer to the retrieved data
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_INV_FILEID</li> Unrecognized file identifier
 *                      <li>HASP_MEM_RANGE</li> Request exceeds the memory
 *                        range of a HASP file
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_SCHAN_ERR</li> Communication error occurred
 *                        in secure channel OR HASP HL Firmware too old (update
 *                        to 3.25 or later)
 *                      <li>HASP_BROKEN_SESSION</li> Session has been
 *                        interrupted
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                    </ul>
 *
 * \remark
 * Use the \ref hasp_get_size function to determine the size of the file you
 * want to read.
 *
 * \sa hasp_write()
 * \sa hasp_get_size()
 *
 */
hasp_status_t HASP_CALLCONV hasp_read(hasp_handle_t handle, 
                hasp_fileid_t fileid,
                hasp_size_t offset,
                hasp_size_t length,
                void *buffer);

/**
 * \brief Writes to the memory of a Sentinel HASP protection key.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * Valid File ID is \ref HASP_FILEID_RW.
 *
 * <b>Legacy HASP Remarks</b><br>
 * Depending on the provided session handle (either logged into the
 * default Feature or any other Feature), write access to the FAS memory
 * (\ref HASP_FILEID_LICENSE) is not permitted.
 *
 * \param handle      Handle for the session
 * \param fileid      Identifier for the file that is to be written
 * \param offset      Byte offset in the file
 * \param length      Number of bytes to be written to the file
 * \param buffer      Pointer to the data
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_INV_FILEID</li> Unrecognized file identifier
 *                      <li>HASP_MEM_RANGE</li> Request exceeds the memory
 *                        range of a HASP file
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_SCHAN_ERR</li> Communication error occurred
 *                        in secure channel OR HASP HL Firmware too old (update
 *                        to 3.25 or later)
 *                      <li>HASP_BROKEN_SESSION</li> Session has been
 *                        interrupted
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                    </ul>
 *
 * \remark
 * Use the \ref hasp_get_size function to determine the size of the file you
 * want to write.
 *
 * \sa hasp_read()
 * \sa hasp_get_size()
 *
 */
hasp_status_t HASP_CALLCONV hasp_write(hasp_handle_t handle, 
                hasp_fileid_t fileid,
                hasp_size_t offset,
                hasp_size_t length,
                const void *buffer);

/**
 * \brief Retrieves the byte size of a memory file from a HASP protection key.
 *
 * This function is used to determine the file size of a HASP
 * memory file.
 *
 * \param handle      Handle for the session
 * \param fileid      Identifier for the file that is to be queried.
 * \param size        Pointer to the resulting file size
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_INV_FILEID</li> Unrecognized file identifier
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_BROKEN_SESSION</li> Session has been
 *                        interrupted
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                    </ul>
 *
 * \sa hasp_read()
 * \sa hasp_write()
 *
 */
hasp_status_t HASP_CALLCONV hasp_get_size(hasp_handle_t handle, 
                hasp_fileid_t fileid,
                hasp_size_t *size);

/**
 * \brief Reads the current time.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * Only HASP HL keys with a Real-time clock (rtc) and HASP SL keys can
 * provide the current time.
 *
 * Primarily used to obtain reliable timestamps that are independent from
 * the system clock.
 *
 * Time values are returned as the number of seconds that have elapsed
 * since Jan-01-1970 0:00:00 UTC.
 *
 * <b>Legacy HASP Remarks</b><br>
 * This request is only supported on locally accessed keys. Trying to
 * get the time from a remotely accessed key will return HASP_NO_TIME.
 *
 * \param handle      Handle for the session
 * \param time        Pointer to the current time
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_BROKEN_SESSION</li> Session has been
 *                        interrupted
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                      <li>HASP_NO_BATTERY_POWER</li> Real-time clock has
 *                        run out of power
 *                      <li>HASP_NO_TIME</li> Real-time clock is not
 *                        available
 *                    </ul>
 *
 * \sa hasp_datetime_to_hasptime()
 * \sa hasp_hasptime_to_datetime()
 *
 */
hasp_status_t HASP_CALLCONV hasp_get_rtc(hasp_handle_t handle, 
                hasp_time_t *time);

/**
 * @}
 */



/**
 * @defgroup hasp_classic Legacy HASP functionality for backward compatibility
 *
 * @{
 */

/**
 * \brief Legacy HASP4 compatible encryption function.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * This function does not work with a Sentinel HASP login handle.
 *
 * <b>Legacy HASP Remarks</b><br>
 * The handle must have been obtained by calling \ref hasp_login() with
 * a "prognum" Feature ID.
 *
 * \param handle      Handle for the session
 * \param buffer      Pointer to the buffer to be encrypted
 * \param length      Size in bytes of the buffer (min. 8 bytes)
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_TOO_SHORT</li> Encryption data length is
 *                        too short
 *                      <li>HASP_ENC_NOT_SUPP</li> Sentinel HASP protection key
 *                        does not support encryption type
 *                      <li>HASP_HASP_NOT_FOUND</li> Required HASP
 *                        protection key not found
 *                    </ul>
 *
 * \sa hasp_legacy_decrypt()
 * \sa hasp_encrypt()
 * \sa hasp_decrypt()
 *
 */
hasp_status_t HASP_CALLCONV hasp_legacy_encrypt(hasp_handle_t handle, 
                void *buffer,
                hasp_size_t length);

/**
 * \brief Legacy HASP4 compatible decryption function.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * This function does not work with a Sentinel HASP login handle.
 *
 * <b>Legacy HASP Remarks</b><br>
 * The handle must have been obtained by calling \ref hasp_login() with
 * a "prognum" Feature ID.
 *
 * \param handle      Handle for the session
 * \param buffer      Pointer to the buffer to be decrypted
 * \param length      Size in bytes of the buffer (min. 8 bytes)
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_TOO_SHORT</li> Encryption data length is
 *                        too short
 *                      <li>HASP_ENC_NOT_SUPP</li> Sentinel HASP protection key
 *                        does not support encryption type
 *                      <li>HASP_HASP_NOT_FOUND</li> Required HASP
 *                        protection key not found
 *                    </ul>
 *
 * \sa hasp_legacy_encrypt()
 * \sa hasp_decrypt()
 * \sa hasp_encrypt()
 *
 */
hasp_status_t HASP_CALLCONV hasp_legacy_decrypt(hasp_handle_t handle, 
                void *buffer,
                hasp_size_t length);

/**
 * \brief Writes to HASP4-compatible real-time clock
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * This function does not work with a Sentinel HASP login handle.
 *
 * <b>Legacy HASP Remarks</b><br>
 * The handle must have been obtained by calling \ref hasp_login() with
 * a "prognum" Feature ID.
 *
 * This request is only supported on locally accessed keys. Attempting to
 * set the time on a remotely accessed key will return HASP_NO_TIME.
 *
 * \param handle      Handle for the session
 * \param new_time    The new time value
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_HASP_NOT_FOUND</li> Required HASP
 *                        protection key not found
 *                      <li>HASP_NO_TIME</li> Real-time clock not available
 *                        OR access remote
 *                    </ul>
 *
 * \sa hasp_get_rtc()
 * \sa hasp_datetime_to_hasptime()
 * \sa hasp_hasptime_to_datetime()
 *
 */
hasp_status_t HASP_CALLCONV hasp_legacy_set_rtc(hasp_handle_t handle, 
                hasp_time_t new_time);

/**
 * \brief Set the LM idle time.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * This function does not work with a Sentinel HASP login handle.
 *
 * <b>Legacy HASP Remarks</b><br>
 * The handle must have been obtained by calling \ref hasp_login() with
 * a "prognum" Feature ID.
 *
 * \param handle      Handle for the session
 * \param idle_time   The idle time in minutes
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_HASP_NOT_FOUND</li> Required HASP
 *                        protection key not found
 *                      <li>HASP_REQ_NOT_SUPP</li> Tried to set the idle time
 *                        for a local license
 *                    </ul>
 *
 */
hasp_status_t HASP_CALLCONV hasp_legacy_set_idletime(hasp_handle_t handle, 
                hasp_u16_t idle_time);

/**
 * @}
 */



/**
 * @defgroup hasp_extended Extended Sentinel HASP functionality
 *
 * The extended API consists of functions that provide extended
 * functionality. This advanced functionality is sometimes necessary,
 * and addresses the "advanced" user.
 *
 * @{
 */

/**
 * \brief Retrieves information about system components, according to
 * customizable search parameters, and presents it according to
 * customizable formats.
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * You do not need to be logged in to a Sentinel HASP Feature in order to use
 * this function.
 *
 * This function is used to specify conditions about where to search for
 * information. In addition, it enables you to specify conditions about
 * the format in which the retrieved information is presented. If
 * retrieved information is appropriately formatted, it can be used as
 * a template in the hasp_login_scope() function.
 *
 * The requisite Vendor Codes are stored in a VendorCodes folder in
 * your system. Without the correct Vendor Code, the function call
 * cannot succeed.
 *
 * This function allocates memory for the information it retrieves. To
 * release allocated memory resources, use the \ref hasp_free function.
 *
 * This function cannot be used to retrieve legacy HASP Features.
 *
 * \param scope       Definition of the data that is to be searched, in XML
 *                      format. For more information, see the accompanying 
 *                      Sentinel HASP Run-time API help documentation
 * \param format      Definition of the format in which the data is to be
 *                      displayed, in XML format. For more information, see the
 *                      accompanying Sentinel HASP Run-time API help 
 *                      documentation
 * \param vendor_code Pointer to the Vendor Code
 * \param info        Pointer to the information that is retrieved, in XML 
 *                      format
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_SCOPE_RESULTS_EMPTY</li> Unable to locate
 *                        a Feature matching the scope
 *                      <li>HASP_INSUF_MEM</li> Out of memory
 *                      <li>HASP_INV_VCODE</li> Invalid Vendor Code
 *                      <li>HASP_UNKNOWN_VCODE</li> Vendor Code not recognized
 *                      <li>HASP_INVALID_PARAMETER</li> Scope or format string
 *                        too long (max. length 32 kb)
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                      <li>HASP_INV_FORMAT</li> Unrecognized format string
 *                      <li>HASP_INV_SCOPE</li> Unrecognized scope string
 *                      <li>HASP_BROKEN_SESSION</li> Session has been
 *                        interrupted
 *                    </ul>
 *
 * \sa hasp_get_sessioninfo()
 * \sa hasp_free()
 *
 */
hasp_status_t HASP_CALLCONV hasp_get_info(const char *scope, 
                const char *format,
                hasp_vendor_code_t vendor_code,
                char **info);


/**
 * \brief Retrieves information regarding a session context.
 *
 * Memory for the information is allocated by this function and has to
 * be freed with the \ref hasp_free function.
 *
 * \param handle      Handle for the session
 * \param format      Definition for the type of output data structure, in 
 *                      XML format.
 *                    There are three format options:
 *                    <ul>
 *                      <li>HASP_KEYINFO</li> For retrieving information on
 *                        the Sentinel HASP protection key
 *                      <li>HASP_SESSIONINFO</li> For retrieving information on
 *                        the login session
 *                      <li>HASP_UPDATEINFO</li> For retrieving information on
 *                        a license update usually contained in a C2V file.
 *                        The retrieved information includes the current
 *                        state of the key, including update counters,
 *                        license availability and memory images
 *                    </ul>
 * \param info        Pointer to the information that is retrieved as XML text
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INSUF_MEM</li> Out of memory
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                      <li>HASP_INV_FORMAT</li> Unrecognized format string
 *                      <li>HASP_INV_HND</li> Invalid input handle
 *                      <li>HASP_BROKEN_SESSION</li> Session has been
 *                        interrupted
 *                      <li>HASP_TIME_ERR</li> System time has been tampered
 *                        with
 *                    </ul>
 *
 *  \sa hasp_free()
 *  \sa HASP_UPDATEINFO
 *  \sa HASP_SESSIONINFO
 *  \sa HASP_KEYINFO
 */
hasp_status_t HASP_CALLCONV hasp_get_sessioninfo(hasp_handle_t handle, 
                const char *format,
                char **info);

/**
 * \brief Frees memory resources utilized by other API functions.
 *
 * Used only in C code to release memory resources allocated to storing
 * retrieved data from API calls.
 *
 * \param info         Pointer to the memory resources to be released
 *
 * \sa hasp_get_info()
 * \sa hasp_get_sessioninfo()
 * \sa hasp_update()
 *
 */
void HASP_CALLCONV hasp_free(char *info);


/**
 * \brief Updates a Sentinel HASP protection key.
 *
 * This function writes update information. Note that the Sentinel HASP 
 * protection key must be locally connected.
 *
 * The update code contains all necessary data to perform the update on
 * a deployed Sentinel HASP protection key including:
 * <ul>
 *   <li>The Sentinel HASP protection key on which the updated information is 
 *      to be written</li>
 *   <li>The necessary Vendor Code that is required to access the 
 *      Sentinel HASP key</li>
 *   <li>The actual update information</li>
 * </ul>
 *
 * Depending on the update data, the function returns an acknowledgement
 * code that is signed/encrypted by the key. The code is evidence that
 * an update has been applied to a license. This function allocates memory 
 * for the acknowledge data. To release allocated memory resources, use
 * \a hasp_free().
 *
 * \param update_data Pointer to the complete update data.
 * \param ack_data    Pointer to a buffer to retrieve the acknowledge data.
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_KEYID_NOT_FOUND</li> The HASP protection 
 *                        key was not found
 *                      <li>HASP_INV_UPDATE_DATA</li> Required XML tags
 *                        not found; Contents in binary data are missing
 *                        or invalid
 *                      <li>HASP_INV_UPDATE_NOTSUPP</li> Update request is
 *                        not supported by Sentinel HASP protection key
 *                      <li>HASP_INV_UPDATE_CNTR</li> Update counter not
 *                        set correctly
 *                      <li>HASP_INSUF_MEM</li> Out of memory
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                      <li>HASP_NO_ACK_SPACE</li> Acknowledge data
 *                        requested by the update, but ack_data parameter
 *                        is NULL
 *                      <li>HASP_UNKNOWN_ALG</li> Unknown algorithm used
 *                        in V2C file
 *                      <li>HASP_INV_SIG</li> Signature verification failed
 *                      <li>HASP_TOO_MANY_KEYS</li> Too many Sentinel HASP
 *                        protection keys currently connected
 *                      <li>HASP_HARDWARE_MODIFIED</li> Conflict between
 *                        HASP SL key data and machine hardware data;
 *                        HASP SL key locked to different hardware
 *                      <li>HASP_UPDATE_TOO_OLD</li> Trying to install a
 *                        V2C file with an update counter that is out of
 *                        sequence with the update counter in the Sentinel HASP
 *                        protection key. The values of the update counter
 *                        in the file are lower than those in the Sentinel HASP
 *                        protection key
 *                      <li>HASP_UPDATE_TOO_NEW</li> Trying to install a
 *                        V2C file with an update counter that is out of
 *                        sequence with the update counter in the Sentinel HASP
 *                        protection key. The first value in the file is more
 *                        than 1 greater than the value in the Sentinel HASP
 *                        protection key
 *                      <li>HASP_UNKNOWN_RECIPIENT</li> In case of an H2R:
 *                        Update can only be applied to the Recipient 
 *                        specified in hasp_detach(), not to this machine
 *                    </ul>
 *
 * \sa hasp_free()
 *
 */
hasp_status_t HASP_CALLCONV hasp_update(const char *update_data, 
                char **ack_data);


/**
 * \brief Detaches or cancels an attached license, according to customizable 
 * parameters
 *
 * <b>Sentinel HASP Run-time API Usage Notes</b><br>
 * You do not need to be logged in to a Sentinel HASP Feature in order to use
 * this function.
 *
 * This function is used to detach a license for a Product (i.e. all Sentinel 
 * HASP Features and Memory files which belong to this Product) from a HASP 
 * SL Protection key. The function returns a H2R file which must then be 
 * applied on the recipient machine using hasp_update() or the ACC.
 *
 * This function only works with HASP SL Protection Keys; HASP HL 
 * Protection Keys are ignored.
 *
 * This function can also be used on the recipient machine to cancel an 
 * attached license. In this case, the recipient parameter is ignored and 
 * should be set to NULL. For cancelling, the function returns a R2H file 
 * which must be applied on the host machine using hasp_update() or the 
 * ACC. If the detached Product is already expired, no R2H file will be 
 * returned.
 *
 * The required Vendor Codes are stored in a VendorCodes folder in
 * your system. Without the correct Vendor Code, the function call 
 * cannot succeed. 
 *
 * \param detach_action Parameters for the operation, in XML format. For
 *                      more information, see the accompanying
 *                      Sentinel HASP Run-time API help documentation.
 * \param scope       Search parameters for the Product that is to be 
 *                      detached
 * \param vc          Pointer to the Vendor Code
 * \param recipient   Definition in XML format of the recipient computer,
 *                      on which the detached Product will be installed.
 *                      This information can be retrieved using either 
 *                      @a hasp_get_info or @a hasp_get_sessioninfo
 *                      together with the format specifier 
 *                      @a HASP_RECIPIENT.
 *                      Set to NULL if an attached protection key is cancelled.
 * \param info        Pointer to the information that is retrieved, in 
 *                      XML format. This information is a V2C, which can
 *                      then be installed on the recipient computer
 *                      via @a hasp_update. Use @a hasp_free to release this
 *                      pointer after use.
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_DETACH_ACTION</li> Invalid XML 
 *                        "detach_action" parameter
 *                      <li>HASP_INV_RECIPIENT</li> Invalid XML "recipient"
 *                        parameter
 *                      <li>HASP_TOO_MANY_PRODUCTS</li> Scope for hasp_detach
 *                        does not specify a unique Parameter
 *                      <li>HASP_TOO_MANY_USERS</li> Too many users currently
 *                        connected, or: at least one detachable Feature
 *                        does not have enough network seats available
 *                      <li>HASP_ACCESS_DENIED</li> Request cannot be
 *                        processed due to ACC restrictions
 *                      <li>HASP_FEATURE_EXPIRED</li> All detachable
 *                        Features are expired
 *                      <li>HASP_INV_PRODUCT</li> Invalid Product information
 *                      <li>HASP_INV_DURATION</li> In the case of a new
 *                        detachable license, duration exceeds maximum allowed
 *                        OR, in the case of a detachable license extension,
 *                        expiration date earlier than original date
 *                        or too short (if an existing detached Product is
 *                        extended, and the new expiration date is earlier
 *                        than the original expiration date)
 *                      <li>HASP_INSUF_MEM</li> Out of memory
 *                      <li>HASP_DEVICE_ERR</li> Input/Output error in HASP
 *                        SL secure storage, OR in case of a HASP HL key,
 *                        USB communication error
 *                      <li>HASP_LOCAL_COMM_ERR</li> Communication error
 *                        between API and local HASP License Manager
 *                      <li>HASP_REMOTE_COMM_ERR</li> Communication error
 *                        between local and remote HASP License Manager
 *                    </ul>
 *
 * \sa hasp_get_info()
 * \sa hasp_update()
 * \sa hasp_free()
 */
hasp_status_t HASP_CALLCONV hasp_detach(const char *detach_action, 
                const char *scope,
                hasp_vendor_code_t vc,
                const char *recipient,
                char **info);

/**
 * @}
 */



/**
 * @defgroup hasp_util Utility functions
 *
 * @{
 */

/**
 * \brief Retrieves version and build number of the HASP library
 *
 * \param major_version Pointer to retrieve the major version number
 * \param minor_version Pointer to retrieve the minor version number
 * \param build_server  Pointer to retrieve the build server id
 * \param build_number  Pointer to retrieve the build number
 * \param vendor_code   Pointer to the Vendor Code
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                    </ul>
 *
 * \remark Any pointer other than the vendor_code can be NULL if its
 * information is not required.
 *
 */
hasp_status_t HASP_CALLCONV hasp_get_version(unsigned int *major_version, 
                unsigned int *minor_version,
                unsigned int *build_server,
                unsigned int *build_number,
                hasp_vendor_code_t vendor_code);

/**
 * \brief Converts a date and time value to hasptime (the number of
 * elapsed seconds since January 1 1970).
 *
 * \param day         Input for day value (range 1-31)
 * \param month       Input for month value (range 1-12)
 * \param year        Input for year value (range 1970+)
 * \param hour        Input for hour value (range 0-23)
 * \param minute      Input for minute value (range 0-59)
 * \param second      Input for second value (range 0-59)
 * \param time        Pointer to the resulting time value
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_TIME</li> Passed time value is outside
 *                        the supported value range
 *                    </ul>
 *
 * \remark Time values are in UTC.
 *
 * \sa hasp_hasptime_to_datetime()
 *
 */
hasp_status_t HASP_CALLCONV hasp_datetime_to_hasptime(unsigned int day, 
                unsigned int month,
                unsigned int year,
                unsigned int hour,
                unsigned int minute,
                unsigned int second,
                hasp_time_t *time);

/**
 * \brief Converts a time value (elapsed seconds since January 1 1970)
 * into a date and time
 *
 * \param time        The input time value
 * \param day         Pointer to the day value
 * \param month       Pointer to the month value
 * \param year        Pointer to the year value
 * \param hour        Pointer to the hour value
 * \param minute      Pointer to the minute value
 * \param second      Pointer to the second value
 *
 * \return            <ul>
 *                      <li>HASP_STATUS_OK</li> Request was successfully
 *                        completed
 *                      <li>HASP_INV_TIME</li> Passed time value outside
 *                        of supported value range
 *                    </ul>
 *
 * \remark Time values are in UTC.
 *
 * \sa hasp_datetime_to_hasptime()
 *
 */
hasp_status_t HASP_CALLCONV hasp_hasptime_to_datetime(hasp_time_t time, 
                unsigned int *day,
                unsigned int *month,
                unsigned int *year,
                unsigned int *hour,
                unsigned int *minute,
                unsigned int *second);

/**
 * @}
 */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __HASP_API_H__ */

