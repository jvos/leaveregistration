<?php
function leaveregistration_view($form, &$form_state, $user_id, $id)
{
  // get civicrm contact id from drupal user
  $session = CRM_Core_Session::singleton();
  $user_cid = $session->get('userID');  
  
  if(isset($_GET['cid']) and '' != $_GET['cid']){
    $cid = $_GET['cid'];
    
  }else {
    $cid = $user_cid;
  }
    
  $year = '';
  $years = array();  
  if(isset($_GET['year']) and '' != $_GET['year']){
    $year = $_GET['year'];
    $years[] = $_GET['year']-1;
    $years[] = $_GET['year'];
    
  }else {
    $year = date('Y');
    $years[] = date('Y')-1;
    $years[] = date('Y');
  } 
  
  $lrp = new CRM_Leaveregistration_Page_LeaveRegistration();
  $lrp->run(array(
    'error_id' => 'leaveregistration_view',
    'error_platform' => 'drupal', 
    'cid' => $cid,
    'user_cid' => $user_cid,
    'user_id' => $user_id,
    'years' => $years,
    'year' => $year,
    'months' => array(),
    'month' => 'all',
    'type' => 'form',
    'return' => 'return',
  ));
  
  $form['#attributes']['class'][] = 'leaveregistration';
  
  $form['css']['#markup'] = $lrp->css();
  $form['js']['#markup'] = $lrp->js();
  
  $form['display_name'] = array
  (
    '#markup' => '<h1>' . $lrp->get_display_name() . '</h1>'
  );
  
  $form['back'] = array
  (
    '#type' => 'link',
    '#title' => t('Back'),
    '#href' => '/user/' . $user_id . '/leave/overview/?year=' . $year . '&cid=' . $cid,
    '#options' => array('external' => TRUE),
    '#prefix' => '<div id="edit-back">',
    '#suffix' => '</div>'
  );
  
  $form['request'] = array
  (
    '#type' => 'fieldset',
    '#title' => t('View')
  );
       
  $form['request']['msg'] = array();
  $form['request']['msg']['#markup'] = implode('<br />', leaveregistration_body($lrp->lr, $lrp->lr->request[$id]));
    
  return $form;
}

/*
 * Create array with all the information of the request
 * Each key is one row, with explode it can create the body in html or plain text
 * Used in view, delete, mail and ical
 */
function leaveregistration_body($lr, $data){  
  $body = array(); 
    
  $body[] = t('Leave type: ') . t($lr->option_groups['leave_request_leave_type']['options'][$data['leave_type']]);
  
  switch($data['leave_type']){
    case 'time_for_time':
    case 'doctor_visit':
    case 'normal_leave_less_one_day':
    case 'sick_less_one_day':
      $body[] = t('Date: ') . strftime("%d-%m-%Y", strtotime($data['date']));
      $body[] = t('Duration: ') . $data['duration'];
      break;

    case 'mom_dad_day':
    case 'study_leave':
    case 'care':
      $body[] = t('From date: ') . strftime("%d-%m-%Y", strtotime($data['from_date']));
      $body[] = t('To date: ') . strftime("%d-%m-%Y", strtotime($data['to_date']));
            
      switch($data['daily_weekly_monthly_annually']){
        case 'daily':
          switch($data['daily_every_day_working_day']){
            case 'day':
              // Elke 5 dag(en)
              // Every 5 days
              $body[] = t('Every') . ' ' . t($data['daily_each']) .  ' ' . t('day(s)');
              break;
            case 'working_day':
              // Elke werkdag
              // Every working day
              $body[] = t('Every working day');
              break;
          }
          break;
        
        case 'weekly':
          // Elke 1 week/weken op maandag, dinsdag
          // Every 1 week / weeks on Monday, Tuesday
          $weekly_day_of_the_week = explode(CRM_Core_DAO::VALUE_SEPARATOR, $data['weekly_day_of_the_week']);
          
          // if the day is not checked than it has a value of 0
          // because of this, i have to remove the days with the value of 0          
          $weekly_day_of_the_week_clean = array();
          foreach($weekly_day_of_the_week as $key => $value){
            if(!empty($value)){
              $weekly_day_of_the_week_clean[] = t($value);
            }
          }
                    
          $body[] = t('Every') . ' ' . t($data['weekly_each']) . ' ' . t('week(s)') . ' ' . t('on') . ' ' . implode(', ', $weekly_day_of_the_week_clean);
          break;
        
        case 'monthly':
          switch($data['monthly_every_day_of_the_month_day_of_the_week']){
            case 'every_day_of_the_month':
              // Elke 1 maand(en) op dag 2
              // Every 1 month (s) on day 2
              $body[] = t('Every') . ' ' . t($data['monthly_each']) . ' ' . t('month(s)') . ' ' . t('on day') . ' ' . t($data['monthly_day_of_the_month']);
              break;
            case 'every_day_of_the_week':
              // Elke 2 maanden op eerste dinsdag
              // Every 2 months on the first Tuesday
              $body[] = t('Every') . ' ' . t($data['monthly_each']) . ' ' . t('month(s)') . ' ' . t('on the') . ' ' . t($data['monthly_on']) . ' ' . t($data['monthly_day_of_the_week']);
              break;
          }
          break;
        
        case 'annually':
          switch($data['annually_every_day_of_the_month_day_of_the_week']){
            case 'every_day_of_the_month':
              // Elke 2 jaar / jaren op september 2
              // Every 2 year(s) on September 2
              $body[] = t('Every') . ' ' . t($data['annually_each']) . ' ' . t('year(s)') . ' ' . t('on') . ' ' . t($data['annually_month']) . ' ' . t($data['annually_day_of_the_month']);
              break;
            case 'every_day_of_the_week':
              // Elke 2 jaar / jaren op de eerste dinsdag in september
              // Every 2 year(s) on the first Tuesday in September
              $body[] = t('Every') . ' ' . t($data['annually_each']) . ' ' . t('year(s)') . ' ' . t('on the') . ' ' . t($data['annually_on']) . ' ' . t($data['annually_day_of_the_week']) . ' ' . t('in') . ' ' . t($data['annually_month_of']);
              break;
          }
          break;
      }
      
      $body[] = t('Duration: ') . $data['duration'];
      break;

    default:
      $body[] = t('From date: ') . strftime("%d-%m-%Y", strtotime($data['from_date']));
      $body[] = t('To date: ') . strftime("%d-%m-%Y", strtotime($data['to_date']));
  }
  
  $body[] = t('The status of the leave request is: ') . t($lr->option_groups['leave_request_status']['options'][$data['status']]);
        
  if(!empty($data['reason'])){
    $body[] = t('Reason: ');
    // if the reason contains al end of line, it will not be displayed correctly in the iCal
    $reasons = explode(PHP_EOL, $data['reason']);
    foreach($reasons as $reason){
      $reason = str_replace(array(PHP_EOL, "\n\r", "\n", "\r"), '', $reason);
      $body[] = $reason;
    }
  }

  $body[] = '';
  
  return $body;
}

