/*! 
\file WT3_Travel.c
\brief Contains functions specific to Travel booking functionality in WebTours app (hence the WT3_ prefix).

	// WT3_T20_Travel_Data()
	// WT3_T21_Travel_Home()
	// WT3_T22_Travel_Search_Flight()
	// WT3_T23_Travel_Flight_Lookup()
	// WT3_T24_Find_Flight()
	// WT3_T25_Travel_Payment_Details()
	// WT3_T26_Travel_Invoice()
	// WT3_T27_Travel_Click_Book_Another()
	// WT3_T33_Travel_Check_Itinerary()

*/

WT3_T20_Travel_Data(){
	int x;
	
	lr_save_datetime("%m/%d/%Y", DATE_NOW       			,"parm_departDate"); // 04/14/2015 format
	lr_save_datetime("%m/%d/%Y", DATE_NOW + (7 * ONE_DAY)	,"parm_returnDate"); // Hard coded 7 days?
	
	// Do 80% round-trip based on random selection:
	x=rand() % 100; // random number between 0 and 100 (using % modulo operator).
	if( x >= 80 ){
		lr_save_string("on","parm_roundtrip");
	}else{
		lr_save_string("<OFF>","parm_roundtrip"); // one-way.
	} // TODO: Record and use an attribute to control variation "<OFF>" to "on".

	lr_save_string("1","parm_numPassengers");
	lr_save_string("None","parm_seatPref");
	lr_save_string("Coach","parm_seatType");

	return 0;
}

WT3_Travel(){ // call from within Action.c.
	int rc=LR_PASS; int i=0;

	WT3_T20_Travel_Data();
	
	// Search UseCase
	// Book UseCase
	// Itinerary UseCase
	// No check-in UseCase
	
	if( stricmp("All"   ,LPCSTR_UseCase ) == FOUND
	||  stricmp("Search",LPCSTR_UseCase ) == FOUND
	||  stricmp("Book"  ,LPCSTR_UseCase ) == FOUND
	){
			lr_save_string("WT3_T22_Travel_Search_Flight","pTransName");
			web_reg_find("Text=Find Flight", "Fail=NotFound", LAST);
			rc=WT3_T22_Travel_Search_Flight();
			if( rc != LR_PASS ){ return rc; }
	}


	if( stricmp("Search",LPCSTR_UseCase ) == FOUND
	){
			//lr_save_string("WT3_T22_Travel_Search_Flight","pTransName");
			//web_reg_find("Text=Find Flight", "Fail=NotFound", LAST);
			//rc=WT3_T22_Travel_Search_Flight();
			//if( rc != LR_PASS ){ return rc; }

			lr_save_string("WT3_T23_Travel_Flight_Lookup","pTransName");
			web_reg_find("Text=Flight departing from", "Fail=NotFound", LAST);
			rc=WT3_T23_Travel_Flight_Lookup();
			if( rc != LR_PASS ){ return rc; }
			
			lr_save_string("WT3_T24_Find_Flight","pTransName");
			web_reg_find("Text=Payment Details", "Fail=NotFound", LAST);
			rc=WT3_T24_Find_Flight();
			if( rc != LR_PASS ){ return rc; }
	}


	if( stricmp("All",LPCSTR_UseCase ) == FOUND
	||  stricmp("Book",LPCSTR_UseCase ) == FOUND
	){

		// TODO: C int variable to specify Loops through several flights before checkout.
		// for(i=1; i<=1; i++){ // 1,2,3 (3 times)
			
			lr_save_string("WT3_T23_Travel_Flight_Lookup","pTransName");
			web_reg_find("Text=Flight departing from", "Fail=NotFound", LAST);
			rc=WT3_T23_Travel_Flight_Lookup();
			if( rc != LR_PASS ){ return rc; }
			
			lr_save_string("WT3_T24_Find_Flight","pTransName");
			web_reg_find("Text=Payment Details", "Fail=NotFound", LAST);
			rc=WT3_T24_Find_Flight();
			if( rc != LR_PASS ){ return rc; }
			
			lr_save_string("WT3_T25_Travel_Payment_Details","pTransName");
			web_reg_find("Text=Invoice", "Fail=NotFound", LAST);
			 rc=WT3_T25_Travel_Payment_Details();
			if( rc != LR_PASS ){ return rc; }
			
			//rc=WT3_T26_Travel_Invoice(); // does not send data to server.

			lr_save_string("WT3_T27_Travel_Click_Book_Another","pTransName");
			web_reg_find("Text=Find Flight", "Fail=NotFound", LAST);
			rc=WT3_T27_Travel_Click_Book_Another();
			if( rc != LR_PASS ){ return rc; }
				
		// }//for(i=1; i<2; i++){ // 1,2,3 (3 times)
	}

	if( stricmp("All",LPCSTR_UseCase ) == FOUND
	||  stricmp("Book",LPCSTR_UseCase ) == FOUND
	||  stricmp("Itinerary",LPCSTR_UseCase ) == FOUND
	||  stricmp("Cancel",LPCSTR_UseCase ) == FOUND
	){
		lr_save_string("WT3_T33_Travel_Check_Itinerary","pTransName");
		rc=WT3_T33_Travel_Check_Itinerary(); // Itineraries_count is updated inside.	
		if( rc != LR_PASS ){ return rc; }
	}

	if( Itineraries_count <= 0 ){
		return LR_PASS;
	}
	
		if( stricmp("All",LPCSTR_UseCase ) == FOUND
		||  stricmp("Cancel",LPCSTR_UseCase ) == FOUND
   		){
			lr_save_string("WT3_T34_Cancel_Itinerary","pTransName");
			rc=WT3_T34_Cancel_Itinerary();			
			if( rc != LR_PASS ){ return rc; }
			
			// Because the menu frame doesn't appear:
			lr_save_string("WT3_T03_URL_Landing_T","pTransName");
	 		rc=WT3_URL_Landing(); // just for establishing state to signin or invoke run conitions.
		 	if( rc != LR_PASS ){ return rc; } 
		 	
			lr_save_string("WT3_T07_SignIn_T","pTransName");
			rc=WT3_SignIn();
		 	if( rc != LR_PASS ){ return rc; } 
		}

/*
	if( stricmp("All",LPCSTR_UseCase ) == FOUND
	||  stricmp("Home",LPCSTR_UseCase ) == FOUND
	){
		lr_save_string("WT3_T21_Travel_Home","pTransName");
		rc=WT3_T21_Travel_Home();
		if( rc != LR_PASS ){ return rc; }
	}
*/

	return rc;
}


