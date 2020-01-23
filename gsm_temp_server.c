#include<avr/io.h>
#define F_CPU 16000000UL
#include<util/delay.h>
#include<iot.h>


unsigned int x=0;

void ADC_data()
{
    x=ADC_read(2);
	x=x*4;
	x=x/10;
}

  
void SMS(unsigned int val)
{

    uart_string("AT+CMGF=1\r\n");
	_delay_ms(1000);
	uart_string("AT+CMGS=\"9821511509\"\r\n");
	_delay_ms(1000);
	uart_string("ALERT!!!\r\n");
	uart_string("The current temperature at your home is: ");
	uart_number(val);
	uart_string(" deg celsius\r\n");
	_delay_ms(300);
	uart_send(0x1A);
	_delay_ms(300);
}

void main()
{
	DDRA=0xf0;	//adc-data read
	ADC_init();
	uart_init();

	while(1)
	{
		PORTA=0x00;
		ADC_data();
    	uart_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
		_delay_ms(5000);
		uart_string("AT+CIPSEND\r\n");
		_delay_ms(5000);
		uart_string("GET https://api.thingspeak.com/update?api_key=DTE0564CUU6HI7WW&field1=0");
		uart_number(x);
		uart_string("\r\n");
		_delay_ms(300);
		uart_send(0x1A);
		_delay_ms(300);
		uart_string("AT+CIPSHUT\r\n");
		_delay_ms(5000);


		while(x>35)
		{
		    PORTA=0x80;
			SMS(x);
			uart_string("AT+CIPMUX=0\r\n");
			_delay_ms(5000);
			uart_string("AT+CIPCSGP=1,\"www\"\r\n");
			_delay_ms(5000);
    		uart_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
			_delay_ms(5000);
			uart_string("AT+CIPSEND\r\n");
			_delay_ms(5000);
			uart_string("GET https://api.thingspeak.com/update?api_key=DTE0564CUU6HI7WW&field1=0");
			uart_number(x);
			uart_string("\r\n");
			_delay_ms(300);
			uart_send(0x1A);
			_delay_ms(300);
			uart_string("AT+CIPSHUT\r\n");
			_delay_ms(5000);
        	ADC_data();
		 }
          
	}
}
