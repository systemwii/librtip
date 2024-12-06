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

#include <gccore.h>
#include <ogc/machine/processor.h>
#include <stdio.h>
#include <string.h>
#include "patches.h"
#include "rtip.h"

#define MEM_REG_BASE 0xd8b4000
#define MEM_PROT (MEM_REG_BASE + 0x20a)

/* Set foreground color */
static void TextColor(u8 color, u8 bold) {
	printf("\x1b[%u;%um", color + 30, bold);
}
#define COLOR(printfCall, color)  TextColor(color, 1); printfCall; TextColor(7, 1)

static inline void disable_memory_protection(void) {
	write32(MEM_PROT, read32(MEM_PROT) & 0x0000FFFF);
}

static u8 apply_patch(const char *name, const u8 *old, u32 old_size, const u8 *patch, size_t patch_size, u32 patch_offset, bool verbose) {
	u8 *ptr_start = (u8*)*((u32*)0x80003134);
	u8 *ptr_end = (u8*)0x94000000;
	u8 *location = NULL;
	u8 found = 0;
	if (verbose) { printf("\t\t    Patching %-30s", name); }
	while (ptr_start < (ptr_end - patch_size)) {
		if (!memcmp(ptr_start, old, old_size)) {
			found++;
			location = ptr_start + patch_offset;
			u8 *start = location;
			u32 i;
			for (i = 0; i < patch_size; i++) {
				*location++ = patch[i];
			}
			DCFlushRange((u8 *)(((u32)start) >> 5 << 5), (patch_size >> 5 << 5) + 64);
			ICInvalidateRange((u8 *)(((u32)start) >> 5 << 5), (patch_size >> 5 << 5) + 64);
		}
		ptr_start++;
	}
	if (verbose) { COLOR( printf("\t\t%d patched\n", found) , found?2:1 ); }
	return found;
}

s32 IosPatch_AHBPROT(bool verbose) {
	if (!AHBPROT_DISABLED) { return ERROR_AHBPROT; }
	disable_memory_protection();
	s32 ret = apply_patch("es_set_ahbprot", es_set_ahbprot_old, sizeof(es_set_ahbprot_old), es_set_ahbprot_patch, sizeof(es_set_ahbprot_patch), 25, verbose);
	if (!ret) { return ERROR_PATCH; }
	return ret;
}