WT3_T21_Travel_Home(){
	int rc=LR_PASS;	
	int i;
	for(i=1; i < iRequestRetries; i++){ // 5 times retry: 1,2,3,4,5
		wi_retry_add_time( i );

		web_reg_find("Text=Welcome","Fail=NotFound","SaveCount=Found_count", LAST );
		wi_start_transaction();
		web_image("Home Button", 
			"Alt=Home Button", 
			"Snapshot=t32.inf", 
			LAST);

		if( atoi( lr_eval_string("{Found_count}") ) >= 1 ){
			rc=LR_PASS;
			rc=wi_end_transaction(rc);
			break; // out of loop.			
		}else{
			// cycle through loop again to retry.
			rc=LR_FAIL; // Fall-out of loop when retries are exhausted.
			rc=wi_end_transaction(rc);
		}
	} 	

	return rc;
} //WT3_T21_Travel_Home

			
WT3_T22_Travel_Search_Flight(){ // Find Flight is the response heading.
	int rc=LR_PASS;
	int i;
	for(i=1; i < iRequestRetries; i++){ // 5 times retry: 1,2,3,4,5
		wi_retry_add_time( i );

		web_reg_find("Text=Find Flight","Fail=NotFound","SaveCount=Found_count", LAST );
		
		// TODO: Capture list returned to identify the ordinal on list and its values.

		wi_start_transaction();
		// Click "Fights" button on Home screen after login:
		web_image("Search Flights Button", 
			"Alt=Search Flights Button", 
			"Snapshot=t33.inf", 
			LAST);
		rc=wi_end_transaction(rc);
		if( atoi( lr_eval_string("{Found_count}") ) >= 1 ){
			rc=LR_PASS;
			break; // out of loop.			
		}else{
			// cycle through loop again to retry.
			rc=LR_FAIL; // Fall-out of loop when retries are exhausted.
		}
	} 	

	return rc;
} //WT3_T22_Travel_Search_Flight
	
