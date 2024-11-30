// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// Copyright (C) 2010		Joseph Jordan <joe.ftpii@psychlaw.com.au>
// Copyright (C) 2012-2013	damysteryman
// Copyright (C) 2012-2015	Christopher Bratusek <nano@jpberlin.de>
// Copyright (C) 2013		DarkMatterCore
// Copyright (C) 2014		megazig
// Copyright (C) 2015		FIX94

#ifndef __RUNTIMEIOSPATCH_H__
#define __RUNTIMEIOSPATCH_H__

/**
 * Version information for Libruntimeiospatch.
 */
#define LIB_RUNTIMEIOSPATCH_VERSION "s2.0"

//==============================================================================
// HW_RVL header
//==============================================================================
#if defined(HW_RVL) /* defined(HW_RVL) */

/**
 *Returns true when HW_AHBPROT access can be applied
 */
#define AHBPROT_DISABLED (*(vu32*)0xcd800064 == 0xFFFFFFFF)

//==============================================================================
// Error code definitions:
//==============================================================================
#define ERROR_AHBPROT       -5
#define ERROR_PATCH         -7

//==============================================================================
// C++ header
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif
/* __cplusplus */

//==============================================================================
// Patchsets:
//==============================================================================

// options for "platforms" argument
// to select multiple, join them with |
// to select all, set the argument to ~0
enum Patchsets {
    PATCH_WII = 1<<0,       // base patchset used for Wii
    PATCH_SCIIFII = 1<<1,   // extra patches for Sciifii
    PATCH_VWII = 1<<2,      // extra patches for vWii
};

// options for "enabled" argument (see above for usage notes)
// note that PATCH_HASH_CHECK and PATCH_NEW_HASH_CHECK have the same effect 
// but apply to different IOSes 
enum Patches {
    // Wii
    PATCH_DI_READLIMIT = 1<<0,
    PATCH_ISFS_PERMISSIONS = 1<<1,
    PATCH_ES_SETUID = 1<<2,
    PATCH_ES_IDENTIFY = 1<<3,
    PATCH_HASH_CHECK = 1<<4,        // aka FakeSign/Trucha
    PATCH_NEW_HASH_CHECK = 1<<5,    // aka New FakeSign/Trucha
    PATCH_ISFS_SETATTR = 1<<6,
    PATCH_SSL = 1<<7,                  
    // Sciifii
    PATCH_MEM2_PROT = 1<<8,
    PATCH_ES_OPENTITLECONTENT = 1<<9,
    PATCH_ES_READCONTENT_PROT = 1<<10,
    PATCH_ES_CLOSECONTENT = 1<<11,
    PATCH_ES_TITLEVERSIONCHECK = 1<<12,
    PATCH_ES_TITLEDELETECHECK = 1<<13,
    // VWii
    PATCH_KILL_ANTISYSTITLEINSTALLV3 = 1<<14,
};

//==============================================================================
// Functions:
//==============================================================================

/**
 * This function can be used to keep HW_AHBPROT access when going to reload IOS
 * @param verbose Flag determining whether or not to print messages on-screen
 * @example 
 *      if(AHBPROT_DISABLED) {
 *          s32 ret;
 *          ret = IosPatch_AHBPROT(false);
 *          if (ret) {
 *              IOS_ReloadIOS(36);
 *          } else {
 *              printf("IosPatch_AHBPROT failed.");
 *          }
 *      }
 * @return Signed 32bit integer representing code
 *      > 0             : Success   - return equals number of applied patches
 *      ERROR_AHBPROT   : Error     - No HW_AHBPROT access
 */
s32 IosPatch_AHBPROT(bool verbose);


/**
 * This function applies patches onto currently loaded IOS
 * @see Patchsets
 * @param platforms Bitmask enabling platform patchsets.
 * @param enabled Bitmask enabling individual patches; ~0 enables all.
 * @param verbose Flag determining whether or not to print messages on screen.
 * @example if(AHBPROT_DISABLED) IosPatch_FULL(PATCH_WII | PATCH_VWII, ~0, false);
 * @return Signed 32bit integer representing code
 *      > 0             : Success   - return equals number of applied patches
 *      ERROR_AHBPROT   : Error     - No HW_AHBPROT access
 *      ERROR_PATCH     : Error     - Patching HW_AHBPROT access failed
 */
s32 IosPatch_RUNTIME(u32 platforms, u32 enabled, bool verbose);


/**
 * This function combines IosPatch_AHBPROT + IOS_ReloadIOS + IosPatch_RUNTIME
 * @see Patchsets
 * @param platforms Bitmask enabling platform patchsets.
 * @param enabled Bitmask enabling individual patches; ~0 enables all.
 * @param verbose Flag determining whether or not to print messages on screen.
 * @param IOS Which IOS to reload into.
 * @example if(AHBPROT_DISABLED) IosPatch_FULL(PATCH_WII, ~0, false, 58);
 * @return Signed 32bit integer representing code
 *      > 0             : Success   - return equals to number of applied patches
 *      ERROR_AHBPROT   : Error     - No HW_AHBPROT access
 *      ERROR_PATCH     : Error     - Patching HW_AHBPROT access failed
 */
s32 IosPatch_FULL(u32 platforms, u32 enabled, bool verbose, int IOS);

//==============================================================================
// C++ footer
//==============================================================================
#ifdef __cplusplus
}
#endif /* __cplusplus */

//==============================================================================
// HW_RVL footer
//==============================================================================
#endif /* defined(HW_RVL) */

#endif
