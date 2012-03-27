

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Sep 02 15:50:23 2011
 */
/* Compiler settings for RGBFilters.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __iRGBFilters_h__
#define __iRGBFilters_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __RateSource_FWD_DEFINED__
#define __RateSource_FWD_DEFINED__

#ifdef __cplusplus
typedef class RateSource RateSource;
#else
typedef struct RateSource RateSource;
#endif /* __cplusplus */

#endif 	/* __RateSource_FWD_DEFINED__ */


#ifndef __AlphaRenderer_FWD_DEFINED__
#define __AlphaRenderer_FWD_DEFINED__

#ifdef __cplusplus
typedef class AlphaRenderer AlphaRenderer;
#else
typedef struct AlphaRenderer AlphaRenderer;
#endif /* __cplusplus */

#endif 	/* __AlphaRenderer_FWD_DEFINED__ */


#ifndef __AlphaSource_FWD_DEFINED__
#define __AlphaSource_FWD_DEFINED__

#ifdef __cplusplus
typedef class AlphaSource AlphaSource;
#else
typedef struct AlphaSource AlphaSource;
#endif /* __cplusplus */

#endif 	/* __AlphaSource_FWD_DEFINED__ */


#ifndef __Source8Bit_FWD_DEFINED__
#define __Source8Bit_FWD_DEFINED__

#ifdef __cplusplus
typedef class Source8Bit Source8Bit;
#else
typedef struct Source8Bit Source8Bit;
#endif /* __cplusplus */

#endif 	/* __Source8Bit_FWD_DEFINED__ */


#ifndef __Source555Bit_FWD_DEFINED__
#define __Source555Bit_FWD_DEFINED__

#ifdef __cplusplus
typedef class Source555Bit Source555Bit;
#else
typedef struct Source555Bit Source555Bit;
#endif /* __cplusplus */

#endif 	/* __Source555Bit_FWD_DEFINED__ */


#ifndef __Source565Bit_FWD_DEFINED__
#define __Source565Bit_FWD_DEFINED__

#ifdef __cplusplus
typedef class Source565Bit Source565Bit;
#else
typedef struct Source565Bit Source565Bit;
#endif /* __cplusplus */

#endif 	/* __Source565Bit_FWD_DEFINED__ */


#ifndef __Source24Bit_FWD_DEFINED__
#define __Source24Bit_FWD_DEFINED__

#ifdef __cplusplus
typedef class Source24Bit Source24Bit;
#else
typedef struct Source24Bit Source24Bit;
#endif /* __cplusplus */

#endif 	/* __Source24Bit_FWD_DEFINED__ */


#ifndef __Source32Bit_FWD_DEFINED__
#define __Source32Bit_FWD_DEFINED__

#ifdef __cplusplus
typedef class Source32Bit Source32Bit;
#else
typedef struct Source32Bit Source32Bit;
#endif /* __cplusplus */

#endif 	/* __Source32Bit_FWD_DEFINED__ */


#ifndef __TransNull8_FWD_DEFINED__
#define __TransNull8_FWD_DEFINED__

#ifdef __cplusplus
typedef class TransNull8 TransNull8;
#else
typedef struct TransNull8 TransNull8;
#endif /* __cplusplus */

#endif 	/* __TransNull8_FWD_DEFINED__ */


#ifndef __TransNull555_FWD_DEFINED__
#define __TransNull555_FWD_DEFINED__

#ifdef __cplusplus
typedef class TransNull555 TransNull555;
#else
typedef struct TransNull555 TransNull555;
#endif /* __cplusplus */

#endif 	/* __TransNull555_FWD_DEFINED__ */


#ifndef __TransNull565_FWD_DEFINED__
#define __TransNull565_FWD_DEFINED__

#ifdef __cplusplus
typedef class TransNull565 TransNull565;
#else
typedef struct TransNull565 TransNull565;
#endif /* __cplusplus */

#endif 	/* __TransNull565_FWD_DEFINED__ */


#ifndef __TransNull24_FWD_DEFINED__
#define __TransNull24_FWD_DEFINED__

#ifdef __cplusplus
typedef class TransNull24 TransNull24;
#else
typedef struct TransNull24 TransNull24;
#endif /* __cplusplus */

#endif 	/* __TransNull24_FWD_DEFINED__ */


#ifndef __TransNull32_FWD_DEFINED__
#define __TransNull32_FWD_DEFINED__

