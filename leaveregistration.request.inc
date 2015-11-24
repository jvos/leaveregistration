<?php
function leaveregistration_view($form, &$form_state, $user_id, $id)
{
  global $user;
  $user_cid = leaveregistration_get_cid($user->uid);
  
  $cids = array();
  if(isset($_GET['cid']) and '' != $_GET['cid']){
    $cid = $_GET['cid'];
    $cids[] = $cid;
  }
    
  $years = array();  
  if(isset($_GET['year']) and '' != $_GET['year']){
    $year = $_GET['year'];
    $years[] = $_GET['year']-1;
    $years[] = $_GET['year'];  
  }
  
  if(empty($cids)){
    drupal_set_message( t('No contact id`s !'), 'error');
    return false;
  }
  
  if(empty($years)){
    drupal_set_message( t('No years !'), 'error');
    return false;
  }
  
  if('' == $year){
    drupal_set_message( t('No year !'), 'error');
    return false;
  }
  
  if(0 == $user_cid or '' == $user_cid){
    drupal_set_message( t('No user contact id !'), 'error');
    return false;
  }
  
  if(0 == $user_id or '' == $user_id){
    drupal_set_message( t('No user id !'), 'error');
    return false;
  }
  
  $data = array
  (    
    'error_platform' => 'drupal',
    
    'cids' => $cids,
    'user_cid' => $user_cid,
    'user_id' => $user_id,
    
    'years' => $years,
    'months' => array(),
    'cid' => $cid,
        
    'year' => $year,
    'month' => 'all',
    
    'type' => 'form',
    'return' => 'return',
    
    'elements' => array('display_name')
  );
  
  $lrp = new CRM_Leaveregistration_Page_LeaveRegistration();
  $form_elements = $lrp->run($data);

  if(empty($form_elements)){
    drupal_set_message( t('No form elements !'), 'error');
    return $form;
  }
  
  $form['#attributes']['class'][] = 'leaveregistration';
  
  $form['display_name'] = array
  (
    '#markup' => '<h1>' . $form_elements['display_name'] . '</h1>'
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
  
  $request = $lrp->get_request_by_id($id);
  $option_group = $lrp->get_option_group();
     
  $form['request']['msg'] = array();
  $form['request']['msg']['#markup'] = t('Leave type: ') . t($option_group['leave_request_leave_type'][$request['leave_type']]) . '<br />';
  
  if('time_for_time' == $request['leave_type'] or 'doctor_visit' == $request['leave_type'] or 'normal_leave_less_one_day' == $request['leave_type'] or 'sick_less_one_day' == $request['leave_type']){
    $form['request']['msg']['#markup'] .= t('Date: ') . strftime("%d-%m-%Y", strtotime($request['date'])) . '<br />';
    $form['request']['msg']['#markup'] .= t('Duration: ') . $request['duration'] . '<br />';
        
  }else if('mom_dad_day' == $request['leave_type'] or 'study_leave' == $request['leave_type'] or 'care' == $request['leave_type']){
    $form['request']['msg']['#markup'] .= t('From date: ') . strftime("%d-%m-%Y", strtotime($request['from_date'])) . '<br />';
    $form['request']['msg']['#markup'] .= t('To date: ') . strftime("%d-%m-%Y", strtotime($request['to_date'])) . '<br />';
    $form['request']['msg']['#markup'] .= t('Each: ') . $request['each'] . ' ' . t($option_group['leave_request_day_week_month_year'][$request['day_week_month_year']]);
    if('nvt' != $request['on']){
      $form['request']['msg']['#markup'] .= ' ' . t('on') . ' ' . t($option_group['leave_request_on'][$request['on']]);
    }
    if('nvt' != $request['day_of_the_week']){
      $form['request']['msg']['#markup'] .= ' ' . t($option_group['leave_request_day_of_the_week'][$request['day_of_the_week']]);
    }
    
    $form['request']['msg']['#markup'] .= ' <br />';
    
  }else {
    $form['request']['msg']['#markup'] .= t('From date: ') . strftime("%d-%m-%Y", strtotime($request['from_date'])) . '<br />';
    $form['request']['msg']['#markup'] .= t('To date: ') . strftime("%d-%m-%Y", strtotime($request['to_date'])) . '<br />';
  }
  
  if(!empty($request['reason'])){
    $form['request']['msg']['#markup'] .= ' <br />';
    $form['request']['msg']['#markup'] .= t('Reason: ');
    $form['request']['msg']['#markup'] .= ' <br />';
    $form['request']['msg']['#markup'] .= $request['reason'];
    $form['request']['msg']['#markup'] .= ' <br />';
  }
    
  return $form;
}
// change
function leaveregistration_request($form, &$form_state, $user_id, $id = 0)
{        
  global $user;
  $user_cid = leaveregistration_get_cid($user->uid);
  
  $cids = array();
  if(isset($_GET['cid']) and '' != $_GET['cid']){
    $cid = $_GET['cid'];
    $cids[] = $cid;
  }
  
  $years = array();  
  if(isset($_GET['year']) and '' != $_GET['year']){
    $year = $_GET['year'];
    $years[] = $_GET['year']-1;
    $years[] = $_GET['year'];
    
  }
    
  if(empty($cids)){
    drupal_set_message( t('No contact id`s !'), 'error');
    return false;
  }
  
  if(empty($years)){
    drupal_set_message( t('No years !'), 'error');
    return false;
  }
  
  if('' == $year){
    drupal_set_message( t('No year !'), 'error');
    return false;
  }
  
  if(0 == $user_cid or '' == $user_cid){
    drupal_set_message( t('No user contact id !'), 'error');
    return false;
  }
  
  if(0 == $user_id or '' == $user_id){
    drupal_set_message( t('No user id !'), 'error');
    return false;
  }
  
  $data = array
  (    
    'error_platform' => 'drupal',
    
    'cids' => $cids,
    'user_cid' => $user_cid,
    'user_id' => $user_id,
    
    'years' => $years,
    'months' => array(),
    'cid' => $cid,
        
    'year' => $year,
    'month' => 'all',
    
    'type' => 'form',
    'return' => 'return',
    
    'elements' => array('display_name', 'year', 'legend_leave_type', 'credit', 'months', 'show_colleagues', 'calendar_year')
  );
  
  $lrp = new CRM_Leaveregistration_Page_LeaveRegistration();
  $form_elements = $lrp->run($data);

  if(empty($form_elements)){
    drupal_set_message( t('No form elements !'), 'error');
    return $form;
  }
  
  $form['#attributes']['class'][] = 'leaveregistration';
  
  $form['#validate'][] = 'leaveregistration_request_validate';
	$form['#submit'][] = 'leaveregistration_request_submit';
      
  $form['display_name'] = array
  (
    '#markup' => '<h1>' . $form_elements['display_name'] . '</h1>'
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
  
  $option_group = $lrp->get_option_group();  
  
  // if the id is empty
  if(!empty($id)){
    $request = $lrp->get_request_by_id($id);

    if(!isset($form_state['values']) and empty($form_state['values'])){

      $form_state['values']['leave_type'] = $request['leave_type'];
      $form_state['values']['reason'] = $request['reason'];
      $form_state['values']['status'] = $request['status'];

      $form_state['values']['from_date'] = $request['from_date'];
      $form_state['values']['to_date'] = $request['to_date'];

      $form_state['values']['each'] = $request['each'];
      $form_state['values']['day_week_month_year'] = $request['day_week_month_year'];
      $form_state['values']['on'] = $request['on'];
      $form_state['values']['day_of_the_week'] = $request['day_of_the_week'];

      $form_state['values']['date'] = $request['date'];
      $form_state['values']['duration'] = $request['duration'];
    }
    
  }else {
    $form_state['values']['from_date'] = date('Y-m-d') . ' 00:00:00';
    $form_state['values']['to_date'] = date('Y-m-d') . ' 00:00:00';

    $form_state['values']['each'] = '1';
    $form_state['values']['day_week_month_year'] = 'week';
    $form_state['values']['on'] = 'nvt';
    $form_state['values']['day_of_the_week'] = 'nvt';

    $form_state['values']['date'] = date('Y-m-d') . ' 00:00:00';
    $form_state['values']['duration'] = '0:00';
  }
  
  // hidden form elements
  $form['request']['id'] = array('#type' => 'hidden','#value' => $id);
  
  $form['request']['cid'] = array('#type' => 'hidden','#value' => $cid);
  $form['request']['depid'] = array('#type' => 'hidden','#value' => $lrp->get_depid($cid)); 
  $form['request']['collids'] = array('#type' => 'hidden','#value' => $lrp->get_collids_only($cid));
  if($lrp->show_department_head($cid)){
    $form['request']['dphids'] = array('#type' => 'hidden','#value' => $lrp->get_dphids_only($cid));
  }else {
    $form['request']['dphids'] = array('#type' => 'hidden','#value' => array());
  }
  
  $form['request']['user_id'] = array('#type' => 'hidden','#value' => $user_id);   
  $form['request']['user_cid'] = array('#type' => 'hidden','#value' => $user_cid);   
  $form['request']['year'] = array('#type' => 'hidden','#value' => $year);
  
  $form['request']['leave_type'] = array
  (
    '#type' => 'select',
    '#title' => t('Leave type'),
    '#options' => $option_group['leave_request_leave_type'],
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
  
  if($lrp->is_department_head()){
    $form['request']['status'] = array
    (
      '#type' => 'select',
      '#title' => t('Status'),
      '#options' => $option_group['leave_request_status'],
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
  
  $form['request']['each'] = array
  (
    '#type' => 'textfield',
    '#title' => t('Each'),
    '#prefix' => '<div id="div-each">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['each']) and !empty($form_state['values']['each'])){
    $form['request']['each']['#default_value'] = $form_state['values']['each'];
  }
  
  $form['request']['day_week_month_year'] = array
  (
    '#type' => 'select',
    '#title' => t('Day / Week / Month / Year'),
    '#options' => $option_group['leave_request_day_week_month_year'],
    '#prefix' => '<div id="div-day_week_month_year">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['day_week_month_year']) and !empty($form_state['values']['day_week_month_year'])){
    $form['request']['day_week_month_year']['#default_value'] = $form_state['values']['day_week_month_year'];
  }
  
  $form['request']['on'] = array
  (
    '#type' => 'select',
    '#title' => t('On'),
    '#options' => $option_group['leave_request_on'],
    '#prefix' => '<div id="div-on">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  
  if(isset($form_state['values']['on']) and !empty($form_state['values']['on'])){
    $form['request']['on']['#default_value'] = $form_state['values']['on'];
  }
  
  $form['request']['day_of_the_week'] = array
  (
    '#type' => 'select',
    '#title' => t('Day of the week'),
    '#options' => $option_group['leave_request_day_of_the_week'],
    '#prefix' => '<div id="div-day_of_the_week">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
  
  if(isset($form_state['values']['day_of_the_week']) and !empty($form_state['values']['day_of_the_week'])){
    $form['request']['day_of_the_week']['#default_value'] = $form_state['values']['day_of_the_week'];
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

  foreach($form_elements as $name => $markup){
    if('display_name' != $name){
      $form[$name]['#markup'] = $markup;
    }
  }

  $form['js']['#markup'] .= <<<JS
    <script type="text/javascript">
      (function($) {
        $(document).ready(function() {
              
          $('#edit-leave-type').bind('change', function () {

            if('time_for_time' == $(this).val() || 'doctor_visit' == $(this).val() || 'normal_leave_less_one_day' == $(this).val() || 'sick_less_one_day' == $(this).val()){
              $('#div-from_date').hide();
              $('#div-to_date').hide();
      
              $('#div-each').hide();
              $('#div-day_week_month_year').hide();
              $('#div-on').hide();
              $('#div-day_of_the_week').hide();
            
              $('#div-date').show();
              $('#div-duration').show();
      
            }else if('mom_dad_day' == $(this).val() || 'study_leave' == $(this).val() || 'care' == $(this).val()){
              $('#div-from_date').show();
              $('#div-to_date').show();
      
              $('#div-each').show();
              $('#div-day_week_month_year').show();
              $('#div-on').show();
              $('#div-day_of_the_week').show();
            
              $('#div-date').hide();
              $('#div-duration').show();
      
            }else {
              $('#div-from_date').show();
              $('#div-to_date').show();
              
              $('#div-each').hide();
              $('#div-day_week_month_year').hide();
              $('#div-on').hide();
              $('#div-day_of_the_week').hide();
      
              $('#div-date').hide();
              $('#div-duration').hide();
      
              //$('#edit-duration').val('0:00');
            }
          });
          
          if('time_for_time' == $('#edit-leave-type').val() || 'doctor_visit' == $('#edit-leave-type').val() || 'normal_leave_less_one_day' == $('#edit-leave-type').val() || 'sick_less_one_day' == $('#edit-leave-type').val()){
            $('#div-from_date').hide();
            $('#div-to_date').hide();

            $('#div-each').hide();
            $('#div-day_week_month_year').hide();
            $('#div-on').hide();
            $('#div-day_of_the_week').hide();
      
            $('#div-date').show();
            $('#div-duration').show();
            
          }else if('mom_dad_day' == $('#edit-leave-type').val() || 'study_leave' == $('#edit-leave-type').val() || 'care' == $('#edit-leave-type').val()){
            $('#div-from_date').show();
            $('#div-to_date').show();

            $('#div-each').show();
            $('#div-day_week_month_year').show();
            $('#div-on').show();
            $('#div-day_of_the_week').show();

            $('#div-date').hide();
            $('#div-duration').show();
      
          }else {
            $('#div-from_date').show();
            $('#div-to_date').show();

            $('#div-each').hide();
            $('#div-day_week_month_year').hide();
            $('#div-on').hide();
            $('#div-day_of_the_week').hide();
      
            $('#div-date').hide();
            $('#div-duration').hide();

            //$('#edit-duration').val('0:00');
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
  $lr = new leaveregistration('drupal');
  $lr->set_contacts($form_state['values']['cid'], $form_state['values']['user_cid']);
  $lr->set_data(array($form_state['values']['year']));
  
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

      $form_state['values']['each'] = '1';
      $form_state['values']['day_week_month_year'] = 'week';
      $form_state['values']['on'] = 'nvt';
      $form_state['values']['day_of_the_week'] = 'nvt';
      
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
          //$errors['request'][] = t('There is already a ') . t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['time_for_time']['leave_type']]) . t(' on ') . $day . '-' . $month . '-' . $year . ' !';
          $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['time_for_time']['leave_type']])) . ' !';
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
              //$errors['request'][] = t('There is already a ') . t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']]) . t(' on ') . $day . '-' . $month . '-' . $year . ' !';
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']])) . ' !';
            }
            
            // already exists
            if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['id']){ // if leave_type exists
              //$errors['request'][] = t('There is already a ') . t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']]) . t(' on ') . $day . '-' . $month . '-' . $year . ' !';
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']])) . ' !';
            }
          }
          
          // check if colleage already request
          $collids = array_merge($form_state['values']['collids'], $form_state['values']['dphids']);
          foreach($collids as $collid){
            // is not holiday
            if(!isset($lr->data[$collid][$year][$month][$day]['holiday'])){

              // already exists, normal_leave
              if(isset($lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']) and $form_state['values']['id'] != $lr->data[$collid][$year][$month][$day]['normal_leave']['id']){ // if leave_type exists
                //drupal_set_message($lr->employees[$collid]['display_name'] . t(' has already a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . t(' on ') . $day . '-' . $month . '-' . $year . ' !');
                drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lr->employees[$collid]['display_name'] . t(' a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . ' !');
              }

              // already exists
              if(isset($lr->data[$collid][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$collid][$year][$month][$day]['request']['id']){ // if leave_type exists
                //drupal_set_message($lr->employees[$collid]['display_name'] . t(' has already a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['request']['leave_type']])) . t(' on ') . $day . '-' . $month . '-' . $year . ' !');
                drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lr->employees[$collid]['display_name'] . t(' a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['request']['leave_type']])) . ' !');
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
              //$errors['request'][] = t('There is already a ') . t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']]) . t(' on ') . $day . '-' . $month . '-' . $year . ' !';
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']])) . ' !';
            }
            
            // already exists
            if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['id']){ // if leave_type exists
              //$errors['request'][] = t('There is already a ') . t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']]) . t(' on ') . $day . '-' . $month . '-' . $year . ' !';
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']])) . ' !';
            }
          }
        }
    
        // check if colleage already request
        $collids = array_merge($form_state['values']['collids'], $form_state['values']['dphids']);
        foreach($collids as $collid){
          // is not holiday
          if(!isset($lr->data[$collid][$year][$month][$day]['holiday'])){
            
            // already exists, normal_leave
            if(isset($lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']) and $form_state['values']['id'] != $lr->data[$collid][$year][$month][$day]['normal_leave']['id']){ // if leave_type exists
              //drupal_set_message($lr->employees[$collid]['display_name'] . t(' has already a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . t(' on ') . $day . '-' . $month . '-' . $year . ' !');
              drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lr->employees[$collid]['display_name'] . t(' a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . ' !');
            }
            
            // already exists
            if(isset($lr->data[$collid][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$collid][$year][$month][$day]['request']['id']){ // if leave_type exists
              //drupal_set_message($lr->employees[$collid]['display_name'] . t(' has already a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['request']['leave_type']])) . t(' on ') . $day . '-' . $month . '-' . $year . ' !');
              drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lr->employees[$collid]['display_name'] . t(' a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['request']['leave_type']])) . ' !');
            }
          }
        }
      }
      break;
      
    case 'mom_dad_day':
    case 'study_leave':
    case 'care':
      // each
      $form_state['values']['each'] = trim($form_state['values']['each']);

      // if it is a whole number
      if(!preg_match('/^[0-9]{1,2}$/', $form_state['values']['each'], $matches)){
        form_set_error('each', t('Each has wrong value, must be a whole number, with maximum of 2 digits !'));
      }

      if('0' == $form_state['values']['each']){
        form_set_error('each', t('Each can`t be zero !'));

      }else {
        if('0' == substr($form_state['values']['each'], 0, 1)){
          form_set_error('each', t('First number can`t be zero !'));
        }
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
      
      // see leaveregistration.class.php function set_request
      // check already request
      $patterns = $lr->set_pattern($form_state['values'], 'request');
      
      foreach($patterns as $cid => $years){        
        foreach($years as $year => $months){
          foreach($months as $month => $days){
            foreach($days as $day => $pattern){
              
              // is not holiday
              if(!isset($lr->data[$cid][$year][$month][$day]['holiday'])){
                
                // is adjustments
                $duration = 0;
                if(isset($lr->data[$cid][$year][$month][$day]['adjustments']['duration']) and '' !== $lr->data[$cid][$year][$month][$day]['adjustments']['duration']){
                  $duration = $lr->data[$cid][$year][$month][$day]['adjustments']['duration'];

                }else if(isset($lr->data[$cid][$year][$month][$day]['overall_adjustments']['duration']) and '' !== $lr->data[$cid][$year][$month][$day]['overall_adjustments']['duration']){
                  $duration = $lr->data[$cid][$year][$month][$day]['overall_adjustments']['duration'];

                }else if(isset($lr->data[$cid][$year][$month][$day]['general']['duration']) and '' !== $lr->data[$cid][$year][$month][$day]['general']['duration']){
                  $duration = $lr->data[$cid][$year][$month][$day]['general']['duration'];
                }
                
                // if no adjustments
                if(0 != $duration){
                  
                  // already exists
                  if(isset($lr->data[$cid][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$cid][$year][$month][$day]['request']['id']){ // if leave_type exists
                    //$errors['request'][] = t('There is already a ') . t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$cid][$year][$month][$day]['request']['leave_type']]) . t(' on ') . $day . '-' . $month . '-' . $year . ' !';
                    $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']])) . ' !';
                  }
                }
              }
              
              // check if colleage already request
              $collids = array_merge($form_state['values']['collids'], $form_state['values']['dphids']);
              foreach($collids as $collid){
                // is not holiday
                if(!isset($lr->data[$collid][$year][$month][$day]['holiday'])){
                  
                  // is adjustments
                  $duration = 0;
                  if(isset($lr->data[$collid][$year][$month][$day]['adjustments']['duration']) and '' !== $lr->data[$collid][$year][$month][$day]['adjustments']['duration']){
                    $duration = $lr->data[$collid][$year][$month][$day]['adjustments']['duration'];

                  }else if(isset($lr->data[$collid][$year][$month][$day]['overall_adjustments']['duration']) and '' !== $lr->data[$collid][$year][$month][$day]['overall_adjustments']['duration']){
                    $duration = $lr->data[$collid][$year][$month][$day]['overall_adjustments']['duration'];

                  }else if(isset($lr->data[$collid][$year][$month][$day]['general']['duration']) and '' !== $lr->data[$collid][$year][$month][$day]['general']['duration']){
                    $duration = $lr->data[$collid][$year][$month][$day]['general']['duration'];
                  }

                  // if no adjustments
                  if(0 != $duration){
                    
                    // already exists, normal_leave
                    if(isset($lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']) and $form_state['values']['id'] != $lr->data[$collid][$year][$month][$day]['normal_leave']['id']){ // if leave_type don exists
                      //drupal_set_message($lr->employees[$collid]['display_name'] . t(' has already a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . t(' on ') . $day . '-' . $month . '-' . $year . ' !');
                      drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lr->employees[$collid]['display_name'] . t(' a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . ' !');
                    }
                    
                    // already exists
                    if(isset($lr->data[$collid][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$collid][$year][$month][$day]['request']['id']){ // if leave_type don exists
                      //drupal_set_message($lr->employees[$collid]['display_name'] . t(' has already a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['request']['leave_type']])) . t(' on ') . $day . '-' . $month . '-' . $year . ' !');
                      drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lr->employees[$collid]['display_name'] . t(' a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['request']['leave_type']])) . ' !');
                    }
                  }
                }
              }
            }
          }
        }
      }
      break;
      
    default:
      // to date is lower than from date
      if($form_state['values']['from_date'] > $form_state['values']['to_date']){
        form_set_error('to_date', t('To date must be higher than from date !'));
      }

      $form_state['values']['date'] = '1970-01-01 00:00:00'; // 1970-01-01 00:00:00
      $form_state['values']['each'] = '1';
      $form_state['values']['day_week_month_year'] = 'week';
      $form_state['values']['on'] = 'nvt';
      $form_state['values']['day_of_the_week'] = 'nvt';
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
              //$errors['request'][] = t('There is already a ') . t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']]) . t(' on ') . $day . '-' . $month . '-' . $year . ' !';
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['normal_leave']['leave_type']])) . ' !';
            }
          }
          
        }else {
          // already exists
          if(isset($lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['id']){
            // if no adjustments
            if(0 != $duration){
              //$errors['request'][] = t('There is already a ') . t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']]) . t(' on ') . $day . '-' . $month . '-' . $year . ' !';
              $errors['request'][] = t('On ') . $day . '-' . $month . '-' . $year . t(' you have a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$form_state['values']['cid']][$year][$month][$day]['request']['leave_type']])) . ' !';
            }
          }
        }
        
        // check if colleage already request
        $collids = array_merge($form_state['values']['collids'], $form_state['values']['dphids']);
        
        foreach($collids as $collid){
          // is not holiday
          if(!isset($lr->data[$collid][$year][$month][$day]['holiday'])){
            
            // is adjustments
            $duration = 0;
            if(isset($lr->data[$collid][$year][$month][$day]['adjustments']['duration']) and '' !== $lr->data[$collid][$year][$month][$day]['adjustments']['duration']){
              $duration = $lr->data[$collid][$year][$month][$day]['adjustments']['duration'];

            }else if(isset($lr->data[$collid][$year][$month][$day]['overall_adjustments']['duration']) and '' !== $lr->data[$collid][$year][$month][$day]['overall_adjustments']['duration']){
              $duration = $lr->data[$collid][$year][$month][$day]['overall_adjustments']['duration'];

            }else if(isset($lr->data[$collid][$year][$month][$day]['general']['duration']) and '' !== $lr->data[$collid][$year][$month][$day]['general']['duration']){
              $duration = $lr->data[$collid][$year][$month][$day]['general']['duration'];
            }

            // if no adjustments
            if(0 != $duration){
              
              // already exists, normal_leave
              if(isset($lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']) and $form_state['values']['id'] != $lr->data[$collid][$year][$month][$day]['normal_leave']['id']){ // if leave_type don exists
                //drupal_set_message($lr->employees[$collid]['display_name'] . t(' has already a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . t(' on ') . $day . '-' . $month . '-' . $year . ' !');
                drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lr->employees[$collid]['display_name'] . t(' a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['normal_leave']['leave_type']])) . ' !');
              }

              // already exists
              if(isset($lr->data[$collid][$year][$month][$day]['request']['leave_type']) and $form_state['values']['id'] != $lr->data[$collid][$year][$month][$day]['request']['id']){ // if leave_type don exists
                //drupal_set_message($lr->employees[$collid]['display_name'] . t(' has already a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['request']['leave_type']])) . t(' on ') . $day . '-' . $month . '-' . $year . ' !');
                drupal_set_message(t('On ') . $day . '-' . $month . '-' . $year . t(' has ') . $lr->employees[$collid]['display_name'] . t(' a ') . strtolower(t($lr->option_group['leave_request_leave_type']['options'][$lr->data[$collid][$year][$month][$day]['request']['leave_type']])) . ' !');
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

function leaveregistration_request_submit($form, &$form_state)
{      
  $values = array();
  
  switch($form_state['values']['status'])
  {
    case 'approved':
    case 'rejected':
      $values['status_id'] = '2';
      $values['assignee_contact_id'] = $form_state['values']['cid'];
      break;
    
    default:
      $values['status_id'] = '1';
      $values['assignee_contact_id'] = $form_state['values']['depid'];
  }
  
  $values['target_contact_id'] = $form_state['values']['cid'];
  
  $values['leave_type'] = $form_state['values']['leave_type'];
  $values['reason'] = $form_state['values']['reason'];
  $values['status'] = $form_state['values']['status'];

  $values['from_date'] = $form_state['values']['from_date'];
  $values['to_date'] = $form_state['values']['to_date'];
  $values['date'] = $form_state['values']['date'];
  
  $values['each'] = $form_state['values']['each'];
  $values['day_week_month_year'] = $form_state['values']['day_week_month_year'];
  $values['on'] = $form_state['values']['on'];
  $values['day_of_the_week'] = $form_state['values']['day_of_the_week'];
  
  $values['duration'] = $form_state['values']['duration'];
  
  $lr = new leaveregistration('drupal');
    
  $error = false;
  if(!empty($form_state['values']['id'])){
    if($id = $lr->update($form_state['values']['id'], $form_state['values']['cid'], $values)){
      drupal_set_message(t('The request is updated !'), 'status');
      
    }else{
      form_set_error('request', t('A database error occured when updating a request !'));
      $error = true;
    }
    
  }else {
    // create
    if($id = $lr->create($form_state['values']['cid'], $values)){    
      drupal_set_message(t('The request is created !'), 'status');
      
    }else{
      form_set_error('request', t('A database error occured when creating a request !'));
      $error = true;
    }
  }
  
  if(!$error){
    // mail
    global $language;
    $lr->set_contacts($form_state['values']['cid'], true, true, true);

    // Send one email to yourself
    $params = array();
    $params = array_merge($form_state['values'], array('employee_display_name' => $lr->employees[$form_state['values']['cid']]['display_name']));
    $params = array_merge($params, array('id' => $id));
    $params = array_merge($params, array('option_group' => $lr->option_group));

    if(!empty($form_state['values']['id'])){
      drupal_mail('leaveregistration', 'change', $lr->employees[$form_state['values']['cid']]['email'], $language->language, $params);
    }else {
      drupal_mail('leaveregistration', 'request', $lr->employees[$form_state['values']['cid']]['email'], $language->language, $params);
    }

    // Send one email to department head, expect if it is yourself
    foreach($lr->department_heads as $cid => $department_head){
      if($form_state['values']['cid'] != $cid and $form_state['values']['user_cid'] != $cid){
        $params = array();
        $params = array_merge($form_state['values'], array('employee_display_name' => $lr->employees[$form_state['values']['cid']]['display_name']));
        $params = array_merge($params, array('departement_head_display_name' => $department_head['display_name']));
        $params = array_merge($params, array('id' => $id));
        $params = array_merge($params, array('option_group' => $lr->option_group));
        
        if(!empty($form_state['values']['id'])){
          drupal_mail('leaveregistration', 'change', $department_head['email'], $language->language, $params);
        }else {
          drupal_mail('leaveregistration', 'request', $department_head['email'], $language->language, $params);
        }
      }
    }

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
  global $user;
  $user_cid = leaveregistration_get_cid($user->uid);
  
  $cids = array();
  if(isset($_GET['cid']) and '' != $_GET['cid']){
    $cid = $_GET['cid'];
    $cids[] = $cid;
  }
    
  $years = array();  
  if(isset($_GET['year']) and '' != $_GET['year']){
    $year = $_GET['year'];
    $years[] = $_GET['year']-1;
    $years[] = $_GET['year'];  
  }
  
  if(empty($cids)){
    drupal_set_message( t('No contact id`s !'), 'error');
    return false;
  }
  
  if(empty($years)){
    drupal_set_message( t('No years !'), 'error');
    return false;
  }
  
  if('' == $year){
    drupal_set_message( t('No year !'), 'error');
    return false;
  }
  
  if(0 == $user_cid or '' == $user_cid){
    drupal_set_message( t('No user contact id !'), 'error');
    return false;
  }
  
  if(0 == $user_id or '' == $user_id){
    drupal_set_message( t('No user id !'), 'error');
    return false;
  }
  
  $data = array
  (    
    'error_platform' => 'drupal',
    
    'cids' => $cids,
    'user_cid' => $user_cid,
    'user_id' => $user_id,
    
    'years' => $years,
    'months' => array(),
    'cid' => $cid,
        
    'year' => $year,
    'month' => 'all',
    
    'type' => 'form',
    'return' => 'return',
    
    'elements' => array('display_name')
  );
  
  $lrp = new CRM_Leaveregistration_Page_LeaveRegistration();
  $form_elements = $lrp->run($data);

  if(empty($form_elements)){
    drupal_set_message( t('No form elements !'), 'error');
    return $form;
  }
  
  $form['#attributes']['class'][] = 'leaveregistration';
  
  $form['display_name'] = array
  (
    '#markup' => '<h1>' . $form_elements['display_name'] . '</h1>'
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
    '#title' => t('Delete')
  );
  
  // hidden form elements
  $form['request']['cid'] = array('#type' => 'hidden','#value' => $cid);    
  $form['request']['user_id'] = array('#type' => 'hidden','#value' => $user_id); 
  $form['request']['year'] = array('#type' => 'hidden','#value' => $year);
  $form['request']['id'] = array('#type' => 'hidden','#value' => $id);
  
  $lr = new leaveregistration('drupal');
  $data = $lr->get($id);
    
  $form['request']['msg'] = array();  
  $form['request']['msg']['#markup'] = t('Leave type: ') . t($lr->option_group['leave_request_leave_type']['options'][$data['leave_type']]) . '<br />';
  
  if('time_for_time' == $data['leave_type'] or 'doctor_visit' == $data['leave_type'] or 'normal_leave_less_one_day' == $data['leave_type'] or 'sick_less_one_day' == $data['leave_type']){
    $form['request']['msg']['#markup'] .= t('Date: ') . strftime("%d-%m-%Y", strtotime($data['date'])) . '<br />';
    $form['request']['msg']['#markup'] .= t('Duration: ') . $data['duration'] . '<br />';
        
  }else if('mom_dad_day' == $data['leave_type'] or 'study_leave' == $data['leave_type'] or 'care' == $data['leave_type']){
    $form['request']['msg']['#markup'] .= t('From date: ') . strftime("%d-%m-%Y", strtotime($data['from_date'])) . '<br />';
    $form['request']['msg']['#markup'] .= t('To date: ') . strftime("%d-%m-%Y", strtotime($data['to_date'])) . '<br />';
    $form['request']['msg']['#markup'] .= t('Each: ') . $data['each'] . ' ' . t($lr->option_group['leave_request_day_week_month_year']['options'][$data['day_week_month_year']]);
    if('nvt' != $data['on']){
      $form['request']['msg']['#markup'] .= ' ' . t('on') . ' ' . t($lr->option_group['leave_request_on']['options'][$data['on']]);
    }
    if('nvt' != $data['day_of_the_week']){
      $form['request']['msg']['#markup'] .= ' ' . t($lr->option_group['leave_request_day_of_the_week']['options'][$data['day_of_the_week']]);
    }
    
    $form['request']['msg']['#markup'] .= ' <br />';
    
  }else {
    $form['request']['msg']['#markup'] .= t('From date: ') . strftime("%d-%m-%Y", strtotime($data['from_date'])) . '<br />';
    $form['request']['msg']['#markup'] .= t('To date: ') . strftime("%d-%m-%Y", strtotime($data['to_date'])) . '<br />';
  }
  
  if(!empty($data['reason'])){
    $form['request']['msg']['#markup'] .= ' <br />';
    $form['request']['msg']['#markup'] .= t('Reason: ');
    $form['request']['msg']['#markup'] .= ' <br />';
    $form['request']['msg']['#markup'] .= $data['reason'];
    $form['request']['msg']['#markup'] .= ' <br />';
  }
    
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
  $lr = new leaveregistration('drupal');
  
  if($lr->delete($form_state['values']['id'])){
    drupal_set_message(t('The request is deleted !'), 'status');
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
    
    switch($key)
    {
      case 'request':
        $message['subject'] = t('Leave registration - There is a request on ') . variable_get('site_name', '');

        if(isset($params['departement_head_display_name'])){
          $message['body'][] = t('Dear') . ' ' . $params['departement_head_display_name'];
          $message['body'][] = t('The leave request of ') . $params['employee_display_name'] . t(' is added.');

        }else {
          $message['body'][] = t('Dear') . ' ' . $params['employee_display_name'];
          $message['body'][] = t('Your leave request is added.');
        }
        
        $message['body'][] = '';
        
        $message['body'][] = t('Leave type: ') . t($params['option_group']['leave_request_leave_type']['options'][$params['leave_type']]);
        
        switch($params['leave_type'])
        {
          case 'time_for_time':
          case 'doctor_visit':
          case 'normal_leave_less_one_day':
          case 'sick_less_one_day':
            $message['body'][] = t('Date: ') . strftime("%d-%m-%Y", strtotime($params['date']));
            $message['body'][] = t('Duration: ') . $params['duration'];
            break;
          
          case 'mom_dad_day':
          case 'study_leave':
          case 'care':
            $message['body'][] = t('From date: ') . strftime("%d-%m-%Y", strtotime($params['from_date']));
            $message['body'][] = t('To date: ') . strftime("%d-%m-%Y", strtotime($params['to_date']));
            $message['body'][] = t('Each: ') . t($params['each']) . ' ' . t($params['day_week_month_year']);
            if('nvt' != $params['on']){
              $message['body'][count($message['body'])-1] .= ' ' . t('on') . ' ' . t($params['option_group']['leave_request_on']['options'][$params['on']]);
            }
            if('nvt' != $params['day_of_the_week']){
              $message['body'][count($message['body'])-1] .= ' ' . t($params['day_of_the_week']);
            }
            break;

          default:
            $message['body'][] = t('From date: ') . strftime("%d-%m-%Y", strtotime($params['from_date']));
            $message['body'][] = t('To date: ') . strftime("%d-%m-%Y", strtotime($params['to_date']));
        }
        
        $message['body'][] = t('The status of the leave request is: ') . t($params['option_group']['leave_request_status']['options'][$params['status']]);
        
        if(!empty($params['reason'])){
          $message['body'][] = t('Reason: ');
          $message['body'][] = $params['reason'];
        }
        
        $message['body'][] = '';
                
        // link to request
        $message['body'][] = t('Click the link below to go to the leave request.');
        $message['body'][] = $base_url . $base_path . 'user/' . $params['user_id'] .'/leave/request/' . $params['id'] .'/change?year=' . $params['year'] .'&cid=' . $params['cid'];
        
        // login first link
        $message['body'][] = t('If you are not logged in, you must log in first. Click here to login.');
        $message['body'][] = $base_url . $base_path . 'user';
        
        $message['body'][] = '';
        
        $message['body'][] = t('Yours sincerely ') . variable_get('site_name', '');

        break;

      case 'change':
        $message['subject'] = t('Leave registration - there is a change on ') . variable_get('site_name', '');

        if(isset($params['departement_head_display_name'])){
          $message['body'][] = t('Dear') . ' ' . $params['departement_head_display_name'];
          $message['body'][] = t('The leave request of ') . $params['employee_display_name'] . t(' is changed.');

        }else {
          $message['body'][] = t('Dear') . ' ' . $params['employee_display_name'];
          $message['body'][] = t('Your leave request is changed.');
        }

        $message['body'][] = '';
        
        $message['body'][] = t('Leave type: ') . t($params['option_group']['leave_request_leave_type']['options'][$params['leave_type']]);
        
        switch($params['leave_type'])
        {
          case 'time_for_time':
          case 'doctor_visit':
          case 'normal_leave_less_one_day':
          case 'sick_less_one_day':
            $message['body'][] = t('Date: ') . strftime("%d-%m-%Y", strtotime($params['date']));
            $message['body'][] = t('Duration: ') . $params['duration'];
            break;

          case 'mom_dad_day':
            $message['body'][] = t('From date: ') . strftime("%d-%m-%Y", strtotime($params['from_date']));
            $message['body'][] = t('To date: ') . strftime("%d-%m-%Y", strtotime($params['to_date']));
            $message['body'][] = t('Each: ') . t($params['each']) . ' ' . t($params['day_week_month_year']);
            if('nvt' != $params['on']){
              $message['body'][count($message['body'])-1] .= ' ' . t('on') . ' ' . t($params['option_group']['leave_request_on']['options'][$params['on']]);
            }
            if('nvt' != $params['day_of_the_week']){
              $message['body'][count($message['body'])-1] .= ' ' . t($params['day_of_the_week']);
            }
            break;

          default:
            $message['body'][] = t('From date: ') . strftime("%d-%m-%Y", strtotime($params['from_date']));
            $message['body'][] = t('To date: ') . strftime("%d-%m-%Y", strtotime($params['to_date']));
        }      
        
        $message['body'][] = t('The status of the leave request is: ') . t($params['option_group']['leave_request_status']['options'][$params['status']]);
        
        if(!empty($params['reason'])){
          $message['body'][] = t('Reason: ');
          $message['body'][] = $params['reason'];
        }
        
        $message['body'][] = '';
                
        // link to request
        $message['body'][] = t('Click the link below to go to the leave request.');
        $message['body'][] = $base_url . $base_path . 'user/' . $params['user_id'] .'/leave/request/' . $params['id'] .'/change?year=' . $params['year'] .'&cid=' . $params['cid'];
        
        // login first link
        $message['body'][] = t('If you are not logged in, you must log in first. Click here to login.');
        $message['body'][] = $base_url . $base_path . 'user';
        
        $message['body'][] = '';
        
        $message['body'][] = t('Yours sincerely ') . variable_get('site_name', '');

        break;
    }
    
    // attachment
    if('approved' == $params['status'] and 'time_for_time' != $params['leave_type']){
      
      $body = leaveregistration_ical_body($params['user_id'], $params['id'], $params['cid'], $params['year']);
      
      if (!$handle = fopen('php://temp/calendar(' . $params['id'] . ').ics', 'w')) {
        drupal_set_message(t('Cannot open calendar.ics file !'), 'error');
        fclose($handle);        
        return false;
      }

      // Write $somecontent to our opened file.
      if (fwrite($handle, $body) === FALSE) {
        drupal_set_message(t('Cannot write calendar.ics file !'), 'error');
        fclose($handle);
        return false;
      }
      fclose($handle);
      
      $params['attachments'][] = array(
        'filepath' => 'php://temp/calendar(' . $params['id'] . ').ics',
        'filecontent' => $body,
        'filename' => 'calendar.ics',
        'filemime' => 'text/calendar',
      );
    }
  }
}

function leaveregistration_ical_date_time($timestamp) {
  return date('Ymd\THis\Z', $timestamp);
}

function leaveregistration_ical_date($timestamp) {
  return date('Ymd', $timestamp);
}

function leaveregistration_ical_escape($string) {
  return preg_replace('/([\,;])/','\\\$1', $string);
}

function leaveregistration_ical($form, &$form_state, $user_id, $id)
{
  // VCALENDAR
  header('Content-type: text/calendar; charset=utf-8');
  header('Content-Disposition: attachment; filename=calendar.ics');
  
  $body = leaveregistration_ical_body($user_id, $id, $_GET['cid'], $_GET['year']);
  
  echo($body);
  exit();
}

function leaveregistration_ical_body($user_id, $id, $cid, $year){
  global $base_url;
  
  $lr = new leaveregistration('drupal');
  $lr->set_contacts($cid, $user_id);
  $lr->set_data(array($year));
  
  $data = $lr->get($id);
  
  // description
  $description = '';  
  $description .= t('Leave type: ') . t($lr->option_group['leave_request_leave_type']['options'][$data['leave_type']]) . '\n\r';
  
  switch($data['leave_type']){
    case 'time_for_time':
    case 'doctor_visit':
    case 'normal_leave_less_one_day':
    case 'sick_less_one_day':
      $description .=  t('Date: ') . strftime("%d-%m-%Y", strtotime($data['date'])) . '\n\r';
      $description .=  t('Duration: ') . $data['duration'] . '\n\r';
      break;

    case 'mom_dad_day':
    case 'study_leave':
    case 'care':
      $description .=  t('From date: ') . strftime("%d-%m-%Y", strtotime($data['from_date'])) . '\n\r';
      $description .=  t('To date: ') . strftime("%d-%m-%Y", strtotime($data['to_date'])) . '\n\r';
      $description .=  t('Each: ') . $data['each'] . ' ' . t($lr->option_group['leave_request_day_week_month_year']['options'][$data['day_week_month_year']]);

      if('nvt' != $data['on']){
        $description .=  ' ' . t('on') . ' ' . t($lr->option_group['leave_request_on']['options'][$data['on']]);
      }
      if('nvt' != $data['day_of_the_week']){
        $description .=  ' ' . t($lr->option_group['leave_request_day_of_the_week']['options'][$data['day_of_the_week']]);
      }
      $description .=  '\n\r';
      break;

    default:
      $description .=  t('From date: ') . strftime("%d-%m-%Y", strtotime($data['from_date'])) . '\n\r';
      $description .=  t('To date: ') . strftime("%d-%m-%Y", strtotime($data['to_date'])) . '\n\r';
  }
  
  if(!empty($data['reason'])){
    $description .=  '\n\r';
    $description .=  t('Reason: ');
    $description .=  '\n\r';
    $description .=  $data['reason'];
    $description .=  '\n\r';
  }
    
  // body
  $location = 'Bosgoed Company';
  $summary = t('Leave') . ' - ' . $lr->employees[$cid]['display_name'] . ' - ' . t($lr->option_group['leave_request_leave_type']['options'][$data['leave_type']]);
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
      $patterns = $lr->set_pattern($data, 'request');

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