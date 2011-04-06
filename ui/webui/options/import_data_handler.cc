// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/webui/options/import_data_handler.h"

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/string_number_conversions.h"
#include "base/string_util.h"
#include "base/threading/thread_restrictions.h"
#include "base/utf_string_conversions.h"
#include "base/values.h"
#include "chrome/browser/importer/external_process_importer_host.h"
#include "chrome/browser/importer/importer_host.h"
#include "chrome/browser/profiles/profile.h"
#include "grit/chromium_strings.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"

ImportDataHandler::ImportDataHandler() : importer_host_(NULL) {
}

ImportDataHandler::~ImportDataHandler() {
  if (importer_list_)
    importer_list_->SetObserver(NULL);

  if (importer_host_)
    importer_host_->SetObserver(NULL);
}

void ImportDataHandler::GetLocalizedValues(
    DictionaryValue* localized_strings) {
  DCHECK(localized_strings);

  static OptionsStringResource resources[] = {
    { "importFromLabel", IDS_IMPORT_FROM_LABEL },
    { "importLoading", IDS_IMPORT_LOADING_PROFILES },
    { "importDescription", IDS_IMPORT_ITEMS_LABEL },
    { "importHistory", IDS_IMPORT_HISTORY_CHKBOX },
    { "importFavorites", IDS_IMPORT_FAVORITES_CHKBOX },
    { "importSearch", IDS_IMPORT_SEARCH_ENGINES_CHKBOX },
    { "importPasswords", IDS_IMPORT_PASSWORDS_CHKBOX },
    { "importCommit", IDS_IMPORT_COMMIT },
    { "noProfileFound", IDS_IMPORT_NO_PROFILE_FOUND },
  };

  RegisterStrings(localized_strings, resources, arraysize(resources));
  RegisterTitle(localized_strings, "importDataOverlay",
                IDS_IMPORT_SETTINGS_TITLE);
}

void ImportDataHandler::Initialize() {
  importer_list_ = new ImporterList;
  importer_list_->DetectSourceProfiles(this);
}

void ImportDataHandler::RegisterMessages() {
  web_ui_->RegisterMessageCallback(
      "importData", NewCallback(this, &ImportDataHandler::ImportData));
}

void ImportDataHandler::ImportData(const ListValue* args) {
  std::string string_value;

  int browser_index;
  if (!args->GetString(0, &string_value) ||
      !base::StringToInt(string_value, &browser_index)) {
    NOTREACHED();
    return;
  }

  uint16 selected_items = importer::NONE;
  if (args->GetString(1, &string_value) && string_value == "true") {
    selected_items |= importer::HISTORY;
  }
  if (args->GetString(2, &string_value) && string_value == "true") {
    selected_items |= importer::FAVORITES;
  }
  if (args->GetString(3, &string_value) && string_value == "true") {
    selected_items |= importer::PASSWORDS;
  }
  if (args->GetString(4, &string_value) && string_value == "true") {
    selected_items |= importer::SEARCH_ENGINES;
  }

  const importer::SourceProfile& source_profile =
      importer_list_->GetSourceProfileAt(browser_index);
  uint16 supported_items = source_profile.services_supported;

  uint16 import_services = (selected_items & supported_items);
  if (import_services) {
    FundamentalValue state(true);
    web_ui_->CallJavascriptFunction("ImportDataOverlay.setImportingState",
                                    state);

    // TODO(csilv): Out-of-process import has only been qualified on MacOS X,
    // so we will only use it on that platform since it is required. Remove this
    // conditional logic once oop import is qualified for Linux/Windows.
    // http://crbug.com/22142
#if defined(OS_MACOSX)
    importer_host_ = new ExternalProcessImporterHost;
#else
    importer_host_ = new ImporterHost;
#endif
    importer_host_->SetObserver(this);
    Profile* profile = web_ui_->GetProfile();
    importer_host_->StartImportSettings(source_profile, profile,
                                        import_services,
                                        new ProfileWriter(profile), false);
  } else {
    LOG(WARNING) << "There were no settings to import from '"
        << source_profile.description << "'.";
  }
}

void ImportDataHandler::SourceProfilesLoaded() {
  ListValue browser_profiles;
  int profiles_count = importer_list_->GetAvailableProfileCount();
  for (int i = 0; i < profiles_count; i++) {
    const importer::SourceProfile& source_profile =
        importer_list_->GetSourceProfileAt(i);
    uint16 browser_services = source_profile.services_supported;

    DictionaryValue* browser_profile = new DictionaryValue();
    browser_profile->SetString("name", source_profile.description);
    browser_profile->SetInteger("index", i);
    browser_profile->SetBoolean("history",
        (browser_services & importer::HISTORY) != 0);
    browser_profile->SetBoolean("favorites",
        (browser_services & importer::FAVORITES) != 0);
    browser_profile->SetBoolean("passwords",
        (browser_services & importer::PASSWORDS) != 0);
    browser_profile->SetBoolean("search",
        (browser_services & importer::SEARCH_ENGINES) != 0);

    browser_profiles.Append(browser_profile);
  }

  web_ui_->CallJavascriptFunction(
      "options.ImportDataOverlay.updateSupportedBrowsers", browser_profiles);
}

void ImportDataHandler::ImportStarted() {
}

void ImportDataHandler::ImportItemStarted(importer::ImportItem item) {
  // TODO(csilv): show progress detail in the web view.
}

void ImportDataHandler::ImportItemEnded(importer::ImportItem item) {
  // TODO(csilv): show progress detail in the web view.
}

void ImportDataHandler::ImportEnded() {
  importer_host_->SetObserver(NULL);
  importer_host_ = NULL;

  web_ui_->CallJavascriptFunction("ImportDataOverlay.dismiss");
}
