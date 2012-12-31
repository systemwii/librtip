// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// Copyright (C) 2010	Joseph Jordan <joe.ftpii@psychlaw.com.au>
// Copyright (C) 2012	damysteryman
// Copyright (C) 2012	Christopher Bratusek <nano@tuxfamily.org>

#include <gccore.h>
#include <ogc/machine/processor.h>
#include <stdio.h>
#include <string.h>

#include "runtimeiospatch.h"

#define MEM_REG_BASE 0xd8b4000
#define MEM_PROT (MEM_REG_BASE + 0x20a)

//const s8 check_tmd_patch1[] = { 0x23, 0x01, 0x42, 0x5B };


static void disable_memory_protection() {
	write32(MEM_PROT, read32(MEM_PROT) & 0x0000FFFF);
}

static const s8 di_readlimit_old[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0A, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
	0x7E, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08
};
static const s8 di_readlimit_patch[] = { 0x7e, 0xd4 };

const s8 isfs_permissions_old[] = { 0x42, 0x8B, 0xD0, 0x01, 0x25, 0x66 };
const s8 isfs_permissions_patch[] = { 0x42, 0x8B, 0xE0, 0x01, 0x25, 0x66 };
const s8 setuid_old[] = { 0xD1, 0x2A, 0x1C, 0x39 };
const s8 setuid_patch[] = { 0x46, 0xC0 };
const s8 es_identify_old[] = { 0x28, 0x03, 0xD1, 0x23 };
const s8 es_identify_patch[] = { 0x00, 0x00 };
const s8 hash_old[] = { 0x20, 0x07, 0x4B, 0x0B };
const s8 hash_patch[] = { 0x00 };
const s8 new_hash_old[] = { 0x20, 0x07, 0x4B, 0x0B };
const s8 addticket_vers_check[] = { 0xD2, 0x01, 0x4E, 0x56 };
const s8 addticket_patch[] = { 0xE0 };
const s8 es_set_ahbprot_old[] = { 0x68, 0x5B, 0x22, 0xEC, 0x00, 0x52, 0x18, 0x9B, 0x68, 0x1B, 0x46, 0x98, 0x07, 0xDB };
const s8 es_set_ahbprot_patch[]   = { 0x01 };

// Following patches added to iospatch.c by damysteryman, taken from sciifii v5
const s8 MEM2_prot_old[] = { 0xB5, 0x00, 0x4B, 0x09, 0x22, 0x01, 0x80, 0x1A, 0x22, 0xF0 };
const s8 MEM2_prot_patch[] = { 0xB5, 0x00, 0x4B, 0x09, 0x22, 0x00, 0x80, 0x1A, 0x22, 0xF0 };
const s8 ES_OpenTitleContent1_old[] = { 0x9D, 0x05, 0x42, 0x9D, 0xD0, 0x03 };
const s8 ES_OpenTitleContent1_patch[] = { 0x9D, 0x05, 0x42, 0x9D, 0xE0, 0x03 };
const s8 ES_OpenTitleContent2_old[] = { 0xD4, 0x01, 0x4C, 0x36, 0xE0, 0x3B };
const s8 ES_OpenTitleContent2_patch[] = { 0xE0, 0x01, 0x4C, 0x36, 0xE0, 0x3B };
const s8 ES_ReadContent_old[] = { 0xFC, 0x0F, 0xB5, 0x30, 0x1C, 0x14, 0x1C, 0x1D, 0x4B,
				   0x0E, 0x68, 0x9B, 0x2B, 0x00, 0xD0, 0x03, 0x29, 0x00, 0xDB, 0x01,
				   0x29, 0x0F, 0xDD, 0x01 };
const s8 ES_ReadContent_patch[] = { 0xFC, 0x0F, 0xB5, 0x30, 0x1C, 0x14, 0x1C, 0x1D, 0x4B,
				   0x0E, 0x68, 0x9B, 0x2B, 0x00, 0x46, 0xC0, 0x29, 0x00, 0x46, 0xC0,
				   0x29, 0x0F, 0xE0, 0x01 };
