//Setup Variables
int ledPin = 13; //store the first pin number

//Setup(): runs once to initialize the board
void setup() 
{
  digitalPinMode(ledPin, 1); //set LED pin to output  
}



//Loop Variables

//Loop(): runs repeatedly
void loop()
{ 
  digitalWritePin(ledPin, HIGH); //turn LED on
  delay(1000); //delay 1 second
  digitalWritePin(ledPin, LOW); //turn LED off
  delay(1000); //delay 1 second
}



//digitalPinMode(): set mode of digital pin
void digitalPinMode(uint8_t pinNumber, int dataDirection)
{
  if (pinNumber > 7) //if pin is in Port B
  {
    pinNumber = pinNumber - 8; //convert pin number to Port B pin value (8 -> 0, etc...)

    if (dataDirection == 0) //if pin is to be input
    {
      DDRB = DDRB & (0 << pinNumber); //set pin to input    
    }
    else //else pin is to be output
    {
      DDRB = DDRB | (1 << pinNumber); //set pin to output
    }     
  }
  else //else pin is in Port D
  {
    if (dataDirection == 0) //if pin is to be input
    {
      DDRD = DDRD & (0 << pinNumber); //set pin to input    
    }
    else //else pin is to be output
    {
      DDRD = DDRD | (1 << pinNumber); //set pin to output
    }
  }
}



//digitalWritePin(): write to a digital pin
void digitalWritePin(uint8_t pinNumber, int pinSetting)
{
  if (pinNumber > 7) //if pin is in Port B
  {
    pinNumber = pinNumber - 8; //convert pin number to Port B pin value (8 -> 0, etc...)    
  
    if (pinSetting == LOW) //if pin is to be LOW
    {
      PORTB = PORTB & (pinSetting << pinNumber); //set pin to LOW
    }
    else //else pin is to be HIGH
    {
      PORTB = PORTB | (pinSetting << pinNumber); //set pin to HIGH
    } 
  }
  else //else pin is in Port D
  {
    if (pinSetting == LOW) //if pin is to be LOW
    {
      PORTD = PORTD & (pinSetting << pinNumber); //set pin to LOW
    }
    else //else pin is to be HIGH
    {
      PORTD = PORTD | (pinSetting << pinNumber); //set pin to HIGH
    } 
  }



//digitalReadPin(): read from a digital pin
int digitalReadPin(uint8_t pinNumber)
{
  if (pinNumber > 7) //if pin is in Port B
  {
    pinNumber = pinNumber - 8; //convert pin number to Port B pin value (8 -> 0, etc...)    

    if (PINB & (1 << pinNumber) == LOW) //if pin is in LOW state
    {
      return LOW; //something happened  
    }
    else //else pin is in HIGH state
    {
      return HIGH; //nothing happened
    }
  }
  else //else pin is in Port D
  {
    if (PIND & (1 << pinNumber) == LOW) //if pin is in LOW state
    {
      return LOW; //something happened  
    }
    else //else pin is in HIGH state
    {
      return HIGH; //nothing happened
    }
  }  
}