// change
function leaveregistration_request($form, &$form_state, $user_id, $id = 0)
{        
  // get civicrm contact id from drupal user
  $session = CRM_Core_Session::singleton();
  $user_cid = $session->get('userID');  
  
  if(isset($_GET['cid']) and '' != $_GET['cid']){
    $cid = $_GET['cid'];
    
  }else {
    $cid = $user_cid;
  }
  
  $years = array();  
  if(isset($_GET['year']) and '' != $_GET['year']){
    $year = $_GET['year'];
    $years[] = $_GET['year']-1;
    $years[] = $_GET['year'];
    
  }
    
  $lrp = new CRM_Leaveregistration_Page_LeaveRegistration();
  $lrp->run(array(
    'error_id' => 'leaveregistration_request',
    'error_platform' => 'drupal', 
    'cid' => $cid,
    'user_cid' => $user_cid,
    'user_id' => $user_id,
    'years' => $years,
    'year' => $year,
    'months' => array(),
    'month' => 'all',
    'type' => 'form',
    'return' => 'return',
  ));
  
  $form['#attributes']['class'][] = 'leaveregistration';
  
  $form['css']['#markup'] = $lrp->css();
  $form['js']['#markup'] = $lrp->js();
  
  $form['#validate'][] = 'leaveregistration_request_validate';
  $form['#submit'][] = 'leaveregistration_request_submit';
      
  $form['display_name'] = array
  (
    '#markup' => '<h1>' . $lrp->get_display_name() . '</h1>'
  );
  
  $form['back'] = array
  (
    '#type' => 'link',
    '#title' => t('Back'),
    '#href' => '/user/' . $user_id . '/leave/overview/?year=' . $year . '&cid=' . $cid,
    '#options' => array('external' => TRUE),
    '#prefix' => '<div id="edit-back">',
    '#suffix' => '</div>'
  );
  
  
  $form['request'] = array
  (
    '#type' => 'fieldset',
    '#title' => t('Request')
  );
  
  // if the id is empty
  if(!empty($id)){
    $request = $lrp->lr->request[$id];
    
    if(!isset($form_state['values']) and empty($form_state['values'])){

      $form_state['values']['leave_type'] = $request['leave_type'];
      $form_state['values']['reason'] = $request['reason'];
      $form_state['values']['status'] = $request['status'];

      $form_state['values']['from_date'] = $request['from_date'];
      $form_state['values']['to_date'] = $request['to_date'];

      $form_state['values']['date'] = $request['date'];
      
      $form_state['values']['daily_weekly_monthly_annually'] = $request['daily_weekly_monthly_annually'];
      
      $form_state['values']['daily_every_day_working_day'] = $request['daily_every_day_working_day'];
      $form_state['values']['daily_each'] = $request['daily_each'];
      
      $form_state['values']['weekly_each'] = $request['weekly_each'];
      $form_state['values']['weekly_day_of_the_week'] = explode(CRM_Core_DAO::VALUE_SEPARATOR, $request['weekly_day_of_the_week']);
      
      $form_state['values']['monthly_each'] = $request['monthly_each'];
      $form_state['values']['monthly_every_day_of_the_month_day_of_the_week'] = $request['monthly_every_day_of_the_month_day_of_the_week'];
      $form_state['values']['monthly_day_of_the_month'] = $request['monthly_day_of_the_month'];
      $form_state['values']['monthly_on'] = $request['monthly_on'];
      $form_state['values']['monthly_day_of_the_week'] = $request['monthly_day_of_the_week'];
      
      $form_state['values']['annually_each'] = $request['annually_each'];
      $form_state['values']['annually_every_day_of_the_month_day_of_the_week'] = $request['annually_every_day_of_the_month_day_of_the_week'];
      $form_state['values']['annually_month'] = $request['annually_month'];
      $form_state['values']['annually_day_of_the_month'] = $request['annually_day_of_the_month'];
      $form_state['values']['annually_on'] = $request['annually_on'];
      $form_state['values']['annually_day_of_the_week'] = $request['annually_day_of_the_week'];
      $form_state['values']['annually_month_of'] = $request['annually_month_of'];
      
      $form_state['values']['duration'] = $request['duration'];
    }
  }
    
  if(!isset($form_state['values']['from_date']) or empty($form_state['values']['from_date'])){
    $form_state['values']['from_date'] = $year . '-' . date('m-d') . ' 00:00:00';
  }
  if(!isset($form_state['values']['to_date']) or empty($form_state['values']['to_date'])){
    $form_state['values']['to_date'] = $year . '-' . date('m-d') . ' 00:00:00';
  }
  if(!isset($form_state['values']['date']) or empty($form_state['values']['date'])){
    $form_state['values']['date'] = $year . '-' . date('m-d') . ' 00:00:00';
  }
  
  if(!isset($form_state['values']['daily_weekly_monthly_annually']) or empty($form_state['values']['daily_weekly_monthly_annually'])){
    $form_state['values']['daily_weekly_monthly_annually'] = $lrp->lr->custom_fields['leave_request_daily_weekly_monthly_annually']['default_value'];
  }
  
  // daily
  if(!isset($form_state['values']['daily_every_day_working_day']) or empty($form_state['values']['daily_every_day_working_day'])){
    $form_state['values']['daily_every_day_working_day'] = $lrp->lr->custom_fields['leave_request_daily_every_day_working_day']['default_value'];
  }
  if(!isset($form_state['values']['daily_each']) or empty($form_state['values']['daily_each'])){
    $form_state['values']['daily_each'] = $lrp->lr->custom_fields['leave_request_daily_each']['default_value'];
  }
  
  // weekly
  if(!isset($form_state['values']['weekly_each']) or empty($form_state['values']['weekly_each'])){
    $form_state['values']['weekly_each'] = $lrp->lr->custom_fields['leave_request_weekly_each']['default_value'];
  }
  if(!isset($form_state['values']['weekly_day_of_the_week']) or empty($form_state['values']['weekly_day_of_the_week'])){
    $form_state['values']['weekly_day_of_the_week'] = $lrp->lr->custom_fields['leave_request_weekly_day_of_the_week']['default_value'];
  }
  
  // monthly
  if(!isset($form_state['values']['monthly_each']) or empty($form_state['values']['monthly_each'])){
    $form_state['values']['monthly_each'] = $lrp->lr->custom_fields['leave_request_monthly_each']['default_value'];
  }
  if(!isset($form_state['values']['monthly_every_day_of_the_month_day_of_the_week']) or empty($form_state['values']['monthly_every_day_of_the_month_day_of_the_week'])){
    $form_state['values']['monthly_every_day_of_the_month_day_of_the_week'] = $lrp->lr->custom_fields['leave_request_monthly_every_day_of_the_month_day_of_the_week']['default_value'];
  }
  if(!isset($form_state['values']['monthly_day_of_the_month']) or empty($form_state['values']['monthly_day_of_the_month'])){
    $form_state['values']['monthly_day_of_the_month'] = $lrp->lr->custom_fields['leave_request_monthly_day_of_the_month']['default_value'];
  }
  if(!isset($form_state['values']['monthly_on']) or empty($form_state['values']['monthly_on'])){
    $form_state['values']['monthly_on'] = $lrp->lr->custom_fields['leave_request_monthly_on']['default_value'];
  }
  if(!isset($form_state['values']['monthly_day_of_the_week']) or empty($form_state['values']['monthly_day_of_the_week'])){
    $form_state['values']['monthly_day_of_the_week'] = $lrp->lr->custom_fields['leave_request_monthly_day_of_the_week']['default_value'];
  }
  
  // annually
  if(!isset($form_state['values']['annually_each']) or empty($form_state['values']['annually_each'])){
    $form_state['values']['annually_each'] = $lrp->lr->custom_fields['leave_request_annually_each']['default_value'];
  }
  if(!isset($form_state['values']['annually_every_day_of_the_month_day_of_the_week']) or empty($form_state['values']['annually_every_day_of_the_month_day_of_the_week'])){
    $form_state['values']['annually_every_day_of_the_month_day_of_the_week'] = $lrp->lr->custom_fields['leave_request_annually_every_day_of_the_month_day_of_the_week']['default_value'];
  }
  if(!isset($form_state['values']['annually_month']) or empty($form_state['values']['annually_month'])){
    $form_state['values']['annually_month'] = $lrp->lr->custom_fields['leave_request_annually_month']['default_value'];
  }
  if(!isset($form_state['values']['annually_day_of_the_month']) or empty($form_state['values']['annually_day_of_the_month'])){
    $form_state['values']['annually_day_of_the_month'] = $lrp->lr->custom_fields['leave_request_annually_day_of_the_month']['default_value'];
  }
  if(!isset($form_state['values']['annually_on']) or empty($form_state['values']['annually_on'])){
    $form_state['values']['annually_on'] = $lrp->lr->custom_fields['leave_request_annually_on']['default_value'];
  }
  if(!isset($form_state['values']['annually_day_of_the_week']) or empty($form_state['values']['annually_day_of_the_week'])){
    $form_state['values']['annually_day_of_the_week'] = $lrp->lr->custom_fields['leave_request_annually_day_of_the_week']['default_value'];
  }
  if(!isset($form_state['values']['annually_month_of']) or empty($form_state['values']['annually_month_of'])){
    $form_state['values']['annually_month_of'] = $lrp->lr->custom_fields['leave_request_annually_month_of']['default_value'];
  }
  
  if(!isset($form_state['values']['duration']) or empty($form_state['values']['duration'])){
    $form_state['values']['duration'] = $lrp->lr->custom_fields['leave_request_duration']['default_value'];
  }
    
  // hidden form elements
  $form['request']['id'] = array('#type' => 'hidden','#value' => $id);
  $form['request']['cid'] = array('#type' => 'hidden','#value' => $cid);   
  $form['request']['user_id'] = array('#type' => 'hidden','#value' => $user_id);   
  $form['request']['user_cid'] = array('#type' => 'hidden','#value' => $user_cid);   
  $form['request']['year'] = array('#type' => 'hidden','#value' => $year);
  
  
  $option_groups = array();
    
  foreach($lrp->lr->option_groups as $name => $option_group){
    $option_groups[$name] = array();
    // translate option group
    foreach($option_group['options'] as $value => $label){
      $option_groups[$name][$value] = t($label);
    }
  }
  
  $form['request']['leave_type'] = array
  (
    '#type' => 'select',
    '#title' => t('Leave type'),
    '#options' => $option_groups['leave_request_leave_type'],
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['leave_type']) and !empty($form_state['values']['leave_type'])){
    $form['request']['leave_type']['#default_value'] = $form_state['values']['leave_type'];
  }
  
  $form['request']['reason'] = array
  (
    '#type' => 'textarea',
    '#title' => t('Reason'),
    '#required' => FALSE,
  );
  
  if(isset($form_state['values']['reason']) and !empty($form_state['values']['reason'])){
    $form['request']['reason']['#default_value'] = $form_state['values']['reason'];
  }
  
  if($lrp->lr->is_department_head[$user_cid]){
    $form['request']['status'] = array
    (
      '#type' => 'select',
      '#title' => t('Status'),
      '#options' => $option_groups['leave_request_status'],
      '#required' => TRUE,
    );
    
    if(isset($form_state['values']['status']) and !empty($form_state['values']['status'])){
      $form['request']['status']['#default_value'] = $form_state['values']['status'];
    }
    
  }else {
    $form['request']['status'] = array
    (
      '#type' => 'hidden',
      '#value' => 'request',
      '#required' => TRUE,
    );
    
    if(isset($form_state['values']['status']) and !empty($form_state['values']['status'])){
      $form['request']['status']['#default_value'] = $form_state['values']['status'];
    }
  }
  
  $form['request']['from_date'] = array
  (
    '#type' => 'date_popup',
    '#date_format' => 'j-F-Y', // Uses the PHP date() format - http://php.net/manual/en/function.date.php
    '#date_year_range' => '-5:+2', // Limits the year range to the next two upcoming years
    
    '#title' => t('From date'),
    '#prefix' => '<div id="div-from_date">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['from_date']) and !empty($form_state['values']['from_date'])){
    $form['request']['from_date']['#default_value'] = $form_state['values']['from_date'];
  }
  
  $form['request']['to_date'] = array
  (
    '#type' => 'date_popup',
    '#date_format' => 'j-F-Y', // Uses the PHP date() format - http://php.net/manual/en/function.date.php
    '#date_year_range' => '-5:+2', // Limits the year range to the next two upcoming years
    
    '#title' => t('To date'),
    '#prefix' => '<div id="div-to_date">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['to_date']) and !empty($form_state['values']['to_date'])){
    $form['request']['to_date']['#default_value'] = $form_state['values']['to_date'];
  }
  
  $form['request']['date'] = array
  (
    '#type' => 'date_popup',
    '#date_format' => 'j-F-Y', // Uses the PHP date() format - http://php.net/manual/en/function.date.php
    '#date_year_range' => '-5:+2', // Limits the year range to the next two upcoming years
    
    '#title' => t('Date'),
    '#prefix' => '<div id="div-date">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['date']) and !empty($form_state['values']['date'])){
    $form['request']['date']['#default_value'] = $form_state['values']['date'];
  }
  
  $form['request']['daily_weekly_monthly_annually'] = array
  (
    '#type' => 'radios',
    '#title' => t('Daily / Weekly / Monthly / Annually'),
    '#options' => $option_groups['leave_request_daily_weekly_monthly_annually'],
    '#prefix' => '<div id="div-daily_weekly_monthly_annually">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['daily_weekly_monthly_annually']) and !empty($form_state['values']['daily_weekly_monthly_annually'])){
    $form['request']['daily_weekly_monthly_annually']['#default_value'] = $form_state['values']['daily_weekly_monthly_annually'];
  }
  
  // daily
  $form['request']['daily'] = array
  (
    '#type' => 'fieldset', 
    '#title' => t('Daily'), 
  );
  
  $form['request']['daily']['daily_every_day_working_day'] = array
  (
    '#type' => 'radios',
    '#title' => t('Every day / Working day'),
    '#options' => $option_groups['leave_request_daily_every_day_working_day'],
    '#prefix' => '<div id="div-daily_every_day_working_day">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['daily_every_day_working_day']) and !empty($form_state['values']['daily_every_day_working_day'])){
    $form['request']['daily']['daily_every_day_working_day']['#default_value'] = $form_state['values']['daily_every_day_working_day'];
  }
    
  $form['request']['daily']['daily_each'] = array
  (
    '#type' => 'textfield',
    '#title' => t('Each'),
    '#prefix' => '<div id="div-daily_each">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['daily_each']) and !empty($form_state['values']['daily_each'])){
    $form['request']['daily']['daily_each']['#default_value'] = $form_state['values']['daily_each'];
  }
  
  // weekly
  $form['request']['weekly'] = array
  (
    '#type' => 'fieldset', 
    '#title' => t('Weekly'), 
  );
  
  $form['request']['weekly']['weekly_each'] = array
  (
    '#type' => 'textfield',
    '#title' => t('Each'),
    '#prefix' => '<div id="div-weekly_each">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['weekly_each']) and !empty($form_state['values']['weekly_each'])){
    $form['request']['weekly']['weekly_each']['#default_value'] = $form_state['values']['weekly_each'];
  }
  
  $form['request']['weekly']['weekly_day_of_the_week'] = array
  (
    '#type' => 'checkboxes',
    '#title' => t('Day of the week'),
    '#options' => $option_groups['leave_request_weekly_day_of_the_week'],
    '#prefix' => '<div id="div-weekly_day_of_the_week">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['weekly_day_of_the_week']) and !empty($form_state['values']['weekly_day_of_the_week'])){
    $form['request']['weekly']['weekly_day_of_the_week']['#default_value'] = $form_state['values']['weekly_day_of_the_week'];
  }
  
  // monthly
  $form['request']['monthly'] = array
  (
    '#type' => 'fieldset', 
    '#title' => t('Monthly'), 
  );
  
  $form['request']['monthly']['monthly_each'] = array
  (
    '#type' => 'textfield',
    '#title' => t('Each'),
    '#prefix' => '<div id="div-monthly_each">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['monthly_each']) and !empty($form_state['values']['monthly_each'])){
    $form['request']['monthly']['monthly_each']['#default_value'] = $form_state['values']['monthly_each'];
  }
    
  $form['request']['monthly']['monthly_every_day_of_the_month_day_of_the_week'] = array
  (
    '#type' => 'radios',
    '#title' => t('Every day / Day of the week'),
    '#options' => $option_groups['leave_request_monthly_annually_every_day_of_the_month_day_of_the'],
    '#prefix' => '<div id="div-monthly_every_day_of_the_month_day_of_the_week">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['monthly_every_day_of_the_month_day_of_the_week']) and !empty($form_state['values']['monthly_every_day_of_the_month_day_of_the_week'])){
    $form['request']['monthly']['monthly_every_day_of_the_month_day_of_the_week']['#default_value'] = $form_state['values']['monthly_every_day_of_the_month_day_of_the_week'];
  }
  
  $form['request']['monthly']['monthly_day_of_the_month'] = array
  (
    '#type' => 'textfield',
    '#title' => t('Every day'),
    '#prefix' => '<div id="div-monthly_day_of_the_month">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['monthly_day_of_the_month']) and !empty($form_state['values']['monthly_day_of_the_month'])){
    $form['request']['monthly']['monthly_day_of_the_month']['#default_value'] = $form_state['values']['monthly_day_of_the_month'];
  }
  
  $form['request']['monthly']['monthly_on'] = array
  (
    '#type' => 'select',
    '#title' => t('On'),
    '#options' => $option_groups['leave_request_monthly_annually_on'],
    '#prefix' => '<div id="div-monthly_on">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['monthly_on']) and !empty($form_state['values']['monthly_on'])){
    $form['request']['monthly']['monthly_on']['#default_value'] = $form_state['values']['monthly_on'];
  }
  
  $form['request']['monthly']['monthly_day_of_the_week'] = array
  (
    '#type' => 'select',
    '#title' => t('Day of the week'),
    '#options' => $option_groups['leave_request_monthly_annually_day_of_the_week'],
    '#prefix' => '<div id="div-monthly_day_of_the_week">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['monthly_day_of_the_week']) and !empty($form_state['values']['monthly_day_of_the_week'])){
    $form['request']['monthly']['monthly_day_of_the_week']['#default_value'] = $form_state['values']['monthly_day_of_the_week'];
  }
  
  // annually
  $form['request']['annually'] = array
  (
    '#type' => 'fieldset', 
    '#title' => t('Annually'), 
  );
  
  $form['request']['annually']['annually_each'] = array
  (
    '#type' => 'textfield',
    '#title' => t('Each'),
    '#prefix' => '<div id="div-annually_each">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['annually_each']) and !empty($form_state['values']['annually_each'])){
    $form['request']['annually']['annually_each']['#default_value'] = $form_state['values']['annually_each'];
  }
  
  $form['request']['annually']['annually_every_day_of_the_month_day_of_the_week'] = array
  (
    '#type' => 'radios',
    '#title' => t('Every day / day of the week of the month'),
    '#options' => $option_groups['leave_request_monthly_annually_every_day_of_the_month_day_of_the'],
    '#prefix' => '<div id="div-annually_every_day_of_the_month_day_of_the_week">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['annually_every_day_of_the_month_day_of_the_week']) and !empty($form_state['values']['annually_every_day_of_the_month_day_of_the_week'])){
    $form['request']['annually']['annually_every_day_of_the_month_day_of_the_week']['#default_value'] = $form_state['values']['annually_every_day_of_the_month_day_of_the_week'];
  }
  
  $form['request']['annually']['annually_month'] = array
  (
    '#type' => 'select',
    '#title' => t('Month'),
    '#options' => $option_groups['leave_request_annually_month'],
    '#prefix' => '<div id="div-annually_month">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['annually_month']) and !empty($form_state['values']['annually_month'])){
    $form['request']['annually']['annually_month']['#default_value'] = $form_state['values']['annually_month'];
  }
  
  $form['request']['annually']['annually_day_of_the_month'] = array
  (
    '#type' => 'textfield',
    '#title' => t('Every day'),
    '#prefix' => '<div id="div-annually_day_of_the_month">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['annually_day_of_the_month']) and !empty($form_state['values']['annually_day_of_the_month'])){
    $form['request']['annually']['annually_day_of_the_month']['#default_value'] = $form_state['values']['annually_day_of_the_month'];
  }
  
  $form['request']['annually']['annually_on'] = array
  (
    '#type' => 'select',
    '#title' => t('On'),
    '#options' => $option_groups['leave_request_monthly_annually_on'],
    '#prefix' => '<div id="div-monthly_annually_on">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['annually_on']) and !empty($form_state['values']['annually_on'])){
    $form['request']['annually']['annually_on']['#default_value'] = $form_state['values']['annually_on'];
  }
    
  $form['request']['annually']['annually_day_of_the_week'] = array
  (
    '#type' => 'select',
    '#title' => t('Day of the week'),
    '#options' => $option_groups['leave_request_monthly_annually_day_of_the_week'],
    '#prefix' => '<div id="div-annually_day_of_the_week">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['annually_day_of_the_week']) and !empty($form_state['values']['annually_day_of_the_week'])){
    $form['request']['annually']['annually_day_of_the_week']['#default_value'] = $form_state['values']['annually_day_of_the_week'];
  }
  
  $form['request']['annually']['annually_month_of'] = array
  (
    '#type' => 'select',
    '#title' => t('Maand'),
    '#options' => $option_groups['leave_request_annually_month'],
    '#prefix' => '<div id="div-annually_month_of">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['annually_month_of']) and !empty($form_state['values']['annually_month_of'])){
    $form['request']['annually']['annually_month_of']['#default_value'] = $form_state['values']['annually_month_of'];
  }
  
  $form['request']['duration'] = array
  (
    '#type' => 'textfield',
    '#title' => t('Duration'),
    '#description' => t('Hours and minutes seperated by :, like 0:00.'),
    '#default_value' => '0:00',
    '#prefix' => '<div id="div-duration">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['duration']) and !empty($form_state['values']['duration'])){
    $form['request']['duration']['#default_value'] = $form_state['values']['duration'];
  }
  
  if(!empty($id)){
    $form['request']['submit'] = array
    (
      '#type' => 'submit',
      '#value' => t('Change')
    );
    
  }else {
    $form['request']['submit'] = array
    (
      '#type' => 'submit',
      '#value' => t('Request')
    );
  }
  
  $form['year']['#markup'] = $lrp->get_year_form();
  $form['request']['#markup'] = $lrp->get_request_form();
  $form['legend']['#markup'] = $lrp->get_legend_form();
  $form['credit']['#markup'] = $lrp->get_credit_form();
  $form['dephead_months']['#markup'] = $lrp->get_dephead_months_form();
  $form['dephead_show_colleagues']['#markup'] = $lrp->get_dephead_show_colleagues_form();
  $form['dephead_calendar_year']['#markup'] = $lrp->get_dephead_calendar_year_form();
  $form['months']['#markup'] = $lrp->get_months_form();
  $form['show_colleagues']['#markup'] = $lrp->get_show_colleagues_form();
  $form['calendar_year']['#markup'] = $lrp->get_calendar_year_form();

  $form['js']['#markup'] .= <<<JS
    <script type="text/javascript">
      (function($) {
        $(document).ready(function() {
              
          // leave type
          $('#edit-leave-type').bind('change', function () {
            leaveregistration_request_change();
          });
          
          // daily weekly monthly annually          
          $('input[name=daily_weekly_monthly_annually]').bind('change', function () {
            leaveregistration_request_change();
          });  
            
          // daily every day working day
          $('input[name=daily_every_day_working_day]').bind('change', function () {
            leaveregistration_request_change();
          });
                    
          // monthly every day day of the week
          $('input[name=monthly_every_day_of_the_month_day_of_the_week]').bind('change', function () {
            leaveregistration_request_change();
          });
          
          // annually every day day of the week
          $('input[name=annually_every_day_of_the_month_day_of_the_week]').bind('change', function () {
            leaveregistration_request_change();
          });
          
          leaveregistration_request_change();
          
          // show or hide fields
          function leaveregistration_request_change(){
                      
            // hide / show
            switch ($('#edit-leave-type').val()) {
              case '':
                $('#div-from_date').hide();
                $('#div-to_date').hide();

                $('#div-date').hide();
                $('#div-duration').hide();

                $('#div-daily_weekly_monthly_annually').hide();
                $('#edit-daily').hide();
                $('#edit-weekly').hide();
                $('#edit-monthly').hide();
                $('#edit-annually').hide(); 
                break; 
              
              case 'time_for_time':
              case 'doctor_visit':
              case 'normal_leave_less_one_day':
              case 'sick_less_one_day':
                $('#div-from_date').hide();
                $('#div-to_date').hide();

                $('#div-date').show();
                $('#div-duration').show();

                $('#div-daily_weekly_monthly_annually').hide();
                $('#edit-daily').hide();
                $('#edit-weekly').hide();
                $('#edit-monthly').hide();
                $('#edit-annually').hide();
                break; 
                
              case 'mom_dad_day':
              case 'study_leave':
              case 'care':
                $('#div-from_date').show();
                $('#div-to_date').show();

                $('#div-date').hide();
                $('#div-duration').show();

                $('#div-daily_weekly_monthly_annually').show();
                $('#edit-daily').show();
                $('#edit-weekly').show();
                $('#edit-monthly').show();
                $('#edit-annually').show();
                
                switch ($('input[name=daily_weekly_monthly_annually]:checked').val()) {
                  case '':
                    $('#edit-daily').hide();
                    $('#edit-weekly').hide();
                    $('#edit-monthly').hide();
                    $('#edit-annually').hide(); 
                    break;

                  case 'daily':
                    $('#edit-daily').show();
                    $('#edit-weekly').hide();
                    $('#edit-monthly').hide();
                    $('#edit-annually').hide();
          
                    switch ($('input[name=daily_every_day_working_day]:checked').val()) {
                      case 'day':
                        $('#div-daily_each').show();
                        break;
                      
                      default:
                        $('#div-daily_each').hide();
                    }                       
                    break;

                  case 'weekly':
                    $('#edit-daily').hide();
                    $('#edit-weekly').show();
                    $('#edit-monthly').hide();
                    $('#edit-annually').hide();                
                    break;

                  case 'monthly':
                    $('#edit-daily').hide();
                    $('#edit-weekly').hide();
                    $('#edit-monthly').show();
                    $('#edit-annually').hide();
                    
                    switch ($('input[name=monthly_every_day_of_the_month_day_of_the_week]:checked').val()) {
                      case '':
                        $('#div-monthly_day_of_the_month').hide();
                        $('#div-monthly_on').hide();
                        $('#div-monthly_day_of_the_week').hide();
                        break;
          
                       case 'every_day_of_the_month':
                        $('#div-monthly_day_of_the_month').show();
                        $('#div-monthly_on').hide();
                        $('#div-monthly_day_of_the_week').hide();
                        break;
          
                      case 'every_day_of_the_week':
                        $('#div-monthly_day_of_the_month').hide();
                        $('#div-monthly_on').show();
                        $('#div-monthly_day_of_the_week').show();
                        break;
                    }     
                    break;

                  case 'annually':
                    $('#edit-daily').hide();
                    $('#edit-weekly').hide();
                    $('#edit-monthly').hide();
                    $('#edit-annually').show();
          
                    switch ($('input[name=annually_every_day_of_the_month_day_of_the_week]:checked').val()) {
                      case '':
                        $('#div-annually_month').hide();
                        $('#div-annually_day_of_the_month').hide();
                        $('#div-monthly_annually_on').hide();
                        $('#div-annually_day_of_the_week').hide();
                        $('#div-annually_month_of').hide();
                        break;
          
                       case 'every_day_of_the_month':
                        $('#div-annually_month').show();
                        $('#div-annually_day_of_the_month').show();
                        $('#div-monthly_annually_on').hide();
                        $('#div-annually_day_of_the_week').hide();
                        $('#div-annually_month_of').hide();
                        break;
          
                      case 'every_day_of_the_week':
                        $('#div-annually_month').hide();
                        $('#div-annually_day_of_the_month').hide();
                        $('#div-monthly_annually_on').show();
                        $('#div-annually_day_of_the_week').show();
                        $('#div-annually_month_of').show();
                        break;
                    } 
                    break;
                }
                break;
                
              default: 
                $('#div-from_date').show();
                $('#div-to_date').show();

                $('#div-date').hide();
                $('#div-duration').hide();

                $('#div-daily_weekly_monthly_annually').hide();
                $('#edit-daily').hide();
                $('#edit-weekly').hide();
                $('#edit-monthly').hide();
                $('#edit-annually').hide();
            } 
          }
          
          var obj = document.getElementById("edit-from-date-datepicker-popup-0");
          obj.onchange = function() {
            $('#edit-to-date-datepicker-popup-0').val( $('#edit-from-date-datepicker-popup-0').val() );
          };
      
        });
      })(jQuery_leave_registration);
    </script>
