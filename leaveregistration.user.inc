<?php
/**
 * Implementation of hook_user_insert
 * 
 * Update civicrm contact whit sub contact type employee based on the drupal employee role
 * 
 * @param type $edit
 * @param type $account
 * @param type $category
 */
function leaveregistration_user_insert(&$edit, $account, $category)
{
  $email = $account->mail;
  $roles = array();
  foreach($account->roles as $rid => $name){
    $role = user_role_load($rid);
    $roles[$rid] = $role->name;
  }

  $results = civicrm_api("Contact","get", array ('version' => '3','sequential' =>'1', 'email' => $email));
  
  if (in_array('employee', $roles)) {
     $results = civicrm_api("Contact","update", array ('version' => '3','sequential' =>'1', 'id' => $results['values'][0]['contact_id'], 'contact_sub_type' =>'Employee'));
  }
}

/**
 * Implementation of hook_user_update
 * 
 * Update civicrm contact whit sub contact type employee based on the drupal employee role
 * 
 * @param type $edit
 * @param type $account
 * @param type $category
 */
function leaveregistration_user_update(&$edit, $account, $category)
{
  $email = $account->mail;
  $roles = array();
  foreach($account->roles as $rid => $name){
    $role = user_role_load($rid);
    $roles[$rid] = $role->name;
  }

  $results = civicrm_api("Contact","get", array ('version' => '3','sequential' =>'1', 'email' => $email));

  if (in_array('employee', $roles)) {
     $results = civicrm_api("Contact","update", array ('version' => '3','sequential' =>'1', 'id' => $results['values'][0]['contact_id'], 'contact_sub_type' =>'Employee'));
  }
}
?>
