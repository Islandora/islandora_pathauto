<?php

function islandora_pathauto_admin_settings(array $form, array &$form_state){
  module_load_include('inc', 'islandora', 'includes/utilities');
  drupal_add_css(drupal_get_path('module', 'islandora_image_annotation') . '/css/islandora_annotation.css');
  $all_cmodels = islandora_get_content_models();
  $already_chosen = variable_get('islandora_pathauto_selected_cmodels', array());

  foreach ($already_chosen as $chosen) {
    $value  = isset($all_cmodels[$chosen]) ? $all_cmodels[$chosen] : array('pid' => '', 'label' => '');
    unset($all_cmodels[$chosen]);
    $all_cmodels = array($chosen => $value) + $all_cmodels;
  }

  $form['content_model_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure Content Models'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['content_model_wrapper']['pathauto_table'] = array(
    '#type' => 'item',
    '#title' => t('Enable pathauto for these content models'),
    '#description' => t('Configure aliases under Pathauto config: ') . l('admin/config/search/path/patterns', 'admin/config/search/path/patterns'),
    '#tree' => TRUE,
    '#theme' => 'islandora_pathauto_admin_table',
  );
  $alink_options = array(
    'attributes' => array('target' => '_blank'),
    'html' => TRUE,
  );
  $options = array();

  foreach ($all_cmodels as $cmodel) {
    $pid = $cmodel['pid'];
    $label = $cmodel['label'];
    $options[$pid] = '';
    $form['content_model_wrapper']['pathauto_table']['name'][$pid] = array(
      '#type' => 'item',
      '#markup' => l($label, "islandora/object/$pid", $alink_options),
    );
    $form['content_model_wrapper']['pathauto_table']['content_model'][$pid] = array(
      '#type' => 'item',
      '#markup' => $pid,
    );
  }
  $form['content_model_wrapper']['pathauto_table']['enabled'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $already_chosen,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#weight' => 0,
    '#submit' => array('islandora_pathauto_admin_settings_submit'),
  );
  return $form;
}

/**
 * Submit handler for form
 *
 */
function islandora_pathauto_admin_settings_submit($form, &$form_state){
  $enabled = array_filter($form_state['values']['pathauto_table']['enabled']);
  variable_set('islandora_pathauto_selected_cmodels', array_keys($enabled));

}


/**
 * Themes the form table.
 *
 * @param array $variables
 *   An array containing variables used in the theme.
 *
 * @return string
 *   The marked up output.
 */
function theme_islandora_pathauto_admin_table(array $variables) {
  // Set form.
  $form = $variables['form'];
  $rows = array();
  foreach ($form['name'] as $key => $element) {
    // Do not take form control structures.
    if (is_array($element) && element_child($key)) {
      $row = array();
      $row[] = array(
        'data' => drupal_render($form['enabled'][$key]),
        'class' => 'islandora-annotation-checkbox');
      $row[] = array(
        'data' => drupal_render($form['name'][$key]),
        'title' => $key);
      $row[] = array(
        'data' => drupal_render($form['content_model'][$key]));
      // Add to rows.
      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'));
    }
  }
  // Individual table headers.
  $header = array();
  $header[] = array('data' => t('Enabled'));
  $header[] = array('data' => t('Content Model'));
  $header[] = array('data' => t('PID'));

  // Render form.
  $output = '';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'annotation_admin_table',
    ),
  ));
  $output .= drupal_render_children($form);
  return $output;
}
