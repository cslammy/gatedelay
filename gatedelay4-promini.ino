
const int GATEIN_PIN = 9;
const int GATESTATUS_PIN = 13;
int LENGTH_PIN = A0;
int DELAY_PIN = A1;

int CYCLE = 0;

int SKIP_DELAY = 0;
int SKIP_LENGTH = 0;

const int GATEOUT_PIN = 5;
const int EOGL_PIN = 2;  // length pot
const int EOGD_PIN = 3;   // delay pot

boolean EOGL_SW = HIGH;
boolean EOGD_SW = HIGH;
boolean GATEIN = LOW;
boolean GATEIN2 = LOW;
boolean GATEIN3 = LOW;
boolean GATESTATUS = LOW;
int x = 0;

unsigned long EOGL = 0;  //value of EOGL pot must be UL compared to time
unsigned long EOGD = 0;  // value of EOGD pot, ditto.


//timer variables
unsigned long begin_delay=0;
unsigned long begin_length=0;
unsigned long current_delay = 0;
unsigned long current_length = 0;
unsigned long test_delay = 0;
unsigned long test_length =0;

void setup() {
  // put your setup code here, to run once:
pinMode(GATEIN_PIN,INPUT);
pinMode(DELAY_PIN,INPUT);
pinMode(LENGTH_PIN,INPUT);
pinMode(EOGL_PIN,INPUT);
pinMode(EOGD_PIN,INPUT);



pinMode(GATESTATUS_PIN,OUTPUT);
pinMode(GATEOUT_PIN,OUTPUT);

digitalWrite(GATEOUT_PIN,LOW);

Serial.begin(9600);


} // end setup





void getstatus() {
    
    GATESTATUS = digitalRead(GATEIN_PIN);
    if (GATESTATUS == HIGH)
      {
        digitalWrite(GATESTATUS_PIN,HIGH);
      }
     else
      { 
        digitalWrite(GATESTATUS_PIN,LOW); 
      }
}  // end getstatus ()




void loop() 

{
//Serial.println("*****BEGINNING MAIN LOOP!*****")  ;



GATEIN = digitalRead(GATEIN_PIN);
delay(3);

//Serial.println("==========================CYCLE===========================");
//Serial.println(CYCLE);

if (GATEIN == LOW)
   {
    CYCLE = 0;

   }
   
if ((GATEIN == HIGH) && (CYCLE < 2 ))

      {
      CYCLE++;
      
      getstatus();
      begin_delay = millis();
      EOGD = 100;
      test_delay = 0;


/*
           Serial.println("OUTSIDE EOGD WHILE");
           Serial.println ("OUTSIDE test_delay is:");
           Serial.println (test_delay);
           Serial.println ("OUTSIDE EOGD is:");
           Serial.println (EOGD);
*/
   
      while ((EOGD > test_delay) && (SKIP_DELAY == 0)) 
           {
            getstatus();
           
            digitalWrite(GATEOUT_PIN,LOW);
            EOGD = (analogRead(DELAY_PIN) * 10);
            delay(3);
            current_delay = millis();
            test_delay = current_delay - begin_delay;    
      

     /*

           Serial.println("****INSIDE EOGD WHILE****");
           Serial.println ("INSIDE testdelay value is:");
           Serial.println (test_delay);
           Serial.println ("INSIDE EODG value is:");
           Serial.println (EOGD);
*/
           
  

          // check for EOGD_SW high and gate low.If that's the case skip rest of delay cycle.
          GATEIN2 = digitalRead(GATEIN_PIN);
          EOGD_SW = digitalRead(EOGD_PIN);


          if ((EOGD_SW == HIGH) && (SKIP_DELAY == 0) && (GATEIN2 == LOW))
              { 
                Serial.println("you are in SKIP delay");
               SKIP_DELAY = 5;
              }


          Serial.print("EOGD_SW value:  ");
          Serial.println(EOGD);
          Serial.print("SKIP_DELAY value:  ");
          Serial.println(SKIP_DELAY);
          Serial.print("GATEIN2 value:  ");
          Serial.println(GATEIN2);
          
    
                
           } // end EODG while 
         

         digitalWrite(GATEOUT_PIN,HIGH);    
           
 //DELAY IS OVER, GATE IS HIGH, NOW MOVE ON TO LENGTH  
          
          
          begin_length = millis();
          EOGL = 100;
          test_length = 0;
  
           /*
           Serial.println("OUTSIDE EOGL WHILE");
           Serial.println ("testlength is:");
           Serial.println (test_length);
           Serial.println ("EOGL is:");
           Serial.println (EOGL);
          */
 while ((EOGL > test_length)  && (SKIP_LENGTH == 0))
           {
            getstatus();
             EOGL = (analogRead(LENGTH_PIN) * 10);
            delay(3);   
             current_length = millis();      
            test_length = current_length - begin_length;        

           /*
           Serial.println ("INSIDE testlength is:");
           Serial.println (test_length);
           Serial.println ("INSIDE EOGL is:");
           Serial.println (EOGL);
           */
             // check status of EOGL switch and GATE  
  
        
// check for EOGL_SW high and gate low.If that's the case skip rest of length cycle.
          GATEIN3 = digitalRead(GATEIN_PIN);
          EOGL_SW = digitalRead(EOGL_PIN);
          if ((EOGL_SW == HIGH) && (SKIP_LENGTH == 0) && (GATEIN3 == LOW))
              { 
               SKIP_LENGTH = 5;
              Serial.println("You are in SKIP_LENGTH");
              }
           

             }           //end EOGL while

              digitalWrite(GATEOUT_PIN,LOW);
              delay(3);
              getstatus();
              delay(3);


      }    // end main loop IF 
         else
         {
   //      Serial.println("----------------GATE IS OFF---------------");
         digitalWrite(GATEOUT_PIN,LOW);
         
         // once it's off, stay off dammit!
         
         CYCLE ++;
         if (CYCLE > 6)
         {
          CYCLE = 2;
         }
         }

SKIP_DELAY = 0;
SKIP_LENGTH = 0;
GATEIN2 = HIGH;
GATEIN3 = HIGH;
        
} // end main loop
