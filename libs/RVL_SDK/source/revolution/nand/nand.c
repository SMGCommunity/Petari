#include <cstdio>
#include <revolution/fs.h>
#include <revolution/nand.h>


static BOOL nandInspectPermission(u8);
static void nandSplitPerm(u8, u32 *, u32 *, u32 *);
static void nandGetTypeCallback(ISFSError, void *);
static void nandGetStatusCallback(ISFSError, void *);

ISFSError nandCreate(const char *path, const u8 perm, const u8 attr,
                     NANDCommandBlock *block, BOOL async_flag,
                     BOOL privilege_flag) {
  char absPath[64] = "";
  u32 owner = 0, group = 0, others = 0;
  nandGenerateAbsPath(absPath, path);

  if (!privilege_flag && nandIsPrivatePath(absPath)) {
    return ISFS_ERROR_ACCESS;
  } else if (!nandInspectPermission(perm)) {
    return ISFS_ERROR_INVALID;
  } else {
    nandSplitPerm(perm, &owner, &group, &others);

    if (async_flag) {
      return ISFS_CreateFileAsync((const u8 *)absPath, attr, owner, group,
                                  others, nandCallback, block);
    } else {
      return ISFS_CreateFile((const u8 *)absPath, attr, owner, group, others);
    }
  }
}

s32 NANDCreate(const char *path, const u8 perm, const u8 attr) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(nandCreate(path, perm, attr, NULL, FALSE, FALSE));
}

s32 NANDPrivateCreate(const char *path, u8 perm, u8 attr) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(nandCreate(path, perm, attr, NULL, FALSE, TRUE));
}

s32 NANDPrivateCreateAsync(const char *path, u8 perm, u8 attr, NANDCallback cb,
                           NANDCommandBlock *block) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  block->callback = cb;
  return nandConvertErrorCode(nandCreate(path, perm, attr, block, TRUE, TRUE));
}

static ISFSError nandDelete(const char *path, NANDCommandBlock *block,
                            BOOL async_flag, BOOL privilege_flag) {
  char absPath[64] = "";
  nandGenerateAbsPath(absPath, path);

  if (!privilege_flag && nandIsPrivatePath(absPath)) {
    return ISFS_ERROR_ACCESS;
  } else {
    if (async_flag) {
      return ISFS_DeleteAsync((const u8 *)absPath, nandCallback, block);
    } else {
      return ISFS_Delete((const u8 *)absPath);
    }
  }
}

s32 NANDDelete(const char *path) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(nandDelete(path, NULL, FALSE, FALSE));
}

s32 NANDPrivateDelete(const char *path) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(nandDelete(path, NULL, FALSE, TRUE));
}

s32 NANDPrivateDeleteAsync(const char *path, NANDCallback cb,
                           NANDCommandBlock *block) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  block->callback = cb;
  return nandConvertErrorCode(nandDelete(path, block, TRUE, TRUE));
}

s32 NANDRead(NANDFileInfo *info, void *buf, const u32 length) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(
      ISFS_Read(info->fileDescriptor, (u8 *)buf, length));
}

s32 NANDReadAsync(NANDFileInfo *info, void *buf, const u32 length,
                  NANDCallback cb, NANDCommandBlock *block) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  block->callback = cb;
  return nandConvertErrorCode(ISFS_ReadAsync(info->fileDescriptor, (u8 *)buf,
                                             length, nandCallback, block));
}

s32 NANDWrite(NANDFileInfo *info, const void *buf, const u32 length) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(
      ISFS_Write(info->fileDescriptor, (const u8 *)buf, length));
}

s32 NANDWriteAsync(NANDFileInfo *info, const void *buf, const u32 length,
                   NANDCallback cb, NANDCommandBlock *block) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  block->callback = cb;
  return nandConvertErrorCode(ISFS_WriteAsync(
      info->fileDescriptor, (const u8 *)buf, length, nandCallback, block));
}

static ISFSError nandSeek(const IOSFd fd, const s32 offset, const s32 whence,
                          NANDCommandBlock *block, const BOOL async_flag) {
  u32 w = 0xFFFFFFFF;

  switch (whence) {
  case 0:
    w = 0;
    break;
  case 1:
    w = 1;
    break;
  case 2:
    w = 2;
    break;
  default:
    break;
  }

  if (async_flag) {
    return ISFS_SeekAsync(fd, offset, w, nandCallback, block);
  } else {
    return ISFS_Seek(fd, offset, w);
  }
}

