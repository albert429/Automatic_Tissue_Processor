#define LCD_ENABLE_PIN PIN_E0
#define LCD_RS_PIN     PIN_E2
#define LCD_RW_PIN     PIN_E1
#define LCD_DATA4      PIN_A5
#define LCD_DATA5      PIN_A4
#define LCD_DATA6      PIN_A3
#define LCD_DATA7      PIN_A2

#include <16F877A.h>
#fuses NOBROWNOUT, NOLVP, HS
#use delay(clock = 20MHz)
#include <lcd.c>
#use fast_io(B)
#use I2C(master, SDA = PIN_C4, SCL = PIN_C3, FORCE_HW)
#use rs232(baud=9600,xmit=pin_c6,rcv=pin_c7)

#define RTC_Write_ADDRESS 0b11010000 // RTC write address in binary
#define RTC_Read_ADDRESS 0b11010001  // RTC read address in binary

#define ROW2 PIN_B7
#define ROW3 PIN_B6
#define COL3 PIN_B5
#define ROW4 PIN_B4
#define COL1 PIN_B3
#define ROW1 PIN_B2
#define COL2 PIN_B1

#define POWER_OUT_PIN PIN_c2

char time[] = "TIME:00:00:00";
char calendar[] = "DATE:00/00/20  ";
unsigned int8 second = 0, minute = 0, hour = 0, date = 0, month = 0, year = 0;
unsigned int8 currentsec=0,currentmin=0,currenthour=0;
unsigned int8 requiredHour, requiredMinute, requiredSecond;

void write_RTC(unsigned int8 address, int8 data_) 
{
    i2c_start();                     // Start I2C communication
    i2c_write(RTC_Write_ADDRESS);    // Specify the RTC's write address
    i2c_write(address);              // Specify the register address in the RTC to write to
    i2c_write(data_);                // Send the data byte to write
    i2c_stop();                      // Stop I2C communication
}
void Read_RTC(unsigned int8 address, int8* data_) 
{
    i2c_start();                     // Start I2C communication
    i2c_write(RTC_Write_ADDRESS);    // Specify the RTC's write address
    i2c_write(address);              // Specify the register address in the RTC to read from
    i2c_start();                     // Issue a repeated start condition
    i2c_write(RTC_Read_ADDRESS);     // Specify the RTC's read address
    *data_ = i2c_read(0);            // Read the data from the RTC register
    i2c_stop();                      // Stop I2C communication
}
void updateRTC() 
{
   // Continuously read the time and date from the RTC and update the display
    Read_RTC(0x00, &second);     // Read seconds
    Read_RTC(0x01, &minute);     // Read minutes
    Read_RTC(0x02, &hour);       // Read hours
    Read_RTC(0x04, &date);       // Read day of the month
    Read_RTC(0x05, &month);      // Read month
    Read_RTC(0x06, &year);       // Read year
   
    second = (second >> 4) * 10 + (second & 0x0F);
    minute = (minute >> 4) * 10 + (minute & 0x0F);    // from hex to uint8
    //hour &= 0x1F;
    hour = (hour >> 4) * 10 + (hour & 0x0F);
    date = (date >> 4) * 10 + (date & 0x0F);
    month = (month >> 4) * 10 + (month & 0x0F);
    year = (year >> 4) * 10 + (year & 0x0F);

    currentsec=second;
    currentmin=minute;
    currenthour=hour;

    time[12] = second % 10 + '0'; // Get the least significant digit of seconds
    time[11] = second / 10 + '0'; // Get the most significant digit of seconds
    time[9] = minute % 10 + '0';  // Least significant digit of minutes
    time[8] = minute / 10 + '0';  // Most significant digit of minutes
    time[6] = hour % 10 + '0';    // Least significant digit of hours
    time[5] = hour / 10 + '0';    // Most significant digit of hours

    calendar[14] = year % 10 + '0';  // Least significant digit of year
    calendar[13] = year / 10 + '0';  // Most significant digit of year
    calendar[9] = month % 10 + '0';  // Least significant digit of month
    calendar[8] = month / 10 + '0';  // Most significant digit of month
    calendar[6] = date % 10 + '0';   // Least significant digit of date
    calendar[5] = date / 10 + '0';   // Most significant digit of date
}

