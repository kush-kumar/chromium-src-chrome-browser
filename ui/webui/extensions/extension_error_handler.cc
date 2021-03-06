// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/webui/extensions/extension_error_handler.h"

#include "base/bind.h"
#include "base/file_util.h"
#include "base/files/file_path.h"
#include "base/location.h"
#include "base/strings/string16.h"
#include "base/strings/utf_string_conversions.h"
#include "base/threading/sequenced_worker_pool.h"
#include "base/values.h"
#include "chrome/browser/devtools/devtools_window.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "extensions/browser/extension_error.h"
#include "extensions/browser/extension_system.h"
#include "extensions/browser/file_highlighter.h"
#include "extensions/common/constants.h"
#include "extensions/common/extension.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"

namespace extensions {

namespace {

// Keys for objects passed to and from extension error UI.
const char kPathSuffixKey[] = "pathSuffix";
const char kTitleKey[] = "title";

std::string ReadFileToString(const base::FilePath& path) {
  std::string data;
  base::ReadFileToString(path, &data);
  return data;
}

}  // namespace

ExtensionErrorHandler::ExtensionErrorHandler(Profile* profile)
    : profile_(profile) {
}

ExtensionErrorHandler::~ExtensionErrorHandler() {
}

void ExtensionErrorHandler::GetLocalizedValues(
    content::WebUIDataSource* source) {
  source->AddString(
      "extensionErrorsShowMore",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_ERRORS_SHOW_MORE));
  source->AddString(
      "extensionErrorsShowFewer",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_ERRORS_SHOW_FEWER));
  source->AddString(
      "extensionErrorViewDetails",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_ERROR_VIEW_DETAILS));
  source->AddString(
      "extensionErrorViewManifest",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_ERROR_VIEW_MANIFEST));
  source->AddString(
      "extensionErrorOverlayContextUrl",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_ERROR_CONTEXT));
  source->AddString(
      "extensionErrorOverlayStackTrace",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_ERROR_STACK_TRACE));
  source->AddString(
      "extensionErrorOverlayAnonymousFunction",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_ERROR_ANONYMOUS_FUNCTION));
  source->AddString(
      "extensionErrorOverlayLaunchDevtools",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_ERROR_LAUNCH_DEVTOOLS));
  source->AddString(
      "extensionErrorOverlayContextUnknown",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_ERROR_CONTEXT_UNKNOWN));
  source->AddString(
      "extensionErrorOverlayNoCodeToDisplay",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_ERROR_NO_CODE_TO_DISPLAY));
}

void ExtensionErrorHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "extensionErrorRequestFileSource",
      base::Bind(&ExtensionErrorHandler::HandleRequestFileSource,
                 base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "extensionErrorOpenDevTools",
      base::Bind(&ExtensionErrorHandler::HandleOpenDevTools,
                 base::Unretained(this)));
}

void ExtensionErrorHandler::HandleRequestFileSource(
    const base::ListValue* args) {
  // There should only be one argument, a dictionary. Use this instead of a list
  // because it's more descriptive, harder to accidentally break with minor
  // modifications, and supports optional arguments more easily.
  CHECK_EQ(1u, args->GetSize());

  const base::DictionaryValue* dict = NULL;

  // Three required arguments: extension_id, path_suffix, and error_message.
  std::string extension_id;
  base::FilePath::StringType path_suffix_string;
  base::string16 error_message;

  if (!args->GetDictionary(0, &dict) ||
      !dict->GetString(kPathSuffixKey, &path_suffix_string) ||
      !dict->GetString(ExtensionError::kExtensionIdKey, &extension_id) ||
      !dict->GetString(ExtensionError::kMessageKey, &error_message)) {
    NOTREACHED();
    return;
  }

  const Extension* extension =
      ExtensionSystem::Get(Profile::FromWebUI(web_ui()))->
          extension_service()->GetExtensionById(extension_id,
                                                true /* include disabled */ );

  // Under no circumstances should we ever need to reference a file outside of
  // the extension's directory. If it tries to, abort.
  base::FilePath path_suffix(path_suffix_string);
  if (path_suffix.ReferencesParent())
    return;

  base::FilePath path = extension->path().Append(path_suffix);

  // Setting the title and the error message is the same for all file types.
  scoped_ptr<base::DictionaryValue> results(new base::DictionaryValue);
  results->SetString(kTitleKey,
                     base::UTF8ToUTF16(extension->name()) +
                         base::ASCIIToUTF16(": ") +
                         path.BaseName().LossyDisplayName());
  results->SetString(ExtensionError::kMessageKey, error_message);

  base::Callback<void(const std::string&)> reply;
  if (path_suffix_string == kManifestFilename) {
    std::string manifest_key;
    if (!dict->GetString(ManifestError::kManifestKeyKey, &manifest_key)) {
      NOTREACHED();
      return;
    }

    // A "specific" location is optional.
    std::string specific;
    dict->GetString(ManifestError::kManifestSpecificKey, &specific);

    reply = base::Bind(&ExtensionErrorHandler::GetManifestFileCallback,
                       base::Unretained(this),
                       base::Owned(results.release()),
                       manifest_key,
                       specific);
  } else {
    int line_number = 0;
    dict->GetInteger(RuntimeError::kLineNumberKey, &line_number);

    reply = base::Bind(&ExtensionErrorHandler::GetSourceFileCallback,
                       base::Unretained(this),
                       base::Owned(results.release()),
                       line_number);
  }

  base::PostTaskAndReplyWithResult(
      content::BrowserThread::GetBlockingPool(),
      FROM_HERE,
      base::Bind(&ReadFileToString, path),
      reply);
}

