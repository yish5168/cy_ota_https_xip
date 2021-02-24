/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2020 Cypress Semiconductor Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 /*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
 /*******************************************************************************/

#ifdef MCUBOOT_HAVE_ASSERT_H
#include "mcuboot_config/mcuboot_assert.h"
#else
#include <assert.h>
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "mcuboot_config/mcuboot_config.h"
#include "flash_map_backend/flash_map_backend.h"
#include <sysflash/sysflash.h>

#include "bootutil/bootutil_log.h"

#include "cy_pdl.h"

#ifdef CY_BOOT_USE_EXTERNAL_FLASH
#include "cy_smif_psoc6.h"
#endif

#ifndef CY_BOOTLOADER_START_ADDRESS
#define CY_BOOTLOADER_START_ADDRESS        (0x10000000)
#endif

#ifdef CY_FLASH_MAP_EXT_DESC
static struct flash_area bootloader =
{
    .fa_id = FLASH_AREA_BOOTLOADER,
    .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
    .fa_off = CY_BOOTLOADER_START_ADDRESS,
    .fa_size = CY_BOOT_BOOTLOADER_SIZE
};

static struct flash_area primary_1 =
{
    .fa_id = FLASH_AREA_IMAGE_PRIMARY(0),
    .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
    .fa_off = CY_FLASH_BASE + CY_BOOT_BOOTLOADER_SIZE,
    .fa_size = CY_BOOT_PRIMARY_1_SIZE
};

#if (MCUBOOT_IMAGE_NUMBER == 1)
static struct flash_area secondary_1 =
{
    .fa_id = FLASH_AREA_IMAGE_SECONDARY(0),
    .fa_device_id = FLASH_DEVICE_EXTERNAL_FLASH(CY_BOOT_EXTERNAL_DEVICE_INDEX),
    .fa_off = CY_SMIF_BASE_MEM_OFFSET,
    .fa_size = CY_BOOT_SECONDARY_1_SIZE
};
#endif

#if (MCUBOOT_IMAGE_NUMBER == 2) /* if dual-image */
static struct flash_area primary_2 =
{
    .fa_id = FLASH_AREA_IMAGE_PRIMARY(1),
    .fa_device_id = FLASH_DEVICE_EXTERNAL_FLASH(CY_BOOT_EXTERNAL_DEVICE_INDEX),
    .fa_off = CY_SMIF_BASE_MEM_OFFSET,
    .fa_size = CY_BOOT_PRIMARY_2_SIZE
};

static struct flash_area secondary_1 =
{
    .fa_id = FLASH_AREA_IMAGE_SECONDARY(0),
    .fa_device_id = FLASH_DEVICE_EXTERNAL_FLASH(CY_BOOT_EXTERNAL_DEVICE_INDEX),
    .fa_off = CY_SMIF_BASE_MEM_OFFSET +\
                CY_BOOT_PRIMARY_2_SIZE,
    .fa_size = CY_BOOT_SECONDARY_1_SIZE
};

static struct flash_area secondary_2 =
{
    .fa_id = FLASH_AREA_IMAGE_SECONDARY(1),
    /* it is for external flash memory */
    .fa_device_id = FLASH_DEVICE_EXTERNAL_FLASH(CY_BOOT_EXTERNAL_DEVICE_INDEX),
    .fa_off = CY_SMIF_BASE_MEM_OFFSET +\
                CY_BOOT_PRIMARY_2_SIZE +\
                0x200000,
    .fa_size = CY_BOOT_SECONDARY_2_SIZE
};
#endif

#ifdef MCUBOOT_SWAP_USING_SCRATCH
static struct flash_area scratch =
{
    .fa_id = FLASH_AREA_IMAGE_SCRATCH,
    .fa_device_id = FLASH_DEVICE_INTERNAL_FLASH,
    .fa_off = CY_FLASH_BASE +\
               CY_BOOT_BOOTLOADER_SIZE +\
               CY_BOOT_APP_CM0P_SIZE +\
               CY_BOOT_APP_CM4_SIZE,
    .fa_size = CY_BOOT_SCRATCH_SIZE
};
#endif

struct flash_area *boot_area_descs[] =
{
    &bootloader,
    &primary_1,
    &secondary_1,
#if (MCUBOOT_IMAGE_NUMBER == 2) /* if dual-image */
    &primary_2,
    &secondary_2,
#endif
#ifdef MCUBOOT_SWAP_USING_SCRATCH
    &scratch,
#endif
    NULL
};
#endif

