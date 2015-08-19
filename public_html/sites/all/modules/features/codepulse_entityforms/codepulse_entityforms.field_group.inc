<?php
/**
 * @file
 * codepulse_entityforms.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function codepulse_entityforms_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_enquiries|entityform|contact_us|form';
  $field_group->group_name = 'group_enquiries';
  $field_group->entity_type = 'entityform';
  $field_group->bundle = 'contact_us';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'General enquires',
    'weight' => '0',
    'children' => array(
      0 => 'group_personal',
      1 => 'group_message',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'General enquires',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-enquiries field-group-div',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_enquiries|entityform|contact_us|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_message|entityform|contact_us|form';
  $field_group->group_name = 'group_message';
  $field_group->entity_type = 'entityform';
  $field_group->bundle = 'contact_us';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_enquiries';
  $field_group->data = array(
    'label' => 'Message',
    'weight' => '8',
    'children' => array(
      0 => 'field_message',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Message',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-message field-group-div',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_message|entityform|contact_us|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_personal|entityform|contact_us|form';
  $field_group->group_name = 'group_personal';
  $field_group->entity_type = 'entityform';
  $field_group->bundle = 'contact_us';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_enquiries';
  $field_group->data = array(
    'label' => 'Personal details',
    'weight' => '7',
    'children' => array(
      0 => 'field_name',
      1 => 'field_phone_number',
      2 => 'field_email',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Personal details',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-personal field-group-div',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_personal|entityform|contact_us|form'] = $field_group;

  return $export;
}
