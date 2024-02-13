#include <16F877A.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset

#use delay(crystal=4000000)
#use FIXED_IO( A_outputs=PIN_A0 )

#define LED   PIN_A0
#define LCD_ENABLE_PIN PIN_C5
#define LCD_RS_PIN PIN_C4
#define LCD_RW_PIN PIN_C6
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7
#include <lcd.c>

//Define keypad pins
#define ROW1 PIN_B7
#define ROW2 PIN_B6
#define ROW3 PIN_B5
#define ROW4 PIN_B4
#define COL1 PIN_B3
#define COL2 PIN_B2
#define COL3 PIN_B1
#define COL4 PIN_B0
#include "communication.h"

void keypadinit()
{
   
  // initialize GPIO for the keypad
    output_high(ROW1);
    output_high(ROW2);
    output_high(ROW3);
    output_high(ROW4);
    port_b_pullups(1);
}

void ledcheck()
{
   //led
    output_high(PIN_A0);
    delay_ms(1000);
    output_low(PIN_A0);
    delay_ms(1000);
}

void lcdDisplayMenu(){
     lcd_gotoxy(1, 1);
     printf(lcd_putc,"1: Start");
     lcd_gotoxy(1, 2);
     printf(lcd_putc,"2: Test");
     lcd_gotoxy(21, 1);
     printf(lcd_putc,"3: Settings");
     lcd_gotoxy(21, 2);
     printf(lcd_putc,"4: More");
}

char keypadScan(char key)
{
   int count=0;  
    output_low(ROW1);
    if (!input(COL1))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL1))
            count++;
      }
      if(count>50)
      {  key= '1';   }  
    }
    count=0;
    output_high(ROW1);
///    
    output_low(ROW2);
    if (!input(COL1))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL1))
            count++;
      }
      if(count>50)
      {  key= '4';   }  
    }
    count=0;
    output_high(ROW2);   
///        
    output_low(ROW3);
    if (!input(COL1))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL1))
            count++;
      }
      if(count>50)
      {  key= '7';   }  
    }
    count=0;
    output_high(ROW3);
///       
    output_low(ROW4);
    if (!input(COL1))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL1))
            count++;
      }
      if(count>50)
      {  key= '*';   }  
    }
    count=0;
    output_high(ROW4);
 /////////////////////////////////////////   
    output_low(ROW1);
    if (!input(COL2))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL2))
            count++;
      }
      if(count>50)
      {  key= '2';   }  
    }
    count=0;
    output_high(ROW1);
////    
    output_low(ROW2);
    if (!input(COL2))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL2))
            count++;
      }
      if(count>50)
      {  key= '5';   }  
    }
    count=0;
    output_high(ROW2);
///      
    output_low(ROW3);
    if (!input(COL2))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL2))
            count++;
      }
      if(count>50)
      {  key= '8';   }  
    }
    count=0;
    output_high(ROW3);
////       
    output_low(ROW4);
    if (!input(COL2))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL2))
            count++;
      }
      if(count>50)
      {  key= '0';   }  
    }
    count=0;
    output_high(ROW4);
////////////////////////////////////////////////       
    output_low(ROW1);
    if (!input(COL3))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL3))
            count++;
      }
      if(count>50)
      {  key= '3';   }  
    }
    count=0;
    output_high(ROW1);
    
    output_low(ROW2);
    if (!input(COL3))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL3))
            count++;
      }
      if(count>50)
      {  key= '6';   }  
    }
    count=0;
    output_high(ROW2);
    
        
    output_low(ROW3);
    if (!input(COL3))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL3))
            count++;
      }
      if(count>50)
      {  key= '9';   }  
    }
    count=0;
    output_high(ROW3);
    
        
    output_low(ROW4);
    if (!input(COL3))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL3))
            count++;
      }
      if(count>50)
      {  key= '#';   }  
    }
    count=0;
    output_high(ROW4);    
/////////////////////////////////////////   
     output_low(ROW1);
    if (!input(COL4))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL4))
            count++;
      }
      if(count>50)
      {  key= 'A';   }  
    }
    count=0;
    output_high(ROW1);
    
    output_low(ROW2);
    if (!input(COL4))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL4))
            count++;
      }
      if(count>50)
      {  key= 'B';   }  
    }
    count=0;
    output_high(ROW2);
    
        
    output_low(ROW3);
    if (!input(COL4))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL4))
            count++;
      }
      if(count>50)
      {  key= 'C';   }  
    }
    count=0;
    output_high(ROW3);
    
        
    output_low(ROW4);
    if (!input(COL4))
    {
      for(int i=0; i<100 ;i++)
      {
         if (!input(COL4))
            count++;
      }
      if(count>50)
      {  key= 'D';   }  
    }
    count=0;
    output_high(ROW4);     
    
    return key;
}

