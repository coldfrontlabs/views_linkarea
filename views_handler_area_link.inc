<?php

class views_handler_area_link extends views_handler_area {

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    $options['path'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Link text'),
      '#default_value' => $this->options['text'],
      '#description' => t('The text of the link'),
    );
    $form['path'] = array(
      '#type' => 'textfield',
      '#title' => t('Link path'),
      '#default_value' => $this->options['path'],
      '#description' => t('The Drupal path or full URL to which to link'),
    );
  }

  function options_submit(&$form, &$form_state) {
    //$form_state['values']['options']['format'] = $form_state['values']['format'];
    parent::options_submit($form, $form_state);
  }

  function render($empty = FALSE) {
    if (!$empty || !empty($this->options['empty'])) {
      return l($this->options['text'], $this->options['path']);
    }
    return '';
  }
}