JS;
  
  return $form;
}

function leaveregistration_request_validate($form, &$form_state){
  
  $lr = new leaveregistration('drupal', 'leaveregistration_request_validate');
  $lr->set_fields();
  $lr->set_contacts(array($form_state['values']['cid'], $form_state['values']['user_cid']));
  
  // get all the years, for validation
  switch($form_state['values']['leave_type']){
    case 'time_for_time':
    case 'doctor_visit':
    case 'normal_leave_less_one_day':
    case 'sick_less_one_day':
      $timestamp = strtotime($form_state['values']['date']);
          
      $year = date('Y', $timestamp);
      $month = date('m', $timestamp);
      $day = date('d', $timestamp);

      $years[$year] = $year;
      break;
    default: 
      // to date is lower than from date
      if($form_state['values']['from_date'] > $form_state['values']['to_date']){
        form_set_error('to_date', t('To date must be higher than from date !'));
      }
      
      $from_date_totime = strtotime($form_state['values']['from_date']);
      $to_date_totime = strtotime($form_state['values']['to_date']);
      
      for($timestamp = $from_date_totime; $timestamp <= $to_date_totime; $timestamp = strtotime( '+1 year' , $timestamp )){ 
        $year = date('Y', $timestamp);
        $month = date('m', $timestamp);
        $day = date('d', $timestamp);
        $years[$year] = $year;
      }
      break;
  }  
  
  $lr->set_data($years); 
  
  // all the colleages
  $colleages = array();
  switch ($lr->settings[$form_state['values']['cid']]['show_all_colleagues']){
    case 'business':
      foreach($lr->business_colleages_ids[$form_state['values']['cid']] as $bid => $business){
        foreach($business['employees'] as $cid => $employee){
          if($cid != $form_state['values']['cid']){
            $colleages[htmlentities($employee['display_name'])] = $employee;
          }
        }
      }
      break;
    /*case 'main_business':
      foreach($this->lr->department_heads_colleages_ids[$this->data['user_cid']] as $did => $department){
        foreach($department['employees'] as $cid => $employee){
          if($cid != $this->data['user_cid']){
            $colleages[htmlentities($employee['display_name'])] = $employee;
          }
        }
      }
      break;*/
    default:
      foreach($lr->department_colleages_ids[$form_state['values']['cid']] as $did => $department){
        foreach($department['employees'] as $cid => $employee){
          if($cid != $form_state['values']['cid']){
            $colleages[htmlentities($employee['display_name'])] = $employee;
          }
        }
      }
  }
  
  // all the department heads
  if($lr->settings[$form_state['values']['cid']]['show_department_head']){
    foreach ($lr->department_heads[$form_state['values']['cid']] as $dhid => $department_head){
      if($dhid != $form_state['values']['cid']){
        $colleages[htmlentities($department_head['display_name'])] = $department_head;
      }
    }
  }
  
  $colids = array();      
  ksort($colleages);
  foreach($colleages as $display_name => $employee){
    $colids[] = $employee['id'];
  }      
  
  if(!empty($colids)){
    $lrcol = new leaveregistration('drupal', 'leaveregistration_request_validate colids');
    $lrcol->set_fields();
    $lrcol->set_contacts($colids);
    $lrcol->set_data(array($form_state['values']['year']));
  }
  
  $errors = array();
    
  // validate input
  switch($form_state['values']['leave_type']){
    case 'time_for_time':
    case 'doctor_visit':
    case 'normal_leave_less_one_day':
    case 'sick_less_one_day':
      $form_state['values']['duration'] = trim($form_state['values']['duration']);
    
      // if it is a whole number    
      if(preg_match('/^[0-9]{1,2}$/', $form_state['values']['duration'], $matches)){
        $form_state['values']['duration'] .= ':00'; 
      }

      // if it start with 0
      if('0' == substr($form_state['values']['duration'], 0, 1) and '0' == substr($form_state['values']['duration'], 1, 2)){
        $form_state['values']['duration'] = substr($form_state['values']['duration'], 1, strlen($form_state['values']['duration']));
      }

      // if 0:00 or 00:00
      if(!preg_match('/^[0-9]{1,2}[:]{1}[0-9]{2}$/', $form_state['values']['duration'], $matches)){
        form_set_error('duration', t('Duration has wrong value !'));
      }

      // if 0:00
      if('0:00' == $form_state['values']['duration']){
        form_set_error('duration', t('Duration can`t be zero !'));
      }

      $form_state['values']['from_date'] = '1970-01-01 00:00:00'; // 1970-01-01 00:00:00
      $form_state['values']['to_date'] = '1970-01-01 00:00:00'; // 1970-01-01 00:00:00

      $form_state['values']['daily_weekly_monthly_annually'] = $lr->custom_fields['leave_request_daily_weekly_monthly_annually']['default_value'];
    
      $form_state['values']['daily_every_day_working_day'] = $lr->custom_fields['leave_request_daily_every_day_working_day']['default_value'];
      $form_state['values']['daily_each'] = $lr->custom_fields['leave_request_daily_each']['default_value'];

      $form_state['values']['weekly_each'] = $lr->custom_fields['leave_request_weekly_each']['default_value'];
      $form_state['values']['weekly_day_of_the_week'] = implode(CRM_Core_DAO::VALUE_SEPARATOR, $lr->custom_fields['leave_request_weekly_day_of_the_week']['default_value']);

      $form_state['values']['monthly_each'] = $lr->custom_fields['leave_request_monthly_each']['default_value'];
      $form_state['values']['monthly_every_day_of_the_month_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_every_day_of_the_month_day_of_the_week']['default_value'];
      $form_state['values']['monthly_day_of_the_month'] = $lr->custom_fields['leave_request_monthly_day_of_the_month']['default_value'];
      $form_state['values']['monthly_on'] = $lr->custom_fields['leave_request_monthly_on']['default_value'];
      $form_state['values']['monthly_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_day_of_the_week']['default_value'];

      $form_state['values']['annually_each'] = $lr->custom_fields['leave_request_annually_each']['default_value'];
      $form_state['values']['annually_every_day_of_the_month_day_of_the_week'] = $lr->custom_fields['leave_request_annually_every_day_of_the_month_day_of_the_week']['default_value'];
      $form_state['values']['annually_month'] = $lr->custom_fields['leave_request_annually_month']['default_value'];
      $form_state['values']['annually_day_of_the_month'] = $lr->custom_fields['leave_request_annually_day_of_the_month']['default_value'];
      $form_state['values']['annually_on'] = $lr->custom_fields['leave_request_annually_on']['default_value'];
      $form_state['values']['annually_day_of_the_week'] = $lr->custom_fields['leave_request_annually_day_of_the_week']['default_value'];
      $form_state['values']['annually_month_of'] = $lr->custom_fields['leave_request_annually_month_of']['default_value'];
      
      // check already request
      $timestamp = strtotime($form_state['values']['date']);
          
      $year = date('Y', $timestamp);
      $month = date('m', $timestamp);
      $day = date('d', $timestamp);
      
      list($hours, $minutes) = explode(':', $form_state['values']['duration']);
      $duration = ($hours * 60) + $minutes;
      
      // see leaveregistration.class.php function set_request
      if('time_for_time' == $form_state['values']['leave_type']){
        // already time for time exists
        if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['time_for_time']['leave_type']) and $form_state['values']['id'] != $lr->data[$form_state['values']['cid']][$year][$month][$day]['time_for_time']['id']){ // if leave_type exists
          $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have ') . strtolower(t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['time_for_time']['leave_type']])) . ' !';
        }
      
      }else if('normal_leave_less_one_day' == $form_state['values']['leave_type']){
        
        // is holiday
        if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['holiday']) and $lr->data[$form_state['values']['cid']][$year][$month][$day]['holiday']['is_holiday']){
          $errors['request'][] = t('The request is on a holiday !');
          
        // is not holiday
        }else {
          
          // is adjustments
          $duration = 0;
          if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['adjustments']['duration']) and '' !== $lr->data[$form_state['values']['cid']][$year][$month][$day]['adjustments']['duration']){
            $duration = $lr->data[$form_state['values']['cid']][$year][$month][$day]['adjustments']['duration'];

          }else if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['overall_adjustments']['duration']) and '' !== $lr->data[$form_state['values']['cid']][$year][$month][$day]['overall_adjustments']['duration']){
            $duration = $lr->data[$form_state['values']['cid']][$year][$month][$day]['overall_adjustments']['duration'];

          }else if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['general']['duration']) and '' !== $lr->data[$form_state['values']['cid']][$year][$month][$day]['general']['duration']){
            $duration = $lr->data[$form_state['values']['cid']][$year][$month][$day]['general']['duration'];
          }
          
          // if a adjustments
          if(0 == $duration){
            $errors['request'][] = t('You do not work on that day !');
            
          }else {  
            // already exists, normal leave
            if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']) and $form_state['values']['id'] != $lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['id']){ // if leave_type exists
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have ') . strtolower(t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']])) . ' !';
            }
            
            // already exists
            if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['id']){ // if leave_type exists
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have ') . strtolower(t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']])) . ' !';
            }
          }
          
          // check if colleage and department heads
          foreach($colids as $collid){
            // is not holiday
            if(!isset($lrcol->data[$collid][$year][$month][$day]['holiday'])){

              // already exists, normal_leave
              if(isset($lrcol->data[$collid][$year][$month][$day]['normal_leave']['leave_type']) and $form_state['values']['id'] != $lrcol->data[$collid][$year][$month][$day]['normal_leave']['id']){ // if leave_type exists
                drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lrcol->employees[$collid]['display_name'] . ' ' . strtolower(t($lrcol->option_groups['leave_request_leave_type']['options'][$lrcol->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . ' !');
              }

              // already exists
              if(isset($lrcol->data[$collid][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lrcol->data[$collid][$year][$month][$day]['request']['id']){ // if leave_type exists
                drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lrcol->employees[$collid]['display_name'] . ' ' . strtolower(t($lrcol->option_groups['leave_request_leave_type']['options'][$lrcol->data[$collid][$year][$month][$day]['request']['leave_type']])) . ' !');
              }
            }
          }
        }
        
      }else {
        // is holiday
        if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['holiday']) and $lr->data[$form_state['values']['cid']][$year][$month][$day]['holiday']['is_holiday']){
          $errors['request'][] = t('The request is on a holiday !');
          
        // is not holiday
        }else {
          
          // is adjustments          
          $duration = 0;
          if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['adjustments']['duration']) and '' !== $lr->data[$form_state['values']['cid']][$year][$month][$day]['adjustments']['duration']){
            $duration = $lr->data[$form_state['values']['cid']][$year][$month][$day]['adjustments']['duration'];

          }else if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['overall_adjustments']['duration']) and '' !== $lr->data[$form_state['values']['cid']][$year][$month][$day]['overall_adjustments']['duration']){
            $duration = $lr->data[$form_state['values']['cid']][$year][$month][$day]['overall_adjustments']['duration'];

          }else if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['general']['duration']) and '' !== $lr->data[$form_state['values']['cid']][$year][$month][$day]['general']['duration']){
            $duration = $lr->data[$form_state['values']['cid']][$year][$month][$day]['general']['duration'];
          }
          
          // if a adjustments
          if(0 == $duration){
            $errors['request'][] = t('You do not work on that day !');
            
          }else {  
            // already exists, normal leave less one day
            if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']) and 'normal_leave_less_one_day' == $lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type'] and $form_state['values']['id'] != $lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['id']){ // if leave_type exists
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have ') . strtolower(t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']])) . ' !';
            }
            
            // already exists
            if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['id']){ // if leave_type exists
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have ') . strtolower(t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']])) . ' !';
            }
          }
        }
        
        // check if colleages and department heads
        foreach($colids as $collid){
          // is not holiday
          if(!isset($lrcol->data[$collid][$year][$month][$day]['holiday'])){
            
            // already exists, normal_leave
            if(isset($lrcol->data[$collid][$year][$month][$day]['normal_leave']['leave_type']) and $form_state['values']['id'] != $lrcol->data[$collid][$year][$month][$day]['normal_leave']['id']){ // if leave_type exists
              drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lrcol->employees[$collid]['display_name'] . ' ' . strtolower(t($lrcol->option_groups['leave_request_leave_type']['options'][$lrcol->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . ' !');
            }
            
            // already exists
            if(isset($lrcol->data[$collid][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lrcol->data[$collid][$year][$month][$day]['request']['id']){ // if leave_type exists
              drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lrcol->employees[$collid]['display_name'] . ' ' . strtolower(t($lrcol->option_groups['leave_request_leave_type']['options'][$lrcol->data[$collid][$year][$month][$day]['request']['leave_type']])) . ' !');
            }
          }
        }
      }
      break;
      
    case 'mom_dad_day':
    case 'study_leave':
    case 'care':  
      // to date is lower than from date
      if($form_state['values']['from_date'] > $form_state['values']['to_date']){
        form_set_error('to_date', t('To date must be higher than from date !'));
      }
      
      // duration
      $form_state['values']['duration'] = trim($form_state['values']['duration']);

      // if it is a whole number    
      if(preg_match('/^[0-9]{1,2}$/', $form_state['values']['duration'], $matches)){
        $form_state['values']['duration'] .= ':00'; 
      }

      // if it start with 0
      if('0' == substr($form_state['values']['duration'], 0, 1) and '0' == substr($form_state['values']['duration'], 1, 2)){
        $form_state['values']['duration'] = substr($form_state['values']['duration'], 1, strlen($form_state['values']['duration']));
      }

      // if 0:00 or 00:00
      if(!preg_match('/^[0-9]{1,2}[:]{1}[0-9]{2}$/', $form_state['values']['duration'], $matches)){
        form_set_error('duration', t('Duration has wrong value !'));
      }

      // if 0:00
      if('0:00' == $form_state['values']['duration']){
        form_set_error('duration', t('Duration can`t be zero !'));
      }

      $form_state['values']['date'] = '1970-01-01 00:00:00'; // 1970-01-01 00:00:00
      
      switch($form_state['values']['daily_weekly_monthly_annually']){
        case 'daily':
          $form_state['values']['weekly_each'] = $lr->custom_fields['leave_request_weekly_each']['default_value'];
          $form_state['values']['weekly_day_of_the_week'] = implode(CRM_Core_DAO::VALUE_SEPARATOR, $lr->custom_fields['leave_request_weekly_day_of_the_week']['default_value']);

          $form_state['values']['monthly_each'] = $lr->custom_fields['leave_request_monthly_each']['default_value'];
          $form_state['values']['monthly_every_day_of_the_month_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_every_day_of_the_month_day_of_the_week']['default_value'];
          $form_state['values']['monthly_day_of_the_month'] = $lr->custom_fields['leave_request_monthly_day_of_the_month']['default_value'];
          $form_state['values']['monthly_on'] = $lr->custom_fields['leave_request_monthly_on']['default_value'];
          $form_state['values']['monthly_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_day_of_the_week']['default_value'];

          $form_state['values']['annually_each'] = $lr->custom_fields['leave_request_annually_each']['default_value'];
          $form_state['values']['annually_every_day_of_the_month_day_of_the_week'] = $lr->custom_fields['leave_request_annually_every_day_of_the_month_day_of_the_week']['default_value'];
          $form_state['values']['annually_month'] = $lr->custom_fields['leave_request_annually_month']['default_value'];
          $form_state['values']['annually_day_of_the_month'] = $lr->custom_fields['leave_request_annually_day_of_the_month']['default_value'];
          $form_state['values']['annually_on'] = $lr->custom_fields['leave_request_annually_on']['default_value'];
          $form_state['values']['annually_day_of_the_week'] = $lr->custom_fields['leave_request_annually_day_of_the_week']['default_value'];
          $form_state['values']['annually_month_of'] = $lr->custom_fields['leave_request_annually_month_of']['default_value'];
          
          switch($form_state['values']['daily_every_day_working_day']){
            case 'day':
              // each
              $form_state['values']['daily_each'] = trim($form_state['values']['daily_each']);

              // if it is a whole number
              if(!preg_match('/^[0-9]{1,2}$/', $form_state['values']['daily_each'], $matches)){
                form_set_error('daily_each', t('Each has wrong value, must be a whole number, with maximum of 2 digits !'));
              }

              if('0' == $form_state['values']['daily_each']){
                form_set_error('daily_each', t('Each can`t be zero !'));

              }else {
                if('0' == substr($form_state['values']['daily_each'], 0, 1)){
                  form_set_error('daily_each', t('Each, first number can`t be zero !'));
                }
              }
              break;
            case 'working_day':
              $form_state['values']['daily_each'] = $lr->custom_fields['leave_request_daily_each']['default_value'];
              break;
          }
          break;
        
        case 'weekly':          
          $form_state['values']['daily_every_day_working_day'] = $lr->custom_fields['leave_request_daily_every_day_working_day']['default_value'];
          $form_state['values']['daily_each'] = $lr->custom_fields['leave_request_daily_each']['default_value'];
                              
          $form_state['values']['weekly_day_of_the_week'] = implode(CRM_Core_DAO::VALUE_SEPARATOR, $form_state['values']['weekly_day_of_the_week']);
          
          $form_state['values']['monthly_each'] = $lr->custom_fields['leave_request_monthly_each']['default_value'];
          $form_state['values']['monthly_every_day_of_the_month_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_every_day_of_the_month_day_of_the_week']['default_value'];
          $form_state['values']['monthly_day_of_the_month'] = $lr->custom_fields['leave_request_monthly_day_of_the_month']['default_value'];
          $form_state['values']['monthly_on'] = $lr->custom_fields['leave_request_monthly_on']['default_value'];
          $form_state['values']['monthly_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_day_of_the_week']['default_value'];

          $form_state['values']['annually_each'] = $lr->custom_fields['leave_request_annually_each']['default_value'];
          $form_state['values']['annually_every_day_of_the_month_day_of_the_week'] = $lr->custom_fields['leave_request_annually_every_day_of_the_month_day_of_the_week']['default_value'];
          $form_state['values']['annually_month'] = $lr->custom_fields['leave_request_annually_month']['default_value'];
          $form_state['values']['annually_day_of_the_month'] = $lr->custom_fields['leave_request_annually_day_of_the_month']['default_value'];
          $form_state['values']['annually_on'] = $lr->custom_fields['leave_request_annually_on']['default_value'];
          $form_state['values']['annually_day_of_the_week'] = $lr->custom_fields['leave_request_annually_day_of_the_week']['default_value'];
          $form_state['values']['annually_month_of'] = $lr->custom_fields['leave_request_annually_month_of']['default_value'];
          
          // each
          $form_state['values']['weekly_each'] = trim($form_state['values']['weekly_each']);

          // if it is a whole number
          if(!preg_match('/^[0-9]{1,2}$/', $form_state['values']['weekly_each'], $matches)){
            form_set_error('weekly_each', t('Each has wrong value, must be a whole number, with maximum of 2 digits !'));
          }

          if('0' == $form_state['values']['weekly_each']){
            form_set_error('weekly_each', t('Each can`t be zero !'));

          }else {
            if('0' == substr($form_state['values']['weekly_each'], 0, 1)){
              form_set_error('weekly_each', t('Each, first number can`t be zero !'));
            }
          }
          break;
        
        case 'monthly':
          $form_state['values']['daily_every_day_working_day'] = $lr->custom_fields['leave_request_daily_every_day_working_day']['default_value'];
          $form_state['values']['daily_each'] = $lr->custom_fields['leave_request_daily_each']['default_value'];

          $form_state['values']['weekly_each'] = $lr->custom_fields['leave_request_weekly_each']['default_value'];
          $form_state['values']['weekly_day_of_the_week'] = implode(CRM_Core_DAO::VALUE_SEPARATOR, $lr->custom_fields['leave_request_weekly_day_of_the_week']['default_value']);

          $form_state['values']['annually_each'] = $lr->custom_fields['leave_request_annually_each']['default_value'];
          $form_state['values']['annually_every_day_of_the_month_day_of_the_week'] = $lr->custom_fields['leave_request_annually_every_day_of_the_month_day_of_the_week']['default_value'];
          $form_state['values']['annually_month'] = $lr->custom_fields['leave_request_annually_month']['default_value'];
          $form_state['values']['annually_day_of_the_month'] = $lr->custom_fields['leave_request_annually_day_of_the_month']['default_value'];
          $form_state['values']['annually_on'] = $lr->custom_fields['leave_request_annually_on']['default_value'];
          $form_state['values']['annually_day_of_the_week'] = $lr->custom_fields['leave_request_annually_day_of_the_week']['default_value'];
          $form_state['values']['annually_month_of'] = $lr->custom_fields['leave_request_annually_month_of']['default_value'];
          
          // each
          $form_state['values']['monthly_each'] = trim($form_state['values']['monthly_each']);

          // if it is a whole number
          if(!preg_match('/^[0-9]{1,2}$/', $form_state['values']['monthly_each'], $matches)){
            form_set_error('monthly_each', t('Each has wrong value, must be a whole number, with maximum of 2 digits !'));
          }

          if('0' == $form_state['values']['monthly_each']){
            form_set_error('monthly_each', t('Each can`t be zero !'));

          }else {
            if('0' == substr($form_state['values']['monthly_each'], 0, 1)){
              form_set_error('monthly_each', t('Each, first number can`t be zero !'));
            }
          }
          
          switch($form_state['values']['monthly_every_day_of_the_month_day_of_the_week']){
            case 'every_day_of_the_month':
              $form_state['values']['monthly_on'] = $lr->custom_fields['leave_request_monthly_on']['default_value'];
              $form_state['values']['monthly_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_day_of_the_week']['default_value'];
              
              // every_day
              $form_state['values']['monthly_day_of_the_month'] = trim($form_state['values']['monthly_day_of_the_month']);

              // if it is a whole number
              if(!preg_match('/^[0-9]{1,2}$/', $form_state['values']['monthly_day_of_the_month'], $matches)){
                form_set_error('monthly_day_of_the_month', t('Every day has wrong value, must be a whole number, with maximum of 2 digits !'));
              }

              if('0' == $form_state['values']['monthly_day_of_the_month']){
                form_set_error('monthly_day_of_the_month', t('Every day can`t be zero !'));

              }else {
                if('0' == substr($form_state['values']['monthly_day_of_the_month'], 0, 1)){
                  form_set_error('monthly_day_of_the_month', t('Every day, first number can`t be zero !'));
                }
              }
              break;
            
            case 'every_day_of_the_week':
              $form_state['values']['monthly_day_of_the_month'] = $lr->custom_fields['leave_request_monthly_day_of_the_month']['default_value'];
              break;
          }
          break;
        
        case 'annually':
          $form_state['values']['daily_every_day_working_day'] = $lr->custom_fields['leave_request_daily_every_day_working_day']['default_value'];
          $form_state['values']['daily_each'] = $lr->custom_fields['leave_request_daily_each']['default_value'];

          $form_state['values']['weekly_each'] = $lr->custom_fields['leave_request_weekly_each']['default_value'];
          $form_state['values']['weekly_day_of_the_week'] = implode(CRM_Core_DAO::VALUE_SEPARATOR, $lr->custom_fields['leave_request_weekly_day_of_the_week']['default_value']);

          $form_state['values']['monthly_each'] = $lr->custom_fields['leave_request_monthly_each']['default_value'];
          $form_state['values']['monthly_every_day_of_the_month_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_every_day_of_the_month_day_of_the_week']['default_value'];
          $form_state['values']['monthly_day_of_the_month'] = $lr->custom_fields['leave_request_monthly_day_of_the_month']['default_value'];
          $form_state['values']['monthly_on'] = $lr->custom_fields['leave_request_monthly_on']['default_value'];
          $form_state['values']['monthly_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_day_of_the_week']['default_value'];
          
          // each
          $form_state['values']['annually_each'] = trim($form_state['values']['annually_each']);

          // if it is a whole number
          if(!preg_match('/^[0-9]{1,2}$/', $form_state['values']['annually_each'], $matches)){
            form_set_error('annually_each', t('Each has wrong value, must be a whole number, with maximum of 2 digits !'));
          }

          if('0' == $form_state['values']['annually_each']){
            form_set_error('annually_each', t('Each can`t be zero !'));

          }else {
            if('0' == substr($form_state['values']['annually_each'], 0, 1)){
              form_set_error('annually_each', t('Each, first number can`t be zero !'));
            }
          }
          
          switch($form_state['values']['annually_every_day_of_the_month_day_of_the_week']){
            case 'every_day_of_the_month':
              $form_state['values']['monthly_on'] = $lr->custom_fields['leave_request_monthly_on']['default_value'];
              $form_state['values']['monthly_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_day_of_the_week']['default_value'];
              $form_state['values']['annually_month_of'] = $lr->custom_fields['leave_request_annually_month_of']['default_value'];
              
              // every_day
              $form_state['values']['annually_day_of_the_month'] = trim($form_state['values']['annually_day_of_the_month']);

              // if it is a whole number
              if(!preg_match('/^[0-9]{1,2}$/', $form_state['values']['annually_day_of_the_month'], $matches)){
                form_set_error('annually_day_of_the_month', t('Every day has wrong value, must be a whole number, with maximum of 2 digits !'));
              }

              if('0' == $form_state['values']['annually_day_of_the_month']){
                form_set_error('annually_day_of_the_month', t('Every day can`t be zero !'));

              }else {
                if('0' == substr($form_state['values']['annually_day_of_the_month'], 0, 1)){
                  form_set_error('annually_day_of_the_month', t('Every day, first number can`t be zero !'));
                }
              }
              break;
            
            case 'every_day_of_the_week':
              $form_state['values']['annually_month'] = $lr->custom_fields['leave_request_annually_month']['default_value'];
              $form_state['values']['annually_day_of_the_month'] = $lr->custom_fields['leave_request_annually_day_of_the_month']['default_value'];
              break;
          }
          break;
      }
      
      break;
      
    default:
      // to date is lower than from date
      if($form_state['values']['from_date'] > $form_state['values']['to_date']){
        form_set_error('to_date', t('To date must be higher than from date !'));
      }

      $form_state['values']['date'] = '1970-01-01 00:00:00'; // 1970-01-01 00:00:00
      
      $form_state['values']['daily_weekly_monthly_annually'] = $lr->custom_fields['leave_request_daily_weekly_monthly_annually']['default_value'];
    
      $form_state['values']['daily_every_day_working_day'] = $lr->custom_fields['leave_request_daily_every_day_working_day']['default_value'];
      $form_state['values']['daily_each'] = $lr->custom_fields['leave_request_daily_each']['default_value'];

      $form_state['values']['weekly_each'] = $lr->custom_fields['leave_request_weekly_each']['default_value'];
      $form_state['values']['weekly_day_of_the_week'] = implode(CRM_Core_DAO::VALUE_SEPARATOR, $lr->custom_fields['leave_request_weekly_day_of_the_week']['default_value']);

      $form_state['values']['monthly_each'] = $lr->custom_fields['leave_request_monthly_each']['default_value'];
      $form_state['values']['monthly_every_day_of_the_month_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_every_day_of_the_month_day_of_the_week']['default_value'];
      $form_state['values']['monthly_day_of_the_month'] = $lr->custom_fields['leave_request_monthly_day_of_the_month']['default_value'];
      $form_state['values']['monthly_on'] = $lr->custom_fields['leave_request_monthly_on']['default_value'];
      $form_state['values']['monthly_day_of_the_week'] = $lr->custom_fields['leave_request_monthly_day_of_the_week']['default_value'];

      $form_state['values']['annually_each'] = $lr->custom_fields['leave_request_annually_each']['default_value'];
      $form_state['values']['annually_every_day_of_the_month_day_of_the_week'] = $lr->custom_fields['leave_request_annually_every_day_of_the_month_day_of_the_week']['default_value'];
      $form_state['values']['annually_month'] = $lr->custom_fields['leave_request_annually_month']['default_value'];
      $form_state['values']['annually_day_of_the_month'] = $lr->custom_fields['leave_request_annually_day_of_the_month']['default_value'];
      $form_state['values']['annually_on'] = $lr->custom_fields['leave_request_annually_on']['default_value'];
      $form_state['values']['annually_day_of_the_week'] = $lr->custom_fields['leave_request_annually_day_of_the_week']['default_value'];
      $form_state['values']['annually_month_of'] = $lr->custom_fields['leave_request_annually_month_of']['default_value'];
      
      $form_state['values']['duration'] = '0:00';
      
      // see leaveregistration.class.php function set_request
      // check already request
      $from_date_totime = strtotime($form_state['values']['from_date']);
      $to_date_totime = strtotime($form_state['values']['to_date']);
      
      for($timestamp = $from_date_totime; $timestamp <= $to_date_totime; $timestamp = strtotime( '+1 day' , $timestamp )){  
        
        $year = date('Y', $timestamp);
        $month = date('m', $timestamp);
        $day = date('d', $timestamp);
        
        $duration = 0;
        if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['adjustments']['duration']) and '' !== $lr->data[$form_state['values']['cid']][$year][$month][$day]['adjustments']['duration']){
          $duration = $lr->data[$form_state['values']['cid']][$year][$month][$day]['adjustments']['duration'];

        }else if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['overall_adjustments']['duration']) and '' !== $lr->data[$form_state['values']['cid']][$year][$month][$day]['overall_adjustments']['duration']){
          $duration = $lr->data[$form_state['values']['cid']][$year][$month][$day]['overall_adjustments']['duration'];

        }else if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['general']['duration']) and '' !== $lr->data[$form_state['values']['cid']][$year][$month][$day]['general']['duration']){
          $duration = $lr->data[$form_state['values']['cid']][$year][$month][$day]['general']['duration'];
        }
        
        // is holiday
        if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['holiday']) and ($lr->data[$form_state['values']['cid']][$year][$month][$day]['holiday']['is_holiday'])){
          $duration = 0;
        }
        
        if('normal_leave' == $form_state['values']['leave_type']){
          
          // already exists normal_leave
          if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']) and $form_state['values']['id'] != $lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['id']){
            // if no adjustments
            if(0 != $duration){
              //$errors['request'][] = t('There is already a ') . t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']]) . t(' on ') . $day . '-' . $month . '-' . $year . ' !';
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have ') . strtolower(t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']])) . ' !';
            }
          }
          
        }else {
          // already exists
          if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['id']){
            // if no adjustments
            if(0 != $duration){
              //$errors['request'][] = t('There is already a ') . t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']]) . t(' on ') . $day . '-' . $month . '-' . $year . ' !';
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have ') . strtolower(t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']])) . ' !';
            }
          }
        }
        
        // check if colleage and department heads        
        foreach($colids as $collid){
          // is not holiday
          if(!isset($lrcol->data[$collid][$year][$month][$day]['holiday'])){
            
            // is adjustments
            $duration = 0;
            if(isset($lrcol->data[$collid][$year][$month][$day]['adjustments']['duration']) and '' !== $lrcol->data[$collid][$year][$month][$day]['adjustments']['duration']){
              $duration = $lrcol->data[$collid][$year][$month][$day]['adjustments']['duration'];

            }else if(isset($lrcol->data[$collid][$year][$month][$day]['overall_adjustments']['duration']) and '' !== $lrcol->data[$collid][$year][$month][$day]['overall_adjustments']['duration']){
              $duration = $lrcol->data[$collid][$year][$month][$day]['overall_adjustments']['duration'];

            }else if(isset($lrcol->data[$collid][$year][$month][$day]['general']['duration']) and '' !== $lrcol->data[$collid][$year][$month][$day]['general']['duration']){
              $duration = $lrcol->data[$collid][$year][$month][$day]['general']['duration'];
            }

            // if no adjustments
            if(0 != $duration){
              
              // already exists, normal_leave
              if(isset($lrcol->data[$collid][$year][$month][$day]['normal_leave']['leave_type']) and $form_state['values']['id'] != $lrcol->data[$collid][$year][$month][$day]['normal_leave']['id']){ // if leave_type don exists
                //drupal_set_message($lr->employees[$collid]['display_name'] . t(' has already a ') . strtolower(t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . t(' on ') . $day . '-' . $month . '-' . $year . ' !');
                drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lrcol->employees[$collid]['display_name'] . ' ' . strtolower(t($lrcol->option_groups['leave_request_leave_type']['options'][$lrcol->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . ' !');
              }

              // already exists
              if(isset($lrcol->data[$collid][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lrcol->data[$collid][$year][$month][$day]['request']['id']){ // if leave_type don exists
                //drupal_set_message($lr->employees[$collid]['display_name'] . t(' has already a ') . strtolower(t($lr->option_groups['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['request']['leave_type']])) . t(' on ') . $day . '-' . $month . '-' . $year . ' !');
                drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lrcol->employees[$collid]['display_name'] . ' ' . strtolower(t($lrcol->option_groups['leave_request_leave_type']['options'][$lrcol->data[$collid][$year][$month][$day]['request']['leave_type']])) . ' !');
              }
            }
          }
        }
      }
  }
    
  foreach($errors as $field => $error){
    $form_set_error = '';
    foreach($error as $key => $message){
      $form_set_error .= $message . '<br/>';
    }
    
    form_set_error($field, $form_set_error);
  }
}