const s8 ES_CloseContent_old[] = { 0xB5, 0x10, 0x4B, 0x10, 0x68, 0x9B, 0x2B, 0x00, 0xD0,
				   0x03, 0x29, 0x00, 0xDB, 0x01, 0x29, 0x0F, 0xDD, 0x01 };
const s8 ES_CloseContent_patch[] = { 0xB5, 0x10, 0x4B, 0x10, 0x68, 0x9B, 0x2B, 0x00, 0x46,
				   0xC0, 0x29, 0x00, 0x46, 0xC0, 0x29, 0x0F, 0xE0, 0x01 };
const s8 ES_TitleVersionCheck_old[] = { 0xD2, 0x01, 0x4E, 0x56 };
const s8 ES_TitleVersionCheck_patch[] = { 0xE0, 0x01, 0x4E, 0x56 };
const s8 ES_TitleDeleteCheck_old[] = { 0xD8, 0x00, 0x4A, 0x04 };
const s8 ES_TitleDeleteCheck_patch[] = { 0xE0, 0x00, 0x4A, 0x04 };

// Following patches made my damysteryman for use with Wii U's vWii
const s8 Kill_AntiSysTitleInstallv2_pt1_old[] = { 0x68, 0x1A, 0x2A, 0x01, 0xD0, 0x05 };	// Make sure that the pt1
const s8 Kill_AntiSysTitleInstallv2_pt1_patch[] = { 0x68, 0x1A, 0x2A, 0x01, 0x46, 0xC0 }; // patch is applied twice. -dmm
const s8 Kill_AntiSysTitleInstallv2_pt2_old[] = { 0x4B, 0x29, 0x42, 0x9A, 0xD0, 0x02 };
const s8 Kill_AntiSysTitleInstallv2_pt2_patch[] = { 0x4B, 0x29, 0x42, 0x9A, 0x46, 0xC0 };
const s8 Kill_AntiSysTitleInstallv2_pt3_old[] = { 0xD0, 0x02, 0x33, 0x06 };
const s8 Kill_AntiSysTitleInstallv2_pt3_patch[] = { 0x46, 0xC0, 0x33, 0x06 };
const s8 Kill_AntiSysTitleInstallv2_pt4_old[] = { 0x2C, 0x01, 0xD0, 0x07};
const s8 Kill_AntiSysTitleInstallv2_pt4_patch[] = { 0x2C, 0x01, 0x46, 0xC0};
const s8 Kill_AntiSysTitleInstallv2_pt5_old[] = { 0x42, 0x94, 0xD0, 0x03, 0x9C, 0x03 };
const s8 Kill_AntiSysTitleInstallv2_pt5_patch[] = { 0x42, 0x94, 0x46, 0xC0, 0x9C, 0x03 };

static s32 apply_patch(char *name, const s8 *old, const s8 *patch, s32 patch_offset, bool verbose) {
	s32 patch_size = sizeof(patch);
	s8 *ptr_start = (s8*)*((s32*)0x80003134), *ptr_end = (s8*)0x94000000;
	if(verbose) printf("    Patching %-30s", name);
	s32 found = 0;
	s8 *location = NULL;
	while (ptr_start < (ptr_end - patch_size)) {
		if (!memcmp(ptr_start, old, sizeof(old))) {
			found++;
			location = ptr_start + patch_offset;
			s8 *start = location;
			s32 i;
			for (i = 0; i < patch_size; i++) {
				*location++ = patch[i];
			}
			DCFlushRange((s8 *)(((s32)start) >> 5 << 5), (patch_size >> 5 << 5) + 64);
			ICInvalidateRange((s8 *)(((s32)start) >> 5 << 5), (patch_size >> 5 << 5) + 64);
		}
		ptr_start++;
	}
	if(verbose){
		if (found)
			printf(" patched\n");
		else
			printf(" not patched\n");
	}

	return found;
}

