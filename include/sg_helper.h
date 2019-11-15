//
// Do NOT modify or remove this copyright and license
//
// Copyright (c) 2012 - 2018 Seagate Technology LLC and/or its Affiliates, All Rights Reserved
//
// This software is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// ******************************************************************************************
// 
// \file sg_helper.h
// \brief Defines the constants structures specific to Linux OS. 
//
#ifndef SG_HELPER_H
#define SG_HELPER_H

#include "scsi_helper.h"
#include "sat_helper.h"
#if !defined(DISABLE_NVME_PASSTHROUGH)
#include "nvme_helper.h"
#endif
#include "common_public.h"

#if defined (__cplusplus)
extern "C"
{
#endif

// \file sg_helper.h
// \brief Defines the constants structures and function headers to help parse scsi drives.

    #include <stdlib.h> // for size_t types
    #include <stdio.h>  // for printf
    #include <string.h> // For memset
    #include <unistd.h> // For getpagesize
// \todo Figure out which scsi.h & sg.h should we be including kernel specific or in /usr/..../include
    #include <scsi/sg.h>
    #include <scsi/scsi.h>
#if !defined(DISABLE_NVME_PASSTHROUGH)
    #if defined (__has_include)//GCC5 and higher support this, BUT only if a C standard is specified. The -std=gnuXX does not support this properly for some odd reason.
        #if __has_include (<linux/nvme_ioctl.h>)
            #pragma message "Using linux/nvme_ioctl.h"
            #include <linux/nvme_ioctl.h>
            #if !defined (SEA_NVME_IOCTL_H)
                #define SEA_NVME_IOCTL_H
            #endif
        #elif __has_include (<linux/nvme.h>)
            #pragma message "Using linux/nvme.h"
            #include <linux/nvme.h>
            #if !defined (SEA_NVME_IOCTL_H)
                #define SEA_NVME_IOCTL_H
            #endif
        #elif __has_include (<uapi/nvme.h>)
            #pragma message "Using uapi/nvme.h"
            #include <uapi/nvme.h>
            #if !defined (SEA_UAPI_NVME_H)
                #define SEA_UAPI_NVME_H
            #endif
        #else //__has_include could not locate the header, check if it was specified by the user through a define.
            #if defined (SEA_NVME_IOCTL_H)
                #include <linux/nvme_ioctl.h>
            #elif defined (SEA_NVME_H)
                #include <linux/nvme.h>
            #elif defined (SEA_UAPI_NVME_H)
                #include <uapi/nvme.h>
            #else
                #pragma GCC error "Please define one of the following to include the correct NVMe header: SEA_NVME_IOCTL_H, SEA_NVME_H, or SEA_UAPI_NVME_H\nThese specify whether the NVMe IOCTL is in /usr/include/linux/nvme_ioctl.h, /usr/include/linux/nvme.h, or /usr/include/uapi/nvme.h"
            #endif
        #endif
    #else
        #if defined (SEA_NVME_IOCTL_H)
            #include <linux/nvme_ioctl.h>
        #elif defined (SEA_NVME_H)
            #include <linux/nvme.h>
        #elif defined (SEA_UAPI_NVME_H)
            #include <uapi/nvme.h>
        #else
            #pragma GCC error "Please define one of the following to include the correct NVMe header: SEA_NVME_IOCTL_H, SEA_NVME_H, or SEA_UAPI_NVME_H\nThese specify whether the NVMe IOCTL is in /usr/include/linux/nvme_ioctl.h, /usr/include/linux/nvme.h, or /usr/include/uapi/nvme.h"
        #endif
    #endif
    #include "nvme_helper.h"
#endif

#define SG_PHYSICAL_DRIVE   "/dev/sg" //followed by a number
#define SD_PHYSICAL_DRIVE   "/dev/sd" //followed by a letter
#define BSG_PHYSICAL_DRIVE  "/dev/bsg/" //remaining part of the handle is h:c:t:l

//SG Driver status's since they are not available through standard includes we're using

#ifndef OPENSEA_SG_ERR_DRIVER_MASK
#define OPENSEA_SG_ERR_DRIVER_MASK 0x0F
#endif

#ifndef OPENSEA_SG_ERR_DRIVER_OK
#define OPENSEA_SG_ERR_DRIVER_OK 0x00
#endif

#ifndef OPENSEA_SG_ERR_DRIVER_BUSY
#define OPENSEA_SG_ERR_DRIVER_BUSY 0x01
#endif

#ifndef OPENSEA_SG_ERR_DRIVER_SOFT
#define OPENSEA_SG_ERR_DRIVER_SOFT 0x02
#endif

#ifndef OPENSEA_SG_ERR_DRIVER_MEDIA
#define OPENSEA_SG_ERR_DRIVER_MEDIA 0x03
#endif

#ifndef OPENSEA_SG_ERR_DRIVER_ERROR
#define OPENSEA_SG_ERR_DRIVER_ERROR 0x04
#endif

#ifndef OPENSEA_SG_ERR_DRIVER_INVALID
#define OPENSEA_SG_ERR_DRIVER_INVALID 0x05
#endif

#ifndef OPENSEA_SG_ERR_DRIVER_TIMEOUT
#define OPENSEA_SG_ERR_DRIVER_TIMEOUT 0x06
#endif

#ifndef OPENSEA_SG_ERR_DRIVER_HARD
#define OPENSEA_SG_ERR_DRIVER_HARD 0x07
#endif

#ifndef OPENSEA_SG_ERR_DRIVER_SENSE
#define OPENSEA_SG_ERR_DRIVER_SENSE 0x08
#endif

//Driver error suggestions
#ifndef OPENSEA_SG_ERR_SUGGEST_MASK
#define OPENSEA_SG_ERR_SUGGEST_MASK 0xF0
#endif

#ifndef OPENSEA_SG_ERR_SUGGEST_NONE
#define OPENSEA_SG_ERR_SUGGEST_NONE 0x00
#endif

#ifndef OPENSEA_SG_ERR_SUGGEST_RETRY
#define OPENSEA_SG_ERR_SUGGEST_RETRY 0x10
#endif

#ifndef OPENSEA_SG_ERR_SUGGEST_ABORT
#define OPENSEA_SG_ERR_SUGGEST_ABORT 0x20
#endif

#ifndef OPENSEA_SG_ERR_SUGGEST_REMAP
#define OPENSEA_SG_ERR_SUGGEST_REMAP 0x30
#endif

#ifndef OPENSEA_SG_ERR_SUGGEST_DIE
#define OPENSEA_SG_ERR_SUGGEST_DIE 0x40
#endif

#ifndef OPENSEA_SG_ERR_SUGGEST_SENSE
#define OPENSEA_SG_ERR_SUGGEST_SENSE 0x80
#endif

//Host errors
#ifndef OPENSEA_SG_ERR_DID_OK
#define OPENSEA_SG_ERR_DID_OK 0x0000
#endif

#ifndef OPENSEA_SG_ERR_DID_NO_CONNECT
#define OPENSEA_SG_ERR_DID_NO_CONNECT 0x0001
#endif

#ifndef OPENSEA_SG_ERR_DID_BUS_BUSY
#define OPENSEA_SG_ERR_DID_BUS_BUSY 0x0002
#endif

#ifndef OPENSEA_SG_ERR_DID_TIME_OUT
#define OPENSEA_SG_ERR_DID_TIME_OUT 0x0003
#endif

#ifndef OPENSEA_SG_ERR_DID_BAD_TARGET
#define OPENSEA_SG_ERR_DID_BAD_TARGET 0x0004
#endif

#ifndef OPENSEA_SG_ERR_DID_ABORT
#define OPENSEA_SG_ERR_DID_ABORT 0x0005
#endif

#ifndef OPENSEA_SG_ERR_DID_PARITY
#define OPENSEA_SG_ERR_DID_PARITY 0x0006
#endif

#ifndef OPENSEA_SG_ERR_DID_ERROR
#define OPENSEA_SG_ERR_DID_ERROR 0x0007
#endif

#ifndef OPENSEA_SG_ERR_DID_RESET
#define OPENSEA_SG_ERR_DID_RESET 0x0008
#endif

#ifndef OPENSEA_SG_ERR_DID_BAD_INTR
#define OPENSEA_SG_ERR_DID_BAD_INTR 0x0009
#endif

#ifndef OPENSEA_SG_ERR_DID_PASSTHROUGH
#define OPENSEA_SG_ERR_DID_PASSTHROUGH 0x000A
#endif

#ifndef OPENSEA_SG_ERR_DID_SOFT_ERROR
#define OPENSEA_SG_ERR_DID_SOFT_ERROR 0x000B
#endif

// \fn send_sg_io(scsiIoCtx * scsiIoCtx)
// \brief Function to send a SG_IO ioctl
// \param scsiIoCtx
    int send_sg_io( ScsiIoCtx *scsiIoCtx );

// \fn send_IO(scsiIoCtx * scsiIoCtx)
// \brief Function to send IO to the device.
// \param scsiIoCtx
    int send_IO( ScsiIoCtx *scsiIoCtx );

#if !defined(DISABLE_NVME_PASSTHROUGH)
//-----------------------------------------------------------------------------
//
//  pci_Read_Bar_Reg()
//
//! \brief   Description:  Function to Read PCI Bar register
//
//  Entry:
//!   \param[in]  device = pointer to device context!   
//!   \param[out] pData =  pointer to data that need to be filled.
//!                        this needs to be at least the size of a page
//!                        e.g. getPageSize() in Linux
//!   \param[out] dataSize =  size of the data
//! 
//!
//  Exit:
//!   \return SUCCESS = pass, !SUCCESS = something when wrong
//
//-----------------------------------------------------------------------------
int pci_Read_Bar_Reg( tDevice * device, uint8_t * pData, uint32_t dataSize );

int send_NVMe_IO(nvmeCmdCtx *nvmeIoCtx);

//to be used with a deep scan???
//int nvme_Namespace_Rescan(int fd);//rescans a controller for namespaces. This must be a file descriptor without a namespace. EX: /dev/nvme0 and NOT /dev/nvme0n1

int os_nvme_Reset(tDevice *device);

int os_nvme_Subsystem_Reset(tDevice *device);


#endif

int map_Block_To_Generic_Handle(char *handle, char **genericHandle, char **blockHandle);

int device_Reset(int fd);

int bus_Reset(int fd);

int host_Reset(int fd);

    #if defined (__cplusplus)
}
    #endif

#endif
