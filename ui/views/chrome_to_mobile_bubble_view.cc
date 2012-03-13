// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/views/chrome_to_mobile_bubble_view.h"

#include "base/bind.h"
#include "base/file_util.h"
#include "base/string16.h"
#include "base/utf_string_conversions.h"
#include "base/values.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/chrome_to_mobile_service.h"
#include "chrome/browser/chrome_to_mobile_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/views/window.h"
#include "grit/generated_resources.h"
#include "grit/theme_resources.h"
#include "ui/base/animation/throb_animation.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/text/bytes_formatting.h"
#include "ui/views/controls/button/checkbox.h"
#include "ui/views/controls/button/radio_button.h"
#include "ui/views/controls/button/text_button.h"
#include "ui/views/controls/label.h"
#include "ui/views/events/event.h"
#include "ui/views/layout/grid_layout.h"
#include "ui/views/layout/layout_constants.h"

using views::GridLayout;

namespace {

// The millisecond duration of the "Sending..." progress throb animation.
const size_t kProgressThrobDurationMS = 2400;

// The bubble's margin for the "Sending..." and "Sent" states.
const size_t kProgressMargin = 20;

// The title label's color; matches the bookmark bubble's title.
const SkColor kTitleColor = 0xFF062D75;

}  // namespace

// Declared in browser_dialogs.h so callers don't have to depend on our header.

namespace browser {

void ShowChromeToMobileBubbleView(views::View* anchor_view, Profile* profile) {
  ChromeToMobileBubbleView::ShowBubble(anchor_view, profile);
}

void HideChromeToMobileBubbleView() {
  ChromeToMobileBubbleView::Hide();
}

bool IsChromeToMobileBubbleViewShowing() {
  return ChromeToMobileBubbleView::IsShowing();
}

}  // namespace browser

// ChromeToMobileBubbleView ----------------------------------------------------

ChromeToMobileBubbleView* ChromeToMobileBubbleView::bubble_ = NULL;

ChromeToMobileBubbleView::~ChromeToMobileBubbleView() {}

// static
void ChromeToMobileBubbleView::ShowBubble(views::View* anchor_view,
                                          Profile* profile) {
  if (IsShowing())
    return;

  bubble_ = new ChromeToMobileBubbleView(anchor_view, profile);
  browser::CreateViewsBubble(bubble_);
  bubble_->Show();
}

// static
bool ChromeToMobileBubbleView::IsShowing() {
  return bubble_ != NULL;
}

void ChromeToMobileBubbleView::Hide() {
  if (IsShowing())
    bubble_->GetWidget()->Close();
}

views::View* ChromeToMobileBubbleView::GetInitiallyFocusedView() {
  return send_;
}

gfx::Rect ChromeToMobileBubbleView::GetAnchorRect() {
  // Compensate for some built-in padding in the arrow image.
  gfx::Rect rect(BubbleDelegateView::GetAnchorRect());
  rect.Inset(0, anchor_view() ? 5 : 0);
  return rect;
}

void ChromeToMobileBubbleView::WindowClosing() {
  // We have to reset |bubble_| here, not in our destructor, because we'll be
  // destroyed asynchronously and the shown state will be checked before then.
  DCHECK(bubble_ == this);
  bubble_ = NULL;
}

bool ChromeToMobileBubbleView::AcceleratorPressed(
    const ui::Accelerator& accelerator) {
  if (accelerator.key_code() == ui::VKEY_RETURN &&
      (send_->HasFocus() || cancel_->HasFocus())) {
    HandleButtonPressed(send_->HasFocus() ? send_ : cancel_);
    return true;
  }
  return BubbleDelegateView::AcceleratorPressed(accelerator);
}

void ChromeToMobileBubbleView::AnimationProgressed(
    const ui::Animation* animation) {
  if (animation == progress_animation_.get()) {
    double animation_value = animation->GetCurrentValue();
    int message = IDS_CHROME_TO_MOBILE_BUBBLE_SENDING_3;
    // Show each of four messages for 1/4 of the animation.
    if (animation_value < 0.25)
      message = IDS_CHROME_TO_MOBILE_BUBBLE_SENDING_0;
    else if (animation_value < 0.5)
      message = IDS_CHROME_TO_MOBILE_BUBBLE_SENDING_1;
    else if (animation_value < 0.75)
      message = IDS_CHROME_TO_MOBILE_BUBBLE_SENDING_2;
    progress_label_->SetText(l10n_util::GetStringUTF16(message));
    // Run Layout but do not resize the bubble for each progress message.
    Layout();
    return;
  }
  views::BubbleDelegateView::AnimationProgressed(animation);
}