s32 IosPatch_RUNTIME(u32 platforms, u32 enabled, bool verbose) {
	if (!AHBPROT_DISABLED) { return ERROR_AHBPROT; }
	disable_memory_protection();
	s32 count = 0;
	if (verbose) printf("\t\t\n");
	if (platforms & PATCH_WII) {
		if (verbose) { COLOR( printf("\t>> Applying base Wii patches:\n") , 6 ); }
		if (enabled & PATCH_DI_READLIMIT) count += apply_patch("di_readlimit", di_readlimit_old, sizeof(di_readlimit_old), di_readlimit_patch, sizeof(di_readlimit_patch), 12, verbose);
		if (enabled & PATCH_ISFS_PERMISSIONS) count += apply_patch("isfs_permissions", isfs_permissions_old, sizeof(isfs_permissions_old), isfs_permissions_patch, sizeof(isfs_permissions_patch), 0, verbose);
		if (enabled & PATCH_ES_SETUID) count += apply_patch("es_setuid", setuid_old, sizeof(setuid_old), setuid_patch, sizeof(setuid_patch), 0, verbose);
		if (enabled & PATCH_ES_IDENTIFY) count += apply_patch("es_identify", es_identify_old, sizeof(es_identify_old), es_identify_patch, sizeof(es_identify_patch), 2, verbose);
		if (enabled & PATCH_HASH_CHECK) count += apply_patch("hash_check", hash_old, sizeof(hash_old), hash_patch, sizeof(hash_patch), 1, verbose);
		if (enabled & PATCH_NEW_HASH_CHECK) count += apply_patch("new_hash_check", new_hash_old, sizeof(new_hash_old), hash_patch, sizeof(hash_patch), 1, verbose);
		if (enabled & PATCH_ISFS_SETATTR) count += apply_patch("isfs_setattr_pt1", isfs_setattr_pt1_old, sizeof(isfs_setattr_pt1_old), isfs_setattr_pt1_patch, sizeof(isfs_setattr_pt1_patch), 0, verbose);
		if (enabled & PATCH_ISFS_SETATTR) count += apply_patch("isfs_setattr_pt2", isfs_setattr_pt2_old, sizeof(isfs_setattr_pt2_old), isfs_setattr_pt2_patch, sizeof(isfs_setattr_pt2_patch), 0, verbose);
		if (enabled & PATCH_SSL) count += apply_patch("ssl_patch1", ssl_patch1_old, sizeof(ssl_patch1_old), ssl_patch1_new, sizeof(ssl_patch1_new), 0, verbose);
		if (enabled & PATCH_SSL) count += apply_patch("ssl_patch2", ssl_patch2_old, sizeof(ssl_patch2_old), ssl_patch2_new, sizeof(ssl_patch2_new), 0, verbose);
		if (enabled & PATCH_SSL) count += apply_patch("ssl_patch3", ssl_patch3_old, sizeof(ssl_patch3_old), ssl_patch3_new, sizeof(ssl_patch3_new), 0, verbose);
		if (enabled & PATCH_SSL) count += apply_patch("ssl_patch4", ssl_patch4_old, sizeof(ssl_patch4_old), ssl_patch4_new, sizeof(ssl_patch4_new), 0, verbose);
	}
	if (platforms & PATCH_SCIIFII) {
		if (verbose) { COLOR ( printf("\t>> Applying Sciifii patches:\n") , 6 ); }
		if (enabled & PATCH_MEM2_PROT) count += apply_patch("MEM2_prot", MEM2_prot_old, sizeof(MEM2_prot_old), MEM2_prot_patch, sizeof(MEM2_prot_patch), 0, verbose);
		if (enabled & PATCH_ES_OPENTITLECONTENT) count += apply_patch("ES_OpenTitleContent1", ES_OpenTitleContent1_old, sizeof(ES_OpenTitleContent1_old), ES_OpenTitleContent1_patch, sizeof(ES_OpenTitleContent1_patch), 0, verbose);
		if (enabled & PATCH_ES_OPENTITLECONTENT) count += apply_patch("ES_OpenTitleContent2", ES_OpenTitleContent2_old, sizeof(ES_OpenTitleContent2_old), ES_OpenTitleContent2_patch, sizeof(ES_OpenTitleContent2_patch), 0, verbose);
		if (enabled & PATCH_ES_READCONTENT_PROT) count += apply_patch("ES_ReadContent_prot", ES_ReadContent_old, sizeof(ES_ReadContent_old), ES_ReadContent_patch, sizeof(ES_ReadContent_patch), 0, verbose);
		if (enabled & PATCH_ES_CLOSECONTENT) count += apply_patch("ES_CloseContent", ES_CloseContent_old, sizeof(ES_CloseContent_old), ES_CloseContent_patch, sizeof(ES_CloseContent_patch), 0, verbose);
		if (enabled & PATCH_ES_TITLEVERSIONCHECK) count += apply_patch("ES_TitleVersionCheck", ES_TitleVersionCheck_old, sizeof(ES_TitleVersionCheck_old), ES_TitleVersionCheck_patch, sizeof(ES_TitleVersionCheck_patch), 0, verbose);
		if (enabled & PATCH_ES_TITLEDELETECHECK) count += apply_patch("ES_TitleDeleteCheck", ES_TitleDeleteCheck_old, sizeof(ES_TitleDeleteCheck_old), ES_TitleDeleteCheck_patch, sizeof(ES_TitleDeleteCheck_patch), 0, verbose);
	}
	if (platforms & PATCH_VWII) {
		if (verbose) { COLOR( printf("\t>> Applying vWii patches:\n") , 6 ); }
		if (enabled & PATCH_KILL_ANTISYSTITLEINSTALLV3) count += apply_patch("Kill_AntiSysTitleInstallv3_pt1", Kill_AntiSysTitleInstallv3_pt1_old, sizeof(Kill_AntiSysTitleInstallv3_pt1_old), Kill_AntiSysTitleInstallv3_pt1_patch, sizeof(Kill_AntiSysTitleInstallv3_pt1_patch), 0, verbose);
		if (enabled & PATCH_KILL_ANTISYSTITLEINSTALLV3) count += apply_patch("Kill_AntiSysTitleInstallv3_pt2", Kill_AntiSysTitleInstallv3_pt2_old, sizeof(Kill_AntiSysTitleInstallv3_pt2_old), Kill_AntiSysTitleInstallv3_pt2_patch, sizeof(Kill_AntiSysTitleInstallv3_pt2_patch), 0, verbose);
		if (enabled & PATCH_KILL_ANTISYSTITLEINSTALLV3) count += apply_patch("Kill_AntiSysTitleInstallv3_pt3", Kill_AntiSysTitleInstallv3_pt3_old, sizeof(Kill_AntiSysTitleInstallv3_pt3_old), Kill_AntiSysTitleInstallv3_pt3_patch, sizeof(Kill_AntiSysTitleInstallv3_pt3_patch), 0, verbose);
	}
	return count;
}

s32 IosPatch_FULL(u32 platforms, u32 enabled, bool verbose, int IOS) {
	if (!AHBPROT_DISABLED) { return ERROR_AHBPROT; }
	s32 ret = IosPatch_AHBPROT(verbose);
	if (!ret) {	return ERROR_PATCH; }
	IOS_ReloadIOS(IOS);
	return IosPatch_RUNTIME(platforms, enabled, verbose);
}
