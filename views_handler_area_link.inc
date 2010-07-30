<?php
// $Id$

class views_handler_area_link extends views_handler_area {

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    $options['path'] = array('default' => '');
    $options['querystring'] = array('default' => '');
    $options['anchor'] = array('default' => '');
    $options['class'] = array('default' => '');
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
    $form['query'] = array(
      '#type' => 'textfield',
      '#title' => t('Link querystring'),
      '#default_value' => $this->options['query'],
      '#description' => t('The query parameters that follow the full path'),
    );
    $form['anchor'] = array(
      '#type' => 'textfield',
      '#title' => t('Link anchor'),
      '#default_value' => $this->options['anchor'],
      '#description' => t('The anchor data that follows the full path and query parameters'),
    );
    $form['class'] = array(
      '#type' => 'textfield',
      '#title' => t('Link CSS class'),
      '#default_value' => $this->options['class'],
      '#description' => t('A custom CSS class to add to the link'),
    );


    $count = 0; // This lets us prepare the key as we want it printed.
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $options[t('Arguments')]['%' . ++$count] = t('@argument title', array('@argument' => $handler->ui_name()));
      $options[t('Arguments')]['!' . $count] = t('@argument input', array('@argument' => $handler->ui_name()));
    }

    if (!empty($options)) {
      $output = t('<p>The following substitution patterns are available for this display based on the arguments for this view. Use the pattern shown on the left to display the value indicated on the right.</p>');
      foreach (array_keys($options) as $type) {
        if (!empty($options[$type])) {
          $items = array();
          foreach ($options[$type] as $key => $value) {
            $items[] = $key . ' == ' . $value;
          }
          $output .= theme('item_list', $items, $type);
        }
      }

      // This construct uses 'hidden' and not markup because process doesn't
      // run. It also has an extra div because the dependency wants to hide
      // the parent in situations like this, so we need a second div to
      // make this work.
      $form['help'] = array(
        '#type' => 'hidden',
        '#id' => 'views-tokens-help',
        '#prefix' => '<div><fieldset id="views-tokens-help"><legend>' . t('Replacement patterns') . '</legend>' . $output . '</fieldset></div>',
        '#process' => array('views_process_dependency'),
      );
    }
  }

  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);
  }

  function render($empty = FALSE) {
    if (!$empty || !empty($this->options['empty'])) {

      $tokens = $this->get_render_tokens();
      $link_options = array();
      if (!empty($this->options['query'])) {
        $link_options['query'] = strtr($this->options['query'], $tokens);
      }
      if (!empty($this->options['anchor'])) {
        $link_options['anchor'] = strtr($this->options['anchor'], $tokens);
      }
      if (!empty($this->options['class'])) {
        $link_options['attributes'] = array('class' => strtr($this->options['class'], $tokens));
      }

      return l(strtr($this->options['text'], $tokens), strtr($this->options['path'], $tokens), $link_options);
    }
    return '';
  }

  // This is a shameless ripoff of views_handler_field's token rendering. Since
  // we're not a field, though, we only need arguments and links will always be
  // passing through XSS filtering. Makes things simpler.

  function get_render_tokens() {
    $tokens = array();
    if (!empty($this->view->build_info['substitutions'])) {
      $tokens = $this->view->build_info['substitutions'];
    }
    $count = 0;
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $token = '%' . ++$count;
      if (!isset($tokens[$token])) {
        $tokens[$token] = '';
      }

      $tokens['!' . $count] = isset($this->view->args[$count - 1]) ? check_plain($this->view->args[$count - 1]) : '';
    }
    return $tokens;
  }
}
