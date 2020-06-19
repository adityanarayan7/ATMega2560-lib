#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define baudrate 9600UL

char details[700] = "";
int details_length = 0;
char p[] = {'g', 'o', 'o', 'd', '\0'};



void USART2_init()
{ 
  unsigned int ubrr_value = (((F_CPU / (baudrate * 16UL))) - 1UL);
  Serial.println(ubrr_value);
  UBRR2H = (unsigned char)(ubrr_value >> 8);
  UBRR2L = (unsigned char)ubrr_value;
  UCSR2A = 0x00;
  UCSR2B = (1<<RXEN2) | (1<<TXEN2) | (1<<RXCIE2);
  UCSR2C = 0x2E;
 
}


void USART_Transmit(char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSR2A & (1<<UDRE2)) );
/* Put data into buffer, sends the data */
UDR2 = data;
}


void transmitData( char st[])
{ 
  int x = 0;
  while(st[x] != '\0')
  {
    USART_Transmit(st[x]);
    x++;
    
  }
}









ISR(USART2_RX_vect)
{ 
  
  //noInterrupts();
  UCSR2B &= 0x7F;
  Serial.println("entered int");
  digitalWrite(LED_BUILTIN, HIGH);
  details_length = 0;
  char c = '@';
  int i = 0;
  while( c != '\n' )
  {
    while ( !(UCSR2A & (1<<RXC2)) ); // wait till new data arrives
    //Serial.print(char(c));
    c = UDR2;
    details[i] = c;
    i++;
  }
  details_length = i;
  details[i] = '\0';
  Serial.println(details);
  digitalWrite(LED_BUILTIN, LOW);
  UCSR2B |= 0x80; 
  //interrupts();
}




void setup()
{ 
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  noInterrupts();
  USART2_init();
  interrupts();
}



void loop()
{
  transmitData(p);
  Serial.println("sent data.");
  delay(5000);
}
