// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview Bubble implementation.
 */

// TODO(xiyuan): Move this into shared.
cr.define('cr.ui', function() {
  /**
   * Creates a bubble div.
   * @constructor
   * @extends {HTMLDivElement}
   */
  var Bubble = cr.ui.define('div');

  Bubble.prototype = {
    __proto__: HTMLDivElement.prototype,

    // Anchor element
    anchor_: undefined,

    /** @inheritDoc */
    decorate: function() {
      this.ownerDocument.addEventListener('click',
                                          this.handleClick_.bind(this));
      this.addEventListener('webkitTransitionEnd',
                            this.handleTransitionEnd_.bind(this));
    },

    /**
     * Shows the bubble for given anchor element.
     * @param {!HTMLElement} el Anchor element of the bubble.
     * @param {HTMLElement} content Content to show in bubble.
     * @public
     */
    showContentForElement: function(el, content) {
      const ARROW_OFFSET = 14;
      const HORIZONTAL_PADDING = 10;
      const VERTICAL_PADDING = 5;

      var elementOrigin = Oobe.getOffset(el);
      var anchorX = elementOrigin.left + HORIZONTAL_PADDING - ARROW_OFFSET;
      var anchorY = elementOrigin.top + el.offsetHeight + VERTICAL_PADDING;

      this.style.left = anchorX + 'px';
      this.style.top = anchorY + 'px';

      this.anchor_ = el;
      this.innerHTML = '';
      this.appendChild(content);
      this.hidden = false;
      this.classList.remove('faded');
    },

    /**
     * Shows the bubble for given anchor element.
     * @param {!HTMLElement} el Anchor element of the bubble.
     * @param {string} text Text content to show in bubble.
     * @public
     */
    showTextForElement: function(el, text) {
      var span = this.ownerDocument.createElement('span');
      span.textContent = text;
      this.showContentForElement(el, span);
    },

    /**
     * Hides the bubble.
     */
    hide: function() {
      if (!this.classList.contains('faded'))
        this.classList.add('faded');
    },

    /**
     * Handler for faded transition end.
     * @private
     */
    handleTransitionEnd_: function(e) {
      if (this.classList.contains('faded'))
        this.hidden = true;
    },

    /**
     * Handler of click event.
     * @private
     */
    handleClick_: function(e) {
      // Ignore clicks on anchor element.
      if (e.target == this.anchor_)
        return;

      if (!this.hidden)
        this.hide();
    }
  };

  return {
    Bubble: Bubble
  };
});