void ChromeToMobileBubbleView::ButtonPressed(views::Button* sender,
                                             const views::Event& event) {
  HandleButtonPressed(sender);
}

void ChromeToMobileBubbleView::SnapshotGenerated(const FilePath& path,
                                                 int64 bytes) {
  if (bytes > 0) {
    snapshot_path_ = path;
    send_copy_->SetText(l10n_util::GetStringFUTF16(
        IDS_CHROME_TO_MOBILE_BUBBLE_SEND_COPY, ui::FormatBytes(bytes)));
    send_copy_->SetEnabled(true);
  } else {
    send_copy_->SetText(l10n_util::GetStringUTF16(
        IDS_CHROME_TO_MOBILE_BUBBLE_SEND_COPY_FAILED));
  }
  Layout();
}

void ChromeToMobileBubbleView::OnSendComplete(bool success) {
  progress_animation_->Stop();
  progress_label_->SetText(l10n_util::GetStringUTF16(success ?
      IDS_CHROME_TO_MOBILE_BUBBLE_SENT : IDS_CHROME_TO_MOBILE_BUBBLE_ERROR));
  SizeToContents();
}

void ChromeToMobileBubbleView::Init() {
  GridLayout* layout = new GridLayout(this);
  SetLayoutManager(layout);

  const size_t single_column_set_id = 0;
  views::ColumnSet* cs = layout->AddColumnSet(single_column_set_id);
  cs->AddColumn(GridLayout::LEADING, GridLayout::LEADING, 0,
                GridLayout::USE_PREF, 0, 0);
  cs->AddPaddingColumn(1, 0);

  const size_t button_column_set_id = 1;
  cs = layout->AddColumnSet(button_column_set_id);
  cs->AddPaddingColumn(1, 0);
  cs->AddColumn(GridLayout::LEADING, GridLayout::TRAILING, 0,
                GridLayout::USE_PREF, 0, 0);
  // Subtract 2px for the natural button padding and to correspond with row
  // separation height; like BookmarkBubbleView.
  cs->AddPaddingColumn(0, views::kRelatedButtonHSpacing - 2);
  cs->AddColumn(GridLayout::LEADING, GridLayout::TRAILING, 0,
                GridLayout::USE_PREF, 0, 0);

  std::vector<DictionaryValue*> mobiles =
      ChromeToMobileServiceFactory::GetForProfile(profile_)->mobiles();
  DCHECK_GT(mobiles.size(), 0U);

  layout->StartRow(0, single_column_set_id);
  views::Label* title_label = new views::Label();
  title_label->SetFont(
      ResourceBundle::GetSharedInstance().GetFont(ResourceBundle::MediumFont));
  title_label->SetEnabledColor(kTitleColor);
  layout->AddView(title_label);

  if (mobiles.size() == 1) {
    selected_mobile_ = mobiles[0];
    string16 mobile_name;
    mobiles[0]->GetString("name", &mobile_name);
    title_label->SetText(l10n_util::GetStringFUTF16(
        IDS_CHROME_TO_MOBILE_BUBBLE_SINGLE_TITLE, mobile_name));
  } else {
    title_label->SetText(l10n_util::GetStringUTF16(
        IDS_CHROME_TO_MOBILE_BUBBLE_MULTI_TITLE));

    const size_t radio_column_set_id = 2;
    cs = layout->AddColumnSet(radio_column_set_id);
    cs->AddPaddingColumn(0, views::kRelatedControlHorizontalSpacing);
    cs->AddColumn(GridLayout::LEADING, GridLayout::CENTER, 0,
                  GridLayout::USE_PREF, 0, 0);

    views::RadioButton* radio;
    layout->AddPaddingRow(0, views::kRelatedControlSmallVerticalSpacing);
    for (std::vector<DictionaryValue*>::const_iterator it = mobiles.begin();
         it != mobiles.end(); ++it) {
      string16 name;
      (*it)->GetString("name", &name);
      radio = new views::RadioButton(name, 0);
      radio->set_listener(this);
      mobile_map_[radio] = *it;
      layout->StartRow(0, radio_column_set_id);
      layout->AddView(radio);
    }
    mobile_map_.begin()->first->SetChecked(true);
    selected_mobile_ = mobile_map_.begin()->second;
  }

  send_copy_ = new views::Checkbox(
      l10n_util::GetStringFUTF16(IDS_CHROME_TO_MOBILE_BUBBLE_SEND_COPY,
          l10n_util::GetStringUTF16(
              IDS_CHROME_TO_MOBILE_BUBBLE_SEND_COPY_GENERATING)));
  send_copy_->SetEnabled(false);
  layout->StartRow(0, single_column_set_id);
  layout->AddView(send_copy_);

  layout->AddPaddingRow(0, views::kRelatedControlSmallVerticalSpacing);
  send_ = new views::NativeTextButton(
      this, l10n_util::GetStringUTF16(IDS_CHROME_TO_MOBILE_BUBBLE_SEND));
  send_->SetIsDefault(true);
  cancel_ = new views::NativeTextButton(
      this, l10n_util::GetStringUTF16(IDS_CANCEL));
  layout->StartRow(0, button_column_set_id);
  layout->AddView(send_);
  layout->AddView(cancel_);

  AddAccelerator(ui::Accelerator(ui::VKEY_RETURN, 0));
}