function leaveregistration_request_submit($form, &$form_state){  
  $values = array();
    
  $values['target_contact_id'] = $form_state['values']['cid'];
  $values['source_record_id'] = $form_state['values']['cid'];
  
  switch($form_state['values']['status'])
  {
    case 'approved':
    case 'rejected':
      $values['status_id'] = '2';
      break;
    
    default:
      $values['status_id'] = '1';
  }
    
  $values['leave_type'] = $form_state['values']['leave_type'];
  $values['reason'] = $form_state['values']['reason'];
  $values['status'] = $form_state['values']['status'];

  $values['from_date'] = $form_state['values']['from_date'];
  $values['to_date'] = $form_state['values']['to_date'];
  $values['date'] = $form_state['values']['date'];
    
  $values['daily_weekly_monthly_annually'] = $form_state['values']['daily_weekly_monthly_annually'];
    
  $values['daily_every_day_working_day'] = $form_state['values']['daily_every_day_working_day'];
  $values['daily_each'] = $form_state['values']['daily_each'];

  $values['weekly_each'] = $form_state['values']['weekly_each'];
  $values['weekly_day_of_the_week'] = $form_state['values']['weekly_day_of_the_week'];

  $values['monthly_each'] = $form_state['values']['monthly_each'];
  $values['monthly_every_day_of_the_month_day_of_the_week'] = $form_state['values']['monthly_every_day_of_the_month_day_of_the_week'];
  $values['monthly_day_of_the_month'] = $form_state['values']['monthly_day_of_the_month'];
  $values['monthly_on'] = $form_state['values']['monthly_on'];
  $values['monthly_day_of_the_week'] = $form_state['values']['monthly_day_of_the_week'];

  $values['annually_each'] = $form_state['values']['annually_each'];
  $values['annually_every_day_of_the_month_day_of_the_week'] = $form_state['values']['annually_every_day_of_the_month_day_of_the_week'];
  $values['annually_month'] = $form_state['values']['annually_month'];
  $values['annually_day_of_the_month'] = $form_state['values']['annually_day_of_the_month'];
  $values['annually_on'] = $form_state['values']['annually_on'];
  $values['annually_day_of_the_week'] = $form_state['values']['annually_day_of_the_week'];
  $values['annually_month_of'] = $form_state['values']['annually_month_of'];
  
  $values['duration'] = $form_state['values']['duration'];
    
  $lr = new leaveregistration('drupal', 'leaveregistration_request_submit');
  $lr->set_fields();
  $lr->set_contacts(array($form_state['values']['cid'], $form_state['values']['user_cid']));
    
  $values['display_name'] = $lr->employees[$form_state['values']['cid']]['display_name'];
    
  $error = false;
  if(!empty($form_state['values']['id'])){
    // update
    $return = $lr->update($form_state['values']['id'], $form_state['values']['cid'], $values);
    if($return['is_error']){
      form_set_error('request', t('A database error occured when updating a request ! Error message: ') . $return['error_message']);
      $error = true;
    }else {
      $id = $return['id'];
      drupal_set_message(t('The request is updated !'), 'status');
    }
    
  }else {
    $values['source_contact_id'] = $form_state['values']['user_cid'];
    
    // create
    $return = $lr->create($form_state['values']['cid'], $values);
        
    if($return['is_error']){
      form_set_error('request', t('A database error occured when creating a request ! Error message: ') . $return['error_message']);
      $error = true;
      
    }else {
      $id = $return['id'];
      drupal_set_message(t('The request is created !'), 'status');
    }
  }
    
  if(!$error){
    // mail
    global $language;
    
    // Send one email to yourself
    $params = array();
    $params = array_merge($form_state['values'], array('employee_display_name' => $lr->employees[$form_state['values']['cid']]['display_name']));
    $params = array_merge($params, array('id' => $id));
    $params = array_merge($params, array('option_group' => $lr->option_groups));

    if(!empty($form_state['values']['id'])){
      drupal_mail('leaveregistration', 'change', $lr->employees[$form_state['values']['cid']]['email'], $language->language, $params);
    }else {
      drupal_mail('leaveregistration', 'request', $lr->employees[$form_state['values']['cid']]['email'], $language->language, $params);
    }

    // Send one email to department head, expect if it is yourself
    foreach($lr->department_heads[$form_state['values']['cid']] as $dhid => $department_head){
      if($form_state['values']['cid'] != $dhid){
        $params = array();
        $params = array_merge($form_state['values'], array('employee_display_name' => $lr->employees[$form_state['values']['cid']]['display_name']));
        $params = array_merge($params, array('departement_head_display_name' => $department_head['display_name']));
        $params = array_merge($params, array('id' => $id));
        $params = array_merge($params, array('option_group' => $lr->option_groups));
        
        if(!empty($form_state['values']['id'])){
          drupal_mail('leaveregistration', 'change', $department_head['email'], $language->language, $params);
        }else {
          drupal_mail('leaveregistration', 'request', $department_head['email'], $language->language, $params);
        }
      }
    }
        
    // credit
    //if('approved' == $form_state['values']['status']){
      $lr->set_data(array($form_state['values']['year']));
      
      $over = $lr->total[$form_state['values']['cid']][$form_state['values']['year']]['over'];
            
      // get credit
      $credit_over = $lr->credit_over_get($form_state['values']['cid'], ($form_state['values']['year']+1));
      if(!empty($credit_over)){
        // update
        $lr->credit_over_update($credit_over['id'], $over);
      }else {
        // create
        $lr->credit_over_create($form_state['values']['cid'], ($form_state['values']['year']+1), $over);
      }
    //}
        
    // ical
    if('approved' == $form_state['values']['status'] and 'time_for_time' != $form_state['values']['leave_type']){
      $_SESSION['leaveregistration']['ical'] = array();
      $_SESSION['leaveregistration']['ical']['user_id'] = $form_state['values']['user_id'];
      $_SESSION['leaveregistration']['ical']['id'] = $id;
      $_SESSION['leaveregistration']['ical']['year'] = $form_state['values']['year'];
      $_SESSION['leaveregistration']['ical']['cid'] = $form_state['values']['cid'];
    }
    
    $form_state['redirect'] = array('user/' . $form_state['values']['user_id'] . '/leave/overview/', array('query' => array('year' => $form_state['values']['year'], 'cid' => $form_state['values']['cid'])));
  }
}
  
