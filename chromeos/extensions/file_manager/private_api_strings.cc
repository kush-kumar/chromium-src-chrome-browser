// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/chromeos/extensions/file_manager/private_api_strings.h"

#include "chrome/browser/chromeos/extensions/file_manager/file_manager_util.h"
#include "chrome/browser/chromeos/system/statistics_provider.h"
#include "grit/app_locale_settings.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/webui/web_ui_util.h"

namespace file_manager {

GetStringsFunction::GetStringsFunction() {
}

GetStringsFunction::~GetStringsFunction() {
}

bool GetStringsFunction::RunImpl() {
  DictionaryValue* dict = new DictionaryValue();
  SetResult(dict);

#define SET_STRING(id, idr) \
  dict->SetString(id, l10n_util::GetStringUTF16(idr))

  SET_STRING("WEB_FONT_FAMILY", IDS_WEB_FONT_FAMILY);
  SET_STRING("WEB_FONT_SIZE", IDS_WEB_FONT_SIZE);

  SET_STRING("ROOT_DIRECTORY_LABEL", IDS_FILE_BROWSER_ROOT_DIRECTORY_LABEL);
  SET_STRING("ARCHIVE_DIRECTORY_LABEL",
             IDS_FILE_BROWSER_ARCHIVE_DIRECTORY_LABEL);
  SET_STRING("REMOVABLE_DIRECTORY_LABEL",
             IDS_FILE_BROWSER_REMOVABLE_DIRECTORY_LABEL);
  SET_STRING("DOWNLOADS_DIRECTORY_LABEL",
             IDS_FILE_BROWSER_DOWNLOADS_DIRECTORY_LABEL);
  SET_STRING("DRIVE_DIRECTORY_LABEL", IDS_FILE_BROWSER_DRIVE_DIRECTORY_LABEL);
  SET_STRING("DRIVE_MY_DRIVE_LABEL", IDS_FILE_BROWSER_DRIVE_MY_DRIVE_LABEL);
  SET_STRING("DRIVE_OFFLINE_COLLECTION_LABEL",
             IDS_FILE_BROWSER_DRIVE_OFFLINE_COLLECTION_LABEL);
  SET_STRING("DRIVE_SHARED_WITH_ME_COLLECTION_LABEL",
             IDS_FILE_BROWSER_DRIVE_SHARED_WITH_ME_COLLECTION_LABEL);
  SET_STRING("DRIVE_RECENT_COLLECTION_LABEL",
             IDS_FILE_BROWSER_DRIVE_RECENT_COLLECTION_LABEL);
  SET_STRING("NAME_COLUMN_LABEL", IDS_FILE_BROWSER_NAME_COLUMN_LABEL);
  SET_STRING("SIZE_COLUMN_LABEL", IDS_FILE_BROWSER_SIZE_COLUMN_LABEL);
  SET_STRING("SIZE_BYTES", IDS_FILE_BROWSER_SIZE_BYTES);
  SET_STRING("SIZE_KB", IDS_FILE_BROWSER_SIZE_KB);
  SET_STRING("SIZE_MB", IDS_FILE_BROWSER_SIZE_MB);
  SET_STRING("SIZE_GB", IDS_FILE_BROWSER_SIZE_GB);
  SET_STRING("SIZE_TB", IDS_FILE_BROWSER_SIZE_TB);
  SET_STRING("SIZE_PB", IDS_FILE_BROWSER_SIZE_PB);

  SET_STRING("SHORTCUT_CTRL", IDS_FILE_BROWSER_SHORTCUT_CTRL);
  SET_STRING("SHORTCUT_ALT", IDS_FILE_BROWSER_SHORTCUT_ALT);
  SET_STRING("SHORTCUT_SHIFT", IDS_FILE_BROWSER_SHORTCUT_SHIFT);
  SET_STRING("SHORTCUT_META", IDS_FILE_BROWSER_SHORTCUT_META);
  SET_STRING("SHORTCUT_SPACE", IDS_FILE_BROWSER_SHORTCUT_SPACE);
  SET_STRING("SHORTCUT_ENTER", IDS_FILE_BROWSER_SHORTCUT_ENTER);

  SET_STRING("TYPE_COLUMN_LABEL", IDS_FILE_BROWSER_TYPE_COLUMN_LABEL);
  SET_STRING("DATE_COLUMN_LABEL", IDS_FILE_BROWSER_DATE_COLUMN_LABEL);
  SET_STRING("PREVIEW_COLUMN_LABEL", IDS_FILE_BROWSER_PREVIEW_COLUMN_LABEL);
  SET_STRING("OFFLINE_COLUMN_LABEL", IDS_FILE_BROWSER_OFFLINE_COLUMN_LABEL);

  SET_STRING("DOWNLOADS_DIRECTORY_WARNING",
             IDS_FILE_BROWSER_DOWNLOADS_DIRECTORY_WARNING);

  SET_STRING("ERROR_CREATING_FOLDER", IDS_FILE_BROWSER_ERROR_CREATING_FOLDER);
  SET_STRING("ERROR_INVALID_CHARACTER",
             IDS_FILE_BROWSER_ERROR_INVALID_CHARACTER);
  SET_STRING("ERROR_RESERVED_NAME", IDS_FILE_BROWSER_ERROR_RESERVED_NAME);
  SET_STRING("ERROR_HIDDEN_NAME", IDS_FILE_BROWSER_ERROR_HIDDEN_NAME);
  SET_STRING("ERROR_WHITESPACE_NAME", IDS_FILE_BROWSER_ERROR_WHITESPACE_NAME);
  SET_STRING("ERROR_NEW_FOLDER_EMPTY_NAME",
             IDS_FILE_BROWSER_ERROR_NEW_FOLDER_EMPTY_NAME);
  SET_STRING("ERROR_LONG_NAME", IDS_FILE_BROWSER_ERROR_LONG_NAME);
  SET_STRING("NEW_FOLDER_BUTTON_LABEL",
             IDS_FILE_BROWSER_NEW_FOLDER_BUTTON_LABEL);
  SET_STRING("NEW_WINDOW_BUTTON_LABEL",
             IDS_FILE_BROWSER_NEW_WINDOW_BUTTON_LABEL);
  SET_STRING("CHANGE_DEFAULT_APP_BUTTON_LABEL",
             IDS_FILE_BROWSER_CHANGE_DEFAULT_APP_BUTTON_LABEL);
  SET_STRING("FILENAME_LABEL", IDS_FILE_BROWSER_FILENAME_LABEL);
  SET_STRING("PREPARING_LABEL", IDS_FILE_BROWSER_PREPARING_LABEL);
  SET_STRING("DRAGGING_MULTIPLE_ITEMS",
             IDS_FILE_BROWSER_DRAGGING_MULTIPLE_ITEMS);

  SET_STRING("DIMENSIONS_LABEL", IDS_FILE_BROWSER_DIMENSIONS_LABEL);
  SET_STRING("DIMENSIONS_FORMAT", IDS_FILE_BROWSER_DIMENSIONS_FORMAT);

  SET_STRING("IMAGE_DIMENSIONS", IDS_FILE_BROWSER_IMAGE_DIMENSIONS);
  SET_STRING("VOLUME_LABEL", IDS_FILE_BROWSER_VOLUME_LABEL);
  SET_STRING("READ_ONLY", IDS_FILE_BROWSER_READ_ONLY);

  SET_STRING("ARCHIVE_MOUNT_FAILED", IDS_FILE_BROWSER_ARCHIVE_MOUNT_FAILED);
  SET_STRING("UNMOUNT_FAILED", IDS_FILE_BROWSER_UNMOUNT_FAILED);
  SET_STRING("MOUNT_ARCHIVE", IDS_FILE_BROWSER_MOUNT_ARCHIVE);
  SET_STRING("FORMAT_DEVICE_BUTTON_LABEL",
             IDS_FILE_BROWSER_FORMAT_DEVICE_BUTTON_LABEL);
  SET_STRING("UNMOUNT_DEVICE_BUTTON_LABEL",
             IDS_FILE_BROWSER_UNMOUNT_DEVICE_BUTTON_LABEL);
  SET_STRING("CLOSE_ARCHIVE_BUTTON_LABEL",
             IDS_FILE_BROWSER_CLOSE_ARCHIVE_BUTTON_LABEL);

  SET_STRING("SEARCH_TEXT_LABEL", IDS_FILE_BROWSER_SEARCH_TEXT_LABEL);

  SET_STRING("ACTION_VIEW", IDS_FILE_BROWSER_ACTION_VIEW);
  SET_STRING("ACTION_OPEN", IDS_FILE_BROWSER_ACTION_OPEN);
  SET_STRING("ACTION_OPEN_GDOC", IDS_FILE_BROWSER_ACTION_OPEN_GDOC);
  SET_STRING("ACTION_OPEN_GSHEET", IDS_FILE_BROWSER_ACTION_OPEN_GSHEET);
  SET_STRING("ACTION_OPEN_GSLIDES", IDS_FILE_BROWSER_ACTION_OPEN_GSLIDES);
  SET_STRING("ACTION_WATCH", IDS_FILE_BROWSER_ACTION_WATCH);
  SET_STRING("ACTION_LISTEN", IDS_FILE_BROWSER_ACTION_LISTEN);
  SET_STRING("INSTALL_CRX", IDS_FILE_BROWSER_INSTALL_CRX);
  SET_STRING("SEND_TO_DRIVE", IDS_FILE_BROWSER_SEND_TO_DRIVE);

  SET_STRING("GALLERY_NO_IMAGES", IDS_FILE_BROWSER_GALLERY_NO_IMAGES);
  SET_STRING("GALLERY_ITEMS_SELECTED", IDS_FILE_BROWSER_GALLERY_ITEMS_SELECTED);
  SET_STRING("GALLERY_MOSAIC", IDS_FILE_BROWSER_GALLERY_MOSAIC);
  SET_STRING("GALLERY_SLIDE", IDS_FILE_BROWSER_GALLERY_SLIDE);
  SET_STRING("GALLERY_DELETE", IDS_FILE_BROWSER_GALLERY_DELETE);
  SET_STRING("GALLERY_SLIDESHOW", IDS_FILE_BROWSER_GALLERY_SLIDESHOW);

  SET_STRING("GALLERY_EDIT", IDS_FILE_BROWSER_GALLERY_EDIT);
  SET_STRING("GALLERY_PRINT", IDS_FILE_BROWSER_GALLERY_PRINT);
  SET_STRING("GALLERY_SHARE", IDS_FILE_BROWSER_GALLERY_SHARE);
  SET_STRING("GALLERY_ENTER_WHEN_DONE",
             IDS_FILE_BROWSER_GALLERY_ENTER_WHEN_DONE);
  SET_STRING("GALLERY_AUTOFIX", IDS_FILE_BROWSER_GALLERY_AUTOFIX);
  SET_STRING("GALLERY_FIXED", IDS_FILE_BROWSER_GALLERY_FIXED);
  SET_STRING("GALLERY_CROP", IDS_FILE_BROWSER_GALLERY_CROP);
  SET_STRING("GALLERY_EXPOSURE", IDS_FILE_BROWSER_GALLERY_EXPOSURE);
  SET_STRING("GALLERY_BRIGHTNESS", IDS_FILE_BROWSER_GALLERY_BRIGHTNESS);
  SET_STRING("GALLERY_CONTRAST", IDS_FILE_BROWSER_GALLERY_CONTRAST);
  SET_STRING("GALLERY_ROTATE_LEFT", IDS_FILE_BROWSER_GALLERY_ROTATE_LEFT);
  SET_STRING("GALLERY_ROTATE_RIGHT", IDS_FILE_BROWSER_GALLERY_ROTATE_RIGHT);
  SET_STRING("GALLERY_UNDO", IDS_FILE_BROWSER_GALLERY_UNDO);
  SET_STRING("GALLERY_REDO", IDS_FILE_BROWSER_GALLERY_REDO);
  SET_STRING("GALLERY_FILE_EXISTS", IDS_FILE_BROWSER_GALLERY_FILE_EXISTS);
  SET_STRING("GALLERY_SAVED", IDS_FILE_BROWSER_GALLERY_SAVED);
  SET_STRING("GALLERY_OVERWRITE_ORIGINAL",
             IDS_FILE_BROWSER_GALLERY_OVERWRITE_ORIGINAL);
  SET_STRING("GALLERY_OVERWRITE_BUBBLE",
             IDS_FILE_BROWSER_GALLERY_OVERWRITE_BUBBLE);
  SET_STRING("GALLERY_UNSAVED_CHANGES",
             IDS_FILE_BROWSER_GALLERY_UNSAVED_CHANGES);
  SET_STRING("GALLERY_READONLY_WARNING",
             IDS_FILE_BROWSER_GALLERY_READONLY_WARNING);
  SET_STRING("GALLERY_IMAGE_ERROR", IDS_FILE_BROWSER_GALLERY_IMAGE_ERROR);
  SET_STRING("GALLERY_IMAGE_TOO_BIG_ERROR",
             IDS_FILE_BROWSER_GALLERY_IMAGE_TOO_BIG_ERROR);
  SET_STRING("GALLERY_VIDEO_ERROR", IDS_FILE_BROWSER_GALLERY_VIDEO_ERROR);
  SET_STRING("GALLERY_VIDEO_DECODING_ERROR",
             IDS_FILE_BROWSER_GALLERY_VIDEO_DECODING_ERROR);
  SET_STRING("GALLERY_VIDEO_LOOPED_MODE",
             IDS_FILE_BROWSER_GALLERY_VIDEO_LOOPED_MODE);
  SET_STRING("AUDIO_ERROR", IDS_FILE_BROWSER_AUDIO_ERROR);
  SET_STRING("GALLERY_IMAGE_OFFLINE", IDS_FILE_BROWSER_GALLERY_IMAGE_OFFLINE);
  SET_STRING("GALLERY_VIDEO_OFFLINE", IDS_FILE_BROWSER_GALLERY_VIDEO_OFFLINE);
  SET_STRING("AUDIO_OFFLINE", IDS_FILE_BROWSER_AUDIO_OFFLINE);
  // Reusing strings, but with alias starting with GALLERY.
  dict->SetString("GALLERY_FILE_HIDDEN_NAME",
      l10n_util::GetStringUTF16(IDS_FILE_BROWSER_ERROR_HIDDEN_NAME));
  dict->SetString("GALLERY_OK_LABEL",
      l10n_util::GetStringUTF16(IDS_FILE_BROWSER_OK_LABEL));
  dict->SetString("GALLERY_CANCEL_LABEL",
      l10n_util::GetStringUTF16(IDS_FILE_BROWSER_CANCEL_LABEL));
  dict->SetString("GALLERY_CONFIRM_DELETE_ONE",
      l10n_util::GetStringUTF16(IDS_FILE_BROWSER_CONFIRM_DELETE_ONE));
  dict->SetString("GALLERY_CONFIRM_DELETE_SOME",
      l10n_util::GetStringUTF16(IDS_FILE_BROWSER_CONFIRM_DELETE_SOME));

  SET_STRING("ACTION_CHOICE_OPENING_METHOD",
             IDS_FILE_BROWSER_ACTION_CHOICE_OPENING_METHOD);
  SET_STRING("ACTION_CHOICE_PHOTOS_DRIVE",
             IDS_FILE_BROWSER_ACTION_CHOICE_PHOTOS_DRIVE);
  SET_STRING("ACTION_CHOICE_DRIVE_NOT_REACHED",
             IDS_FILE_BROWSER_ACTION_CHOICE_DRIVE_NOT_REACHED);
  SET_STRING("ACTION_CHOICE_VIEW_FILES",
             IDS_FILE_BROWSER_ACTION_CHOICE_VIEW_FILES);
  SET_STRING("ACTION_CHOICE_WATCH_SINGLE_VIDEO",
             IDS_FILE_BROWSER_ACTION_CHOICE_WATCH_SINGLE_VIDEO);
  SET_STRING("ACTION_CHOICE_ONCE", IDS_FILE_BROWSER_ACTION_CHOICE_ONCE);
  SET_STRING("ACTION_CHOICE_ALWAYS", IDS_FILE_BROWSER_ACTION_CHOICE_ALWAYS);
  SET_STRING("ACTION_CHOICE_COUNTER_NO_MEDIA",
             IDS_FILE_BROWSER_ACTION_CHOICE_COUNTER_NO_MEDIA);
  SET_STRING("ACTION_CHOICE_COUNTER", IDS_FILE_BROWSER_ACTION_CHOICE_COUNTER);
  SET_STRING("ACTION_CHOICE_LOADING_USB",
             IDS_FILE_BROWSER_ACTION_CHOICE_LOADING_USB);
  SET_STRING("ACTION_CHOICE_LOADING_SD",
             IDS_FILE_BROWSER_ACTION_CHOICE_LOADING_SD);

  SET_STRING("PHOTO_IMPORT_TITLE", IDS_FILE_BROWSER_PHOTO_IMPORT_TITLE);
  SET_STRING("PHOTO_IMPORT_IMPORT_BUTTON",
             IDS_FILE_BROWSER_PHOTO_IMPORT_IMPORT_BUTTON);
  SET_STRING("PHOTO_IMPORT_CANCEL_BUTTON",
             IDS_FILE_BROWSER_PHOTO_IMPORT_CANCEL_BUTTON);
  SET_STRING("PHOTO_IMPORT_DRIVE_ERROR",
             IDS_FILE_BROWSER_PHOTO_IMPORT_DRIVE_ERROR);
  SET_STRING("PHOTO_IMPORT_DESTINATION_ERROR",
             IDS_FILE_BROWSER_PHOTO_IMPORT_DESTINATION_ERROR);
  SET_STRING("PHOTO_IMPORT_SOURCE_ERROR",
             IDS_FILE_BROWSER_PHOTO_IMPORT_SOURCE_ERROR);
  SET_STRING("PHOTO_IMPORT_UNKNOWN_DATE",
             IDS_FILE_BROWSER_PHOTO_IMPORT_UNKNOWN_DATE);
  SET_STRING("PHOTO_IMPORT_NEW_ALBUM_NAME",
             IDS_FILE_BROWSER_PHOTO_IMPORT_NEW_ALBUM_NAME);
  SET_STRING("PHOTO_IMPORT_SELECT_ALBUM_CAPTION",
             IDS_FILE_BROWSER_PHOTO_IMPORT_SELECT_ALBUM_CAPTION);
  SET_STRING("PHOTO_IMPORT_SELECT_ALBUM_CAPTION_PLURAL",
             IDS_FILE_BROWSER_PHOTO_IMPORT_SELECT_ALBUM_CAPTION_PLURAL);
  SET_STRING("PHOTO_IMPORT_IMPORTING_ERROR",
             IDS_FILE_BROWSER_PHOTO_IMPORT_IMPORTING_ERROR);
  SET_STRING("PHOTO_IMPORT_IMPORTING", IDS_FILE_BROWSER_PHOTO_IMPORT_IMPORTING);
  SET_STRING("PHOTO_IMPORT_IMPORT_COMPLETE",
             IDS_FILE_BROWSER_PHOTO_IMPORT_IMPORT_COMPLETE);
  SET_STRING("PHOTO_IMPORT_CAPTION", IDS_FILE_BROWSER_PHOTO_IMPORT_CAPTION);
  SET_STRING("PHOTO_IMPORT_ONE_SELECTED",
             IDS_FILE_BROWSER_PHOTO_IMPORT_ONE_SELECTED);
  SET_STRING("PHOTO_IMPORT_MANY_SELECTED",
             IDS_FILE_BROWSER_PHOTO_IMPORT_MANY_SELECTED);
  SET_STRING("PHOTO_IMPORT_SELECT_ALL",
             IDS_FILE_BROWSER_PHOTO_IMPORT_SELECT_ALL);
  SET_STRING("PHOTO_IMPORT_SELECT_NONE",
             IDS_FILE_BROWSER_PHOTO_IMPORT_SELECT_NONE);
  SET_STRING("PHOTO_IMPORT_DELETE_AFTER",
             IDS_FILE_BROWSER_PHOTO_IMPORT_DELETE_AFTER);
  SET_STRING("PHOTO_IMPORT_MY_PHOTOS_DIRECTORY_NAME",
             IDS_FILE_BROWSER_PHOTO_IMPORT_MY_PHOTOS_DIRECTORY_NAME);

  SET_STRING("CONFIRM_OVERWRITE_FILE", IDS_FILE_BROWSER_CONFIRM_OVERWRITE_FILE);
  SET_STRING("FILE_ALREADY_EXISTS", IDS_FILE_BROWSER_FILE_ALREADY_EXISTS);
  SET_STRING("DIRECTORY_ALREADY_EXISTS",
             IDS_FILE_BROWSER_DIRECTORY_ALREADY_EXISTS);
  SET_STRING("ERROR_RENAMING", IDS_FILE_BROWSER_ERROR_RENAMING);
  SET_STRING("RENAME_PROMPT", IDS_FILE_BROWSER_RENAME_PROMPT);
  SET_STRING("RENAME_BUTTON_LABEL", IDS_FILE_BROWSER_RENAME_BUTTON_LABEL);

  SET_STRING("ERROR_DELETING", IDS_FILE_BROWSER_ERROR_DELETING);
  SET_STRING("DELETE_BUTTON_LABEL", IDS_FILE_BROWSER_DELETE_BUTTON_LABEL);

  SET_STRING("PASTE_BUTTON_LABEL", IDS_FILE_BROWSER_PASTE_BUTTON_LABEL);

  SET_STRING("COPY_BUTTON_LABEL", IDS_FILE_BROWSER_COPY_BUTTON_LABEL);
  SET_STRING("CUT_BUTTON_LABEL", IDS_FILE_BROWSER_CUT_BUTTON_LABEL);
  SET_STRING("ZIP_SELECTION_BUTTON_LABEL",
             IDS_FILE_BROWSER_ZIP_SELECTION_BUTTON_LABEL);
  SET_STRING("CREATE_FOLDER_SHORTCUT_BUTTON_LABEL",
             IDS_FILE_BROWSER_CREATE_FOLDER_SHORTCUT_BUTTON_LABEL);
  SET_STRING("REMOVE_FOLDER_SHORTCUT_BUTTON_LABEL",
             IDS_FILE_BROWSER_REMOVE_FOLDER_SHORTCUT_BUTTON_LABEL);
  SET_STRING("SHARE_BUTTON_LABEL",
             IDS_FILE_BROWSER_SHARE_BUTTON_LABEL);

  SET_STRING("OPEN_WITH_BUTTON_LABEL", IDS_FILE_BROWSER_OPEN_WITH_BUTTON_LABEL);

  SET_STRING("TRANSFER_ITEMS_REMAINING",
             IDS_FILE_BROWSER_TRANSFER_ITEMS_REMAINING);
  SET_STRING("TRANSFER_CANCELLED", IDS_FILE_BROWSER_TRANSFER_CANCELLED);
  SET_STRING("TRANSFER_TARGET_EXISTS_ERROR",
             IDS_FILE_BROWSER_TRANSFER_TARGET_EXISTS_ERROR);
  SET_STRING("TRANSFER_FILESYSTEM_ERROR",
             IDS_FILE_BROWSER_TRANSFER_FILESYSTEM_ERROR);
  SET_STRING("TRANSFER_UNEXPECTED_ERROR",
             IDS_FILE_BROWSER_TRANSFER_UNEXPECTED_ERROR);
  SET_STRING("COPY_FILE_NAME", IDS_FILE_BROWSER_COPY_FILE_NAME);
  SET_STRING("COPY_ITEMS_REMAINING", IDS_FILE_BROWSER_COPY_ITEMS_REMAINING);
  SET_STRING("COPY_CANCELLED", IDS_FILE_BROWSER_COPY_CANCELLED);
  SET_STRING("COPY_TARGET_EXISTS_ERROR",
             IDS_FILE_BROWSER_COPY_TARGET_EXISTS_ERROR);
  SET_STRING("COPY_FILESYSTEM_ERROR", IDS_FILE_BROWSER_COPY_FILESYSTEM_ERROR);
  SET_STRING("COPY_UNEXPECTED_ERROR", IDS_FILE_BROWSER_COPY_UNEXPECTED_ERROR);
  SET_STRING("MOVE_FILE_NAME", IDS_FILE_BROWSER_MOVE_FILE_NAME);
  SET_STRING("MOVE_ITEMS_REMAINING", IDS_FILE_BROWSER_MOVE_ITEMS_REMAINING);
  SET_STRING("MOVE_CANCELLED", IDS_FILE_BROWSER_MOVE_CANCELLED);
  SET_STRING("MOVE_TARGET_EXISTS_ERROR",
             IDS_FILE_BROWSER_MOVE_TARGET_EXISTS_ERROR);
  SET_STRING("MOVE_FILESYSTEM_ERROR", IDS_FILE_BROWSER_MOVE_FILESYSTEM_ERROR);
  SET_STRING("MOVE_UNEXPECTED_ERROR", IDS_FILE_BROWSER_MOVE_UNEXPECTED_ERROR);
  SET_STRING("ZIP_FILE_NAME", IDS_FILE_BROWSER_ZIP_FILE_NAME);
  SET_STRING("ZIP_ITEMS_REMAINING", IDS_FILE_BROWSER_ZIP_ITEMS_REMAINING);
  SET_STRING("ZIP_CANCELLED", IDS_FILE_BROWSER_ZIP_CANCELLED);
  SET_STRING("ZIP_TARGET_EXISTS_ERROR",
             IDS_FILE_BROWSER_ZIP_TARGET_EXISTS_ERROR);
  SET_STRING("ZIP_FILESYSTEM_ERROR", IDS_FILE_BROWSER_ZIP_FILESYSTEM_ERROR);
  SET_STRING("ZIP_UNEXPECTED_ERROR", IDS_FILE_BROWSER_ZIP_UNEXPECTED_ERROR);
  SET_STRING("SHARE_ERROR", IDS_FILE_BROWSER_SHARE_ERROR);

  SET_STRING("DELETED_MESSAGE_PLURAL", IDS_FILE_BROWSER_DELETED_MESSAGE_PLURAL);
  SET_STRING("DELETED_MESSAGE", IDS_FILE_BROWSER_DELETED_MESSAGE);
  SET_STRING("DELETE_ERROR", IDS_FILE_BROWSER_DELETE_ERROR);
  SET_STRING("UNDO_DELETE", IDS_FILE_BROWSER_UNDO_DELETE);

  SET_STRING("CANCEL_LABEL", IDS_FILE_BROWSER_CANCEL_LABEL);
  SET_STRING("OPEN_LABEL", IDS_FILE_BROWSER_OPEN_LABEL);
  SET_STRING("SAVE_LABEL", IDS_FILE_BROWSER_SAVE_LABEL);
  SET_STRING("OK_LABEL", IDS_FILE_BROWSER_OK_LABEL);
  SET_STRING("UPLOAD_LABEL", IDS_FILE_BROWSER_UPLOAD_LABEL);
  SET_STRING("VIEW_LABEL", IDS_FILE_BROWSER_VIEW_LABEL);

  SET_STRING("DEFAULT_NEW_FOLDER_NAME",
             IDS_FILE_BROWSER_DEFAULT_NEW_FOLDER_NAME);
  SET_STRING("MORE_FILES", IDS_FILE_BROWSER_MORE_FILES);

  SET_STRING("CONFIRM_DELETE_ONE", IDS_FILE_BROWSER_CONFIRM_DELETE_ONE);
  SET_STRING("CONFIRM_DELETE_SOME", IDS_FILE_BROWSER_CONFIRM_DELETE_SOME);

  SET_STRING("UNKNOWN_FILESYSTEM_WARNING",
             IDS_FILE_BROWSER_UNKNOWN_FILESYSTEM_WARNING);
  SET_STRING("UNSUPPORTED_FILESYSTEM_WARNING",
             IDS_FILE_BROWSER_UNSUPPORTED_FILESYSTEM_WARNING);
  SET_STRING("FORMATTING_WARNING", IDS_FILE_BROWSER_FORMATTING_WARNING);

  SET_STRING("DRIVE_MENU_HELP", IDS_FILE_BROWSER_DRIVE_MENU_HELP);
  SET_STRING("DRIVE_SHOW_HOSTED_FILES_OPTION",
             IDS_FILE_BROWSER_DRIVE_SHOW_HOSTED_FILES_OPTION);
  SET_STRING("DRIVE_MOBILE_CONNECTION_OPTION",
             IDS_FILE_BROWSER_DRIVE_MOBILE_CONNECTION_OPTION);
  SET_STRING("DRIVE_CLEAR_LOCAL_CACHE",
             IDS_FILE_BROWSER_DRIVE_CLEAR_LOCAL_CACHE);
  SET_STRING("DRIVE_SPACE_AVAILABLE_LONG",
             IDS_FILE_BROWSER_DRIVE_SPACE_AVAILABLE_LONG);
  SET_STRING("DRIVE_BUY_MORE_SPACE", IDS_FILE_BROWSER_DRIVE_BUY_MORE_SPACE);
  SET_STRING("DRIVE_BUY_MORE_SPACE_LINK",
             IDS_FILE_BROWSER_DRIVE_BUY_MORE_SPACE_LINK);
  SET_STRING("DRIVE_VISIT_DRIVE_GOOGLE_COM",
             IDS_FILE_BROWSER_DRIVE_VISIT_DRIVE_GOOGLE_COM);

  SET_STRING("SELECT_FOLDER_TITLE", IDS_FILE_BROWSER_SELECT_FOLDER_TITLE);
  SET_STRING("SELECT_OPEN_FILE_TITLE", IDS_FILE_BROWSER_SELECT_OPEN_FILE_TITLE);
  SET_STRING("SELECT_OPEN_MULTI_FILE_TITLE",
             IDS_FILE_BROWSER_SELECT_OPEN_MULTI_FILE_TITLE);
  SET_STRING("SELECT_SAVEAS_FILE_TITLE",
             IDS_FILE_BROWSER_SELECT_SAVEAS_FILE_TITLE);

  SET_STRING("MANY_FILES_SELECTED", IDS_FILE_BROWSER_MANY_FILES_SELECTED);
  SET_STRING("MANY_DIRECTORIES_SELECTED",
             IDS_FILE_BROWSER_MANY_DIRECTORIES_SELECTED);
  SET_STRING("MANY_ENTRIES_SELECTED", IDS_FILE_BROWSER_MANY_ENTRIES_SELECTED);
  SET_STRING("CALCULATING_SIZE", IDS_FILE_BROWSER_CALCULATING_SIZE);

  SET_STRING("OFFLINE_HEADER", IDS_FILE_BROWSER_OFFLINE_HEADER);
  SET_STRING("OFFLINE_MESSAGE", IDS_FILE_BROWSER_OFFLINE_MESSAGE);
  SET_STRING("OFFLINE_MESSAGE_PLURAL", IDS_FILE_BROWSER_OFFLINE_MESSAGE_PLURAL);
  SET_STRING("HOSTED_OFFLINE_MESSAGE", IDS_FILE_BROWSER_HOSTED_OFFLINE_MESSAGE);
  SET_STRING("HOSTED_OFFLINE_MESSAGE_PLURAL",
             IDS_FILE_BROWSER_HOSTED_OFFLINE_MESSAGE_PLURAL);
  SET_STRING("CONFIRM_MOBILE_DATA_USE",
             IDS_FILE_BROWSER_CONFIRM_MOBILE_DATA_USE);
  SET_STRING("CONFIRM_MOBILE_DATA_USE_PLURAL",
             IDS_FILE_BROWSER_CONFIRM_MOBILE_DATA_USE_PLURAL);
  SET_STRING("DRIVE_OUT_OF_SPACE_HEADER",
             IDS_FILE_BROWSER_DRIVE_OUT_OF_SPACE_HEADER);
  SET_STRING("DRIVE_OUT_OF_SPACE_MESSAGE",
             IDS_FILE_BROWSER_DRIVE_OUT_OF_SPACE_MESSAGE);
  SET_STRING("DRIVE_SERVER_OUT_OF_SPACE_HEADER",
             IDS_FILE_BROWSER_DRIVE_SERVER_OUT_OF_SPACE_HEADER);
  SET_STRING("DRIVE_SERVER_OUT_OF_SPACE_MESSAGE",
             IDS_FILE_BROWSER_DRIVE_SERVER_OUT_OF_SPACE_MESSAGE);
  SET_STRING("DRIVE_WELCOME_TITLE", IDS_FILE_BROWSER_DRIVE_WELCOME_TITLE);
  SET_STRING("DRIVE_WELCOME_TEXT_SHORT",
             IDS_FILE_BROWSER_DRIVE_WELCOME_TEXT_SHORT);
  SET_STRING("DRIVE_WELCOME_TEXT_LONG",
             IDS_FILE_BROWSER_DRIVE_WELCOME_TEXT_LONG);
  SET_STRING("DRIVE_WELCOME_DISMISS", IDS_FILE_BROWSER_DRIVE_WELCOME_DISMISS);
  SET_STRING("DRIVE_WELCOME_TITLE_ALTERNATIVE",
             IDS_FILE_BROWSER_DRIVE_WELCOME_TITLE_ALTERNATIVE);
  SET_STRING("DRIVE_WELCOME_TITLE_ALTERNATIVE_1TB",
             IDS_FILE_BROWSER_DRIVE_WELCOME_TITLE_ALTERNATIVE_1TB);
  SET_STRING("DRIVE_WELCOME_CHECK_ELIGIBILITY",
             IDS_FILE_BROWSER_DRIVE_WELCOME_CHECK_ELIGIBILITY);
  SET_STRING("NO_ACTION_FOR_FILE", IDS_FILE_BROWSER_NO_ACTION_FOR_FILE);
  SET_STRING("NO_ACTION_FOR_EXECUTABLE",
             IDS_FILE_BROWSER_NO_ACTION_FOR_EXECUTABLE);

  // MP3 metadata extractor plugin
  SET_STRING("ID3_ALBUM", IDS_FILE_BROWSER_ID3_ALBUM);                // TALB
  SET_STRING("ID3_BPM", IDS_FILE_BROWSER_ID3_BPM);                    // TBPM
  SET_STRING("ID3_COMPOSER", IDS_FILE_BROWSER_ID3_COMPOSER);          // TCOM
  SET_STRING("ID3_COPYRIGHT_MESSAGE",
             IDS_FILE_BROWSER_ID3_COPYRIGHT_MESSAGE);                 // TCOP
  SET_STRING("ID3_DATE", IDS_FILE_BROWSER_ID3_DATE);                  // TDAT
  SET_STRING("ID3_PLAYLIST_DELAY",
             IDS_FILE_BROWSER_ID3_PLAYLIST_DELAY);                    // TDLY
  SET_STRING("ID3_ENCODED_BY", IDS_FILE_BROWSER_ID3_ENCODED_BY);      // TENC
  SET_STRING("ID3_LYRICIST", IDS_FILE_BROWSER_ID3_LYRICIST);          // TEXT
  SET_STRING("ID3_FILE_TYPE", IDS_FILE_BROWSER_ID3_FILE_TYPE);        // TFLT
  SET_STRING("ID3_TIME", IDS_FILE_BROWSER_ID3_TIME);                  // TIME
  SET_STRING("ID3_TITLE", IDS_FILE_BROWSER_ID3_TITLE);                // TIT2
  SET_STRING("ID3_LENGTH", IDS_FILE_BROWSER_ID3_LENGTH);              // TLEN
  SET_STRING("ID3_FILE_OWNER", IDS_FILE_BROWSER_ID3_FILE_OWNER);      // TOWN
  SET_STRING("ID3_LEAD_PERFORMER",
             IDS_FILE_BROWSER_ID3_LEAD_PERFORMER);                    // TPE1
  SET_STRING("ID3_BAND", IDS_FILE_BROWSER_ID3_BAND);                  // TPE2
  SET_STRING("ID3_TRACK_NUMBER", IDS_FILE_BROWSER_ID3_TRACK_NUMBER);  // TRCK
  SET_STRING("ID3_YEAR", IDS_FILE_BROWSER_ID3_YEAR);                  // TYER
  SET_STRING("ID3_COPYRIGHT", IDS_FILE_BROWSER_ID3_COPYRIGHT);        // WCOP
  SET_STRING("ID3_OFFICIAL_AUDIO_FILE_WEBPAGE",
             IDS_FILE_BROWSER_ID3_OFFICIAL_AUDIO_FILE_WEBPAGE);       // WOAF
  SET_STRING("ID3_OFFICIAL_ARTIST",
             IDS_FILE_BROWSER_ID3_OFFICIAL_ARTIST);                   // WOAR
  SET_STRING("ID3_OFFICIAL_AUDIO_SOURCE_WEBPAGE",
             IDS_FILE_BROWSER_ID3_OFFICIAL_AUDIO_SOURCE_WEBPAGE);     // WOAS
  SET_STRING("ID3_PUBLISHERS_OFFICIAL_WEBPAGE",
             IDS_FILE_BROWSER_ID3_PUBLISHERS_OFFICIAL_WEBPAGE);       // WPUB
  SET_STRING("ID3_USER_DEFINED_URL_LINK_FRAME",
             IDS_FILE_BROWSER_ID3_USER_DEFINED_URL_LINK_FRAME);       // WXXX

  // File types
  SET_STRING("FOLDER", IDS_FILE_BROWSER_FOLDER);
  SET_STRING("GENERIC_FILE_TYPE", IDS_FILE_BROWSER_GENERIC_FILE_TYPE);
  SET_STRING("NO_EXTENSION_FILE_TYPE", IDS_FILE_BROWSER_NO_EXTENSION_FILE_TYPE);
  SET_STRING("DEVICE", IDS_FILE_BROWSER_DEVICE);
  SET_STRING("IMAGE_FILE_TYPE", IDS_FILE_BROWSER_IMAGE_FILE_TYPE);
  SET_STRING("VIDEO_FILE_TYPE", IDS_FILE_BROWSER_VIDEO_FILE_TYPE);
  SET_STRING("AUDIO_FILE_TYPE", IDS_FILE_BROWSER_AUDIO_FILE_TYPE);
  SET_STRING("HTML_DOCUMENT_FILE_TYPE",
             IDS_FILE_BROWSER_HTML_DOCUMENT_FILE_TYPE);
  SET_STRING("ZIP_ARCHIVE_FILE_TYPE", IDS_FILE_BROWSER_ZIP_ARCHIVE_FILE_TYPE);
  SET_STRING("RAR_ARCHIVE_FILE_TYPE", IDS_FILE_BROWSER_RAR_ARCHIVE_FILE_TYPE);
  SET_STRING("TAR_ARCHIVE_FILE_TYPE", IDS_FILE_BROWSER_TAR_ARCHIVE_FILE_TYPE);
  SET_STRING("TAR_BZIP2_ARCHIVE_FILE_TYPE",
             IDS_FILE_BROWSER_TAR_BZIP2_ARCHIVE_FILE_TYPE);
  SET_STRING("TAR_GZIP_ARCHIVE_FILE_TYPE",
             IDS_FILE_BROWSER_TAR_GZIP_ARCHIVE_FILE_TYPE);
  SET_STRING("PLAIN_TEXT_FILE_TYPE", IDS_FILE_BROWSER_PLAIN_TEXT_FILE_TYPE);
  SET_STRING("PDF_DOCUMENT_FILE_TYPE", IDS_FILE_BROWSER_PDF_DOCUMENT_FILE_TYPE);
  SET_STRING("WORD_DOCUMENT_FILE_TYPE",
             IDS_FILE_BROWSER_WORD_DOCUMENT_FILE_TYPE);
  SET_STRING("POWERPOINT_PRESENTATION_FILE_TYPE",
             IDS_FILE_BROWSER_POWERPOINT_PRESENTATION_FILE_TYPE);
  SET_STRING("EXCEL_FILE_TYPE", IDS_FILE_BROWSER_EXCEL_FILE_TYPE);

  SET_STRING("GDOC_DOCUMENT_FILE_TYPE",
             IDS_FILE_BROWSER_GDOC_DOCUMENT_FILE_TYPE);
  SET_STRING("GSHEET_DOCUMENT_FILE_TYPE",
             IDS_FILE_BROWSER_GSHEET_DOCUMENT_FILE_TYPE);
  SET_STRING("GSLIDES_DOCUMENT_FILE_TYPE",
             IDS_FILE_BROWSER_GSLIDES_DOCUMENT_FILE_TYPE);
  SET_STRING("GDRAW_DOCUMENT_FILE_TYPE",
             IDS_FILE_BROWSER_GDRAW_DOCUMENT_FILE_TYPE);
  SET_STRING("GTABLE_DOCUMENT_FILE_TYPE",
             IDS_FILE_BROWSER_GTABLE_DOCUMENT_FILE_TYPE);
  SET_STRING("GLINK_DOCUMENT_FILE_TYPE",
             IDS_FILE_BROWSER_GLINK_DOCUMENT_FILE_TYPE);

  SET_STRING("DRIVE_LOADING", IDS_FILE_BROWSER_DRIVE_LOADING);
  SET_STRING("DRIVE_CANNOT_REACH", IDS_FILE_BROWSER_DRIVE_CANNOT_REACH);
  SET_STRING("DRIVE_LEARN_MORE", IDS_FILE_BROWSER_DRIVE_LEARN_MORE);
  SET_STRING("DRIVE_RETRY", IDS_FILE_BROWSER_DRIVE_RETRY);

  SET_STRING("AUDIO_PLAYER_TITLE", IDS_FILE_BROWSER_AUDIO_PLAYER_TITLE);
  SET_STRING("AUDIO_PLAYER_DEFAULT_ARTIST",
             IDS_FILE_BROWSER_AUDIO_PLAYER_DEFAULT_ARTIST);

  SET_STRING("FILE_ERROR_GENERIC", IDS_FILE_BROWSER_FILE_ERROR_GENERIC);
  SET_STRING("FILE_ERROR_NOT_FOUND", IDS_FILE_BROWSER_FILE_ERROR_NOT_FOUND);
  SET_STRING("FILE_ERROR_SECURITY", IDS_FILE_BROWSER_FILE_ERROR_SECURITY);
  SET_STRING("FILE_ERROR_NOT_READABLE",
             IDS_FILE_BROWSER_FILE_ERROR_NOT_READABLE);
  SET_STRING("FILE_ERROR_NO_MODIFICATION_ALLOWED",
             IDS_FILE_BROWSER_FILE_ERROR_NO_MODIFICATION_ALLOWED);
  SET_STRING("FILE_ERROR_INVALID_STATE",
             IDS_FILE_BROWSER_FILE_ERROR_INVALID_STATE);
  SET_STRING("FILE_ERROR_INVALID_MODIFICATION",
             IDS_FILE_BROWSER_FILE_ERROR_INVALID_MODIFICATION);
  SET_STRING("FILE_ERROR_PATH_EXISTS", IDS_FILE_BROWSER_FILE_ERROR_PATH_EXISTS);
  SET_STRING("FILE_ERROR_QUOTA_EXCEEDED",
             IDS_FILE_BROWSER_FILE_ERROR_QUOTA_EXCEEDED);

  SET_STRING("SEARCH_DRIVE_HTML", IDS_FILE_BROWSER_SEARCH_DRIVE_HTML);
  SET_STRING("SEARCH_NO_MATCHING_FILES_HTML",
             IDS_FILE_BROWSER_SEARCH_NO_MATCHING_FILES_HTML);
  SET_STRING("SEARCH_EXPAND", IDS_FILE_BROWSER_SEARCH_EXPAND);
  SET_STRING("SEARCH_SPINNER", IDS_FILE_BROWSER_SEARCH_SPINNER);

  SET_STRING("CHANGE_DEFAULT_MENU_ITEM",
             IDS_FILE_BROWSER_CHANGE_DEFAULT_MENU_ITEM);
  SET_STRING("CHANGE_DEFAULT_CAPTION", IDS_FILE_BROWSER_CHANGE_DEFAULT_CAPTION);
  SET_STRING("DEFAULT_ACTION_LABEL", IDS_FILE_BROWSER_DEFAULT_ACTION_LABEL);

  SET_STRING("DETAIL_VIEW_TOOLTIP", IDS_FILE_BROWSER_DETAIL_VIEW_TOOLTIP);
  SET_STRING("THUMBNAIL_VIEW_TOOLTIP", IDS_FILE_BROWSER_THUMBNAIL_VIEW_TOOLTIP);

  SET_STRING("TIME_TODAY", IDS_FILE_BROWSER_TIME_TODAY);
  SET_STRING("TIME_YESTERDAY", IDS_FILE_BROWSER_TIME_YESTERDAY);

  SET_STRING("ALL_FILES_FILTER", IDS_FILE_BROWSER_ALL_FILES_FILTER);

  SET_STRING("SPACE_AVAILABLE", IDS_FILE_BROWSER_SPACE_AVAILABLE);
  SET_STRING("WAITING_FOR_SPACE_INFO", IDS_FILE_BROWSER_WAITING_FOR_SPACE_INFO);
  SET_STRING("FAILED_SPACE_INFO", IDS_FILE_BROWSER_FAILED_SPACE_INFO);

  SET_STRING("DRIVE_NOT_REACHED", IDS_FILE_BROWSER_DRIVE_NOT_REACHED);

  SET_STRING("HELP_LINK_LABEL", IDS_FILE_BROWSER_HELP_LINK_LABEL);
#undef SET_STRING

  dict->SetBoolean("PDF_VIEW_ENABLED",
      file_manager_util::ShouldBeOpenedWithPlugin(profile(), ".pdf"));
  dict->SetBoolean("SWF_VIEW_ENABLED",
      file_manager_util::ShouldBeOpenedWithPlugin(profile(), ".swf"));

  webui::SetFontAndTextDirection(dict);

  std::string board;
  chromeos::system::StatisticsProvider* provider =
      chromeos::system::StatisticsProvider::GetInstance();
  if (!provider->GetMachineStatistic(chromeos::system::kMachineInfoBoard,
                                     &board)) {
    board = "unknown";
  }
  dict->SetString(chromeos::system::kMachineInfoBoard, board);
  return true;
}

}  // namespace file_manager
