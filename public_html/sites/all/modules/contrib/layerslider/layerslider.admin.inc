<?php

/**
 * @file
 * Administrative page callbacks for Layerslider module.
 */

/**
 * Menu callback which shows an overview page of all the banner and their descriptions.
 */
function layerslider_overview_page() {
  $result = db_query("SELECT * FROM {layerslider} ORDER BY title", array(), array('fetch' => PDO::FETCH_ASSOC));
  if($result->rowCount() > 0) {
    $header = array(t('Banner Name'), array('data' => t('Operations'), 'colspan' => '3'));
    $rows = array();
    foreach ($result as $layerslider) {
      $row = array(theme('layerslider_admin_overview', array('title' => $layerslider['title'], 'name' => $layerslider['layerslider_name'], 'description' => $layerslider['description'])));
      $row[] = array('data' => l(t('edit banner'), 'admin/config/media/layerslider/manage/' . $layerslider['layerslider_name'] . '/edit'));
      $row[] = array('data' => l(t('list slides'), 'admin/config/media/layerslider/manage/' . $layerslider['layerslider_name']));
      $row[] = array('data' => l(t('add slide'), 'admin/config/media/layerslider/manage/' . $layerslider['layerslider_name'] . '/add'));
      $rows[] = $row;
    }
  }
  else {
    $header = array(t('Banner Name'), array('data' => t('Operations')));
    $rows = array();
    $rows[] = array(array('data' => t('There are no layerslider banner yet. <a href="@link">Add banner</a>.', 
    array('@link' => url('admin/config/media/layerslider/add'))), 'colspan' => '2'));
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Returns HTML for a layerslider title and description for the layerslider overview page.
 *
 * @param $variables
 *   An associative array containing:
 *   - title: The layerslider's title.
 *   - description: The layerslider's description.
 *
 * @ingroup themeable
 */
function theme_layerslider_admin_overview($variables) {
  $output = check_plain($variables['title']);
  $output .= '<div class="description">' . filter_xss_admin($variables['description']) . '</div>';

  return $output;
}

/**
 * Menu callback; Build the form that handles the adding/editing of a layerslider banner.
 */
function layerslider_edit_banner($form, &$form_state, $type, $layerslider = array()) {
  $layerslider += array(
    'layerslider_name' => '',
    'old_name' => !empty($layerslider['layerslider_name']) ? $layerslider['layerslider_name'] : '',
    'title' => '',
    'description' => '',
    'optionset' => '',
  );
  // Allow layerslider_edit_banner_submit() and other form submit handlers to determine
  // whether the layerslider already exists.
  $form['#insert'] = empty($layerslider['old_name']);
  $form['old_name'] = array(
    '#type' => 'value',
    '#value' => $layerslider['old_name'],
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $layerslider['title'],
    '#required' => TRUE,
  );

  $form['layerslider_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Layerslider name'),
    '#default_value' => $layerslider['layerslider_name'],
    '#maxlength' => 27,
    '#description' => t('A unique name to construct the URL for the layerslider. It must only contain lowercase letters, numbers and hyphens.'),
    '#machine_name' => array(
      'exists' => 'layerslider_edit_banner_name_exists',
      'source' => array('title'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
    ),
    // A layerslider's machine name cannot be changed.
    '#disabled' => !empty($layerslider['old_name']),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $layerslider['description'],
  );

  // Options Vertical Tab Group table
  $form['optionset'] = array(
    '#type' => 'vertical_tabs',
  );

  $default_options = layerslider_option_elements(unserialize($layerslider['optionset']));
  // Add the options to the vertical tabs section
  foreach($default_options as $key => $value) {
    $form['optionset'][$key] = $value;
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#access' => $type == 'edit',
    '#submit' => array('layerslider_delete_submit'),
  );
  return $form;
}

/**
 * Returns whether a layerslider name already exists.
 *
 * @see layerslider_edit_banner()
 * @see form_validate_machine_name()
 */
function layerslider_edit_banner_name_exists($value) {
  // 'layerslider-' is added to the layerslider name to avoid name-space conflicts.
  $value = 'layerslider-' . $value;
  $layerslider_exists = db_query_range('SELECT 1 FROM {layerslider} WHERE layerslider_name = :layerslider', 0, 1, array(':layerslider' => $value))->fetchField();

  return $layerslider_exists;
}

/**
 * Submit function for the 'Delete' button on the layerslider editing form.
 */
function layerslider_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/media/layerslider/manage/' . $form_state['values']['layerslider_name'] . '/delete';
}

/**
 * Submit function for adding or editing a layerslider.
 */
function layerslider_edit_banner_submit($form, &$form_state) {
  $layerslider = $form_state['values'];

  $layerslider['optionset']['width']              = $form_state['values']['width'];
  $layerslider['optionset']['height']             = $form_state['values']['height'];
  $layerslider['optionset']['autoStart']          = $form_state['values']['autoStart'];
  $layerslider['optionset']['firstLayer']         = (int)$form_state['values']['firstLayer'];
  $layerslider['optionset']['twoWaySlideshow']    = $form_state['values']['twoWaySlideshow'];
  $layerslider['optionset']['keybNav']            = $form_state['values']['keybNav'];
  $layerslider['optionset']['touchNav']           = $form_state['values']['touchNav'];
  $layerslider['optionset']['imgPreload']         = $form_state['values']['imgPreload'];
  $layerslider['optionset']['navPrevNext']        = $form_state['values']['navPrevNext'];
  $layerslider['optionset']['navStartStop']       = $form_state['values']['navStartStop'];
  $layerslider['optionset']['navButtons']         = $form_state['values']['navButtons'];
  $layerslider['optionset']['skin']               = $form_state['values']['skin'];
  $layerslider['optionset']['skinsPath']          = $form_state['values']['skinsPath'];
  $layerslider['optionset']['pauseOnHover']       = $form_state['values']['pauseOnHover'];
  $layerslider['optionset']['globalBGColor']      = $form_state['values']['globalBGColor'];
  $layerslider['optionset']['globalBGImage']      = $form_state['values']['globalBGImage'];
  $layerslider['optionset']['animateFirstLayer']  = $form_state['values']['animateFirstLayer'];
  $layerslider['optionset']['yourLogo']           = $form_state['values']['yourLogo'];
  $layerslider['optionset']['yourLogoStyle']      = $form_state['values']['yourLogoStyle'];
  $layerslider['optionset']['yourLogoLink']       = $form_state['values']['yourLogoLink'];
  $layerslider['optionset']['yourLogoTarget']     = $form_state['values']['yourLogoTarget'];
  $layerslider['optionset']['loops']              = (int)$form_state['values']['loops'];
  $layerslider['optionset']['forceLoopNum']       = $form_state['values']['forceLoopNum'];
  $layerslider['optionset']['autoPlayVideos']     = $form_state['values']['autoPlayVideos'];
  $layerslider['optionset']['autoPauseSlideshow'] = $form_state['values']['autoPauseSlideshow'];
  $layerslider['optionset']['youtubePreview']     = $form_state['values']['youtubePreview'];

  $path = 'admin/config/media/layerslider/';
  if ($form['#insert']) {
    // Add 'layerslider-' to the layerslider name to help avoid name-space conflicts.
    $layerslider['layerslider_name'] = 'layerslider-' . $layerslider['layerslider_name'];
    layerslider_save($layerslider);
    layerslider_create_stylesheet($layerslider['layerslider_name']);  // create css file for each banner
  }
  else {
    layerslider_save($layerslider);
  }
  drupal_set_message(t('Your configuration has been saved.'));
  $form_state['redirect'] = $path;
}

/**
 * Menu callback; check access and get a confirm form for deletion of a layerslider.
 */
function layerslider_delete_banner($form, &$form_state, $layerslider) {
  $form['#layerslider'] = $layerslider;
  $caption = '';
  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';
  return confirm_form($form, t('Are you sure you want to delete the layerslider %title?', array('%title' => $layerslider['title'])), 
  'admin/config/media/layerslider/', $caption, t('Delete'));
}

/**
 * Delete a layerslider and all slides in it.
 */
function layerslider_delete_banner_submit($form, &$form_state) {
  $layerslider = $form['#layerslider'];
  $form_state['redirect'] = 'admin/config/media/layerslider';

  // Delete the layerslider and all its slides.
  layerslider_delete($layerslider);
  layerslider_delete_stylesheet($layerslider['layerslider_name']); // delete banner css from public directory

  $t_args = array('%title' => $layerslider['title']);
  drupal_set_message(t('The layerslider %title has been deleted.', $t_args));
  watchdog('layerslider', 'Deleted layerslider %title and all its slides.', $t_args, WATCHDOG_NOTICE);
}

/**
 * This function returns an array defining the form elements used to edit the different options.
 *
 * @param array $optionset [optional]
 *  Pass in a set of default values for the options
 * @return array
 *  Returns the option set array
 */
function layerslider_option_elements($optionset = array()) {
  drupal_add_css(drupal_get_path('module', 'layerslider') . '/css/layerslider_admin.css');
  // prepare variable to send javascript file
  $layerslider_var = array(
    'layerslider' => array(
      'basePath' => $GLOBALS['base_path'],
      'imceinstalled' =>  module_exists('imce'),
    )
  );
  drupal_add_js($layerslider_var, 'setting'); // send variable to javascipt file
  $form = array();

  // Appearance Settings
  $form['apperance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Apperance'),
  );
  $form['apperance']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#size' => 5,
    '#maxlength' => 10,
    '#description' => t("(px) Width of slider in pixel."),
    '#element_validate' => array('_layerslider_validate_positive_integer'),
	'#default_value' => isset($optionset['width']) ? $optionset['width'] : 940,
  );
  $form['apperance']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#size' => 5,
    '#maxlength' => 10,
    '#description' => t('(px) Height of the slider in pixel.'),
    '#element_validate' => array('_layerslider_validate_positive_integer'),
	'#default_value' => isset($optionset['height']) ? $optionset['height'] : 300,
  );
  $form['apperance']['globalBGColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background Color'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#description' => t("You can use only valid hexadecimal CSS color value, use 'transparent' for no color."),
    '#element_validate' => array('_layerslider_validate_color_value'),
	'#default_value' => isset($optionset['globalBGColor']) ? $optionset['globalBGColor'] : 'transparent',
	'#attached' => array(
      'library' => array(
        array('system', 'farbtastic'),   // Add Farbtastic color picker.
      ),
    ),
    '#suffix' => "<div id='colorpicker'></div>",
    '#attributes' => array(
      'onfocus' => "(function($){ $('#colorpicker').farbtastic('#edit-globalbgcolor'); $('#colorpicker').show(); })(jQuery)",
      'onblur' => "(function($){ $('#colorpicker').hide(); })(jQuery)",
    ),
  );
  $form['apperance']['globalBGImage'] = array(
    '#type' => 'textfield',
    '#title' => t('Background Image'),
    '#description' => t('This will be a fixed background image of LayerSlider by default.'),
	'#default_value' => isset($optionset['globalBGImage']) ? $optionset['globalBGImage'] : '',
    '#attributes' => array(
      'onclick' => "window.open('".$GLOBALS['base_path'].drupal_get_path('module', 'layerslider')."/plugins/image/image.htm?app=layerslider|url@edit-globalbgimage', '', 'width=480,height=500,resizable=1');",
    ),
  );
  $form['apperance']['skin'] = array(
    '#type' => 'textfield',
    '#title' => t('Skin'),
    '#description' => t("You can change the skin of the Slider, use 'noskin' to hide skin and buttons. (Pre-defined skins are: 'defaultskin', 'lightskin', 'darkskin', 'glass' and 'minimal'.)"),
	'#default_value' => isset($optionset['skin']) ? $optionset['skin'] : 'defaultskin',
  );
  $form['apperance']['skinsPath'] = array(
    '#type' => 'textfield',
    '#title' => t('Skin Path'),
    '#description' => t('You can change the default path of the skins folder. Note, that you must use the slash at the end of the path.'),
	'#default_value' => isset($optionset['skinsPath']) ? $optionset['skinsPath'] : 'sites/all/libraries/layerslider/skins/',
  );
  $form['apperance']['navPrevNext'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prev and Next buttons'),
    '#description' => t('If diabled, Prev and Next buttons will be invisible.'),
    '#default_value' => isset($optionset['navPrevNext']) ? $optionset['navPrevNext'] : true,
  );
  $form['apperance']['navStartStop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Start and Stop buttons'),
    '#description' => t('If disabled, Start and Stop buttons will be invisible.'),
    '#default_value' => isset($optionset['navStartStop']) ? $optionset['navStartStop'] : true,
  );
  $form['apperance']['navButtons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Navigation buttons'),
    '#description' => t('If disabled, slide buttons will be invisible.'),
    '#default_value' => isset($optionset['navButtons']) ? $optionset['navButtons'] : true,
  );


  // Navigation and Control Settings
  $form['navigation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navigation and Control Settings'),
  );
  $form['navigation']['autoStart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically Start Slideshow'),
    '#description' => t('If enabled, slideshow will automatically start after loading the page.'),
    '#default_value' => isset($optionset['autoStart']) ? $optionset['autoStart'] : true,
  );
  $form['navigation']['pauseOnHover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause on hover'),
    '#description' => t('SlideShow will pause when mouse pointer is over LayerSlider.'),
    '#default_value' => isset($optionset['pauseOnHover']) ? $optionset['pauseOnHover'] : true,
  );
  $form['navigation']['firstLayer'] = array(
    '#type' => 'textfield',
    '#title' => t('First layer'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#element_validate' => array('_layerslider_validate_positive_integer'),
    '#description' => t('LayerSlider will begin with this layer.'),
	'#default_value' => isset($optionset['firstLayer']) ? $optionset['firstLayer'] : '1',
  );
  $form['navigation']['animateFirstLayer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Animate first layer'),
    '#description' => t('If enabled, first layer will animate (slide in) instead of fading.'),
    '#default_value' => isset($optionset['animateFirstLayer']) ? $optionset['animateFirstLayer'] : false,
  );
  $form['navigation']['twoWaySlideshow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Two way slideshow'),
    '#description' => t('If enabled, slideshow will go backwards if you click the prev button.'),
    '#default_value' => isset($optionset['twoWaySlideshow']) ? $optionset['twoWaySlideshow'] : false,
  );
  $form['navigation']['loops'] = array(
    '#type' => 'select',
    '#title' => t('Loops'),
    '#options' => array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10),
    '#description' => t('Number of loops if autoStart set enabled (0 means infinite!)'),
	'#default_value' => isset($optionset['loops']) ? $optionset['loops'] : 0,
  );
  $form['navigation']['forceLoopNum'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force the number of loops'),
    '#description' => t('If enabled, the slider will always stop at the given number of loops even if the user restarts the slideshow.'),
    '#default_value' => isset($optionset['forceLoopNum']) ? $optionset['forceLoopNum'] : true,
  );
  $form['navigation']['autoPlayVideos'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically play videos'),
    '#description' => t('If enabled, slider will autoplay youtube / vimeo videos.'),
    '#default_value' => isset($optionset['autoPlayVideos']) ? $optionset['autoPlayVideos'] : true,
  );
  $form['navigation']['autoPauseSlideshow'] = array(
    '#type' => 'select',
    '#title' => t('Automatically pause slideshow'),
    '#description' => t("Auto means, if autoPlayVideos is enabled, slideshow will stop UNTIL the video is playing and after that it continues. Enabled means slideshow will stop and it won't continue after video is played."),
    '#options' => array(
      'true'   => t('enabled'),
      'false'  => t('disabled'),
      'auto'  => t('auto'),
    ),
    '#default_value' => isset($optionset['autoPauseSlideshow']) ? $optionset['autoPauseSlideshow'] : 'auto',
  );
  $form['navigation']['youtubePreview'] = array(
    '#type' => 'select',
    '#title' => t('Yourtube preview'),
    '#description' => t("Default thumbnail picture of YouTube videos. Can be maxresdefault.jpg, hqdefault.jpg, mqdefault.jpg or default.jpg. Note, that maxresdefault.jpg is not available to all (not HD) videos."),
    '#options' => array(
      'maxresdefault.jpg'   => t('maxresdefault.jpg'),
      'hqdefault.jpg'  => t('hqdefault.jpg'),
      'mqdefault.jpg'  => t('mqdefault.jpg'),
      'default.jpg'  => t('default.jpg'),
    ),
    '#default_value' => isset($optionset['youtubePreview']) ? $optionset['youtubePreview'] : 'maxresdefault.jpg',
  );
  $form['navigation']['keybNav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keyboard navigation'),
    '#description' => t('Keyboard navigation. You can navigate with the left and right arrow keys.'),
    '#default_value' => isset($optionset['keybNav']) ? $optionset['keybNav'] : true,
  );
  $form['navigation']['touchNav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Touch navigation'),
    '#description' => t('Touch-control (on mobile devices).'),
    '#default_value' => isset($optionset['touchNav']) ? $optionset['touchNav'] : true,
  );

  // Misc Settings
  $form['misc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Misc'),
  );
  $form['misc']['imgPreload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Image preload'),
    '#description' => t('Image preload. Preloads all images and background-images of the next layer.'),
    '#default_value' => isset($optionset['imgPreload']) ? $optionset['imgPreload'] : true,
  );
  $form['misc']['yourLogo'] = array(
    '#type' => 'textfield',
    '#title' => t('YourLogo'),
    '#description' => t('This is a fixed layer that will be shown above of LayerSlider container. For example if you want to display your own logo, etc., you can upload an image or choose one from the Media Library.'),
	'#default_value' => isset($optionset['yourLogo']) ? $optionset['yourLogo'] : '',
    '#attributes' => array(
      'onclick' => "window.open('".$GLOBALS['base_path'].drupal_get_path('module', 'layerslider')."/plugins/image/image.htm?app=layerslider|url@edit-yourlogo', '', 'width=480,height=500,resizable=1');",
    ),
  );
  $form['misc']['yourLogoStyle'] = array(
    '#type' => 'textfield',
    '#title' => t('YourLogo style'),
    '#description' => t('You can style your logo. You can use any CSS properties, for example you can add left and top properties to place the image inside the LayerSlider container anywhere you want.'),
	'#default_value' => isset($optionset['yourLogoStyle']) ? $optionset['yourLogoStyle'] : 'position: absolute; z-index: 1001; left: 10px; top: 10px;',
  );
  $form['misc']['yourLogoLink'] = array(
    '#type' => 'textfield',
    '#title' => t('YourLogo link'),
    '#description' => t('You can add a link to your logo. Set false is you want to display only an image without a link.'),
	'#default_value' => isset($optionset['yourLogoLink']) ? $optionset['yourLogoLink'] : '',
  );
  $form['misc']['yourLogoTarget'] = array(
    '#type' => 'select',
    '#title' => t('YourLogo link target'),
    '#description' => t("If '_blank', the clicked url will open in a new window."),
    '#options' => array(
      '_blank'  => t('_blank'),
      '_self'  => t('_self'),
    ),
    '#default_value' => isset($optionset['yourLogoTarget']) ? $optionset['yourLogoTarget'] : '_blank',
  );

  return $form;
}

