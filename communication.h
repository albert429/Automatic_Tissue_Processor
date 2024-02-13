#ifndef COMMUNICATION_H
#define COMMUNICATION_H

//HANDSHAKE to be implemented
//theta motor to be implemented

char GoToZComplement()
{
   output_high(PIN_D2);
   delay_ms(15);
   if(input(PIN_C3))
   {
      output_low(PIN_D2);
      return 0;   
   }
   
   output_low(PIN_D2);
   return 1; //error
}

char GoToNextX()
{   
   //START
   output_high(PIN_D1);
   delay_ms(50);
   if(input(PIN_D0))
   {
      output_low(PIN_D1);
      return 0;  
   }
   output_low(PIN_D1);
   return 1;
}

char nextTheta()
{
   output_high(PIN_A1);
   delay_ms(50);
   if(input(PIN_A2))
   {
      output_low(PIN_A1);
      return 0;
   }
   output_low(PIN_A1);
   return 1;
}
#endif  // KEYPAD_H
