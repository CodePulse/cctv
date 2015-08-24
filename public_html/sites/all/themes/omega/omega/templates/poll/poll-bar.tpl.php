<?php

/**
 * @file
 * Default theme implementation to display the bar for a single choice in a
 * poll.
 *
 * Variables available:
 * - $title: The title of the poll.
 * - $votes: The number of votes for this choice
 * - $total_votes: The number of votes for this choice
 * - $percentage: The percentage of votes for this choice.
 * - $vote: The choice number of the current user's vote.
 * - $voted: Set to TRUE if the user voted for this choice.
 *
 * @see template_preprocess_poll_bar()
 */
?>
<div class="poll-bars">
  <div class="poll-bars__text"><?php print $title; ?></div>
  <div class="poll-bars__bar">
    <div style="width: <?php print $percentage; ?>%;" class="foreground"></div>
  </div>
  <div class="poll-bars__percent">
    <?php print $percentage; ?>% (<?php print format_plural($votes, '1 vote', '@count votes'); ?>)
  </div>
</div>


<div class="contact-details-block">
  <div class="left-contact">
    <div class="contact-email">
      <h4 class="contact-title">Email</h4>
      <p>contact@centralsurveillance.co.uk</p>
    </div>
    <div class="contact-number">
      <h4 class="contact-title">Number</h4>
      <p>+44 7805 823 528</p>
    </div>
  </div>
  <div class="right-contact">
  <div class="contact-address">
    <h4 class="contact-title">Address</h4>
    <p>Vintage House<br>
      37 Albert Embankment<br>
      London<br>
      SE1 7TL</p>
  </div>
  </div>
</div>