/**
 * Validate a form element that should have an integer value.
 */
function _layerslider_validate_positive_integer($element, &$form_state) {
  $value = $element['#value'];
  if (!is_numeric($value) || intval($value) != $value || $value < 0) {
    form_error($element, t('%name must be a positive integer.', array('%name' => $element['#title'])));
  }
}

/**
 * Validate a form element that should have an valid color value.
 */
function _layerslider_validate_color_value($element, &$form_state) {
  $value = $element['#value'];
  // Only accept hexadecimal CSS color strings.
  if (!preg_match('/^#([a-f0-9]{3}){1,2}$/iD', $value) && $value != 'transparent') {
    form_error($element, t('%name must be a valid hexadecimal CSS color value.', array('%name' => $element['#title'])));
  }
}

/**
 * Form for editing an entire layerslider slides order at once.
 */
function layerslider_overview_form($form, &$form_state, $layerslider) {
  $form['layerslider_slide']['#tree'] = TRUE;
  $result = db_query("SELECT * FROM {layerslider_slides} WHERE layerslider_name = :layerslider ORDER BY weight", 
  array(':layerslider' => $layerslider['layerslider_name']), array('fetch' => PDO::FETCH_ASSOC));

  foreach ($result as $slide) {
    $form['layerslider_slide'][$slide['slideid']] = array(
      'slide_title' => array(
        '#markup' => check_plain($slide['slide_title']),
      ),
      'slide_markup' => array(
        '#markup' => check_plain($slide['slide_markup']),
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $slide['weight'],
        '#delta' => 50,
        '#title-display' => 'invisible',
      ),
      'edit' => array('#type' => 'link', '#title' => t('edit'), '#href' => "admin/config/media/layerslider/slide/". $slide['slideid'] ."/edit"),
      'delete' => array('#type' => 'link', '#title' => t('delete'), '#href' => "admin/config/media/layerslider/slide/". $slide['slideid'] ."/delete"),
    );
  }

  if (element_children($form['layerslider_slide'])) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $form['#empty_text'] = t('There are no slide yet. <a href="@link">Add slide</a>.', 
    array('@link' => url('admin/config/media/layerslider/manage/'. $layerslider['layerslider_name'] .'/add')));
  }
  return $form;
}