s32 NANDSeek(NANDFileInfo *info, const s32 offset, const s32 whence) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(
      nandSeek(info->fileDescriptor, offset, whence, NULL, FALSE));
}

s32 NANDSeekAsync(NANDFileInfo *info, const s32 offset, const s32 whence,
                  NANDCallback cb, NANDCommandBlock *block) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  block->callback = cb;
  return nandConvertErrorCode(
      nandSeek(info->fileDescriptor, offset, whence, block, TRUE));
}

ISFSError nandCreateDir(const char *path, const u8 perm, const u8 attr,
                        NANDCommandBlock *block, const BOOL async_flag,
                        const BOOL privilege_flag) {
  char absPath[64] = "";

  nandGenerateAbsPath(absPath, path);
  if (!privilege_flag && nandIsPrivatePath(absPath)) {
    return ISFS_ERROR_ACCESS;
  } else if (!nandInspectPermission(perm)) {
    return ISFS_ERROR_INVALID;
  } else {
    u32 owner = 0;
    u32 group = 0;
    u32 others = 0;
    nandSplitPerm(perm, &owner, &group, &others);

    if (async_flag) {
      return ISFS_CreateDirAsync((const u8 *)absPath, attr, owner, group,
                                 others, nandCallback, block);
    } else {
      return ISFS_CreateDir((const u8 *)absPath, attr, owner, group, others);
    }
  }
}

s32 NANDPrivateCreateDir(const char *path, u8 perm, u8 attr) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(
      nandCreateDir(path, perm, attr, NULL, FALSE, TRUE));
}

s32 NANDPrivateCreateDirAsync(const char *path, u8 perm, u8 attr,
                              NANDCallback cb, NANDCommandBlock *block) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  block->callback = cb;
  return nandConvertErrorCode(
      nandCreateDir(path, perm, attr, block, TRUE, TRUE));
}

ISFSError nandMove(const char *path, const char *destDir,
                   NANDCommandBlock *block, const BOOL async_flag,
                   const BOOL privilege_flag) {
  char absOldPath[64] = "";
  char absNewPath[64] = "";
  char relativeName[13] = "";

  nandGenerateAbsPath(absOldPath, path);
  nandGetRelativeName(relativeName, absOldPath);
  nandGenerateAbsPath(absNewPath, destDir);
  if (strcmp(absNewPath, "/") == 0) {
    sprintf(absNewPath, "/%s", relativeName);
  } else {
    strcat(absNewPath, "/");
    strcat(absNewPath, relativeName);
  }

  if (!privilege_flag &&
      (nandIsPrivatePath(absOldPath) || nandIsPrivatePath(absNewPath))) {
    return ISFS_ERROR_ACCESS;
  } else {
    if (async_flag) {
      return ISFS_RenameAsync((const u8 *)absOldPath, (const u8 *)absNewPath,
                              nandCallback, block);
    } else {
      return ISFS_Rename((const u8 *)absOldPath, (const u8 *)absNewPath);
    }
  }
}

s32 NANDMove(const char *path, const char *destDir) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(nandMove(path, destDir, NULL, FALSE, FALSE));
}

static ISFSError nandGetFileStatus(IOSFd fd, u32 *length, u32 *pos) {
  ISFSFileStats fstat __attribute__((aligned(32)));
  ISFSError result = ISFS_GetFileStats(fd, &fstat);
  if (result == ISFS_ERROR_OK) {
    if (length) {
      *length = fstat.size;
    }
    if (pos) {
      *pos = fstat.offset;
    }
  }
  return result;
}

s32 NANDGetLength(NANDFileInfo *info, u32 *length) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(
      nandGetFileStatus(info->fileDescriptor, length, NULL));
}

void nandGetFileStatusAsyncCallback(ISFSError result, void *ctxt) {
  NANDCommandBlock *b = (NANDCommandBlock *)ctxt;
  ISFSFileStats *fstat = (ISFSFileStats *)OSRoundUp32B((u32)(b->absPath));

  if (result == ISFS_ERROR_OK) {
    if (b->length) {
      *(b->length) = fstat->size;
    }
    if (b->pos) {
      *(b->pos) = fstat->offset;
    }
  }

  ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
}

static ISFSError nandGetFileStatusAsync(IOSFd fd, NANDCommandBlock *block) {
  ISFSError result = ISFS_ERROR_UNKNOWN;
  ISFSFileStats *fstat = (ISFSFileStats *)OSRoundUp32B((u32)(block->absPath));
  return ISFS_GetFileStatsAsync(fd, fstat, nandGetFileStatusAsyncCallback,
                                block);
}

