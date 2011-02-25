<?php

/**
 * Implementation of hook_views_data()
 */
function views_linkarea_views_data() {
  $data['views']['link'] = array(
    'title' => t('Link'),
    'help' => t('Provide an internal or external link.'),
    'area' => array(
      'handler' => 'views_handler_area_link',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function views_linkarea_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_linkarea'),
    ),
    'handlers' => array(
      'views_handler_area_link' => array(
        'parent' => 'views_handler_area',
      ),
    ),
  );
}

