<?php

/**
 * @file
 * Admin callbacks for islandora_pathauto.
 */

/**
 * Admin form.
 */
function islandora_pathauto_admin_settings(array $form, array &$form_state) {
  // Get possible query language types.
  $backend_options = module_invoke_all('islandora_basic_collection_query_backends');
  // Remove the legacy one since there would be no difference between legacy
  // SPARQL and current SPARQL.
  unset($backend_options[ISLANDORA_BASIC_COLLECTION_LEGACY_BACKEND]);
  $map_to_title = function ($backend) {
    return $backend['title'];
  };

  module_load_include('inc', 'islandora', 'includes/utilities');
  drupal_add_css(drupal_get_path('module', 'islandora_image_annotation') . '/css/islandora_annotation.css');
  $all_cmodels = islandora_get_content_models();
  $already_chosen = variable_get('islandora_pathauto_selected_cmodels', array());

  foreach ($already_chosen as $chosen) {
    $value = isset($all_cmodels[$chosen]) ? $all_cmodels[$chosen] : array('pid' => '', 'label' => '');
    unset($all_cmodels[$chosen]);
    $all_cmodels = array($chosen => $value) + $all_cmodels;
  }

  $form['content_model_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure which content models are exposed to custom aliases'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['content_model_wrapper']['pathauto_table'] = array(
    '#type' => 'item',
    '#title' => t('Enable custom pathauto settings for these content models'),
    '#description' => t('Configure aliases under Pathauto config: !link', array(
      '!link' => l(t('admin/config/search/path/patterns'), 'admin/config/search/path/patterns'),
    )),
    '#tree' => TRUE,
    '#theme' => 'islandora_pathauto_admin_table',
  );
  // Only add this option if the module is installed.
  if (module_exists('islandora_solr')) {
    $form['solr_options_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => 'Query Islandora objects settings',
      '#description' => 'Options related to Solr search module being installed',
    );
    $form['solr_options_wrapper']['query_language'] = array(
      '#type' => 'radios',
      '#title' => t('Query language'),
      '#options' => array_map($map_to_title, $backend_options),
      '#description' => t('Query language to use for finding the content models in the system for pathauto hooks.'),
      '#default_value' => variable_get('islandora_pathauto_query_language', 'islandora_basic_collection_sparql_query_backend'),
    );
    $form['solr_options_wrapper']['query_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Query limit'),
      '#description' => t('The maximum number of objects to return for a Solr query.'),
      '#default_value' => variable_get('islandora_pathauto_query_limit', '10000'),
    );
  }
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
 * Submit handler for admin form.
 */
function islandora_pathauto_admin_settings_submit($form, &$form_state) {
  $enabled = array_filter($form_state['values']['pathauto_table']['enabled']);
  variable_set('islandora_pathauto_selected_cmodels', array_keys($enabled));
  if (module_exists('islandora_solr')) {
    variable_set('islandora_pathauto_query_language', $form_state['values']['query_language']);
    variable_set('islandora_pathauto_query_limit', $form_state['values']['query_limit']);
  }
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