/**
 * Submit handler for the layerslider overview form.
 * @see layerslider_overview_form()
 */
function layerslider_overview_form_submit($form, &$form_state) {
  // Because the form elements were keyed with the item ids from the database,
  // we can simply iterate through the submitted values.
  foreach ($form_state['values']['layerslider_slide'] as $slideid => $slide) {
    db_query("UPDATE {layerslider_slides} SET weight = :weight WHERE slideid = :slideid", array(':weight' => $slide['weight'], ':slideid' => $slideid));
  }
}

/**
 * Returns HTML for the layerslider overview form into a table.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_layerslider_overview_form($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['layerslider_slide']) as $slideid) {
    $form['layerslider_slide'][$slideid]['weight']['#attributes']['class'] = array('layerslider-slide-weight');

    $rows[] = array(
      'data' => array(
        drupal_render($form['layerslider_slide'][$slideid]['slide_title']),
        drupal_render($form['layerslider_slide'][$slideid]['slide_markup']),
        drupal_render($form['layerslider_slide'][$slideid]['weight']),
        drupal_render($form['layerslider_slide'][$slideid]['edit']),
        drupal_render($form['layerslider_slide'][$slideid]['delete']),
      ),
      'class' => array('draggable'),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => $form['#empty_text'], 'colspan' => '5'));
  }

  $header = array(t('Slide Title'), t('Slide Markup'), t('Weight'), t('Edit'), t('Delete'));
  $table_id = 'layerslider-overview';

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));
  $output .= drupal_render_children($form);
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'layerslider-slide-weight');
  return $output;
}

/**
 * Menu callback; Build the layerslider slides editing form.
 */