// delete
function leaveregistration_delete($form, &$form_state, $user_id, $id)
{
  // get civicrm contact id from drupal user
  $session = CRM_Core_Session::singleton();
  $user_cid = $session->get('userID');
  
  // get cid or set it 
  if(isset($_GET['cid']) and '' != $_GET['cid']){
    $cid = $_GET['cid'];
    
  }else {
    $cid = $user_cid;
  }
    
  $year = '';
  $years = array();  
  if(isset($_GET['year']) and '' != $_GET['year']){
    $year = $_GET['year'];
    $years[] = $_GET['year']-1;
    $years[] = $_GET['year'];
    
  }else {
    $year = date('Y');
    $years[] = date('Y')-1;
    $years[] = date('Y');
  }
    
  $lrp = new CRM_Leaveregistration_Page_LeaveRegistration();
  $lrp->run(array(
    'error_id' => 'leaveregistration_delete',
    'error_platform' => 'drupal', 
    'cid' => $cid,
    'user_cid' => $user_cid,
    'user_id' => $user_id,
    'years' => $years,
    'year' => $year,
    'months' => array(),
    'month' => 'all',
    'type' => 'form',
    'return' => 'return',
  ));
  
  $form['#attributes']['class'][] = 'leaveregistration';
  
  $form['display_name'] = array
  (
    '#markup' => $lrp->get_display_name()
  );
  
  $form['back'] = array(
    '#type' => 'link',
    '#title' => t('Back'),
    '#href' => '/user/' . $user_id . '/leave/overview/?year=' . $year . '&cid=' . $cid,
    '#options' => array('external' => TRUE),
    '#prefix' => '<div id="edit-back">',
    '#suffix' => '</div>'
  );
  
  $form['request'] = array
  (
    '#type' => 'fieldset',
    '#title' => t('Delete')
  );
  
  // hidden form elements
  $form['request']['cid'] = array('#type' => 'hidden','#value' => $cid);    
  $form['request']['user_cid'] = array('#type' => 'hidden','#value' => $user_cid);    
  $form['request']['user_id'] = array('#type' => 'hidden','#value' => $user_id); 
  $form['request']['year'] = array('#type' => 'hidden','#value' => $year);
  $form['request']['id'] = array('#type' => 'hidden','#value' => $id);
  
  $request = $lrp->lr->request[$id];
    
  $form['request']['msg'] = array();  
  $form['request']['msg']['#markup'] = implode('<br />', leaveregistration_body($lrp->lr, $request));
      
  $form['request']['msg']['#markup'] .= '<br />' . t('Are you sure you want to delete this request !') . '<br />';
  
  $form['request']['yes'] = array
  (
    '#type' => 'submit',
    '#value' => t('Yes'),
    '#submit' => array('leaveregistration_delete_yes')
  );
  
  $form['request']['no'] = array
  (
    '#type' => 'submit',
    '#value' => t('No'),
    '#submit' => array('leaveregistration_delete_no')
  );
  
  return $form;
}

