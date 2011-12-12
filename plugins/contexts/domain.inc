<?php

/**
 * @file
 *
 * Plugin to provide a domain context
 */
$plugin = array(
    'title' => t('Domain'),
    'description' => t('A domain record'),
    'context' => 'domain_ctools_context_create_domain',
    'settings form' => 'domain_ctools_domain_context_settings_form',
    'settings form' => 'domain_ctools_domain_context_settings_form',
    'settings form validate' => 'domain_ctools_domain_context_settings_form_validate',
    'keyword' => 'domain',
    'context name' => 'domain',
    'no ui' => FALSE,
    'convert list' => 'domain_ctools_domain_context_convert_list',
    'convert' => 'domain_ctools_domain_context_convert',
);

function domain_ctools_context_create_domain($empty, $data = NULL, $conf = FALSE) {
  global $_domain;
  $context = new ctools_context('domain');
  $context->plugin = 'domain';

  if ($empty) {
    return $context;
  }

  if ($conf && $data['domain_option'] == 'active') {
    $data = (object) $_domain;
  }
  else if ($conf && $data['domain_id']) {
    $data = domain_lookup($data['domain_id']);
  }

  if (!empty($data)) {
    $context->data = $data;
    $context->subdomain = $data->subdomain;
    $context->argument = $data->domain_id;
    $context->sitename = $data->sitename;
    return $context;
  }

  return $context;
}

/**
 * Provide a list of sub-keywords.
 *
 * This is used to provide keywords from the context for use in a content type,
 * pane, etc.
 */
function domain_ctools_domain_context_convert_list() {
  return array(
      'domain_id' => t('Domain ID'),
      'subdomain' => t('Subdomain'),
      'sitename' => t('Site Name'),
      'schema' => t('Schema'),
  );
}

/**
 * Convert a context into a string to be used as a keyword by content types, etc.
 */
function domain_ctools_domain_context_convert($context, $type) {
  return $context->data->{$type};
}

/**
 * Settings form for the domain context
 */
function domain_ctools_domain_context_settings_form($conf) {
  if (empty($conf)) {
    $conf = array(
        'domain_id' => '',
        'description' => '',
    );
  }

  if (!empty($conf['domain_id'])) {
    $info = db_fetch_object(db_query("SELECT * FROM {domain} WHERE domain_id = %d", $conf['domain_id']));
    if ($info) {
      $description = t('Currently set to @domain.', array('@term' => $info->sitename));
    }
  }

  $form['domain_option'] = array(
      '#type' => 'radios',
      '#title' => t('Domain ID'),
      '#options' => array(
          'active' => t('Use Active Domain'),
          'other' => t('Other (specify below)'),
      ),
      '#default_value' => $conf['domain_option'],
  );

  $form['domain_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Domain ID'),
      '#description' => $description,
      '#default)_value' => $conf['domain_id'],
  );

  return $form;
}

function domain_ctools_domain_context_settings_form_validate($form, &$form_values, &$form_state) {
  if (!$form_values['domain_option']) {
    form_set_error('domain_option', t('Please specify a source for the domain id'));
  }
  else if ($form_values['domain_option'] == 'other' && !$form_values['domain_id']) {
    form_set_error('domain_option', t('Please specify the domain id'));
  }
}