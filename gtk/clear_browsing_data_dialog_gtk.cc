// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/gtk/clear_browsing_data_dialog_gtk.h"

#include <string>

#include "app/l10n_util.h"
#include "app/resource_bundle.h"
#include "chrome/browser/browser.h"
#include "chrome/browser/browsing_data_remover.h"
#include "chrome/browser/gtk/accessible_widget_helper_gtk.h"
#include "chrome/browser/gtk/browser_window_gtk.h"
#include "chrome/browser/gtk/gtk_chrome_link_button.h"
#include "chrome/browser/gtk/gtk_util.h"
#include "chrome/browser/pref_service.h"
#include "chrome/browser/profile.h"
#include "chrome/common/pref_names.h"
#include "grit/generated_resources.h"
#include "grit/locale_settings.h"

// static
void ClearBrowsingDataDialogGtk::Show(GtkWindow* parent, Profile* profile) {
  new ClearBrowsingDataDialogGtk(parent, profile);
}

ClearBrowsingDataDialogGtk::ClearBrowsingDataDialogGtk(GtkWindow* parent,
                                                       Profile* profile) :
    profile_(profile), remover_(NULL) {
  // Build the dialog.
  std::string dialog_name = l10n_util::GetStringUTF8(
      IDS_CLEAR_BROWSING_DATA_TITLE);
  GtkWidget* dialog = gtk_dialog_new_with_buttons(
      dialog_name.c_str(),
      parent,
      (GtkDialogFlags) (GTK_DIALOG_MODAL | GTK_DIALOG_NO_SEPARATOR),
      GTK_STOCK_CLOSE,
      GTK_RESPONSE_REJECT,
      NULL);

  accessible_widget_helper_.reset(new AccessibleWidgetHelper(dialog, profile));
  accessible_widget_helper_->SendOpenWindowNotification(dialog_name);

  gtk_widget_realize(dialog);
  gtk_util::SetWindowSizeFromResources(GTK_WINDOW(dialog),
                                       IDS_CLEARDATA_DIALOG_WIDTH_CHARS,
                                       -1,  // height
                                       false);  // resizable
  gtk_util::AddButtonToDialog(dialog,
      l10n_util::GetStringUTF8(IDS_CLEAR_BROWSING_DATA_COMMIT).c_str(),
      GTK_STOCK_APPLY, GTK_RESPONSE_ACCEPT);

  GtkWidget* content_area = GTK_DIALOG(dialog)->vbox;
  gtk_box_set_spacing(GTK_BOX(content_area), gtk_util::kContentAreaSpacing);

  GtkWidget* vbox = gtk_vbox_new(FALSE, gtk_util::kControlSpacing);
  gtk_container_add(GTK_CONTAINER(content_area), vbox);

  // Label on top of the checkboxes.
  GtkWidget* description = gtk_label_new(
      l10n_util::GetStringUTF8(IDS_CLEAR_BROWSING_DATA_LABEL).c_str());
  gtk_misc_set_alignment(GTK_MISC(description), 0, 0);
  gtk_box_pack_start(GTK_BOX(vbox), description, FALSE, FALSE, 0);

  // History checkbox.
  del_history_checkbox_ = gtk_check_button_new_with_label(
      l10n_util::GetStringUTF8(IDS_DEL_BROWSING_HISTORY_CHKBOX).c_str());
  gtk_box_pack_start(GTK_BOX(vbox), del_history_checkbox_, FALSE, FALSE, 0);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(del_history_checkbox_),
      profile_->GetPrefs()->GetBoolean(prefs::kDeleteBrowsingHistory));
  g_signal_connect(del_history_checkbox_, "toggled",
                   G_CALLBACK(HandleOnClickedWidget), this);

  // Downloads checkbox.
  del_downloads_checkbox_ = gtk_check_button_new_with_label(
      l10n_util::GetStringUTF8(IDS_DEL_DOWNLOAD_HISTORY_CHKBOX).c_str());
  gtk_box_pack_start(GTK_BOX(vbox), del_downloads_checkbox_, FALSE, FALSE, 0);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(del_downloads_checkbox_),
      profile_->GetPrefs()->GetBoolean(prefs::kDeleteDownloadHistory));
  g_signal_connect(del_downloads_checkbox_, "toggled",
                   G_CALLBACK(HandleOnClickedWidget), this);

  // Cache checkbox.
  del_cache_checkbox_ = gtk_check_button_new_with_label(
      l10n_util::GetStringUTF8(IDS_DEL_CACHE_CHKBOX).c_str());
  gtk_box_pack_start(GTK_BOX(vbox), del_cache_checkbox_, FALSE, FALSE, 0);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(del_cache_checkbox_),
      profile_->GetPrefs()->GetBoolean(prefs::kDeleteCache));
  g_signal_connect(del_cache_checkbox_, "toggled",
                   G_CALLBACK(HandleOnClickedWidget), this);

  // Cookies checkbox.
  del_cookies_checkbox_ = gtk_check_button_new_with_label(
      l10n_util::GetStringUTF8(IDS_DEL_COOKIES_CHKBOX).c_str());
  gtk_box_pack_start(GTK_BOX(vbox), del_cookies_checkbox_, FALSE, FALSE, 0);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(del_cookies_checkbox_),
      profile_->GetPrefs()->GetBoolean(prefs::kDeleteCookies));
  g_signal_connect(del_cookies_checkbox_, "toggled",
                   G_CALLBACK(HandleOnClickedWidget), this);

  // Passwords checkbox.
  del_passwords_checkbox_ = gtk_check_button_new_with_label(
      l10n_util::GetStringUTF8(IDS_DEL_PASSWORDS_CHKBOX).c_str());
  gtk_box_pack_start(GTK_BOX(vbox), del_passwords_checkbox_, FALSE, FALSE, 0);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(del_passwords_checkbox_),
      profile_->GetPrefs()->GetBoolean(prefs::kDeletePasswords));
  g_signal_connect(del_passwords_checkbox_, "toggled",
                   G_CALLBACK(HandleOnClickedWidget), this);

  // Form data checkbox.
  del_form_data_checkbox_ = gtk_check_button_new_with_label(
      l10n_util::GetStringUTF8(IDS_DEL_FORM_DATA_CHKBOX).c_str());
  gtk_box_pack_start(GTK_BOX(vbox), del_form_data_checkbox_, FALSE, FALSE, 0);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(del_form_data_checkbox_),
      profile_->GetPrefs()->GetBoolean(prefs::kDeleteFormData));
  g_signal_connect(del_form_data_checkbox_, "toggled",
                   G_CALLBACK(HandleOnClickedWidget), this);

  // Create a horizontal layout for the combo box and label.
  GtkWidget* combo_hbox = gtk_hbox_new(FALSE, gtk_util::kLabelSpacing);
  GtkWidget* time_period_label_ = gtk_label_new(
      l10n_util::GetStringUTF8(IDS_CLEAR_BROWSING_DATA_TIME_LABEL).c_str());
  gtk_box_pack_start(GTK_BOX(combo_hbox), time_period_label_, FALSE, FALSE, 0);

  // Time period combo box items.
  time_period_combobox_ = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(time_period_combobox_),
      l10n_util::GetStringUTF8(IDS_CLEAR_DATA_HOUR).c_str());
  gtk_combo_box_append_text(GTK_COMBO_BOX(time_period_combobox_),
      l10n_util::GetStringUTF8(IDS_CLEAR_DATA_DAY).c_str());
  gtk_combo_box_append_text(GTK_COMBO_BOX(time_period_combobox_),
      l10n_util::GetStringUTF8(IDS_CLEAR_DATA_WEEK).c_str());
  gtk_combo_box_append_text(GTK_COMBO_BOX(time_period_combobox_),
      l10n_util::GetStringUTF8(IDS_CLEAR_DATA_4WEEKS).c_str());
  gtk_combo_box_append_text(GTK_COMBO_BOX(time_period_combobox_),
      l10n_util::GetStringUTF8(IDS_CLEAR_DATA_EVERYTHING).c_str());
  gtk_combo_box_set_active(GTK_COMBO_BOX(time_period_combobox_),
      profile_->GetPrefs()->GetInteger(prefs::kDeleteTimePeriod));
  gtk_box_pack_start(GTK_BOX(combo_hbox),
                     time_period_combobox_, FALSE, FALSE, 0);
  g_signal_connect(time_period_combobox_, "changed",
                   G_CALLBACK(HandleOnClickedWidget), this);

  // Add the combo/label time period box to the vertical layout.
  gtk_box_pack_start(GTK_BOX(vbox), combo_hbox, FALSE, FALSE, 0);

  // Add widgets for the area below the accept buttons.
  GtkWidget* flash_link = gtk_chrome_link_button_new(
      l10n_util::GetStringUTF8(IDS_FLASH_STORAGE_SETTINGS).c_str());
  g_signal_connect(G_OBJECT(flash_link), "clicked",
                   G_CALLBACK(HandleOnFlashLinkClicked), this);
  GtkWidget* flash_link_hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(flash_link_hbox), flash_link, FALSE, FALSE, 0);
  gtk_box_pack_end(GTK_BOX(content_area), flash_link_hbox, FALSE, FALSE, 0);

  GtkWidget* separator = gtk_hseparator_new();
  gtk_box_pack_end(GTK_BOX(content_area), separator, FALSE, FALSE, 0);

  // Make sure we can move things around.
  DCHECK_EQ(GTK_DIALOG(dialog)->action_area->parent, content_area);

  // Now rearrange those because they're *above* the accept buttons...there's
  // no way to place them in the correct position with gtk_box_pack_end() so
  // manually move things into the correct order.
  gtk_box_reorder_child(GTK_BOX(content_area), flash_link_hbox, -1);
  gtk_box_reorder_child(GTK_BOX(content_area), separator, -1);
  gtk_box_reorder_child(GTK_BOX(content_area), GTK_DIALOG(dialog)->action_area,
                        -1);

  g_signal_connect(dialog, "response",
                   G_CALLBACK(HandleOnResponseDialog), this);
  gtk_widget_show_all(dialog);
}