s32 IosPatch_AHBPROT(bool verbose) {
	if (AHBPROT_DISABLED) {
		disable_memory_protection();
		//return apply_patch("set_ahbprot", check_tmd_old, check_tmd_patch, 6, verbose);
		s32 ret = apply_patch("es_set_ahbprot", es_set_ahbprot_old, es_set_ahbprot_patch, 25, verbose);
		if (ret)
			return ret;
		else
			return -7;
	}
	return -5;
}

s32 IosPatch_RUNTIME(bool wii, bool sciifii, bool vwii, bool verbose) {
	s32 count = 0;

	if (AHBPROT_DISABLED) {
		disable_memory_protection();
		if(wii)
		{
			if(verbose) printf(">> Applying standard Wii patches:\n");
			count += apply_patch("di_readlimit", di_readlimit_old, di_readlimit_patch, 12, verbose);
			count += apply_patch("isfs_permissions", isfs_permissions_old, isfs_permissions_patch, 0, verbose);
			count += apply_patch("es_setuid", setuid_old, setuid_patch, 0, verbose);
			count += apply_patch("es_identify", es_identify_old, es_identify_patch, 2, verbose);
			count += apply_patch("hash_check", hash_old, hash_patch, 1, verbose);
			count += apply_patch("new_hash_check", new_hash_old, hash_patch, 1, verbose);
		}
		if(sciifii)
		{
			if(verbose) printf(">> Applying Sciifii patches:\n");
			count += apply_patch("MEM2_prot", MEM2_prot_old, MEM2_prot_patch, 0, verbose);
			count += apply_patch("ES_OpenTitleContent1", ES_OpenTitleContent1_old, ES_OpenTitleContent1_patch, 0, verbose);
			count += apply_patch("ES_OpenTitleContent2", ES_OpenTitleContent2_old, ES_OpenTitleContent2_patch, 0, verbose);
			count += apply_patch("ES_ReadContent_prot", ES_ReadContent_old, ES_ReadContent_patch, 0, verbose);
			count += apply_patch("ES_CloseContent", ES_CloseContent_old, ES_CloseContent_patch, 0, verbose);
			count += apply_patch("ES_TitleVersionCheck", ES_TitleVersionCheck_old, ES_TitleVersionCheck_patch, 0, verbose);
			count += apply_patch("ES_TitleDeleteCheck", ES_TitleDeleteCheck_old, ES_TitleDeleteCheck_patch, 0, verbose);
		}
		if(vwii)
		{
			if(verbose) printf(">> Applying vWii patches:\n");
			count += apply_patch("Kill_AntiSysTitleInstallv2_pt1", Kill_AntiSysTitleInstallv2_pt1_old, Kill_AntiSysTitleInstallv2_pt1_patch, 0, verbose);
			count += apply_patch("Kill_AntiSysTitleInstallv2_pt2", Kill_AntiSysTitleInstallv2_pt2_old, Kill_AntiSysTitleInstallv2_pt2_patch, 0, verbose);
			count += apply_patch("Kill_AntiSysTitleInstallv2_pt3", Kill_AntiSysTitleInstallv2_pt3_old, Kill_AntiSysTitleInstallv2_pt3_patch, 0, verbose);
			count += apply_patch("Kill_AntiSysTitleInstallv2_pt4", Kill_AntiSysTitleInstallv2_pt4_old, Kill_AntiSysTitleInstallv2_pt4_patch, 0, verbose);
			count += apply_patch("Kill_AntiSysTitleInstallv2_pt5", Kill_AntiSysTitleInstallv2_pt5_old, Kill_AntiSysTitleInstallv2_pt5_patch, 0, verbose);
		}
		return count;
	}
	return -5;
}

s32 IosPatch_FULL(bool wii, bool sciifii, bool vwii, bool verbose, int IOS) {
	s32 ret = 0;
	s32 xret = 0;

	if (AHBPROT_DISABLED)
		ret = IosPatch_AHBPROT(verbose);
	else
		return -5;

	if (ret) {
		IOS_ReloadIOS(IOS);
		xret = IosPatch_RUNTIME(wii, sciifii, vwii, verbose);
	} else {
		xret = -7;
	}

	return xret;

}
