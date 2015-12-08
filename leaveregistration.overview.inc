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
    'error_id' => 'leaveregistration_overview',
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
  
  $form['dephead_admin_link']['#markup'] = $lrp->get_dephead_admin_link();
  $form['year']['#markup'] = $lrp->get_year_form();
  $form['admin_col_links']['#markup'] = $lrp->get_admin_col_links_form();
  $form['dephead_col_links']['#markup'] = $lrp->get_dephead_col_links_form();
  $form['dephead_request']['#markup'] = $lrp->get_dephead_request_form();
  $form['display_name']['#markup'] = $lrp->get_display_name();
  $form['request']['#markup'] = $lrp->get_request_form();
  $form['legend']['#markup'] = $lrp->get_legend_form();
  $form['credit']['#markup'] = $lrp->get_credit_form();
  $form['dephead_months']['#markup'] = $lrp->get_dephead_months_form();
  $form['dephead_show_colleagues']['#markup'] = $lrp->get_dephead_show_colleagues_form();
  $form['dephead_calendar_year']['#markup'] = $lrp->get_dephead_calendar_year_form();
  $form['months']['#markup'] = $lrp->get_months_form();
  $form['show_colleagues']['#markup'] = $lrp->get_show_colleagues_form();
  $form['calendar_year']['#markup'] = $lrp->get_calendar_year_form();
      
  return $form;
}
?>