#include"SIM800HTTP.h"
boolean sBearer = false;
boolean sHTTP = false;

SIM800HTTP::SIM800HTTP(int x){

}

boolean SIM800HTTP::begin(int baudRate){
   Serial.begin(baudRate);
   delay(5000);
   Serial.flush();
}

boolean SIM800HTTP::configGprs(String apn){
    if(!executeCommand("AT")){
        return false;
    }
    if(!executeCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"")){
        return false;
    }
    if(!executeCommand("AT+SAPBR=3,1,\"APN\",\""+apn+"\"")){
        return false;
    }
    if(!executeCommand("AT+CGATT=1")){
        return false;
    }
    return true;
}

String SIM800HTTP::getLocation(){
	String comm = "AT+CIPGSMLOC=1,1";
    
	Serial.println(comm);
    String res = getResponse();
	res = getResponse();
	
	if(contains(res,"ERROR")){
       return "Não foi possivel pegar localização";
    }
	return res;
}

boolean SIM800HTTP::openBearer(){
	if(sBearer == true){
		return false;
	}
	String comm = "AT+SAPBR=1,1";
    Serial.println(comm);   
	String res = getResponse();
    
    if(contains(res,"OK")){
       sBearer = true;
	   return true; 
    }
     
    delay(1500);
	return false;
}

boolean SIM800HTTP::closeBearer(){
    if(sBearer == false){
		return false;
	}
	String comm = "AT+SAPBR=0,1";
	Serial.println(comm);   
	String res = getResponse();
		
	if(contains(res,"OK")){
		sBearer = false;
		return true; 
	}

	return false;
}

boolean SIM800HTTP::httpInit(){
   if(sHTTP == true){
		return false;
   }
  
   String comm = "AT+HTTPINIT";
   Serial.println(comm);   
   String res = getResponse();
    
   if(contains(res,"OK")){
       sHTTP = true;
	   return true; 
   }
   
   return false;
}

boolean SIM800HTTP::httpTerminate(){
	String comm = "AT+HTTPTERM";
	Serial.println(comm); 
	
	String res = getResponse();

	if(contains(res,"OK")){
		sHTTP = false;
		return true; 
	}

	return false;
}

boolean SIM800HTTP::setURL(String url){
    if(!executeCommand("AT+HTTPPARA=\"URL\",\""+url+"\"")){
        return false;
    }
    return true;
}

boolean SIM800HTTP::setContent(String body, String contentType){
    if(!executeCommand("AT+HTTPPARA=\"CONTENT\",\""+contentType+"\"")){
        return false;
    }	
    if(!executeCommand("AT+HTTPDATA="+String(body.length())+",1500")){
        return false;
    }
    Serial.print(body);
	String res = getResponse();
    return true;
}

boolean SIM800HTTP::httpPost(){
    Serial.println("AT+HTTPACTION=1");
    String res = getResponse();
    if(contains(res,"OK")){    
      res = getResponse();    
    }
	return true;
}

String SIM800HTTP::httpGet(){
    Serial.println("AT+HTTPACTION=0");
	String res = getResponse();
	Serial.println("AT+HTTPREAD");
	res = getResponse();
	return res;
}

boolean SIM800HTTP::contains(String res, String s){
	if(res.indexOf(s) > 0){
		return true;
	}
	return false;
}

boolean SIM800HTTP::executeCommand(String comm){
    Serial.println(comm);
    String res = getResponse();
    if(contains(res,"OK")){
       return true;  
    }
	if(contains(res,"DOWNLOAD")){
       return true;
    }
	if(contains(res,"ERROR")){
       return false;
    }
}

String SIM800HTTP::getResponse(){
  String res = "";
  while(true){ 
	  if (Serial.available()){
		res = Serial.readString();
		res.trim();
		return res;
	  }
  }
}