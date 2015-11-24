<?php
function leaveregistration_overview($form, &$form_state, $user_id)
{ 
  // open ical if approved
  if(isset($_SESSION['leaveregistration']['ical']) and !empty($_SESSION['leaveregistration']['ical'])){
    global $base_url, $base_path;
    echo('<script type="text/javascript">window.open(\'' . $base_url . $base_path . 'user/' . $_SESSION['leaveregistration']['ical']['user_id'] . '/leave/request/' . $_SESSION['leaveregistration']['ical']['id'] . '/ical/?year=' . $_SESSION['leaveregistration']['ical']['year'] . '&cid=' . $_SESSION['leaveregistration']['ical']['cid'] . '\',\'New window\');</script>');
    
    $_SESSION['leaveregistration']['ical'] = array(); // empty 
    unset($_SESSION['leaveregistration']['ical']); // delete
  }
  
  global $user;  
  $user_cid = leaveregistration_get_cid($user->uid);
  
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
    
  if(empty($cid)){
    drupal_set_message( t('No contact id !'), 'error');
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
    
    'cid' => $cid,
    'user_cid' => $user_cid,
    'user_id' => $user_id,
        
    'years' => $years,
    'months' => array(),
        
    'year' => $year,
    'month' => 'all',
    
    'type' => 'form',
    'return' => 'return',
    'elements' => array('department_head_administration_link', 'year', 'administration_collids', 'department_head_collids', 'department_head_request', 'display_name', 'request', 'legend_leave_type', 'credit', 'show_colleagues', 'calendar_year')
  );
  
  $lrp = new CRM_Leaveregistration_Page_LeaveRegistration();
  $form_elements = $lrp->run($data);

  if(empty($form_elements)){
    drupal_set_message( t('No form elements !'), 'error');
    return $form;
  }
  
  $form['#attributes']['class'][] = 'leaveregistration';
    
  foreach($form_elements as $name => $markup){
    $form[$name]['#markup'] = $markup;
  }  
    
  return $form;
}
?>
