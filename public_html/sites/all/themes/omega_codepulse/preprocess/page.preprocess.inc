<?php

/**
 * Implements hook_preprocess_page().
 */
function omega_codepulse_preprocess_page(&$variables) {
  // You can use preprocess hooks to modify the variables before they are passed
  // to the theme function or template file.
//  $variables['#show_messages'] = FALSE;
//  krumo($variables);exit;
  if (drupal_is_front_page()) {
    unset($variables['page']['content']['system_main']['default_message']);
  }

}