function leaveregistration_delete_yes($form, &$form_state)
{
  $lr = new leaveregistration('drupal', 'leaveregistration_delete_yes');
  $lr->set_fields();
  $lr->set_contacts(array($form_state['values']['cid'], $form_state['values']['user_cid']));
  
  if($lr->delete($form_state['values']['id'])){
    
    // credit
    $lr->set_data(array($form_state['values']['year']));
      
    $over = $lr->total[$form_state['values']['cid']][$form_state['values']['year']]['over'];

    // get credit
    $credit_over = $lr->credit_over_get($form_state['values']['cid'], ($form_state['values']['year']+1));
    if(!empty($credit_over)){
      // update
      $lr->credit_over_update($credit_over['id'], $over);
    }else {
      // create
      $lr->credit_over_create($form_state['values']['cid'], ($form_state['values']['year']+1), $over);
    }
    
    // message
    drupal_set_message(t('The request is deleted !'), 'status');
    // redirect
    $form_state['redirect'] = array('user/' . $form_state['values']['user_id'] . '/leave/', array('query' => array('year' => $form_state['values']['year'], 'cid' => $form_state['values']['cid'])));
    
  }else {
    form_set_error('request', t('A database error occured when deleting the request !'));
  }
}

function leaveregistration_delete_no($form, &$form_state)
{
  $form_state['redirect'] = array('user/' . $form_state['values']['user_id'] . '/leave/', array('query' => array('year' => $form_state['values']['year'], 'cid' => $form_state['values']['cid'])));
}

function leaveregistration_mail($key, &$message, $params)
{  
  if('leaveregistration' == $message['module']){
    
    global $base_url;
    global $base_path;
    
    $lr = new leaveregistration('drupal', 'leaveregistration_mail');
    $lr->set_fields();
        
    $message['headers']['MIME-Version'] = '1.0';
    $message['headers']['Content-Type'] = 'multipart/mixed;';
    $message['headers']['Content-Type'] = 'text/html;'; 
  
    switch($key)
    {
      case 'request':
        $message['subject'] = t('Leave registration - There is a request on ') . variable_get('site_name', '');

        if(isset($params['departement_head_display_name'])){
          $message['body'][] = t('Dear') . ' ' . $params['departement_head_display_name'];
          $message['body'][] = t('The leave request of ') . $params['employee_display_name'] . t(' is added.');

        }else {
          $message['body'][] = t('Dear') . ' ' . $params['employee_display_name'] . '<br/>';
          $message['body'][] = t('Your leave request is added.') . '<br/>';
        }
        
        $message['body'][] = '<br/>';
        $message['body'][] = implode('<br/>', leaveregistration_body($lr, $params));
        $message['body'][] = '<br/>';
                
        // link to request
        $message['body'][] = t('Click the link below to go to the leave request.');
        $message['body'][] = $base_url . $base_path . 'user/' . $params['user_id'] .'/leave/request/' . $params['id'] .'/change?year=' . $params['year'] .'&cid=' . $params['cid'] . '<br/>';
        
        // login first link
        $message['body'][] = t('If you are not logged in, you must log in first. Click here to login.');
        $message['body'][] = $base_url . $base_path . 'user';
        
        $message['body'][] = '<br/>';
        
        $message['body'][] = t('Yours sincerely ') . '<br/>' . variable_get('site_name', '');

        break;
        
      case 'change':
        $message['subject'] = t('Leave registration - There is a change on ') . variable_get('site_name', '');

        if(isset($params['departement_head_display_name'])){
          $message['body'][] = t('Dear') . ' ' . $params['departement_head_display_name'] . '<br/>';
          $message['body'][] = t('The leave request of ') . $params['employee_display_name'] . t(' is changed.') . '<br/>';

        }else {
          $message['body'][] = t('Dear') . ' ' . $params['employee_display_name'] . '<br/>';
          $message['body'][] = t('Your leave request is changed.') . '<br/>';
        }
        
        $message['body'][] = '<br/>';
        $message['body'][] = implode('<br/>', leaveregistration_body($lr, $params));
        $message['body'][] = '<br/>';
                
        // link to request
        $message['body'][] = t('Click the link below to go to the leave request.') . '<br/>';
        $message['body'][] = $base_url . $base_path . 'user/' . $params['user_id'] .'/leave/request/' . $params['id'] .'/change?year=' . $params['year'] .'&cid=' . $params['cid'] . '<br/>';
        
        // login first link
        $message['body'][] = t('If you are not logged in, you must log in first. Click here to login.') . '<br/>';
        $message['body'][] = $base_url . $base_path . 'user' . '<br/>';
        
        $message['body'][] = '<br/>';
        
        $message['body'][] = t('Yours sincerely ') . '<br/>' . variable_get('site_name', '') . '<br/>';

        break;
    }
    
    // attachment
    if('approved' == $params['status'] and 'time_for_time' != $params['leave_type']){
      
      $body = leaveregistration_ical_outlook_body($params['user_id'], $params['id'], $params['cid'], $params['year']);
            
      $message['params']['attachments'][] = array(
        'filecontent' => $body,
        'filename' => 'calendar.ics',
        'filemime' => 'text/plain',
      );
    }
  }
}

function leaveregistration_ical_date_time_z($timestamp) {
  return date('Ymd\THis\Z', $timestamp);
}

function leaveregistration_ical_date_time($timestamp) {
  return date('Ymd\THis', $timestamp);
}

function leaveregistration_ical_date($timestamp) {
  return date('Ymd', $timestamp);
}

function leaveregistration_ical_escape($string) {
  return preg_replace('/([\,;])/','\\\$1', $string);
}

function leaveregistration_ical_outlook($form, &$form_state, $user_id, $id){
  // VCALENDAR
  header('Content-type: text/calendar; charset=utf-8');
  header('Content-Disposition: attachment; filename=calendar.ics');
  
  $body = leaveregistration_ical_outlook_body($user_id, $id, $_GET['cid'], $_GET['year']);
  
  echo($body);
  exit();
}