void main()
{  
    keypadinit();
    lcd_init();
    ledcheck();
    
    char key=0;
    
    //communic
    output_low(PIN_D1);
    output_low(PIN_A1);
    output_low(PIN_D2);
    lcd_putc("\fReady...\n");
 
    lcd_putc("\f");

   while(TRUE)
   {   
     key='e';
     lcdDisplayMenu();
     key = keypadScan(key);
     if (key != 'e') {
            lcd_putc("\f");
            switch (key) {
                case '1':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "Starting...");
                    delay_ms(1000);
                    //TO BE CONTINUED
                    break;

                case '2':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "Testing...");
                    delay_ms(1000);
                 ///theta test
                 lcd_gotoxy(1, 1);
                    printf(lcd_putc, "Test theta");
                    delay_ms(1000);
                    char error2 = nextTheta();
                    if(error2)
                    {
                        lcd_gotoxy(12, 1);
                        printf(lcd_putc, "error");
                    }
                    else
                    {
                        lcd_gotoxy(12, 1);
                        printf(lcd_putc, "DONE!!!");   
                    }
                    delay_ms(1000);
                  /////X test
                  lcd_gotoxy(1, 2);
                    printf(lcd_putc, "TEST X AXIS");
                    delay_ms(1000);
                    char error1 = GoToNextX();
                    if(error1)
                    {
                        lcd_gotoxy(13, 2);
                        printf(lcd_putc, "error");                       
                    }
                    else
                    {
                        lcd_gotoxy(13, 2);
                        printf(lcd_putc, "DONE!!!");
                    }
                    delay_ms(1000);
                ////Z test
                lcd_gotoxy(21, 1);
                    printf(lcd_putc, "TEST Z AXIS");
                    delay_ms(1000);
                    char error3 = GoToZComplement();
                    if(error3)
                    {
                        lcd_gotoxy(35, 1);
                        printf(lcd_putc, "error");                       
                    }
                    else
                    {
                        lcd_gotoxy(35, 1);
                        printf(lcd_putc, "DONE!!!");
                    }
                    delay_ms(4000);
                    break;

                case '3':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "Settings...");
                    delay_ms(1000);
                    break;

                case '4':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "More Options...");
                    delay_ms(1000);
                    break;
                 case '5':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "TEST X AXIS");
                    delay_ms(1000);
                    char error4 = GoToNextX();
                    if(error4)
                    {
                        lcd_putc("\f");
                        printf(lcd_putc, "error");                       
                    }
                    else
                    {
                        lcd_putc("\f");
                        printf(lcd_putc, "DONE!!!");
                    }
                    delay_ms(1000);
                    break; 
                 case '6':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "Test theta");
                    delay_ms(1000);
                    char error5 = nextTheta();
                    if(error5)
                    {
                        lcd_putc("\f");
                        printf(lcd_putc, "error");
                    }
                    else
                    {
                        lcd_putc("\f");
                        printf(lcd_putc, "DONE!!!");   
                    }
                    delay_ms(1000);
                    break; 
                case '7':
                  lcd_gotoxy(1, 1);
                    printf(lcd_putc, "TEST Z AXIS");
                    delay_ms(1000);
                    char error6 = GoToZComplement();
                    if(error6)
                    {
                        lcd_putc("\f");
                        printf(lcd_putc, "error");                       
                    }
                    else
                    {
                        lcd_putc("\f");
                        printf(lcd_putc, "DONE!!!");
                    }
                    delay_ms(1000);
                    break; 
               case '8':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "8...");
                    delay_ms(1000);
                    break;
               case '9':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "9...");
                    delay_ms(1000);
                    break;
               case '0':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "0...");
                    delay_ms(1000);
                    break;                    
               case '*':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "*...");
                    delay_ms(1000);
                    break;     
                case 'A':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "A...");
                    delay_ms(1000);
                    break;    
                case 'B':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "B...");
                    delay_ms(1000);
                    break;    
                case 'C':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "C...");
                    delay_ms(1000);
                    break;    
                case 'D':
                lcd_gotoxy(1, 1);
                    printf(lcd_putc, "D...");
                    delay_ms(1000);
                    break;    
                case '#':
                    lcd_gotoxy(1, 1);
                    printf(lcd_putc, "#...");
                    delay_ms(1000);
                    break;    
                default:
                    // Handle other keys if needed
                    break;
            }
            
            lcd_putc("\f");
            key = 0; // Reset the key to 0 after processing
        }
       
        
    }      
    return;  
}