ClearBrowsingDataDialogGtk::~ClearBrowsingDataDialogGtk() {
}

void ClearBrowsingDataDialogGtk::OnDialogResponse(GtkWidget* widget,
                                                  int response) {
  if (response == GTK_RESPONSE_ACCEPT) {
    int period_selected = gtk_combo_box_get_active(
        GTK_COMBO_BOX(time_period_combobox_));

    int items = 0;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(del_history_checkbox_)))
      items |= BrowsingDataRemover::REMOVE_HISTORY;
    if (gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(del_downloads_checkbox_))) {
      items |= BrowsingDataRemover::REMOVE_DOWNLOADS;
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(del_cookies_checkbox_)))
      items |= BrowsingDataRemover::REMOVE_COOKIES;
    if (gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(del_passwords_checkbox_))) {
      items |= BrowsingDataRemover::REMOVE_PASSWORDS;
    }
    if (gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(del_form_data_checkbox_))) {
      items |= BrowsingDataRemover::REMOVE_FORM_DATA;
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(del_cache_checkbox_)))
      items |= BrowsingDataRemover::REMOVE_CACHE;

    // BrowsingDataRemover deletes itself when done.
    remover_ = new BrowsingDataRemover(profile_,
        static_cast<BrowsingDataRemover::TimePeriod>(period_selected),
        base::Time());
    remover_->Remove(items);
  }

  delete this;
  gtk_widget_destroy(GTK_WIDGET(widget));
}