void INIT_RTC() 
{
   // Stop the RTC clock and set the time (set the CH bit of the seconds register)
   write_RTC(0x00, 0b10000000);     // Stop the clock to set time
   write_RTC(0x01, 0x51);           // Set minutes
   write_RTC(0x02, 0x14);           // Set hours and select 24-hour format
   write_RTC(0x04, 0x29);           // Set the day of the month
   write_RTC(0x05, 0x06);           // Set the month
   write_RTC(0x06, 0x24);           // Set the year
   write_RTC(0x00, 0x00);           // Start the clock
}

char keypadScan(char key)
{
   char ROWS[4]={ROW1,ROW2,ROW3,ROW4};
   char COLS[3]={COL1,COL2,COL3};
   char keymatrix[4][3]={{'1','2','3'}
                         {'4','5','6'}
                         {'7','8','9'}
                         {'*','0','#'}};
   int count=0;
   for(int i=0;i<4;i++)
   {
      output_low(ROWS[i]);
      for(int j=0;j<3;j++)
      {
         count=0;
         if(!input(COLS[j]))
         {
            for(int k=0;k<100;k++)
            {
               count++;
            }
            if(count>50)
            {
               key=keymatrix[i][j];
            } 
            while(!input(COLS[j]));
         }
      }
      output_float(ROWS[i]);
   }
   return key;
}
int8 overflow=0;
#INT_TiMER1
void timer1_isr()
{  
   overflow++;
   if(overflow==50)
   {
      updateRTC();
      overflow=0;
   }
}
void updateRequiredTime(char cup)
{
   requiredHour   = currentHour+read_eeprom(0x0000+(cup-'A')*2); 
   requiredMinute = currentmin +read_eeprom(0x0001+(cup-'A')*2);
   requiredSecond = currentsec;
   
   if(requiredSecond>59)
   {
      requiredMinute++;
      requiredSecond=requiredSecond - 60;
   } 
   if(requiredMinute>59)
   {
      requiredHour++;
      requiredMinute = requiredMinute - 60;
   }
   if(requiredHour>23)
   {
      requiredHour = requiredHour - 24;
   }
 
}
void pMenuMain()
{
   lcd_gotoxy(13, 1);
   printf(lcd_putc,"%u:%u:%u",currenthour,currentmin,currentsec);
   lcd_gotoxy(1, 1);
   printf(lcd_putc,"1:Start");
   lcd_gotoxy(1, 2);
   printf(lcd_putc,"2:Test");
   lcd_gotoxy(21, 1);
   printf(lcd_putc,"3:Time");
   lcd_gotoxy(21, 2);
   printf(lcd_putc,"4:Configure");  
}
void pMenuStart(char cup)
{
   lcd_gotoxy(1, 1);
   printf(lcd_putc,"Beaker (%c)",cup);
   lcd_gotoxy(1, 2);
   printf(lcd_putc,"Time:%u:%u:%u ",currenthour,currentMin,currentsec);
   lcd_gotoxy(21, 1);
   printf(lcd_putc,"Cup%c:%u:%u:%u ",cup+1,requiredHour,requiredMinute,requiredSecond);
   lcd_gotoxy(35,2);
   printf(lcd_putc,"#:STOP");
   if(currentsec==59)
   {
      lcd_gotoxy(20,1);
      lcd_putc(' ');
   }
}
void pMenuTest()
{
   lcd_gotoxy(1, 1);
   printf(lcd_putc,"1:SpeedRun");
   lcd_gotoxy(1, 2);
   printf(lcd_putc,"2:UP");
   lcd_gotoxy(21, 1);
   printf(lcd_putc,"3:DOWN");
   lcd_gotoxy(21, 2);
   printf(lcd_putc,"4:Next-DC");
   lcd_gotoxy(12,1);
   printf(lcd_putc,"5:Home-DC");
   lcd_gotoxy(12,2);
   printf(lcd_putc,"6:X-");
   lcd_gotoxy(17,2);
   printf(lcd_putc,"7:X+");
   lcd_gotoxy(32,2);
   printf(lcd_putc,"#:Back");
}
void pMenuTime()
{
             lcd_gotoxy(1, 1);
             printf(lcd_putc,time);
             lcd_gotoxy(1, 2);
             printf(lcd_putc, calendar);
             lcd_gotoxy(35,2);
             lcd_gotoxy(21, 2);
             printf(lcd_putc,"1:Set Time");
             lcd_gotoxy(35, 2);
             printf(lcd_putc,"#:Back");
}
void pMenuSetTime()
{
                   lcd_gotoxy(1,1);
                   printf(lcd_putc,"Hour:");
                   lcd_gotoxy(1,2);
                   printf(lcd_putc,"Minute:");
                   lcd_gotoxy(21,1);
                   printf(lcd_putc,"Date:  /  /    ");
                   lcd_gotoxy(21,2);
                   printf(lcd_putc,"#Cancel");
                   lcd_gotoxy(35,2);
                   printf(lcd_putc,"*:Set");
}
void pMenuConfigure()
{
            lcd_gotoxy(1, 1);
            printf(lcd_putc,"1:Confg Auto Start");
            lcd_gotoxy(1, 2);
            printf(lcd_putc,"2:Confg Each Beaker");
            lcd_gotoxy(35,2);
            printf(lcd_putc,"#:Back");
}
void pMenuViewConfig()
{
   lcd_gotoxy(1,1);
   printf(lcd_putc,"A:%u:%u",read_eeprom(0x0000),read_eeprom(0x0001));
   lcd_gotoxy(1,2);
   printf(lcd_putc,"B:%u:%u",read_eeprom(0x0000+1*2),read_eeprom(0x0001+1*2));  
   lcd_gotoxy(21,1);
   printf(lcd_putc,"C:%u:%u",read_eeprom(0x0000+2*2),read_eeprom(0x0001+2*2));  
   lcd_gotoxy(21,2);
   printf(lcd_putc,"D:%u:%u",read_eeprom(0x0000+3*2),read_eeprom(0x0001+3*2));

   /*                
   lcd_gotoxy(8,1);
   printf(lcd_putc,"E:%u:%u",read_eeprom(0x0000+4),read_eeprom(0x0001+4));
   lcd_gotoxy(8,2);
   printf(lcd_putc,"F:%u:%u",read_eeprom(0x0000+5),read_eeprom(0x0001+5));  
   lcd_gotoxy(28,1);
   printf(lcd_putc,"G:%u:%u",read_eeprom(0x0000+6),read_eeprom(0x0001+6));  
   lcd_gotoxy(28,2);
   printf(lcd_putc,"H:%u:%u",read_eeprom(0x0000+7),read_eeprom(0x0001+7));
   
   lcd_gotoxy(15,1);
   printf(lcd_putc,"I:%u:%u",read_eeprom(0x0000+8),read_eeprom(0x0001+8));
   lcd_gotoxy(15,2);
   printf(lcd_putc,"J:%u:%u",read_eeprom(0x0000+9),read_eeprom(0x0001+9));  
   lcd_gotoxy(25,1);
   printf(lcd_putc,"K:%u:%u",read_eeprom(0x0000+10),read_eeprom(0x0001+10));  
   lcd_gotoxy(25,2);
   printf(lcd_putc,"L:%u:%u",read_eeprom(0x0000+11),read_eeprom(0x0001+11));           
   */
}