WT3_T23_Travel_Flight_Lookup(){
	int rc=LR_PASS;
	int i;
	for(i=1; i < iRequestRetries; i++){ // 5 times retry: 1,2,3,4,5
		wi_retry_add_time( i );

		web_reg_find("Text=Flight departing from","Fail=NotFound","SaveCount=Found_count", LAST );

		// Capture Response Text for use in next transaction: name="outboundFlight" value="820;1441;08/12/2015"
		web_reg_save_param_ex("ParamName=outboundFlight"
		,"LB=name=\"outboundFlight\" value=\""
		,"RB=\"", "Ordinal=ALL","SaveLen=-1"
		,SEARCH_FILTERS,"Scope=body",LAST );
			// 		,"DFEs=UrlEncoding"

		web_reg_save_param_ex("ParamName=returnFlight","NotFound=warning"
		,"LB=name=\"returnFlight\" value=\""
		,"RB=\"", "Ordinal=ALL","SaveLen=-1"
		,SEARCH_FILTERS,"Scope=body",LAST );

		//TODO: WT3_T23_Travel_Flight_Lookup Add Airport starting and endeing route in this function.
		wi_start_transaction();
		web_submit_form("reservations.pl",
			"Snapshot=t34.inf",
			ITEMDATA,
			"Name=depart"				, "Value={WT3_Flights_DepartCity}", ENDITEM,
			"Name=departDate"			, "Value={parm_departDate}", ENDITEM,
			"Name=arrive"				, "Value={WT3_Flights_ArriveCity}", ENDITEM,
			"Name=returnDate"			, "Value={parm_returnDate}", ENDITEM,
			"Name=numPassengers"		, "Value={parm_numPassengers}", ENDITEM,
			"Name=roundtrip"			, "Value={parm_roundtrip}", ENDITEM,
			"Name=seatPref"				, "Value={parm_seatPref}", ENDITEM,
			"Name=seatType"				, "Value={parm_seatType}", ENDITEM,
			"Name=findFlights.x"		, "Value=40", ENDITEM,
			"Name=findFlights.y"		, "Value=6", ENDITEM,
			LAST);
	    rc=wi_end_transaction(rc); // 

	    if( atoi( lr_eval_string("{Found_count}") ) >= 1 ){
	    	
			wi_startPrintingTrace();
	    	lr_output_message(">> {outboundFlight}=%s", lr_eval_string("{outboundFlight_1}") ); // hard coded first one.
	    	lr_output_message(">> {returnFlight}=%s", lr_eval_string("{returnFlight_1}") ); // hard coded first one.
			wi_resetPrinting();

			rc=LR_PASS;
			break; // out of loop.			
		}else{
			// cycle through loop again to retry.
			rc=LR_FAIL; // Fall-out of loop when retries are exhausted.
		}
	} 	
	return rc;
} //WT3_T23_Travel_Flight_Lookup
	
WT3_T24_Find_Flight(){
	int rc=LR_PASS;
	int i; int x; char temp[32];
	for(i=1; i < iRequestRetries; i++){ // 5 times retry: 1,2,3,4,5
		wi_retry_add_time( i );

		web_reg_find("Text=Payment Details","Fail=NotFound","SaveCount=Found_count", LAST );

		// If MSO_SLoad="on", HTTP Status 503 (System Cannot Complete Request)
		// is issued for the % time specified in MSO_ServerLoadProb.

		// Select flight option randomly rather than always the first row:
		x=( rand() % 3 ) + 1; // random number between 0 and 3 (for 0,1,2,3 = 4 values).
		// Assemble lr_ parm name based on index number that starts from 1.
		sprintf( temp, "{outboundFlight_%d}", x);
		// lr_save_string(lr_eval_string("{outboundFlight_1}"),"outboundFlight_x");
		lr_save_string(lr_eval_string(temp),"outboundFlight_x");

		wi_start_transaction();
		// Using parm_roundtrip defined in WT3_T20_Travel_Data(){
		if( strcmp( "on",lr_eval_string("{parm_roundtrip}") ) == FOUND ){

			x=( rand() % 3 ) + 1; // random number between 0 and 3 (for 0,1,2,3 = 4 values).
			sprintf( temp, "{returnFlight_%d}", x);
			lr_save_string(lr_eval_string(temp),"returnFlight_x");
		
			web_submit_form("reservations.pl_1", 
				"Snapshot=t35.inf", 
				ITEMDATA, 
				"Name=outboundFlight", "Value={outboundFlight_x}", ENDITEM, 
				"Name=returnFlight", "Value={returnFlight_x}", ENDITEM, 
				"Name=reserveFlights.x", "Value=46", ENDITEM, 
				"Name=reserveFlights.y", "Value=11", ENDITEM, 
				LAST);
		}else{ // "<OFF>" = One way trip:
			web_submit_form("reservations.pl_1",
				"Snapshot=t35.inf", 
				ITEMDATA, 
				"Name=outboundFlight", "Value={outboundFlight_x}", ENDITEM, 
				"Name=reserveFlights.x", "Value=46", ENDITEM, 
				"Name=reserveFlights.y", "Value=11", ENDITEM, 
				LAST);
		}
		rc=wi_end_transaction(rc);

	    if( atoi( lr_eval_string("{Found_count}") ) >= 1 ){
			rc=LR_PASS;
			break; // out of loop.			
		}else{
			// cycle through loop again to retry.
			rc=LR_FAIL; // Fall-out of loop when retries are exhausted.
		}
	} 	
	return rc;
} // WT3_T24_Find_Flight