#ifdef __cplusplus
typedef class TransNull32 TransNull32;
#else
typedef struct TransNull32 TransNull32;
#endif /* __cplusplus */

#endif 	/* __TransNull32_FWD_DEFINED__ */


#ifndef __TransNull32a_FWD_DEFINED__
#define __TransNull32a_FWD_DEFINED__

#ifdef __cplusplus
typedef class TransNull32a TransNull32a;
#else
typedef struct TransNull32a TransNull32a;
#endif /* __cplusplus */

#endif 	/* __TransNull32a_FWD_DEFINED__ */


#ifndef __TransSmpte_FWD_DEFINED__
#define __TransSmpte_FWD_DEFINED__

#ifdef __cplusplus
typedef class TransSmpte TransSmpte;
#else
typedef struct TransSmpte TransSmpte;
#endif /* __cplusplus */

#endif 	/* __TransSmpte_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __RGBFiltersLib_LIBRARY_DEFINED__
#define __RGBFiltersLib_LIBRARY_DEFINED__

/* library RGBFiltersLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_RGBFiltersLib;

EXTERN_C const CLSID CLSID_RateSource;

#ifdef __cplusplus

class DECLSPEC_UUID("98BEDFA2-C4A1-40ea-BBD2-C77070918F24")
RateSource;
#endif

EXTERN_C const CLSID CLSID_AlphaRenderer;

#ifdef __cplusplus

class DECLSPEC_UUID("66D155F8-1A72-4f6c-8D03-BE4F7CCF739A")
AlphaRenderer;
#endif

EXTERN_C const CLSID CLSID_AlphaSource;

#ifdef __cplusplus

class DECLSPEC_UUID("BE393783-E457-498b-AF1E-E0564F0F0D1F")
AlphaSource;
#endif

EXTERN_C const CLSID CLSID_Source8Bit;

#ifdef __cplusplus

class DECLSPEC_UUID("F39C8EA7-1ACB-46f3-A56D-F56F391F9D7D")
Source8Bit;
#endif

EXTERN_C const CLSID CLSID_Source555Bit;

#ifdef __cplusplus

class DECLSPEC_UUID("345446A2-B638-40b4-BBE5-4098028AA14B")
Source555Bit;
#endif

EXTERN_C const CLSID CLSID_Source565Bit;

#ifdef __cplusplus

class DECLSPEC_UUID("345446A3-B638-40b4-BBE5-4098028AA14B")
Source565Bit;
#endif

EXTERN_C const CLSID CLSID_Source24Bit;

#ifdef __cplusplus

class DECLSPEC_UUID("345446A4-B638-40b4-BBE5-4098028AA14B")
Source24Bit;
#endif

EXTERN_C const CLSID CLSID_Source32Bit;

#ifdef __cplusplus

class DECLSPEC_UUID("345446A5-B638-40b4-BBE5-4098028AA14B")
Source32Bit;
#endif

EXTERN_C const CLSID CLSID_TransNull8;

#ifdef __cplusplus

class DECLSPEC_UUID("345446A6-B638-40b4-BBE5-4098028AA14B")
TransNull8;
#endif

EXTERN_C const CLSID CLSID_TransNull555;

#ifdef __cplusplus

class DECLSPEC_UUID("345446A7-B638-40b4-BBE5-4098028AA14B")
TransNull555;
#endif

EXTERN_C const CLSID CLSID_TransNull565;

#ifdef __cplusplus

class DECLSPEC_UUID("345446A8-B638-40b4-BBE5-4098028AA14B")
TransNull565;
#endif

EXTERN_C const CLSID CLSID_TransNull24;

#ifdef __cplusplus

class DECLSPEC_UUID("345446A9-B638-40b4-BBE5-4098028AA14B")
TransNull24;
#endif

EXTERN_C const CLSID CLSID_TransNull32;

#ifdef __cplusplus

class DECLSPEC_UUID("345446AA-B638-40b4-BBE5-4098028AA14B")
TransNull32;
#endif

EXTERN_C const CLSID CLSID_TransNull32a;

#ifdef __cplusplus

class DECLSPEC_UUID("345446AB-B638-40b4-BBE5-4098028AA14B")
TransNull32a;
#endif

EXTERN_C const CLSID CLSID_TransSmpte;

#ifdef __cplusplus

class DECLSPEC_UUID("345446AC-B638-40b4-BBE5-4098028AA14B")
TransSmpte;
#endif
#endif /* __RGBFiltersLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