function layerslider_edit_slide($form, &$form_state, $type, $layerslider, $slide) {

  if ($type == 'add' || empty($slide)) {
    // This is an add form, initialize the slide.
    $slide = array('layerslider_name' => $layerslider['layerslider_name'], 'slideid' => 0, 'slide_title' => '', 'weight' => 0, 'slide_markup' => '');
  }

  $form['slideid'] = array(
    '#type' => 'hidden',
    '#value' => $slide['slideid'],
  );
  $form['layerslider_name'] = array(
    '#type' => 'hidden',
    '#value' => $slide['layerslider_name'],
  );
  $form['slide_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Slide title'),
    '#default_value' => $slide['slide_title'],
    '#description' => t('The human-readable name of this slide'),
    '#required' => TRUE,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 50,
    '#default_value' => $slide['weight'],
    '#description' => t('Optional. Slides are displayed in ascending order by weight.'),
  );
  $form['slide_markup'] = array(
    '#type' => 'hidden',
    '#default_value' => $slide['slide_markup'], 
  );

  // slide options
  $form['layerslider_slide_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide Options'),
    '#collapsible' => TRUE,
  );
  $slide_options = layerslider_slide_options();
  // Add the slide options to the fieldset
  foreach($slide_options as $key => $value) {
    $form['layerslider_slide_options'][$key] = $value;
  }

  layerslider_slide_creator($form, $slide); // see at layerslider-slide-layer.inc


  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#access' => $type == 'edit',
    '#submit' => array('layerslider_slide_delete_submit'),
  );
  return $form;
}

