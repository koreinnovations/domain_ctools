<?php
// $Id$

/**
 * @file
 * Plugin to provide access control based upon active domain.
 */

/**
 * Implement hook_ctools_access().
 */
function domain_ctools_domain_ctools_access() {
  return array(
    'title' => t('Domain Access'),
    'description' => t('Control access by active domain.'),
    'callback' => 'domain_ctools_domain_access_check',
    'default' => array('domains' => array(), 'domain_site' => TRUE),
    'settings form' => 'domain_ctools_domain_access_settings',
    'summary' => 'domain_ctools_domain_acesss_summary',
  );
}

/**
 * Settings form domain access plugin.
 */
function domain_ctools_domain_access_settings(&$form, &$form_state, $conf) {
  $domains = domain_domains();
  $options = array();
  foreach ($domains as $domain) {
    $key = ($domain['domain_id'] == 0) ? -1 : $domain['domain_id'];
    $options[$key] = check_plain($domain['sitename']);
  }
  $form['settings']['domain_site'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show on all affiliates'),
    '#default_value' => isset($conf['domain_site']) ? $conf['domain_site'] : TRUE,
    '#description' => t('Show content on all domains. This setting will override the domain-specific options below.'),
  );
  $form['settings']['domains'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Domains'),
    '#default_value' => isset($conf['domains']) ? $conf['domains'] : array(),
    '#options' => $options,
    '#description' => t('This content will only be visible on the selected domains.'),
  );
}

/**
 * Check for access based on the currently active domain.
 */
function domain_ctools_domain_access_check($conf, $context) {
  global $_domain;
  if ($conf['domain_site']) {
    return TRUE;
  }
  // Check each domain, converting -1 to 0.
  foreach ($conf['domains'] as $key => $value) {
    $id = ($key == -1) ? 0 : $key;
    if (abs($value) > 0 && $id == $_domain['domain_id']) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Provide a summary description based upon the checked domains.
 */
function domain_ctools_domain_acesss_summary($conf, $context) {
  // Visible on all domains?
  if ($conf['domain_site'] || !isset($conf['domains'])) {
    $conf['domains'] = array();
    $output = t('Content is visible on all domains.');
  }
  // Determine visibility.
  else {
    $domains = domain_domains();
    $names = array();
    foreach (array_filter($conf['domains']) as $key) {
      $id = ($key == -1) ? 0 : $key;
      $names[] = check_plain($domains[$id]['sitename']);
    }
    // Print the proper message.
    if (empty($names) && empty($output)) {
      $output = t('Content is visible on no domains.');
    }
    else {
      $output = t('Visible on !domains.', array('!domains' => implode(', ', $names)));
    }
  }
  return $output;
}
