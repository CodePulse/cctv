<?php

/**
 * Layerslider slide layer creator
 */
function layerslider_slide_creator(&$form, $slide) {
  // add all the necessary css and js file
  drupal_add_css(drupal_get_path('module', 'layerslider') . '/css/layerslider_admin.css');
  drupal_add_library('system', 'ui');
  drupal_add_library('system', 'ui.draggable');
  drupal_add_library('system', 'ui.droppable');
  drupal_add_library('system', 'ui.sortable');
  drupal_add_js(drupal_get_path('module', 'layerslider') . '/js/layerslider_admin.js');

  $layerslider = layerslider_load($slide['layerslider_name']); // get layerslider optionset

  // slide preview
  $form['layerslider_slide_preview'] = array(
    '#type' => 'item',
    '#title' => t('Slide Preview'),
    '#markup' => theme('layerslider_content', array('layerslider_data' => array($slide), 'settings' => unserialize($layerslider['optionset']), 'preview' => TRUE)), // banner preview
  );

  // Animation preview
  $form['layerslider_slide_animation_preview'] = array(
    '#type' => 'link',
    '#title' => t('Animation Preview'),
    '#href' => '',
  );

  // add layer at slide
  $form['layerslider_add_layer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Layer'),
  );
  // show the list of slide layers
  $form['layerslider_slide_layer'] = array(
    '#type' => 'fieldset',
    '#title' => t('List of Layers'),
  );
  // layer settings
  $form['layerslider_slide_settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // add slide element text
  $form['layerslider_add_layer']['text'] = array(
    '#type' => 'link',
    '#title' => t('Add Text'),
    '#href' => '',
  );
  // add slide element image
  $form['layerslider_add_layer']['image'] = array(
    '#type' => 'link',
    '#title' => t('Add Image'),
    '#href' => '',
  );
  // add slide element video
  $form['layerslider_add_layer']['video'] = array(
    '#type' => 'link',
    '#title' => t('Add Video'),
    '#href' => '',
  );

  // Slide element Appereance setting like top, left, style
  $form['layerslider_slide_settings']['layer_apperance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Apperance'),
    '#weight' => 1,
  );
  // Slide layer animation settings
  $form['layerslider_slide_settings']['effects'] = array(
    '#type' => 'fieldset',
    '#title' => t('Effects'),
    '#weight' => 2,
  );

  $form['layerslider_slide_settings']['layer_apperance']['class_list'] = array(
    '#type' => 'select',
    '#title' => t('Class'),
    '#options' => layerslider_parse_class($slide['layerslider_name']),
  );

  //Open CSS file in popup
  $form['layerslider_slide_settings']['layer_apperance']['css_file'] = array(
    '#type' => 'link',
    '#title' => t('Edit CSS file'),
    '#href' => '',
  );


  $form['layerslider_slide_settings']['layer_apperance']['position_top'] = array(
    '#type' => 'textfield',
    '#title' => t('Top'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );
  $form['layerslider_slide_settings']['layer_apperance']['position_left'] = array(
    '#type' => 'textfield',
    '#title' => t('left'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );
  $form['layerslider_slide_settings']['layer_apperance']['text_html'] = array(
    '#type' => 'textarea',
    '#title' => t('Text/Html'),
    '#rows' => 2,
    '#attributes' => array(
      'placeholder' => 'Enter Text',
    ),
  );

  // slide in animation
  $form['layerslider_slide_settings']['effects']['slide_in_animation'] = array(
    '#type' => 'fieldset',
    '#title' => t('In animation'),
  );
  $form['layerslider_slide_settings']['effects']['slide_in_animation']['slidedirection'] = array(
    '#type' => 'select',
    '#title' => t('Direction'),
    '#options' => layerslider_directional_effects_list(),
  );
  $form['layerslider_slide_settings']['effects']['slide_in_animation']['durationin'] = array(
    '#type' => 'textfield',
    '#title' => t('Duration'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#description' => t('In ms'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );
  $form['layerslider_slide_settings']['effects']['slide_in_animation']['easingin'] = array(
    '#type' => 'select',
    '#title' => t('Easing'),
    '#options' => layerslider_easing_effects_list(),
  );
  $form['layerslider_slide_settings']['effects']['slide_in_animation']['delayin'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#description' => t('In ms'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );
  // slide out animation
  $form['layerslider_slide_settings']['effects']['slide_out_animation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Out animation'),
  );
  $form['layerslider_slide_settings']['effects']['slide_out_animation']['slideoutdirection'] = array(
    '#type' => 'select',
    '#title' => t('Direction'),
    '#options' => layerslider_directional_effects_list(),
  );
  $form['layerslider_slide_settings']['effects']['slide_out_animation']['durationout'] = array(
    '#type' => 'textfield',
    '#title' => t('Duration'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#description' => t('In ms'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );
  $form['layerslider_slide_settings']['effects']['slide_out_animation']['easingout'] = array(
    '#type' => 'select',
    '#title' => t('Easing'),
    '#options' => layerslider_easing_effects_list(),
  );
  $form['layerslider_slide_settings']['effects']['slide_out_animation']['delayout'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#description' => t('In ms'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );

  // prepare variable to send javascript file
  $layerslider_var = array(
    'layerslider' => array(
      'basePath' => $GLOBALS['base_path'],
      'cssfile' => $GLOBALS['base_path'].variable_get('file_public_path'). '/layerslider/' . $slide['layerslider_name'] . '.css',
      'cssfileBrowse' => $GLOBALS['base_path']. 'admin/config/media/layerslider/manage/' .$slide['layerslider_name']. '/css',
      'layersliderPath' => drupal_get_path('module', 'layerslider'),
      'imceinstalled' =>  module_exists('imce'),
    )
  );
  drupal_add_js($layerslider_var, 'setting'); // send variable to javascipt file
  $form['#attributes']['onsubmit'] = "this.slide_markup.value = jQuery('.layerslider').html().trim();";
}

/**
 * Directional effects list for layers.
 */
function layerslider_directional_effects_list() {

  $effects = array(
    'auto'   => 'auto',
    'left'   => 'left',
    'right'  => 'right',
    'top'    => 'top', 
    'bottom' => 'bottom',
    'fade'   => 'fade',
  );

  return $effects;
}

/**
 * Easing effects list.
 */
function layerslider_easing_effects_list() {

  $effects = array(
    'linear'           => 'linear',
    'swing'            => 'swing',
    'easeInQuad'       => 'easeInQuad', 
    'easeOutQuad'      => 'easeOutQuad',
    'easeInOutQuad'    => 'easeInOutQuad',
    'easeInCubic'      => 'easeInCubic',
    'easeOutCubic'     => 'easeOutCubic',
    'easeInOutCubic'   => 'easeInOutCubic',
    'easeInQuart'      => 'easeInQuart',
    'easeOutQuart'     => 'easeOutQuart',
    'easeInOutQuart'   => 'easeInOutQuart',
    'easeInQuint'      => 'easeInQuint',
    'easeOutQuint'     => 'easeOutQuint',
    'easeInOutQuint'   => 'easeInOutQuint',
    'easeInSine'       => 'easeInSine',
    'easeOutSine'      => 'easeOutSine',
    'easeInOutSine'    => 'easeInOutSine',
    'easeInExpo'       => 'easeInExpo',
    'easeOutExpo'      => 'easeOutExpo',
    'easeInOutExpo'    => 'easeInOutExpo',
    'easeInCirc'       => 'easeInCirc',
    'easeOutCirc'      => 'easeOutCirc',
    'easeInOutCirc'    => 'easeInOutCirc',
    'easeInElastic'    => 'easeInElastic',
    'easeOutElastic'   => 'easeOutElastic',
    'easeInOutElastic' => 'easeInOutElastic',
    'easeInBack'       => 'easeInBack',
    'easeOutBack'      => 'easeOutBack',
    'easeInOutBack'    => 'easeInOutBack',
    'easeInBounce'     => 'easeInBounce',
    'easeOutBounce'    => 'easeOutBounce',
    'easeInOutBounce'  => 'easeInOutBounce',
  );

  return $effects;
}


/**
 * create css file for each banner to public directory at layerslider.
 */
function layerslider_create_stylesheet($filename) {
  $data = '';
  $filename = $filename . '.css';
  $csspath = 'public://layerslider';
  $uri = $csspath . '/' . $filename;
  // Create the CSS file.
  file_prepare_directory($csspath, FILE_CREATE_DIRECTORY);

  if (!file_exists($uri) && !file_unmanaged_save_data($data, $uri, FILE_EXISTS_REPLACE)) {
    return FALSE;
  }
}

/**
 * delete css file of banner from public directory layerslider.
 */
function layerslider_delete_stylesheet($filename) {
  $data = '';
  $filename = $filename . '.css';
  $csspath = 'public://layerslider';
  $uri = $csspath . '/' . $filename;
  // delete the CSS file.
  file_unmanaged_delete($uri);
}

/**
 * Implement css browsing.
 */
function layerslider_stylesheet($layerslider = array()) {
  module_invoke('admin_menu', 'suppress');//suppress admin_menu
  $jsop = isset($_GET['app']) ? $_GET['app'] : NULL;
  drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
  print layerslider_css_page($layerslider, $jsop);
  exit();
}

/**
 * Returns the layerslider css page.
 */
function layerslider_css_page($layerslider = array(), $jsop = NULL) {
  return theme('layerslider_css_page', array('content' => layerslider_css_content($layerslider, $jsop)));
}

/**
 * return the content css page.
 */
function layerslider_css_content($layerslider = array(), $jsop = NULL) {
  drupal_set_message($jsop);

  $forms = array();
  //process to open css file operations.
  $forms[] = drupal_get_form('layerslider_open_stylesheet', $layerslider);
  $forms = drupal_render($forms);
  return $forms;
}

/**
 * open css file in textarea for modify.
 */
function layerslider_open_stylesheet($form, &$form_state, $layerslider = array()) {
  drupal_add_js(drupal_get_path('module', 'layerslider') . '/js/layerslider_admin.js');
  $filepath = 'public://layerslider/' .$layerslider['layerslider_name']. '.css';

  $form['filename'] = array(
    '#type' => 'hidden',
    '#value' => $layerslider['layerslider_name'],
  );
  $form['class_list'] = array(
    '#type' => 'select',
    '#title' => t('Class'),
    '#options' => layerslider_parse_class($layerslider['layerslider_name']),
    '#states' => array(
      'invisible' => array(
       ':input[name="filename"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['css_content'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS file'),
    '#rows' => 20,
    '#default_value' => file_get_contents($filepath),  // Reads entire file into a string and open it in textarea
    '#description' => t('Use only class selector. Example: <em>.selector { property: value; } </em>'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('layerslider_save_stylesheet'),
  );

  return $form;
}

/**
 * Save the css updated file data to public directory layerslider.
 */
function layerslider_save_stylesheet($form, &$form_state) {

  $data = $form['css_content']['#value'];
  $file = 'public://layerslider/' .$form['filename']['#value']. '.css';
  
  file_unmanaged_save_data($data, $file, FILE_EXISTS_REPLACE);

  // Set standard file permissions for webserver-generated files.
  drupal_chmod($file);
}

/**
 * CSS parser from the file of css.
 */
function layerslider_parse_class($filename) {

  $filepath = 'public://layerslider/' .$filename. '.css';
  $css = drupal_load_stylesheet($filepath, TRUE);

  preg_match_all( '/(?ims)([a-z0-9\s\.\:#_\-@,]+)\{([^\}]*)\}/', $css, $arr);

  $result = array();
  $result[0] = '<None>';
  foreach ($arr[0] as $i => $x) {
    $result[trim($arr[1][$i])] = trim($arr[1][$i]);
  }
  return $result;
}

