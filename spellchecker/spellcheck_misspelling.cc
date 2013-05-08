// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/spellchecker/spellcheck_misspelling.h"

#include "base/values.h"

namespace {

// Builds a value from a list of spellcheck suggestions. The caller owns the
// result.
base::Value* BuildSuggestionsValue(const std::vector<string16>& list) {
  base::ListValue* result = new base::ListValue;
  result->AppendStrings(list);
  return result;
}

// Builds a value from a spellcheck action. The caller owns the result.
base::Value* BuildUserActionValue(const SpellcheckAction& action) {
  base::ListValue* result = new base::ListValue;
  result->Append(action.Serialize());
  return result;
}

}  // namespace

SpellcheckMisspelling::SpellcheckMisspelling()
    : location(0), length(0), timestamp(base::Time::Now()) {
}

SpellcheckMisspelling::SpellcheckMisspelling(
    const string16& context,
    size_t location,
    size_t length,
    const std::vector<string16>& suggestions)
    : context(context),
      location(location),
      length(length),
      suggestions(suggestions),
      timestamp(base::Time::Now()) {
}

SpellcheckMisspelling::~SpellcheckMisspelling() {
}

base::DictionaryValue* SpellcheckMisspelling::Serialize() const {
  base::DictionaryValue* result = new base::DictionaryValue;
  result->SetString("originalText", context);
  result->SetInteger("misspelledStart", location);
  result->SetInteger("misspelledLength", length);
  result->SetDouble("timestamp", timestamp.ToJsTime());
  result->Set("suggestions", BuildSuggestionsValue(suggestions));
  result->Set("userActions", BuildUserActionValue(action));
  return result;
}
