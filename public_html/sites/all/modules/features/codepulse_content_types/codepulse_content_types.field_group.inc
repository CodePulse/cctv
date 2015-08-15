<?php
/**
 * @file
 * codepulse_content_types.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function codepulse_content_types_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_service|node|service|form';
  $field_group->group_name = 'group_service';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'service';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Service',
    'weight' => '0',
    'children' => array(
      0 => 'body',
      1 => 'field_icon',
      2 => 'title',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-service field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_service|node|service|form'] = $field_group;

  return $export;
}