WT3_T25_Travel_Payment_Details(){
	int rc=LR_FAIL; // Unless positive if returned by sub-functions.
	int i;
	for(i=1; i<iRequestRetries; i++){ // 5 times retry: 1,2,3,4,5
		wi_retry_add_time( i );
		
		// Passenger Names: concatenate:
		lr_param_sprintf("parm_pass1","%s %s",lr_eval_string("{parm_userid}"),lr_eval_string("{global_unique_id}") );
			
		web_reg_find("Text=Invoice",LAST); // Also "Thank you for booking"
		web_reg_find("Text=Server Database Error" ,"SaveCount=DBErr_count", LAST );

		wi_start_transaction();
	
		// TODO: If one sample record is found invalid, skip to the next record set.
	
		web_submit_form("reservations.pl_3", 
			"Snapshot=t36.inf", 
			ITEMDATA, 
			"Name=firstName"			,"Value={parm_userid}", ENDITEM,
			"Name=lastName"				,"Value={global_unique_id}", ENDITEM, 
			"Name=address1"				,"Value={SignUp_address1}", ENDITEM,
			"Name=address2"				,"Value={SignUp_address2}", ENDITEM,
			"Name=pass1"				,"Value={parm_pass1}", ENDITEM, 
			"Name=creditCard","Value=12345678", ENDITEM, 
			"Name=expDate", "Value=15/16", ENDITEM, 
			"Name=saveCC", "Value=<OFF>", ENDITEM, 
			"Name=buyFlights.x", "Value=41", ENDITEM, 
			"Name=buyFlights.y", "Value=9", ENDITEM, 
			LAST);
 	    // These values are validated only if MSO_JSVerify="on".

  	    // if run condition MSO_SLoad="on" and selected by probability:
 	    if( atoi( lr_eval_string("{DBErr_count}") ) > 0 ){ // DBErr found:
				//lr_user_data_point(lr_eval_string("{pTransName}"),1); // to track retries occuring due to DBErr.
				lr_user_data_point("WT3_T25_Travel_Payment_Details_retries",i); // to track retries occuring due to DBErr.
				// Continue to loop again to handle error with retry (not end script run)
			rc=LR_FAIL;
			rc=wi_end_transaction(rc);
 			break;
  	    }else{ // normal no error:
			rc=LR_PASS;
			rc=wi_end_transaction(rc);
			break; // out of do/while loop.
  	    }		
	}
	
	return rc;
} //WT3_T25_Travel_Payment_Details

WT3_T26_Travel_Invoice(){
	int rc=LR_PASS;
	lr_start_transaction("WT3_T26_Travel_Invoice");

	// Clicking on Invoice does not cause request to server.
	
	lr_end_transaction("WT3_T26_Travel_Invoice",LR_AUTO);
	return rc;
} //WT3_T26_Travel_Invoice
	
WT3_T27_Travel_Click_Book_Another(){
	int rc=LR_PASS;
	web_reg_find("Text=Find Flight", 
		LAST);
	wi_start_transaction();
	web_submit_form("reservations.pl_4", 
		"Snapshot=t37.inf", 
		ITEMDATA, 
		"Name=Book Another.x", "Value=43", ENDITEM, 
		"Name=Book Another.y", "Value=6", ENDITEM, 
		LAST);
	rc=wi_end_transaction(rc);
	return rc;
} //WT3_T27_Travel_Click_Book_Another
	
	
WT3_T33_Travel_Check_Itinerary(){
	int rc=LR_PASS;
	int i;
	for(i=1; i < iRequestRetries; i++){ // 5 times retry: 1,2,3,4,5
		wi_retry_add_time( i );

		web_reg_find("Text=Itinerary","Fail=NotFound","SaveCount=Found_count", LAST );
//		web_reg_find("Text=No flights have been reserved","Fail=NotFound","SaveCount=No_count", LAST );

		// TODO: Add count of itinerary items returned, as this may impact response time.
		web_reg_save_param_ex(
		    "ParamName=Itineraries", 
		    "LB=flightID\" value=\"","RB=\"",
		    "Ordinal=all",
		    "NotFound=warning",
		    SEARCH_FILTERS,
	        "Scope=body", LAST);

		wi_start_transaction();
		web_image("Itinerary Button", 
			"Alt=Itinerary Button", 
			"Snapshot=t42.inf", 
			LAST);
		
	    if( atoi( lr_eval_string("{Found_count}") ) >= 1 ){ // Itinerary title found.
		 // if( atoi( lr_eval_string("{No_count}") ) != 1 ){ // Itineraries found:
		 // lr_paramarr_len("Itineraries");
		    if( atoi( lr_eval_string("{Itineraries_count}") ) > 0 ){ // Itineraries found:
				Itineraries_count = atoi( lr_eval_string("{Itineraries_count}") );
				wi_startPrintingTrace();
				lr_output_message(">> Itineraries_count=%d, 1st=%s."
				                  ,Itineraries_count
				                  ,lr_eval_string("{Itineraries_1}")
								  );
				wi_resetPrinting();
			}
			rc=LR_PASS;
			rc=wi_end_transaction(rc);
			break; // out of loop.			
		}else{
			// cycle through loop again to retry.
			rc=LR_FAIL; // Fall-out of loop when retries are exhausted.
			rc=wi_end_transaction(rc);
		}
	}

	return rc;
}//WT3_T33_Travel_Check_Itinerary