function leaveregistration_ical_outlook_body($user_id, $id, $cid, $year){
  global $base_url;
  
  // get civicrm contact id from drupal user
  $session = CRM_Core_Session::singleton();
  $user_cid = $session->get('userID'); 
    
  $lr = new leaveregistration('drupal', 'leaveregistration_ical_outlook_body');
  $lr->set_fields();
  $lr->set_contacts(array($cid, $user_cid));
  $lr->set_data(array($year));
  
  $data = $lr->get($id);
  
  // body
  $location = 'Bosgoed Company';
  $summary = t('Leave') . ' - ' . $lr->employees[$cid]['display_name'] . ' - ' . t($lr->option_groups['leave_request_leave_type']['options'][$data['leave_type']]);
  $description = implode('\n\r',leaveregistration_body($lr, $data));
  $description_html = implode('<br/>',leaveregistration_body($lr, $data));
  $uri = $base_url;
  
  $body = 'BEGIN:VCALENDAR' . PHP_EOL;
  $body .= 'PRODID:-//Bosgoed Company//Outlook 14.0 MIMEDIR//NL' . PHP_EOL;
  $body .= 'VERSION:2.0' . PHP_EOL;
  $body .= 'METHOD:PUBLISH' . PHP_EOL;
  $body .= 'X-MS-OLK-FORCEINSPECTOROPEN:TRUE' . PHP_EOL;
  // timezone
  $body .= 'BEGIN:VTIMEZONE' . PHP_EOL;
  $body .= 'TZID:W. Europe Standard Time' . PHP_EOL;
  // timezone standard
  $body .= 'BEGIN:STANDARD' . PHP_EOL;
  //$body .= 'DTSTART:16011028T030000' . PHP_EOL;
  $body .= 'RRULE:FREQ=YEARLY;BYDAY=-1SU;BYMONTH=10' . PHP_EOL;
  $body .= 'TZOFFSETFROM:+0200' . PHP_EOL;
  $body .= 'TZOFFSETTO:+0100' . PHP_EOL;
  $body .= 'END:STANDARD' . PHP_EOL;
  // timezone daylight
  $body .= 'BEGIN:DAYLIGHT' . PHP_EOL;
  $body .= 'DTSTART:16010325T020000' . PHP_EOL;
  $body .= 'RRULE:FREQ=YEARLY;BYDAY=-1SU;BYMONTH=3' . PHP_EOL;
  $body .= 'TZOFFSETFROM:+0100' . PHP_EOL;
  $body .= 'TZOFFSETTO:+0200' . PHP_EOL;
  $body .= 'END:DAYLIGHT' . PHP_EOL;
  $body .= 'END:VTIMEZONE' . PHP_EOL; 
  
  $body .= 'BEGIN:VEVENT' . PHP_EOL;
  $body .= 'CLASS:PUBLIC' . PHP_EOL;
  
  // created
  $body .= 'CREATED:' . leaveregistration_ical_date_time_z(time()) . PHP_EOL;
  // description
  $body .= 'DESCRIPTION:' . leaveregistration_ical_escape($description) . PHP_EOL;
  
  //$body .= 'DTEND;TZID="W. Europe Standard Time":' . leaveregistration_ical_date(time()) . PHP_EOL;
  
  $body .= 'PRIORITY:5' . PHP_EOL;
    
  switch($data['leave_type']){
    case 'time_for_time':
    case 'doctor_visit':
    case 'normal_leave_less_one_day':
    case 'sick_less_one_day':
      $start_date_totime = strtotime($data['date']);
      $end_date_totime = strtotime($data['date']);
      
      $body .= 'DTSTART;VALUE=DATE:' . leaveregistration_ical_date($start_date_totime) . PHP_EOL;
      $body .= 'DTEND;VALUE=DATE:' . leaveregistration_ical_date($end_date_totime + 24*60*60) . PHP_EOL;
      break;

    case 'mom_dad_day':
    case 'study_leave':
    case 'care':
      $start_date_totime = strtotime($data['from_date']);
      //$end_date_totime = strtotime($data['to_date']);
      // en date is start date + one day
      $end_date_totime = strtotime($data['from_date']) + 24*60*60;;
      
      $first_date = ''; // set first day, ical needs to now the exacte start date (that is the first day that occurs in the calender)
      $count = 0; // count how many times
      $patterns = $lr->set_pattern_outlook($data);
      foreach($patterns as $cid => $years){
        foreach($years as $year => $months){
          foreach($months as $month => $days){
            foreach($days as $day => $pattern){
              // get first day
              if(empty($first_date)){
                $first_date = strtotime($year . '-' . $month . '-' . $day);
              }
              // count days
              $count++;
            }
          }
        }
      }              
      
      $duration_expl = explode(':', $data['duration']);
      $duration = ($duration_expl[0] *60*60) + ($duration_expl[1] *60);
      
      switch($data['daily_weekly_monthly_annually']){
        case 'daily':
          
          switch($data['daily_every_day_working_day']){ 
            case 'day':
              $body .= 'DTSTART;VALUE=DATE:' . leaveregistration_ical_date($start_date_totime) . PHP_EOL;
              $body .= 'DTEND;VALUE=DATE:' . leaveregistration_ical_date($start_date_totime + $duration) . PHP_EOL;
              
              $body .= 'RRULE:FREQ=DAILY';
              
              // each
              if(1 < $data['daily_each']){
                $body .= ';INTERVAL=' . $data['daily_each'];
              }
              break;
              
            case 'working_day':
              $body .= 'DTSTART;TZID="W. Europe Standard Time":' . leaveregistration_ical_date_time($first_date) . PHP_EOL;
              $body .= 'DTEND;TZID="W. Europe Standard Time":' . leaveregistration_ical_date_time($first_date + $duration) . PHP_EOL;
              
              $body .= 'RRULE:FREQ=WEEKLY;BYDAY=MO,TU,WE,TH,FR';
              break;
          }
          
          $body .= ';COUNT=' . $count . PHP_EOL;
          break;
        case 'weekly':
          $body .= 'DTSTART;TZID="W. Europe Standard Time":' . leaveregistration_ical_date_time($first_date) . PHP_EOL;
          $body .= 'DTEND;TZID="W. Europe Standard Time":' . leaveregistration_ical_date_time($first_date + $duration) . PHP_EOL;
          
          $body .= 'RRULE:FREQ=WEEKLY';
                   
          // day of the week
          $body .= ';BYDAY=';
          $weekly_day_of_the_week = explode(CRM_Core_DAO::VALUE_SEPARATOR, $data['weekly_day_of_the_week']);
          foreach($weekly_day_of_the_week as $day_of_the_week){
            switch($day_of_the_week){
              case 'monday':
                $body .= 'MO,';
                break;
              case 'tuesday':
                $body .= 'TU,';
                break;
              case 'wednesday':
                $body .= 'WE,';
                break;
              case 'thursday':
                $body .= 'TH,';
                break;
              case 'friday':
                $body .= 'FR,';
                break;
              case 'saturday':
                $body .= 'SA,';
                break;
              case 'sunday':
                $body .= 'SU,';
                break;
            }
          }
          $body = substr($body, 0, -1);
          
          // each
          if(1 < $data['weekly_each']){
            $body .= ';INTERVAL=' . $data['weekly_each'] . 'BYDAY=';
          }
          
          $body .= ';COUNT=' . $count . PHP_EOL;
          break;
          
        case 'monthly':
          $body .= 'DTSTART;TZID="W. Europe Standard Time":' . leaveregistration_ical_date_time($first_date) . PHP_EOL;
          $body .= 'DTEND;TZID="W. Europe Standard Time":' . leaveregistration_ical_date_time($first_date + $duration) . PHP_EOL;
          
          $body .= 'RRULE:FREQ=MONTHLY';
          
          // each
          if(1 < $data['monthly_each']){
            $body .= ';INTERVAL=' . $data['monthly_each'];
          }
          
          switch($data['monthly_every_day_of_the_month_day_of_the_week']){
            case 'every_day_of_the_month':
              $body .= ';BYMONTHDAY=' . $data['monthly_day_of_the_month'];
              break;
            
            case 'every_day_of_the_week':
              // on
              switch($data['monthly_on']){
                case 'first':
                  $body .= ';BYSETPOS=1';
                  break;
                case 'second':
                  $body .= ';BYSETPOS=2';
                  break;
                case 'third':
                  $body .= ';BYSETPOS=3';
                  break;
                case 'fourth':
                  $body .= ';BYSETPOS=4';
                  break;
                case 'last':
                  $body .= ';BYSETPOS=-1';
                  break;
              }              
              
              // day of the week
              $body .= ';BYDAY=';
              switch($data['monthly_day_of_the_week']){
                case 'monday':
                  $body .= 'MO';
                  break;
                case 'tuesday':
                  $body .= 'TU';
                  break;
                case 'wednesday':
                  $body .= 'WE';
                  break;
                case 'thursday':
                  $body .= 'TH';
                  break;
                case 'friday':
                  $body .= 'FR';
                  break;
                case 'saturday':
                  $body .= 'SA';
                  break;
                case 'sunday':
                  $body .= 'SU';
                  break;
              }
              break;
          }
          
          $body .= ';COUNT=' . $count . PHP_EOL;
          break;
        
        case 'annually':          
          $body .= 'DTSTART;TZID="W. Europe Standard Time":' . leaveregistration_ical_date_time($first_date) . PHP_EOL;
          $body .= 'DTEND;TZID="W. Europe Standard Time":' . leaveregistration_ical_date_time($first_date + $duration) . PHP_EOL;
          
          $body .= 'RRULE:FREQ=YEARLY';
          
          // each
          if(1 < $data['annually_each']){
            $body .= ';INTERVAL=' . $data['annually_each'];
          }
                    
          // every day / day of the week month
          switch($data['annually_every_day_of_the_month_day_of_the_week']){
            case 'every_day_of_the_month':
              $body .= ';BYMONTH=';
              
              switch($data['annually_month']){
                case 'january':
                  $body .= '1';
                  break;
                case 'february':
                  $body .= '2';
                  break;
                case 'march':
                  $body .= '3';
                  break;
                case 'april':
                  $body .= '4';
                  break;
                case 'may':
                  $body .= '5';
                  break;
                case 'june':
                  $body .= '6';
                  break;
                case 'july':
                  $body .= '7';
                  break;
                case 'august':
                  $body .= '8';
                  break;
                case 'september':
                  $body .= '9';
                  break;
                case 'october':
                  $body .= '10';
                  break;
                case 'november':
                  $body .= '11';
                  break;
                case 'december':
                  $body .= '12';
                  break;
              }
                           
              // every day
              $body .= ';BYMONTHDAY=' . $data['annually_day_of_the_month'];
              
              break;
            
            case 'every_day_of_the_week':
              // on
              switch($data['annually_on']){
                case 'first':
                  $body .= ';BYSETPOS=1';
                  break;
                case 'second':
                  $body .= ';BYSETPOS=2';
                  break;
                case 'third':
                  $body .= ';BYSETPOS=3';
                  break;
                case 'fourth':
                  $body .= ';BYSETPOS=4';
                  break;
                case 'last':
                  $body .= ';BYSETPOS=-1';
                  break;
              }
              
              // day of the week
              $body .= ';BYDAY=';
              switch($data['annually_day_of_the_week']){
                case 'monday':
                  $body .= 'MO';
                  break;
                case 'tuesday':
                  $body .= 'TU';
                  break;
                case 'wednesday':
                  $body .= 'WE';
                  break;
                case 'thursday':
                  $body .= 'TH';
                  break;
                case 'friday':
                  $body .= 'FR';
                  break;
                case 'saturday':
                  $body .= 'SA';
                  break;
                case 'sunday':
                  $body .= 'SU';
                  break;
              }
                         
              // month
              $body .= ';BYMONTH=';
              switch($data['annually_month_of']){
                case 'january':
                  $body .= '1';
                  break;
                case 'february':
                  $body .= '2';
                  break;
                case 'march':
                  $body .= '3';
                  break;
                case 'april':
                  $body .= '4';
                  break;
                case 'may':
                  $body .= '5';
                  break;
                case 'june':
                  $body .= '6';
                  break;
                case 'july':
                  $body .= '7';
                  break;
                case 'august':
                  $body .= '8';
                  break;
                case 'september':
                  $body .= '9';
                  break;
                case 'october':
                  $body .= '10';
                  break;
                case 'november':
                  $body .= '11';
                  break;
                case 'december':
                  $body .= '12';
                  break;
              }
              
              break;
          }
          
          $body .= ';COUNT=' . $count . PHP_EOL;
          break;
      }
      break;

    default:
      $start_date_totime = strtotime($data['from_date']);
      $end_date_totime = strtotime($data['to_date']);
      
      $body .= 'DTSTART;VALUE=DATE:' . leaveregistration_ical_date($start_date_totime) . PHP_EOL;
      $body .= 'DTEND;VALUE=DATE:' . leaveregistration_ical_date($end_date_totime + 24*60*60) . PHP_EOL;
  }
    
  $body .= 'SEQUENCE:0' . PHP_EOL;
  $body .= 'SUMMARY:' . leaveregistration_ical_escape($summary) . PHP_EOL;
  $body .= 'TRANSP:OPAQUE' . PHP_EOL;
  $body .= 'UID:url|' . leaveregistration_ical_escape($uri) . '|id|'. $id . '|uniqid|' . uniqid() . PHP_EOL;
  $body .= 'X-ALT-DESC;FMTTYPE=text/html:<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN"><HTML><HEAD><META HTTP-EQUIV="Content-Type" CONTENT="text/html\; charset=iso-8859-1"><META NAME="Generator" CONTENT="MS Exchange Server version 14.03.0162.000"><TITLE>' . $summary . '</TITLE></HEAD><BODY>' . $description_html . '</BODY></HTML>' . PHP_EOL;
      
  $body .= 'X-MICROSOFT-CDO-BUSYSTATUS:BUSY' . PHP_EOL;
  $body .= 'X-MICROSOFT-CDO-IMPORTANCE:1' . PHP_EOL;
  $body .= 'X-MICROSOFT-DISALLOW-COUNTER:FALSE' . PHP_EOL;
  $body .= 'X-MS-OLK-AUTOFILLLOCATION:TRUE' . PHP_EOL;
  $body .= 'X-MS-OLK-CONFTYPE:0' . PHP_EOL;
  
  // valarm
  $body .= 'BEGIN:VALARM' . PHP_EOL;
  $body .= 'TRIGGER:-PT15M' . PHP_EOL;
  $body .= 'ACTION:DISPLAY' . PHP_EOL;
  $body .= 'DESCRIPTION:Reminder' . PHP_EOL;
  $body .= 'END:VALARM' . PHP_EOL;
  
  $body .= 'END:VEVENT' . PHP_EOL;
  $body .= 'END:VCALENDAR' . PHP_EOL;
    
  return $body;
}

function leaveregistration_ical_import($form, &$form_state, $user_id, $id){
  // VCALENDAR
  header('Content-type: text/calendar; charset=utf-8');
  header('Content-Disposition: attachment; filename=calendar.ics');
  
  $body = leaveregistration_ical_import_body($user_id, $id, $_GET['cid'], $_GET['year']);
  
  echo($body);
  exit();
}

