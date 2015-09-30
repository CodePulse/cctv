<?php
/**
 * @file
 * Theming functions for the layerslider module.
 * 
 * Preprocessor functions fill variables for templates and helper
 * functions to make theming easier.
 */

/**
 * Template preprocess handler for 'layerslider_content' theme.
 */
function template_preprocess_layerslider_content(&$vars) {
  // Each layerslider instance gets a unique id
  $layerslider_id = &drupal_static('layerslider_id', 0);
  $vars['id'] = ++$layerslider_id;

  // define width, height, bgcolor, bgimage for layerslider
  if(!empty($vars['settings'])) {
    $vars['width']   = !empty($vars['settings']['width']) ? ' width: '.$vars['settings']['width'].'px;' : '';
    $vars['height']  = !empty($vars['settings']['height']) ? ' height: '.$vars['settings']['height'].'px;' : '';
    $vars['bgcolor'] = !empty($vars['settings']['globalBGColor']) ? ' background-color: '.$vars['settings']['globalBGColor'].';' : '';
    $vars['bgimage'] = !empty($vars['settings']['globalBGImage']) ? ' background-image: url('.$vars['settings']['globalBGImage'].');' : '';
  }

  // Attach layerslider JavaScript
  $optionset = _layerslider_optionset($vars['settings']);
  layerslider_add($layerslider_id, $optionset, $vars['preview']);

  $vars['items'] = array();
  foreach ($vars['layerslider_data'] as $slide => $item ) {
    $vars['items'][$slide] = $item['slide_markup'];
  }

}