#ifndef SIM800HTTP_H
#define SIM800HTTP_H
#include <Arduino.h>

class SIM800HTTP{
public:
   SIM800HTTP(int x);
   boolean  begin(int baudRate);
   boolean  configGprs(String apn);
   boolean  openBearer();
   boolean  closeBearer();
   boolean  httpInit();
   boolean  httpTerminate();
   boolean  setURL(String url);
   boolean  setContent(String body, String contentType);
   boolean  httpPost();
   String  httpGet();     
   String   getLocation();   
private:
   boolean sBearer;
   boolean sHTTP;
   boolean contains(String res, String s);
   String getResponse();
   boolean executeCommand(String comm);
};

#endif