// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/command_line.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/extensions/api/declarative/rules_registry_service.h"
#include "chrome/browser/extensions/api/declarative_webrequest/webrequest_constants.h"
#include "chrome/browser/extensions/api/declarative_webrequest/webrequest_rules_registry.h"
#include "chrome/browser/extensions/extension_apitest.h"
#include "chrome/browser/extensions/extension_system_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/browser/browser_thread.h"
#include "extensions/common/extension.h"

using extensions::RulesRegistry;
using extensions::RulesRegistryService;
using extensions::WebRequestRulesRegistry;

namespace {

const char kArbitraryUrl[] = "http://www.example.com";

// The extension in "declarative/redirect_to_data" redirects every navigation to
// a page with title |kTestTitle|.
const char kTestTitle[] = ":TEST:";

}  // namespace

class DeclarativeApiTest : public ExtensionApiTest {
 public:
  std::string GetTitle() {
    string16 title(
        browser()->tab_strip_model()->GetActiveWebContents()->GetTitle());
    return base::UTF16ToUTF8(title);
  }
};

IN_PROC_BROWSER_TEST_F(DeclarativeApiTest, DeclarativeApi) {
  ASSERT_TRUE(RunExtensionTest("declarative/api")) << message_;

  // Check that unloading the page has removed all rules.
  std::string extension_id = GetSingleLoadedExtension()->id();
  UnloadExtension(extension_id);

  // UnloadExtension posts a task to the owner thread of the extension
  // to process this unloading. The next task to retrive all rules
  // is therefore processed after the UnloadExtension task has been executed.

  RulesRegistryService* rules_registry_service =
      extensions::RulesRegistryService::Get(browser()->profile());
  scoped_refptr<RulesRegistry> rules_registry =
      rules_registry_service->GetRulesRegistry(
          RulesRegistry::WebViewKey(0, 0),
          extensions::declarative_webrequest_constants::kOnRequest);

  std::vector<linked_ptr<RulesRegistry::Rule> > known_rules;

  content::BrowserThread::PostTask(
      rules_registry->owner_thread(),
      FROM_HERE,
      base::Bind(base::IgnoreResult(&RulesRegistry::GetAllRules),
                 rules_registry, extension_id, &known_rules));

  content::RunAllPendingInMessageLoop(rules_registry->owner_thread());

  EXPECT_TRUE(known_rules.empty());
}

// PersistRules test first installs an extension, which registers some rules.
// Then after browser restart, it checks that the rules are still in effect.
IN_PROC_BROWSER_TEST_F(DeclarativeApiTest, PRE_PersistRules) {
  ASSERT_TRUE(RunExtensionTest("declarative/redirect_to_data")) << message_;
}

IN_PROC_BROWSER_TEST_F(DeclarativeApiTest, PersistRules) {
  ui_test_utils::NavigateToURL(browser(), GURL(kArbitraryUrl));
  EXPECT_EQ(kTestTitle, GetTitle());
}
