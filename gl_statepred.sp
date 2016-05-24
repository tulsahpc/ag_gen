state predicate = 
assets :
attacker;
webserver;
printer;
	
assertions :
	! quality:printer,ready_message,READY;
	topology:webserver,printer,connected;
.
  
  
