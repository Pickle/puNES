// Windows/FileSystem.h

#ifndef __WINDOWS_FILE_SYSTEM_H
#define __WINDOWS_FILE_SYSTEM_H

#include "../Common/MyString.h"
#include "../Common/Types.h"

namespace NWindows {
namespace NFile {
namespace NSystem {

bool MyGetVolumeInformation(
    CFSTR rootPath  ,
    UString &volumeName,
    LPDWORD volumeSerialNumber,
    LPDWORD maximumComponentLength,
    LPDWORD fileSystemFlags,
    UString &fileSystemName);

UINT MyGetDriveType(CFSTR pathName);

bool MyGetDiskFreeSpace(CFSTR rootPath, UInt64 &clusterSize, UInt64 &totalSize, UInt64 &freeSize);

}}}

#endif