void ExtensionErrorHandler::HandleOpenDevTools(const base::ListValue* args) {
  CHECK_EQ(1U, args->GetSize());

  const base::DictionaryValue* dict = NULL;
  int render_process_id = 0;
  int render_view_id = 0;

  // The render view and render process ids are required.
  if (!args->GetDictionary(0, &dict) ||
      !dict->GetInteger(RuntimeError::kRenderProcessIdKey,
                        &render_process_id) ||
      !dict->GetInteger(RuntimeError::kRenderViewIdKey, &render_view_id)) {
    NOTREACHED();
    return;
  }

  content::RenderViewHost* rvh =
      content::RenderViewHost::FromID(render_process_id, render_view_id);

  // It's possible that the render view was closed since we last updated the
  // links. Handle this gracefully.
  if (!rvh)
    return;

  // If we include a url, we should inspect it specifically (and not just the
  // render view).
  base::string16 url;
  if (dict->GetString(RuntimeError::kUrlKey, &url)) {
    // Line and column numbers are optional; default to the first line.
    int line_number = 1;
    int column_number = 1;
    dict->GetInteger(RuntimeError::kLineNumberKey, &line_number);
    dict->GetInteger(RuntimeError::kColumnNumberKey, &column_number);

    // Line/column numbers are reported in display-friendly 1-based numbers,
    // but are inspected in zero-based numbers.
    DevToolsWindow::OpenDevToolsWindow(rvh,
        DevToolsToggleAction::Reveal(url, line_number - 1, column_number - 1));
  } else {
    DevToolsWindow::OpenDevToolsWindow(rvh);
  }

  // Once we open the inspector, we focus on the appropriate tab...
  content::WebContents* web_contents =
      content::WebContents::FromRenderViewHost(rvh);
  Browser* browser = chrome::FindBrowserWithWebContents(web_contents);
  // ... but background pages have no associated browser (and the inspector
  // opens in its own window), so our work is done.
  if (!browser)
    return;

  TabStripModel* tab_strip = browser->tab_strip_model();
  tab_strip->ActivateTabAt(tab_strip->GetIndexOfWebContents(web_contents),
                           false);  // Not through direct user gesture.
}

void ExtensionErrorHandler::GetManifestFileCallback(
    base::DictionaryValue* results,
    const std::string& key,
    const std::string& specific,
    const std::string& contents) {
  ManifestHighlighter highlighter(contents, key, specific);
  highlighter.SetHighlightedRegions(results);
  web_ui()->CallJavascriptFunction(
      "extensions.ExtensionErrorOverlay.requestFileSourceResponse", *results);
}

void ExtensionErrorHandler::GetSourceFileCallback(
    base::DictionaryValue* results,
    int line_number,
    const std::string& contents) {
  SourceHighlighter highlighter(contents, line_number);
  highlighter.SetHighlightedRegions(results);
  web_ui()->CallJavascriptFunction(
      "extensions.ExtensionErrorOverlay.requestFileSourceResponse", *results);
}

}  // namespace extensions