function leaveregistration_ical_import_body($user_id, $id, $cid, $year){
  global $base_url;
  
  $lr = new leaveregistration('drupal', 'leaveregistration_ical_outlook_body');
  $lr->set_fields();
  $lr->set_contacts($cid, $user_id, array());
  $lr->set_data(array($year));
  
  $data = $lr->get($id);
      
  // body
  $location = 'Bosgoed Company';
  $summary = t('Leave') . ' - ' . $lr->employees[$cid]['display_name'] . ' - ' . t($lr->option_groups['leave_request_leave_type']['options'][$data['leave_type']]);
  $description = implode('\n\r',leaveregistration_body($lr, $data));
  $uri = $base_url;
  
  $body = 'BEGIN:VCALENDAR' . PHP_EOL;
  $body .= 'VERSION:2.0' . PHP_EOL;
  $body .= 'PRODID:-//Bosgoed Company//Outlook 14.0 MIMEDIR//NL' . PHP_EOL;
  $body .= 'METHOD:PUBLISH' . PHP_EOL;
  $body .= 'CALSCALE:GREGORIAN' . PHP_EOL;
  //$body .= 'X-MS-OLK-FORCEINSPECTOROPEN:TRUE' . PHP_EOL;
  $body .= 'X-PRIMARY-CALENDAR:TRUE' . PHP_EOL;
  
  switch($data['leave_type']){
    case 'time_for_time': 
      $timestamp = strtotime($data['date']);
      
      $year = date('Y', $timestamp);
      $month = date('m', $timestamp);
      $day = date('d', $timestamp);

      list($hours, $minutes) = explode(':', $data['duration']);
      $duration = ($hours * 60 * 60) + ($minutes * 60);
      
      $body .= 'DTSTAMP:' . leaveregistration_ical_date(time()) . PHP_EOL;
      $body .= 'DTSTART;VALUE=DATE:' . leaveregistration_ical_date($timestamp) . PHP_EOL;
      $body .= 'DTEND;VALUE=DATE:' . leaveregistration_ical_date($timestamp + $duration) . PHP_EOL;

      $body .= 'UID:url|' . leaveregistration_ical_escape($uri) . '|id|'. $id . '|uniqid|' . uniqid() . PHP_EOL;
      $body .= 'STATUS:CONFIRMED' . PHP_EOL;
      $body .= 'CLASS:PUBLIC' . PHP_EOL;

      $body .= 'LOCATION:' . leaveregistration_ical_escape($location) . PHP_EOL;
      $body .= 'DESCRIPTION:' . leaveregistration_ical_escape($description) . PHP_EOL;
      $body .= 'URL;VALUE=URI:' . leaveregistration_ical_escape($uri) . PHP_EOL;
      $body .= 'SUMMARY:' . leaveregistration_ical_escape($summary) . PHP_EOL;
      $body .= 'X-MICROSOFT-CDO-BUSYSTATUS:FREE' . PHP_EOL; // microsoft outlook busy status

      $body .= 'END:VEVENT' . PHP_EOL;
      
      break;
    
    case 'doctor_visit':
    case 'sick_less_one_day': 
      $timestamp = strtotime($data['date']);

      $year = date('Y', $timestamp);
      $month = date('m', $timestamp);
      $day = date('d', $timestamp);

      list($hours, $minutes) = explode(':', $data['duration']);
      $duration = ($hours * 60) + $minutes;

      // is holiday
      if(isset($lr->data[$data['cid']][$year][$month][$day]['holiday']['is_holiday']) and ($lr->data[$data['cid']][$year][$month][$day]['holiday']['is_holiday'])){
        $duration = 0;
      }
      
      if(0 != $duration){
        if(!isset($lr->data[$data['cid']][$year][$month][$day]['request']['leave_type']) or $id == $lr->data[$data['cid']][$year][$month][$day]['request']['id']){ // if leave_type don exists
          $body .= 'BEGIN:VEVENT' . PHP_EOL;
              
          $body .= 'DTSTAMP:' . leaveregistration_ical_date(time()) . PHP_EOL;
          $body .= 'DTSTART;VALUE=DATE:' . leaveregistration_ical_date($timestamp) . PHP_EOL;
          $body .= 'DTEND;VALUE=DATE:' . leaveregistration_ical_date(($timestamp + ($duration*60))) . PHP_EOL;

          $body .= 'UID:url|' . leaveregistration_ical_escape($uri) . '|id|'. $id . '|uniqid|' . uniqid() . PHP_EOL;
          $body .= 'STATUS:CONFIRMED' . PHP_EOL;
          $body .= 'CLASS:PUBLIC' . PHP_EOL;

          $body .= 'LOCATION:' . leaveregistration_ical_escape($location) . PHP_EOL;
          $body .= 'DESCRIPTION:' . leaveregistration_ical_escape($description) . PHP_EOL;
          $body .= 'URL;VALUE=URI:' . leaveregistration_ical_escape($uri) . PHP_EOL;
          $body .= 'SUMMARY:' . leaveregistration_ical_escape($summary) . PHP_EOL;
          $body .= 'X-MICROSOFT-CDO-BUSYSTATUS:FREE' . PHP_EOL; // microsoft outlook busy status

          $body .= 'END:VEVENT' . PHP_EOL;
        }
      }
      break;

    case 'mom_dad_day':
    case 'study_leave':
    case 'care':      
      $patterns = $lr->set_pattern_outlook($data);

      foreach($patterns as $cid => $years){
        foreach($years as $year => $months){
          foreach($months as $month => $days){
            foreach($days as $day => $pattern){

              if(isset($lr->data[$cid][$year][$month][$day])){     
                
                $timestamp = strtotime($year . '-' . $month . '-' . $day);
                
                list($hours, $minutes) = explode(':', $pattern['duration']);
                $duration = ($hours * 60) + $minutes;

                // is holiday
                if(isset($lr->data[$cid][$year][$month][$day]['holiday']['is_holiday']) and ($lr->data[$cid][$year][$month][$day]['holiday']['is_holiday'])){
                  $duration = 0;
                }
                
                if(0 != $duration){
                  if(!isset($lr->data[$cid][$year][$month][$day]['request'])){
                    $lr->data[$cid][$year][$month][$day]['request'] = array();
                  }

                  // adjustments
                  if(isset($lr->data[$data['cid']][$year][$month][$day]['adjustments']['duration']) and '' !== $lr->data[$data['cid']][$year][$month][$day]['adjustments']['duration']){
                    $duration = $lr->data[$data['cid']][$year][$month][$day]['adjustments']['duration'];

                  }else if(isset($lr->data[$data['cid']][$year][$month][$day]['overall_adjustments']['duration']) and '' !== $lr->data[$data['cid']][$year][$month][$day]['overall_adjustments']['duration']){
                    $duration = $lr->data[$data['cid']][$year][$month][$day]['overall_adjustments']['duration'];

                  }else if(isset($lr->data[$data['cid']][$year][$month][$day]['general']['duration']) and '' !== $lr->data[$data['cid']][$year][$month][$day]['general']['duration']){
                    $duration = $lr->data[$data['cid']][$year][$month][$day]['general']['duration'];
                  }

                  // if adjustment is not 0, if it does he/she is free
                  if(0 != $duration){

                    if(!isset($lr->data[$cid][$year][$month][$day]['request']['leave_type']) or $id == $lr->data[$data['cid']][$year][$month][$day]['request']['id']){ // if leave_type don exists
                      $body .= 'BEGIN:VEVENT' . PHP_EOL;

                      $body .= 'DTSTAMP:' . leaveregistration_ical_date(time()) . PHP_EOL;
                      $body .= 'DTSTART;VALUE=DATE:' . leaveregistration_ical_date($timestamp) . PHP_EOL;
                      $body .= 'DTEND;VALUE=DATE:' . leaveregistration_ical_date(($timestamp + ($duration*60))) . PHP_EOL;

                      $body .= 'UID:url|' . leaveregistration_ical_escape($uri) . '|id|'. $id . '|uniqid|' . uniqid() . PHP_EOL;
                      $body .= 'STATUS:CONFIRMED' . PHP_EOL;
                      $body .= 'CLASS:PUBLIC' . PHP_EOL;

                      $body .= 'LOCATION:' . leaveregistration_ical_escape($location) . PHP_EOL;
                      $body .= 'DESCRIPTION:' . leaveregistration_ical_escape($description) . PHP_EOL;
                      $body .= 'URL;VALUE=URI:' . leaveregistration_ical_escape($uri) . PHP_EOL;
                      $body .= 'SUMMARY:' . leaveregistration_ical_escape($summary) . PHP_EOL;
                      $body .= 'X-MICROSOFT-CDO-BUSYSTATUS:FREE' . PHP_EOL; // microsoft outlook busy status

                      $body .= 'END:VEVENT' . PHP_EOL;
                    }
                  }
                }
              }
            }
          }
        }
      }
      break;

    case 'special_leave':
    case 'mom_dad_day_contiguous':
    case 'maternity':
    case 'sick':
      $from_date_totime = strtotime($data['from_date']);
      $to_date_totime = strtotime($data['to_date']);

      for($timestamp = $from_date_totime; $timestamp <= $to_date_totime; $timestamp = strtotime( '+1 day' , $timestamp )){            
        $year = date('Y', $timestamp);
        $month = date('m', $timestamp);
        $day = date('d', $timestamp);
        
        $duration = 999;
            
        // is holiday
        if(isset($lr->data[$data['cid']][$year][$month][$day]['holiday']['is_holiday']) and ($lr->data[$data['cid']][$year][$month][$day]['holiday']['is_holiday'])){
          $duration = 0;
        }

        if(0 != $duration){
        
          if(!isset($lr->data[$data['cid']][$year][$month][$day]['request'])){
            $lr->data[$data['cid']][$year][$month][$day]['request'] = array();
          }

          $duration = 0;
          if(isset($lr->data[$data['cid']][$year][$month][$day]['adjustments']['duration']) and '' !== $lr->data[$data['cid']][$year][$month][$day]['adjustments']['duration']){
            $duration = $lr->data[$data['cid']][$year][$month][$day]['adjustments']['duration'];

          }else if(isset($lr->data[$data['cid']][$year][$month][$day]['overall_adjustments']['duration']) and '' !== $lr->data[$data['cid']][$year][$month][$day]['overall_adjustments']['duration']){
            $duration = $lr->data[$data['cid']][$year][$month][$day]['overall_adjustments']['duration'];

          }else if(isset($lr->data[$data['cid']][$year][$month][$day]['general']['duration']) and '' !== $lr->data[$data['cid']][$year][$month][$day]['general']['duration']){
            $duration = $lr->data[$data['cid']][$year][$month][$day]['general']['duration'];
          }

          if(0 != $duration){
            if(!isset($lr->data[$data['cid']][$year][$month][$day]['request']['leave_type']) or $id == $lr->data[$data['cid']][$year][$month][$day]['request']['id']){ // if leave_type don exists
              $body .= 'BEGIN:VEVENT' . PHP_EOL;

              $body .= 'DTSTAMP:' . leaveregistration_ical_date(time()) . PHP_EOL;
              $body .= 'DTSTART;VALUE=DATE:' . leaveregistration_ical_date($timestamp) . PHP_EOL;
              $body .= 'DTEND;VALUE=DATE:' . leaveregistration_ical_date(($timestamp + ($duration*60))) . PHP_EOL;

              $body .= 'UID:url|' . leaveregistration_ical_escape($uri) . '|id|'. $id . '|uniqid|' . uniqid() . PHP_EOL;
              $body .= 'STATUS:CONFIRMED' . PHP_EOL;
              $body .= 'CLASS:PUBLIC' . PHP_EOL;

              $body .= 'LOCATION:' . leaveregistration_ical_escape($location) . PHP_EOL;
              $body .= 'DESCRIPTION:' . leaveregistration_ical_escape($description) . PHP_EOL;
              $body .= 'URL;VALUE=URI:' . leaveregistration_ical_escape($uri) . PHP_EOL;
              $body .= 'SUMMARY:' . leaveregistration_ical_escape($summary) . PHP_EOL;
              $body .= 'X-MICROSOFT-CDO-BUSYSTATUS:FREE' . PHP_EOL; // microsoft outlook busy status

              $body .= 'END:VEVENT' . PHP_EOL;
            }
          }
        }
      }
      break;
      
    case 'normal_leave_less_one_day': 
      $timestamp = strtotime($data['date']);

      $year = date('Y', $timestamp);
      $month = date('m', $timestamp);
      $day = date('d', $timestamp);

      list($hours, $minutes) = explode(':', $data['duration']);
      $duration = ($hours * 60) + $minutes;

      // is holiday
      if(isset($lr->data[$data['cid']][$year][$month][$day]['holiday']['is_holiday']) and ($lr->data[$data['cid']][$year][$month][$day]['holiday']['is_holiday'])){
        $duration = 0;
      }

      if(0 != $duration){
        // is not request
        if(!isset($lr->data[$data['cid']][$year][$month][$day]['request']['is_request'])){  
          // is not normal leave
          if(!isset($lr->data[$data['cid']][$year][$month][$day]['normal_leave']['leave_type']) or $id == $lr->data[$data['cid']][$year][$month][$day]['normal_leave']['id']){ // if leave_type don exists
            $body .= 'BEGIN:VEVENT' . PHP_EOL;
              
            $body .= 'DTSTAMP:' . leaveregistration_ical_date(time()) . PHP_EOL;
            $body .= 'DTSTART;VALUE=DATE:' . leaveregistration_ical_date($timestamp) . PHP_EOL;
            $body .= 'DTEND;VALUE=DATE:' . leaveregistration_ical_date(($timestamp + ($duration*60))) . PHP_EOL;

            $body .= 'UID:url|' . leaveregistration_ical_escape($uri) . '|id|'. $id . '|uniqid|' . uniqid() . PHP_EOL;
            $body .= 'STATUS:CONFIRMED' . PHP_EOL;
            $body .= 'CLASS:PUBLIC' . PHP_EOL;

            $body .= 'LOCATION:' . leaveregistration_ical_escape($location) . PHP_EOL;
            $body .= 'DESCRIPTION:' . leaveregistration_ical_escape($description) . PHP_EOL;
            $body .= 'URL;VALUE=URI:' . leaveregistration_ical_escape($uri) . PHP_EOL;
            $body .= 'SUMMARY:' . leaveregistration_ical_escape($summary) . PHP_EOL;
            $body .= 'X-MICROSOFT-CDO-BUSYSTATUS:FREE' . PHP_EOL; // microsoft outlook busy status

            $body .= 'END:VEVENT' . PHP_EOL;
          }
        }
      }
      break;

    case 'normal_leave':
      $from_date_totime = strtotime($data['from_date']);
      $to_date_totime = strtotime($data['to_date']);
      
      for($timestamp = $from_date_totime; $timestamp <= $to_date_totime; $timestamp = strtotime( '+1 day' , $timestamp )){            
        $year = date('Y', $timestamp);
        $month = date('m', $timestamp);
        $day = date('d', $timestamp);
        
        $duration = 999;
        
        // is holiday
        if(isset($lr->data[$data['cid']][$year][$month][$day]['holiday']['is_holiday']) and ($lr->data[$data['cid']][$year][$month][$day]['holiday']['is_holiday'])){
          $duration = 0;
        }
        
        if(0 != $duration){
          $duration = 0;
          if(isset($lr->data[$data['cid']][$year][$month][$day]['adjustments']['duration']) and '' !== $lr->data[$data['cid']][$year][$month][$day]['adjustments']['duration']){
            $duration = $lr->data[$data['cid']][$year][$month][$day]['adjustments']['duration'];

          }else if(isset($lr->data[$data['cid']][$year][$month][$day]['overall_adjustments']['duration']) and '' !== $lr->data[$data['cid']][$year][$month][$day]['overall_adjustments']['duration']){
            $duration = $lr->data[$data['cid']][$year][$month][$day]['overall_adjustments']['duration'];

          }else if(isset($lr->data[$data['cid']][$year][$month][$day]['general']['duration']) and '' !== $lr->data[$data['cid']][$year][$month][$day]['general']['duration']){
            $duration = $lr->data[$data['cid']][$year][$month][$day]['general']['duration'];
          }

          if(0 != $duration){

            // is not request
            if(!isset($lr->data[$data['cid']][$year][$month][$day]['request']['is_request'])){ 
              // is not normal leave
              if(!isset($lr->data[$data['cid']][$year][$month][$day]['normal_leave']['leave_type']) or $id == $lr->data[$data['cid']][$year][$month][$day]['normal_leave']['id']){ // if leave_type don exists
                $body .= 'BEGIN:VEVENT' . PHP_EOL;

                $body .= 'DTSTAMP:' . leaveregistration_ical_date(time()) . PHP_EOL;
                $body .= 'DTSTART;VALUE=DATE:' . leaveregistration_ical_date($timestamp) . PHP_EOL;
                $body .= 'DTEND;VALUE=DATE:' . leaveregistration_ical_date(($timestamp + ($duration*60))) . PHP_EOL;

                $body .= 'UID:url|' . leaveregistration_ical_escape($uri) . '|id|'. $id . '|uniqid|' . uniqid() . PHP_EOL;
                $body .= 'STATUS:CONFIRMED' . PHP_EOL;
                $body .= 'CLASS:PUBLIC' . PHP_EOL;

                $body .= 'LOCATION:' . leaveregistration_ical_escape($location) . PHP_EOL;
                $body .= 'DESCRIPTION:' . leaveregistration_ical_escape($description) . PHP_EOL;
                $body .= 'URL;VALUE=URI:' . leaveregistration_ical_escape($uri) . PHP_EOL;
                $body .= 'SUMMARY:' . leaveregistration_ical_escape($summary) . PHP_EOL;
                $body .= 'X-MICROSOFT-CDO-BUSYSTATUS:FREE' . PHP_EOL; // microsoft outlook busy status

                $body .= 'END:VEVENT' . PHP_EOL;
              }
            }
          }
        }
      }
      break;
  }
  $body .= 'END:VCALENDAR' . PHP_EOL;
    
  return $body;
}
?>