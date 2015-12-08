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