// Thanks to Joel Splatsky at http://www.joelonsoftware.com/articles/fog0000000319.html
char* mystrcat( char* dest, char* src )
{
     while (*dest) dest++;
     while (*dest++ = *src++);
     return --dest;
}

WT3_T34_Cancel_Itinerary(){
	int rc=LR_PASS;
	int i; int n; int x;
	char     charDisplay[1000];
	char     charRequest[1000];
	char *p =charRequest;
	
	//  n=atoi( lr_eval_string("{Itineraries_count}"));
	    n= Itineraries_count; // lr_paramarr_len("Itineraries");
	
	if( n <= 0 ){ return LR_PASS; } // Do below only there were itineraries listed in previous step WT3_T33_Travel_Check_Itinerary.
    	// Loop through itineraries to build a request:

		charRequest[0] = '\0'; // begin array for use by mystrcat().
		for(x=1; x <= n; x++){ // loop through itineraries array:
			// add itineraries[x] to string, (snprintf() not available within LR to do bounds checking):
			// such as "1=on&flightID=210297416-788-jC"
			sprintf(charDisplay,"%d=on&flightID=%s",x,lr_paramarr_idx("Itineraries",x));
			// lr_output_message(">> [%d] %s",n,charDisplay);
			if( x > 1){ // TODO: Replace use of mystrcat with lr_param_sprintf.
				p = mystrcat(p,"&");
			}
 				p = mystrcat(p,charDisplay);
		}
				p = mystrcat(p,"&removeFlights.x=53&removeFlights.y=2");
				wi_startPrintingTrace();
				lr_output_message(">> p=%s",charRequest);
				wi_resetPrinting();

		for(x=1; x<=n; x++){ // loop through itineraries array:
			sprintf(charDisplay,"&.cgifields=%d",x);
			
				wi_startPrintingTrace();
			lr_output_message(">> [%d] %s",n,charDisplay);
				wi_resetPrinting();

			p = mystrcat(p,charDisplay);
		}

		lr_save_string(charRequest,"haha"); // convert string to parameter for use in web_custom_request().
			lr_output_message(">> Body=%s",lr_eval_string("{haha}"));

			for(x=1; i < iRequestRetries; i++){ // 5 times retry: 1,2,3,4,5
				wi_retry_add_time( i );

				// Only if all have been cancelled does this message appear:
 				web_reg_find("Text=No flights have been reserved","Fail=NotFound","SaveCount=Found_count", LAST );

				// count of Itineraries
				wi_start_transaction();
				web_custom_request("cancel_itinerary.pl", "Method=POST",
					"URL={WebToursPath}/cgi-bin/itinerary.pl",
					"Body={haha}",
					"TargetFrame=_SELF",LAST);
					// Instead of:
					//web_submit_form("itinerary.pl", 
					//	"Snapshot=t103.inf",
					//	ITEMDATA, 
					//	"Name=1", "Value=on", ENDITEM, 
					//	LAST);
			
			    if( atoi( lr_eval_string("{Found_count}") ) >= 1 ){
					rc=LR_PASS;
					rc=wi_end_transaction(rc);
					break; // out of loop.			
				}else{
					// cycle through loop again to retry.
				}
				rc=LR_FAIL; // Fall-out of loop when retries are exhausted.
				rc=wi_end_transaction(rc);

			} // for(x=1; i < iRequestRetries; i++)
	// FIXME: Response should include menu frame to click home button.
	return rc;
}//WT3_T34_Cancel_Itinerary	