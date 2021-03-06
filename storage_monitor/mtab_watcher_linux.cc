// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// MtabWatcherLinux implementation.

#include "chrome/browser/storage_monitor/mtab_watcher_linux.h"

#include <mntent.h>
#include <stdio.h>

#include "base/bind.h"
#include "content/public/browser/browser_thread.h"

namespace {

// List of file systems we care about.
const char* const kKnownFileSystems[] = {
  "btrfs",
  "ext2",
  "ext3",
  "ext4",
  "fat",
  "hfsplus",
  "iso9660",
  "msdos",
  "ntfs",
  "udf",
  "vfat",
};

}  // namespace

MtabWatcherLinux::MtabWatcherLinux(const base::FilePath& mtab_path,
                                   base::WeakPtr<Delegate> delegate)
    : mtab_path_(mtab_path),
      delegate_(delegate),
      weak_ptr_factory_(this) {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::FILE));
  bool ret = file_watcher_.Watch(
      mtab_path_, false,
      base::Bind(&MtabWatcherLinux::OnFilePathChanged,
                 weak_ptr_factory_.GetWeakPtr()));
  if (!ret) {
    LOG(ERROR) << "Adding watch for " << mtab_path_.value() << " failed";
    return;
  }

  ReadMtab();
}

MtabWatcherLinux::~MtabWatcherLinux() {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::FILE));
}

void MtabWatcherLinux::ReadMtab() const {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::FILE));

  FILE* fp = setmntent(mtab_path_.value().c_str(), "r");
  if (!fp)
    return;

  MountPointDeviceMap device_map;
  mntent entry;
  char buf[512];

  // We return the same device mounted to multiple locations, but hide
  // devices that have been mounted over.
  while (getmntent_r(fp, &entry, buf, sizeof(buf))) {
    // We only care about real file systems.
    for (size_t i = 0; i < arraysize(kKnownFileSystems); ++i) {
      if (strcmp(kKnownFileSystems[i], entry.mnt_type) == 0) {
        device_map[base::FilePath(entry.mnt_dir)] =
            base::FilePath(entry.mnt_fsname);
        break;
      }
    }
  }
  endmntent(fp);

  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::Bind(&Delegate::UpdateMtab, delegate_, device_map));
}

void MtabWatcherLinux::OnFilePathChanged(
    const base::FilePath& path, bool error) {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::FILE));

  if (path != mtab_path_) {
    // This cannot happen unless FilePathWatcher is buggy. Just ignore this
    // notification and do nothing.
    NOTREACHED();
    return;
  }
  if (error) {
    LOG(ERROR) << "Error watching " << mtab_path_.value();
    return;
  }

  ReadMtab();
}