void ClearBrowsingDataDialogGtk::OnDialogWidgetClicked(GtkWidget* widget) {
  if (widget == del_history_checkbox_) {
    profile_->GetPrefs()->SetBoolean(prefs::kDeleteBrowsingHistory,
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ?
        true : false);
  } else if (widget == del_downloads_checkbox_) {
    profile_->GetPrefs()->SetBoolean(prefs::kDeleteDownloadHistory,
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ?
        true : false);
  } else if (widget == del_cache_checkbox_) {
    profile_->GetPrefs()->SetBoolean(prefs::kDeleteCache,
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ?
        true : false);
  } else if (widget == del_cookies_checkbox_) {
    profile_->GetPrefs()->SetBoolean(prefs::kDeleteCookies,
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ?
        true : false);
  } else if (widget == del_passwords_checkbox_) {
    profile_->GetPrefs()->SetBoolean(prefs::kDeletePasswords,
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ?
        true : false);
  } else if (widget == del_form_data_checkbox_) {
    profile_->GetPrefs()->SetBoolean(prefs::kDeleteFormData,
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ?
        true : false);
  } else if (widget == time_period_combobox_) {
    profile_->GetPrefs()->SetInteger(prefs::kDeleteTimePeriod,
        gtk_combo_box_get_active(GTK_COMBO_BOX(widget)));
  }
}

void ClearBrowsingDataDialogGtk::OnFlashLinkClicked(GtkWidget* button) {
  // We open a new browser window so the Options dialog doesn't get lost
  // behind other windows.
  Browser* browser = Browser::Create(profile_);
  browser->OpenURL(GURL(l10n_util::GetStringUTF8(IDS_FLASH_STORAGE_URL)),
                   GURL(), NEW_FOREGROUND_TAB, PageTransition::LINK);
  browser->window()->Show();
}