ChromeToMobileBubbleView::ChromeToMobileBubbleView(views::View* anchor_view,
                                                   Profile* profile)
    : BubbleDelegateView(anchor_view, views::BubbleBorder::TOP_RIGHT),
      ALLOW_THIS_IN_INITIALIZER_LIST(weak_ptr_factory_(this)),
      profile_(profile),
      selected_mobile_(NULL),
      send_copy_(NULL),
      send_(NULL),
      cancel_(NULL),
      progress_label_(NULL) {
  // Generate the MHTML snapshot now to report its size in the bubble.
  ChromeToMobileServiceFactory::GetForProfile(profile)->
      GenerateSnapshot(weak_ptr_factory_.GetWeakPtr());
}

void ChromeToMobileBubbleView::HandleButtonPressed(views::Button* sender) {
  if (sender == send_) {
    Send();
  } else if (sender == cancel_) {
    GetWidget()->Close();
  } else {
    // The sender is a mobile radio button
    views::RadioButton* radio = static_cast<views::RadioButton*>(sender);
    DCHECK(mobile_map_.find(radio) != mobile_map_.end());
    selected_mobile_ = mobile_map_.find(radio)->second;
  }
}

void ChromeToMobileBubbleView::Send() {
  string16 mobile_id;
  selected_mobile_->GetString("id", &mobile_id);
  ChromeToMobileServiceFactory::GetForProfile(profile_)->SendToMobile(
      mobile_id, send_copy_->checked() ? snapshot_path_ : FilePath(),
      weak_ptr_factory_.GetWeakPtr());

  // Re-initialize the view's contents to show progress sending the page.
  RemoveAllChildViews(true);
  send_copy_ = NULL;
  send_ = NULL;
  cancel_ = NULL;

  GridLayout* layout = new GridLayout(this);
  SetLayoutManager(layout);

  const size_t single_column_set_id = 0;
  views::ColumnSet* cs = layout->AddColumnSet(single_column_set_id);
  cs->AddColumn(GridLayout::LEADING, GridLayout::LEADING, 0,
                GridLayout::USE_PREF, 0, 0);
  set_margin(kProgressMargin);

  // Use the final (longest) progress label string to resize the bubble.
  layout->StartRow(0, single_column_set_id);
  progress_label_ = new views::Label(
    l10n_util::GetStringUTF16(IDS_CHROME_TO_MOBILE_BUBBLE_SENDING_3));
  progress_label_->SetFont(
      ResourceBundle::GetSharedInstance().GetFont(ResourceBundle::MediumFont));
  progress_label_->SetEnabledColor(kTitleColor);
  layout->AddView(progress_label_);
  SizeToContents();

  progress_animation_.reset(new ui::ThrobAnimation(this));
  progress_animation_->SetDuration(kProgressThrobDurationMS);
  progress_animation_->StartThrobbing(-1);
}
