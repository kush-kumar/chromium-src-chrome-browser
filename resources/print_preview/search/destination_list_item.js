// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

cr.define('print_preview', function() {
  'use strict';

  /**
   * Component that renders a destination item in a destination list.
   * @param {!cr.EventTarget} eventTarget Event target to dispatch selection
   *     events to.
   * @param {!print_preview.Destination} destination Destination data object to
   *     render.
   * @constructor
   * @extends {print_preview.Component}
   */
  function DestinationListItem(eventTarget, destination) {
    print_preview.Component.call(this);

    /**
     * Event target to dispatch selection events to.
     * @type {!cr.EventTarget}
     * @private
     */
    this.eventTarget_ = eventTarget;

    /**
     * Destination that the list item renders.
     * @type {!print_preview.Destination}
     * @private
     */
    this.destination_ = destination;
  };

  /**
   * Event types dispatched by the destination list item.
   * @enum {string}
   */
  DestinationListItem.EventType = {
    // Dispatched when the list item is activated.
    SELECT: 'print_preview.DestinationListItem.SELECT'
  };

  /**
   * CSS classes used by the destination list item.
   * @enum {string}
   * @private
   */
  DestinationListItem.Classes_ = {
    ICON: 'destination-list-item-icon',
    NAME: 'destination-list-item-name',
    STALE: 'stale'
  };

  /**
   * URLs of the various destination list item icons.
   * @enum {string}
   * @private
   */
  DestinationListItem.Icons_ = {
    CLOUD: 'images/cloud_printer_32.png',
    CLOUD_SHARED: 'images/cloud_printer_shared_32.png',
    LOCAL: 'images/classic_printer_32.png',
    MOBILE: 'images/mobile_32.png',
    MOBILE_SHARED: 'images/mobile_shared_32.png',
    GOOGLE_PROMOTED: 'images/google_promoted_printer_32.png'
  };

  DestinationListItem.prototype = {
    __proto__: print_preview.Component.prototype,

    /** @override */
    createDom: function() {
      this.setElementInternal(this.cloneTemplateInternal(
          'destination-list-item-template'));

      var iconImg = this.getElement().getElementsByClassName(
          print_preview.DestinationListItem.Classes_.ICON)[0];
      iconImg.src = this.getIconUrl_();

      var nameEl = this.getElement().getElementsByClassName(
          DestinationListItem.Classes_.NAME)[0];
      nameEl.textContent = this.destination_.displayName;

      this.initializeOfflineStatusElement_();
    },

    /** @override */
    enterDocument: function() {
      print_preview.Component.prototype.enterDocument.call(this);
      this.tracker.add(this.getElement(), 'click', this.onActivate_.bind(this));
    },

    /**
     * Initializes the element which renders the print destination's
     * offline status.
     * @private
     */
    initializeOfflineStatusElement_: function() {
      if (arrayContains([print_preview.Destination.ConnectionStatus.OFFLINE,
                         print_preview.Destination.ConnectionStatus.DORMANT],
                        this.destination_.connectionStatus)) {
        this.getElement().classList.add(DestinationListItem.Classes_.STALE);
        var offlineDurationMs = Date.now() - this.destination_.lastAccessTime;
        var offlineMessageId;
        if (offlineDurationMs > 31622400000.0) { // One year.
          offlineMessageId = 'offlineForYear';
        } else if (offlineDurationMs > 2678400000.0) { // One month.
          offlineMessageId = 'offlineForMonth';
        } else if (offlineDurationMs > 604800000.0) { // One week.
          offlineMessageId = 'offlineForWeek';
        } else {
          offlineMessageId = 'offline';
        }
        var offlineStatusEl = this.getElement().querySelector(
            '.offline-status');
        offlineStatusEl.textContent = localStrings.getString(offlineMessageId);
        setIsVisible(offlineStatusEl, true);
      }
    },

    /**
     * @param {!print_preview.Destination} dest Destination to get the icon URL
     *     for.
     * @return {string} URL of the icon for the given print destination.
     * @private
     */
    getIconUrl_: function() {
      var dest = this.destination_;
      if (dest.isGooglePromoted) {
          return DestinationListItem.Icons_.GOOGLE_PROMOTED;
      } else if (dest.isLocal) {
        return DestinationListItem.Icons_.LOCAL;
      } else if (dest.type == print_preview.Destination.Type.MOBILE &&
                 dest.isOwned) {
        return DestinationListItem.Icons_.MOBILE;
      } else if (dest.type == print_preview.Destination.Type.MOBILE &&
                 !dest.isOwned) {
        return DestinationListItem.Icons_.MOBILE_SHARED;
      } else if (dest.type == print_preview.Destination.Type.GOOGLE &&
                 dest.isOwned) {
        return DestinationListItem.Icons_.CLOUD;
      } else {
        return DestinationListItem.Icons_.CLOUD_SHARED;
      }
    },

    /**
     * Called when the destination item is activated. Dispatches a SELECT event
     * on the given event target.
     * @private
     */
    onActivate_: function() {
      var selectEvt = new cr.Event(DestinationListItem.EventType.SELECT);
      selectEvt.destination = this.destination_;
      this.eventTarget_.dispatchEvent(selectEvt);
    }
  };

  // Export
  return {
    DestinationListItem: DestinationListItem
  };
});
