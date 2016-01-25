<?php
/**
 * @file
 * codepulse_image_styles.features.inc
 */

/**
 * Implements hook_image_default_styles().
 */
function codepulse_image_styles_image_default_styles() {
  $styles = array();

  // Exported image style: homepage_slider.
  $styles['homepage_slider'] = array(
    'label' => 'Homepage Slider',
    'effects' => array(
      2 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 1920,
          'height' => 400,
        ),
        'weight' => 2,
      ),
    ),
  );

  return $styles;
}