/**
 * This function returns an array defining the form elements used to edit the different options.
 *
 *  Pass in a set of default values for the options
 * @return array
 */
function layerslider_slide_options() {

  $form = array();

  $form['slide_option'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide Option'),
  );
  $form['slide_option']['background-image'] = array(
    '#type' => 'textfield',
    '#title' => t('Bg Image'),
    '#size' => 15,
	'#default_value' => 'none',
    '#attributes' => array(
      'autocomplete' => 'off',
      'onclick' => "window.open('".$GLOBALS['base_path'].drupal_get_path('module', 'layerslider')."/plugins/image/image.htm?app=layerslider|url@edit-background-image', '', 'width=480,height=500,resizable=1');",
    ),
  );
  $form['slide_option']['background-repeat'] = array(
    '#type' => 'select',
    '#title' => t('Bg Repeat'),
    '#options' => layerslider_background_repeat_list(),
  );
  $form['slide_option']['background-position'] = array(
    '#type' => 'select',
    '#title' => t('Bg Position'),
    '#options' => layerslider_background_position_list(),
  );
  $form['slide_option']['background-color'] = array(
    '#type' => 'textfield',
    '#title' => t('Bg Color'),
    '#size' => 10,
    '#maxlenght' => 4,
	'#default_value' => 'transparent',
	'#attached' => array(
      'library' => array(
        array('system', 'farbtastic'),   // Add Farbtastic color picker.
      ),
    ),
    '#suffix' => "<div id='colorpicker'></div>",
    '#attributes' => array(
      'autocomplete' => 'off',
      'onfocus' => "(function($){ $('#colorpicker').farbtastic('#edit-background-color'); $('#colorpicker').show(); })(jQuery)",
      'onblur' => "(function($){ $('#colorpicker').hide(); })(jQuery)",
    ),
  );
  $form['slide_option']['slideDirection'] = array(
    '#type' => 'select',
    '#title' => t('Direction'),
    '#options' => layerslider_slide_directional_effects_list(),
  );
  $form['slide_option']['slidedelay'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#description' => t('In ms'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );

  // slide in animation
  $form['slide_in_animation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide in animation'),
  );
  $form['slide_in_animation']['durationin'] = array(
    '#type' => 'textfield',
    '#title' => t('Duration'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#description' => t('In ms'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );
  $form['slide_in_animation']['easingin'] = array(
    '#type' => 'select',
    '#title' => t('Easing'),
    '#options' => layerslider_easing_effects_list(),
  );
  $form['slide_in_animation']['delayin'] = array(
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
  $form['slide_out_animation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide out animation'),
  );
  $form['slide_out_animation']['durationout'] = array(
    '#type' => 'textfield',
    '#title' => t('Duration'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#description' => t('In ms'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );
  $form['slide_out_animation']['easingout'] = array(
    '#type' => 'select',
    '#title' => t('Easing'),
    '#options' => layerslider_easing_effects_list(),
  );
  $form['slide_out_animation']['delayout'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay'),
    '#size' => 10,
    '#maxlenght' => 4,
    '#description' => t('In ms'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
  );

  return $form;
}

/**
 * Submit function for the delete button on the slide editing form.
 */
function layerslider_slide_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/media/layerslider/slide/' . $form_state['values']['slideid'] . '/delete';
}

/**
 * Process layerslider slide add/edit form submissions.
 */
function layerslider_edit_slide_submit($form, &$form_state) {
  $slide = &$form_state['values'];
  $path = 'admin/config/media/layerslider/manage/';
  layerslider_slide_save($slide);
  drupal_set_message(t('Your configuration has been saved.'));
  $form_state['redirect'] = $path . $slide['layerslider_name'];
}

/**
 * Menu callback; check access and get a confirm form for deletion of a slide.
 */
function layerslider_delete_slide($form, &$form_state, $type, $slide) {
  $form['#slide'] = $slide;
  return confirm_form($form, t('Are you sure you want to delete the slide %slide?', array('%slide' => $slide['slide_title'])), 'admin/config/media/layerslider/manage/' . $slide['layerslider_name']);
}

/**
 * Process slide delete form submissions.
 */
function layerslider_delete_slide_submit($form, &$form_state) {
  $slide = $form['#slide'];
  layerslider_slide_delete($slide['slideid']);
  $t_args = array('%title' => $slide['slide_title']);
  drupal_set_message(t('The layerslider slide %title has been deleted.', $t_args));
  watchdog('layerslider', 'Deleted layerslider slide %title.', $t_args, WATCHDOG_NOTICE);
  $form_state['redirect'] = 'admin/config/media/layerslider/manage/' . $slide['layerslider_name'];
}

/**
 * Background Position list.
 */
function layerslider_background_position_list() {
  $bgposition = array(
    'center center' => t('center center'),
    'center top'    => t('center top'),
    'center bottom' => t('center bottom'),
    'left center'   => t('left center'),
    'left top'      => t('left top'),
    'left bottom'   => t('left bottom'),
    'right center'  => t('right center'),
    'right top'     => t('right top'),
    'right bottom'  => t('right bottom'),
  );
  return $bgposition;
}

/**
 * Background Repeat list.
 */
function layerslider_background_repeat_list() {
  $bgrepeat = array(
    'no-repeat' => t('no-repeat'),
    'repeat'    => t('repeat'),
    'repeat-x'  => t('repeat-x'),
    'repeat-y'  => t('repeat-y'),
  );
  return $bgrepeat;
}

/**
 * Directional effects list for slide.
 */
function layerslider_slide_directional_effects_list() {
  $effects = array(
    'right'   => 'right',
    'left'    => 'left',
    'top'     => 'top', 
    'bottom'  => 'bottom',
  );
  return $effects;
}

//layerslider slide layer creator functions.
include_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'layerslider') . '/layerslider-slide-layer.inc';
