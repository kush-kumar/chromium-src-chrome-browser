// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/autofill/country_combobox_model.h"

#include "components/autofill/core/browser/autofill_country.h"
#include "components/autofill/core/browser/test_personal_data_manager.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/libaddressinput/chromium/cpp/include/libaddressinput/address_ui.h"
#include "third_party/libaddressinput/chromium/cpp/include/libaddressinput/address_ui_component.h"

namespace autofill {

namespace {
const char kTestCountry[] = "AQ";
}

TEST(CountryComboboxModel, RespectsManagerDefaultCountry) {
  TestPersonalDataManager manager;
  manager.set_timezone_country_code(kTestCountry);

  CountryComboboxModel model(manager);
  EXPECT_EQ(kTestCountry, model.GetDefaultCountryCode());
}

// http://crbug.com/341329
#if defined(TOOLKIT_GTK)
#define MAYBE_AllCountriesHaveComponents DISABLED_AllCountriesHaveComponents
#else
#define MAYBE_AllCountriesHaveComponents AllCountriesHaveComponents
#endif

TEST(CountryComboboxModel, MAYBE_AllCountriesHaveComponents) {
  TestPersonalDataManager manager;
  CountryComboboxModel model(manager);

  for (int i = 0; i < model.GetItemCount(); ++i) {
    if (model.IsItemSeparatorAt(i))
      continue;

    std::string country_code = model.countries()[i]->country_code();
    std::vector< ::i18n::addressinput::AddressUiComponent> components =
        ::i18n::addressinput::BuildComponents(country_code);
    EXPECT_FALSE(components.empty());
  }
}

}  // namespace autofill