s32 NANDGetLengthAsync(NANDFileInfo *info, u32 *length, NANDCallback cb,
                       NANDCommandBlock *block) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  block->callback = cb;
  block->length = length;
  block->pos = NULL;
  return nandConvertErrorCode(
      nandGetFileStatusAsync(info->fileDescriptor, block));
}

void nandComposePerm(u8 *perm, const u32 ownerAcc, const u32 groupAcc,
                     const u32 othersAcc) NO_INLINE {
  u32 p = 0;

  if (ownerAcc & 1) {
    p = p | 0x10;
  }

  if (ownerAcc & 2) {
    p = p | 0x20;
  }

  if (groupAcc & 1) {
    p = p | 4;
  }

  if (groupAcc & 2) {
    p = p | 8;
  }

  if (othersAcc & 1) {
    p = p | 1;
  }

  if (othersAcc & 2) {
    p = p | 2;
  }

  *perm = (u8)p;
}

static void nandSplitPerm(u8 perm, u32 *ownerAcc, u32 *groupAcc,
                          u32 *othersAcc) {
  *ownerAcc = 0;
  *groupAcc = 0;
  *othersAcc = 0;

  if (perm & 0x10) {
    *ownerAcc = *ownerAcc | 1;
  }

  if (perm & 0x20) {
    *ownerAcc = *ownerAcc | 2;
  }

  if (perm & 4) {
    *groupAcc = *groupAcc | 1;
  }

  if (perm & 8) {
    *groupAcc = *groupAcc | 2;
  }

  if (perm & 1) {
    *othersAcc = *othersAcc | 1;
  }

  if (perm & 2) {
    *othersAcc = *othersAcc | 2;
  }
}

ISFSError nandGetStatus(const char *path, NANDStatus *stat,
                        NANDCommandBlock *block, const BOOL async_flag,
                        const BOOL privilege_flag) {
  char absPath[64] = "";

  nandGenerateAbsPath(absPath, path);
  if (!privilege_flag && nandIsUnderPrivatePath(absPath)) {
    return ISFS_ERROR_ACCESS;
  } else {
    if (async_flag) {
      return ISFS_GetAttrAsync(
          (const u8 *)absPath, &(stat->ownerId), &(stat->groupId),
          &(block->attr), &(block->ownerAcc), &(block->groupAcc),
          &(block->othersAcc), nandGetStatusCallback, block);
    } else {
      u32 attr = 0, ownerAcc = 0, groupAcc = 0, othersAcc = 0;
      ISFSError result =
          ISFS_GetAttr((const u8 *)absPath, &stat->ownerId, &stat->groupId,
                       &attr, &ownerAcc, &groupAcc, &othersAcc);
      if (result == ISFS_ERROR_OK) {
        nandComposePerm(&stat->permission, ownerAcc, groupAcc, othersAcc);
        stat->attribute = (u8)attr;
      }

      return result;
    }
  }
}

void nandGetStatusCallback(ISFSError result, void *ctxt) {
  NANDCommandBlock *b = (NANDCommandBlock *)ctxt;
  if (result == ISFS_ERROR_OK) {
    NANDStatus *stat = (NANDStatus *)(b->status);
    stat->attribute = (u8)(b->attr);
    nandComposePerm(&(stat->permission), b->ownerAcc, b->groupAcc,
                    b->othersAcc);
  }
  ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
}

s32 NANDGetStatus(const char *path, NANDStatus *stat) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(nandGetStatus(path, stat, NULL, FALSE, FALSE));
}

s32 NANDPrivateGetStatus(const char *path, NANDStatus *stat) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  return nandConvertErrorCode(nandGetStatus(path, stat, NULL, FALSE, TRUE));
}

s32 NANDPrivateGetStatusAsync(const char *path, NANDStatus *stat,
                              NANDCallback cb, NANDCommandBlock *block) {
  if (!nandIsInitialized()) {
    return NAND_RESULT_FATAL_ERROR;
  }

  block->callback = cb;
  block->status = stat;
  return nandConvertErrorCode(nandGetStatus(path, stat, block, TRUE, TRUE));
}

void NANDSetUserData(NANDCommandBlock *block, void *data) {
  block->userData = data;
}

void *NANDGetUserData(const NANDCommandBlock *block) { return block->userData; }

static BOOL nandInspectPermission(const u8 perm) {
  if (perm & 0x10) {
    return TRUE;
  } else {
    return FALSE;
  }
}