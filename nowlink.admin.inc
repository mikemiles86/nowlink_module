<?php
/**
 * @file nowlink.admin.inc
 * contains admin functionality
 */
 
 
/*
 * system settings form for the Now Link  module
 */
function nowlink_settings_form($form,&$form_state){
  $default_settings = nowlink_get_default_settings();
  
  $available_node_types   = array();
  $available_date_fields  = array();
  $available_view_modes  	= array();
  //if already set in form_state, get the values
  if (isset($form_state['#available_node_types'])){
    $available_node_types   = $form_state['#available_node_types'];
    $available_date_fields  = $form_state['#available_date_fields'];
    $available_view_modes  	= $form_state['#available_view_modes'];
  // else, generate the values  
  }else{
    //get list of all node types
    $node_types = node_type_get_names();
    foreach ($node_types as $machine_name => $name){
      //get all of the fields for the node type
      $node_fields = field_info_instances('node',$machine_name);
      //find all the date fields
      foreach ($node_fields as $field_name => $value){
        $field_info = field_info_field($field_name);
        //is a date type?
        if ($field_info['module'] == 'date'){
          //save the field as an option
          $available_date_fields[$machine_name][$field_name] = $field_name;
          //if have not already done so, save node type as an option
          if (isset($available_node_types[$machine_name])){
          //do nothing
          }else{
            $available_node_types[$machine_name] = $name;
          }
        }
      }
    }
    //get list of the different node types
    $entity_info = entity_get_info('node');
    foreach ($entity_info['view modes'] as $machine_name => $data){
      $available_view_modes[$machine_name] = $data['label'];
    }
    //store to form state, so do not have to keep fetching.
    $form_state['#available_node_types'] = $available_node_types;
    $form_state['#available_view_modes']  = $available_view_modes;
    $form_state['#available_date_fields'] = $available_date_fields;  
  }
  
  //determin default value for node type field
  $selected_node_type = $default_settings['node_type'];
  if (isset($form_state['values']['nowlink_target_node_type'])){
    $selected_node_type = $form_state['values']['nowlink_target_node_type'];
  }
  
  $form['nowlink_target_node_type'] = array(
    '#type'      => 'select',
    '#title'      => 'Entity Type',
    '#options'    => $available_node_types,
    '#default_value'  => $selected_node_type,
		//set ajax callback to update list of date fields when node type changes
    '#ajax'      => array(
      'event'    => 'change',
      'callback'  => 'nowlink_admin_settings_ajax_callback',
      'wrapper'  => 'date_field_select_replace',
    ),
  );
  
  //determine the default value for date_field field
  $selected_date_field = $default_settings['field'];
  if (isset($form_state['values']['nowlink_target_date_field'])){
    $selected_date_field = $form_state['values']['nowlink_target_date_field'];
  }
  
  $form['nowlink_target_date_field'] = array(
    '#type'  => 'select',
    '#title'  => 'Date Field',
    '#options'=> $available_date_fields[$selected_node_type],
    '#default_value'  => $selected_date_field,
    '#prefix'  => '<div id="date_field_select_replace">',
    '#suffix'  => '</div>',
  );
  
  //determine the default value for the view_mode field
  $selected_view_mode = $default_settings['view_mode'];
  if (isset($form_state['values']['nowlink_target_view_mode'])){
    $selected_view_mode = $form_state['values']['nowlink_target_view_mode'];
  }
  
  $form['nowlink_target_view_mode'] = array(
    '#type'  => 'select',
    '#title'  => 'View Mode',
    '#options'  => $available_view_modes,
    '#default_value'  => $selected_view_mode,
  );
  
  //determine the default value for tne link_text field
  $selected_link_text = $default_settings['link_text'];
  if (isset($form_state['values']['nowlink_link_text'])){
    $selected_link_text = $form_state['values']['nowlink_link_text'];
  }
  
  $form['nowlink_link_text'] = array(
    '#type'  => 'textfield',
    '#title'  => 'Link Text',
    '#default_value' => $selected_link_text,
  );
  
  //return a system settings form so that drupal will handle the work
  return system_settings_form($form);  
}
/**
 * AJAX callback for updating the date field select list in teh admin settings form
 */
function nowlink_admin_settings_ajax_callback($form,$form_state){
	//just return the data_field select field which will have the correct values.
  return $form['nowlink_target_date_field'];
}