void saveState(char cup)
{
   write_eeprom(0x0095,cup);
   write_eeprom(0x0096,requiredhour-currenthour);
   write_eeprom(0x0097,requiredMinute-currentmin);
   write_eeprom(0x0098,requiredSecond-currentsec);
}

int1 wasPowerOut()
{
   return read_eeprom(0x0099);
}

void clearPowerOut()
{
   write_eeprom(0x0099,0);
}

void CheckNSetPowerOut(char cup)
{
   if(input(POWER_OUT_PIN))
   {
      write_eeprom(0x0099,1);
      saveState(cup);
      char key='e';
      while(input(POWER_OUT_PIN))
      {
         lcd_gotoxy(21,2);printf(lcd_putc,"POWER ERROR!"); 
         lcd_gotoxy(15,1);printf(lcd_putc,"IN:%d",input(POWER_OUT_PIN));
         key='e';
         key=keypadScan(key);
         if(key=='#')
         {
         clearPowerOut();
         break;
         }
      };
      lcd_gotoxy(21,2);printf(lcd_putc,"            ");
      delay_ms(2000);
   }
}

void powerOutUpdateRequiredTime()
{
   
   requiredHour=read_eeprom(0x0096)+currentHour;
   requiredMinute=read_eeprom(0x0097)+currentMin;
   requiredSecond=read_eeprom(0x0098)+currentsec;

   if(requiredSecond>59)
   {
      requiredMinute++;
      requiredSecond=requiredSecond - 60;
   } 
   if(requiredMinute>59)
   {
      requiredHour++;
      requiredMinute = requiredMinute - 60;
   }
   if(requiredHour>23)
   {
      requiredHour = requiredHour - 24;
   }
}
  
             
void main() {
   
   lcd_init();                      // Initialize the LCD display
   lcd_putc('\f');                  // Clear the display
   //iINIT_RTC();                      // Initialize the RTC with pre-defined time and date ONLY WHEN SETTING TIME the first time !!!!!!!!!!!!!!
   enable_interrupts(global);
   enable_interrupts(int_timer1);
   setup_timer_1(T1_INTERNAL || T1_DIV_BY_8);
   port_b_pullups(1);
   char key=0;
   char cup='A';
   int8 xcursor=0,ycursor=0;
   output_high(PIN_A0);
   delay_ms(1000);
   output_low(PIN_A0);
    
   while (TRUE) {
      key = '0';
      cup='A';
      key = keypadScan(key);
      updateRTC();                   // Update the LCD display with the new time and date
      pMenuMain();

      if((currentHour == read_eeprom(0x0090)) && (currentMin == read_eeprom(0x0091)) && (currentsec < 10))
      {
         key='1';
      }
     
      //if Power was out Start
      if(wasPowerOut()==1)
      {
         key='1';
      }
      //

      switch(key)
      {
         case '1':                                                          ////////////////////////////1:START
            lcd_putc('\f');
            updateRequiredTime(cup);
            putc(cup);
            
            while(key!='#')
            {  
               key='e';
               key = keypadScan(key);
               pMenuStart(cup);
               lcd_gotoxy(15,1);
               printf(lcd_putc,"IN:%d",input(POWER_OUT_PIN));
               
               //if PowerWasOut Update cup,time and clear POFa
               if(wasPowerOut() == 1)
               {
                  cup = read_eeprom(0x0095);
                  powerOutUpdateRequiredTime();
                  lcd_putc('\f');
                  clearPowerOut();
                  lcd_gotoxy(5,2);printf(lcd_putc,"RECOVERED!");
                  lcd_gotoxy(15,1);printf(lcd_putc,"IN:%d",input(POWER_OUT_PIN));
                  delay_ms(2000);
                  lcd_putc('\f');  
               }
               //if PowerOut true 1-raisePOF ,2-print:"PowerOut!!" ,3-wait till Powerback and recover 
               CheckNSetPowerOut(cup);
               //
               
               if(currentHour==requiredHour && currentmin==requiredMinute && currentsec==requiredSecond)
               { 
                  cup++;
                  putc(cup);
                  updateRequiredTime(cup);
                  lcd_putc('\f');
               }
               if(cup=='M' || key=='#')
               {
                  putc('#');
                  clearPowerOut();
                  break;
               }
            }
            lcd_putc('\f');
            break;
         case '2':                                                          ////////////////////////////////////////2:TEST
            lcd_putc('\f');
            pMenuTest();
            delay_ms(250);
            while(key!='#')
            {
               //////// Limit Switch
               lcd_gotoxy(32,1);
               printf(lcd_putc,"LS=%d",input(pin_B0));
               /////////
               delay_ms(100);
               key='e';
               key = keypadScan(key);
               if(key=='2')
               {putc('a');delay_ms(220);}
               else if(key == '3')
               {putc('b');delay_ms(220);}
               else if(key == '4')
               {putc('e');delay_ms(220);}
               else if(key == '5')
               {putc('f');delay_ms(220);}
               else if(key == '6')
               {putc('c');delay_ms(220);}
               else if(key == '7')
               {putc('d');delay_ms(220);}
               else if(key== '1')
               {
                  ///////////////////////////////////////////
                  cup='A';
                  lcd_putc('\f');
                  putc(cup);

                  requiredHour=currenthour;
                  requiredMinute=currentMin;
                  requiredSecond=(40+currentsec);
                  if(requiredSecond>59)
                  {
                     requiredMinute++;
                     requiredSecond -= 60;
                  }
                  if(requiredMinute>59)
                  {
                     requiredHour++;
                     requiredMinute -= 60;
                  }
                  if(requiredHour>23)
                  {
                     requiredHour-=24;
                  }
                  while(key!='#')
                  {
                     key='e';
                     key = keypadScan(key);
                     pMenuStart(cup);
                     if(currentHour==requiredHour && currentmin==requiredMinute && currentsec==requiredSecond)
                     {  
                        requiredHour=currenthour;
                        requiredMinute=currentMin;
                        requiredSecond=(40+currentsec);
                        if(requiredSecond>59)
                        {
                           requiredMinute++;
                           requiredSecond -= 60;
                        }
                        if(requiredMinute>59)
                        {
                           requiredHour++;
                           requiredMinute -= 60;
                        }
                        if(requiredHour>23)
                        {
                           requiredHour-=24;
                        }
                        cup++;
                        putc(cup);
                        lcd_putc('\f');
                     }
                     if(cup=='M')
                     {
                        key='#';
                        break;
                     }
                     }
                  lcd_putc('\f');
                  //////////////////////////////////////////////
               }
             }
             lcd_putc('\f');
             break;
         case '3':                                                          //////////////////////////////////////////////3:TIME
             lcd_putc('\f');
             pMenuTime();
             while(key!='#' && key!='*')
             {
                key=keypadScan(key);
                if(key == '1')
                {  
                   lcd_putc('\f');
                   pMenuSetTime();
                   int xcursor=1,ycursor=6;
                   while(key!='#' && key!='*')
                   {
                      lcd_cursor_on(1);
                      key='e';
                      delay_ms(200);
                      lcd_gotoxy(ycursor,xcursor);
                      key = keypadScan(key);
                      if(key!='e')
                      {
                         printf(lcd_putc,"%c",key);
                         ycursor++;
                         if(ycursor==8 && xcursor==1)
                         {
                            xcursor=2;
                            ycursor=8;
                         }
                         if(ycursor==10 && xcursor==2)
                         {
                            xcursor=1;
                            ycursor=26;
                         }
                         if((ycursor==28 && xcursor==1) || (ycursor==31 && xcursor==1))
                         {
                            ycursor++;
                         }
                      }
                   }
                }
             }
             if(key=='*')
             {  
                write_RTC(0x00, 0b10000000);     // Stop the clock to set time
                write_RTC(0x01, ((lcd_getc(8,2)-'0')<<4)+(lcd_getc(9,2)-'0'));           // Set minutes
                write_RTC(0x02, ((lcd_getc(6,1)-'0')<<4)+(lcd_getc(7,1)-'0'));           // Set hours and select 24-hour format
                write_RTC(0x04, ((lcd_getc(26,1)-'0')<<4)+(lcd_getc(27,1)-'0'));           // Set the day of the month
                write_RTC(0x05, ((lcd_getc(29,1)-'0')<<4)+(lcd_getc(30,1)-'0'));           // Set the mont
                write_RTC(0x06, (((lcd_getc(33,1)-'0')<<8)+((lcd_getc(34,1)-'0')<<4)+(lcd_getc(35,1)-'0')));           // Set the year
                write_RTC(0x00, 0x00);           // Start the clock  
             }
             lcd_cursor_on(0);
             enable_interrupts(global);
             lcd_putc('\f');
             break;
         case '4':                                                          //////////////////////////////////////////////////// 4:configure Time
            lcd_putc('\f');
            clearPowerOut();
            pMenuConfigure();
            while(key!='#')
            {
               key='e';
               key=keypadScan(key);
              
               if(key=='1')
               {
                  lcd_putc('\f');
                  
                  lcd_gotoxy(1,1);
                  printf(lcd_putc,"Auto Start Time-24Hr");
                  lcd_gotoxy(1,2);
                  printf(lcd_putc,"HH/MM:  /  ");
                  lcd_gotoxy(35,1);
                  printf(lcd_putc,"*:Set"); 
                  lcd_gotoxy(35,2);
                  printf(lcd_putc,"#:Back");  
                  lcd_gotoxy(21,2);
                  printf(lcd_putc,"Current=%u:%u",read_eeprom(0x0090),read_eeprom(0x0091));
                  xcursor=2,ycursor=7;
                  lcd_gotoxy(ycursor,xcursor);
                  lcd_cursor_on(1); 
                  while(key!='*' && key != '#')
                  {
                         delay_ms(200);
                         lcd_gotoxy(ycursor,xcursor);
                         key='e';
                         key = keypadScan(key);
                         if(key!='e')
                         {
                            printf(lcd_putc,"%c",key);
                            ycursor++;
                            if(ycursor==9 && xcursor==2)
                            {
                               ycursor++;
                            }
                         }
                    }
                    if(key=='*')
                    {
                        write_eeprom(0x0090,((lcd_getc(7,2)-'0')*10)+((lcd_getc(8,2)-'0')));
                        write_eeprom(0x0091,((lcd_getc(10,2)-'0')*10)+((lcd_getc(11,2)-'0')));
                        key='#';
                    }
                    lcd_putc('\f');
               }
                //////////Configure Each Beaker Time
                if(key=='2')
                {
                     lcd_putc('\f');  

                     lcd_gotoxy(1,1);
                     printf(lcd_putc,"Beaker %c Time:", cup);
                     lcd_gotoxy(1,2);
                     printf(lcd_putc,"HH/MM:  /  ");
                     lcd_gotoxy(35,1);
                     printf(lcd_putc,"*:Set"); 
                     lcd_gotoxy(35,2);
                     printf(lcd_putc,"#:Next");
                     lcd_gotoxy(21,2);
                     printf(lcd_putc,"T(%c)=%u:%u",cup,read_eeprom(0x0000+(cup-'A')*2),read_eeprom(0x0001+(cup-'A')*2));
                     
                     ycursor=7,xcursor=2;
                     lcd_cursor_on(1);
                     while(cup!='M')
                     {
                        lcd_gotoxy(1,1);
                        printf(lcd_putc,"Beaker %c Time:", cup);
                        lcd_gotoxy(ycursor,xcursor);
                        key='e';
                        key=keypadScan(key);
                        
                        if(key!='e')
                        {
                              if(key>='0' && key <='9')
                              {
                                 printf(lcd_putc,"%c",key);
                                 ycursor++;
                                 if(ycursor==9 && xcursor==2)
                                 {
                                    ycursor++;
                                 }
                              }
                              if(key=='*')
                              {
                                 write_eeprom(0x0000+(cup-'A')*2,((lcd_getc(7,2)-'0')*10)+((lcd_getc(8,2)-'0')));
                                 write_eeprom(0x0001+(cup-'A')*2,((lcd_getc(10,2)-'0')*10)+((lcd_getc(11,2)-'0')));
                              }
                              if(key=='#' || key =='*')
                              {
                                 cup++;
                                 ycursor=7,xcursor=2;
                                 lcd_gotoxy(21,2);
                                 printf(lcd_putc,"T(%c)=%u:%u",cup,read_eeprom(0x0000+(cup-'A')*2),read_eeprom(0x0001+(cup-'A')*2));
                              }
                              delay_ms(200);
                        }
                        
                     }
                }
                
                if(key=='3')
                {
                  lcd_putc('\f');
                  lcd_gotoxy(1,1);
                }
                ///////////////////////End of Configure Each Beaker Time
                
               if(key=='4')
               {
                  lcd_putc('\f');
                  pMenuViewConfig();
                  while(key!='#')
                  {key=keypadScan(key);}
               }
               
            }
            lcd_putc('\f');
            lcd_cursor_on(0);
            break;
         default:
            break;
      }   
   }
}

