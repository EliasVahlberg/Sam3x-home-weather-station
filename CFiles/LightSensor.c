/**
* @file
*     LightSensor.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/
//#include "ArduinoBaseInclude.c"

#pragma region Functions
void    adc_setup                   (                   );
void    ADC_Handler                 (                   );
void    light_measure               (                   );
void    adc_measure                 (                   );
#pragma endregion Functions


void adc_setup(){ 
  *AT91C_PMC_PCER         |= (1 << 11);     //Enable PMC, starting peripheral clock PIOA  Light
  *AT91C_PMC_PCER1         = 1<<5;               //Enables the PMC for the ADC

  *AT91C_PIOA_PER         |= 1<<4;               //Enable Digital Pin A6
  *AT91C_PIOA_PER         |= 1<<3;               //Enable Digital Pin A5
  *AT91C_ADCC_MR           = 1<<9;                 
  // Division value: MCK/((ADC MR>>8 + 1)*2) Prescaler to 2 in ADC_MR

  *AT91C_ADCC_CWR          = 0x1000000;           //= ADC_EMR, Used to: "append the channel number to the conversion result in ADC_LDCR register"
  NVIC_ClearPendingIRQ(ADC_IRQn);
  NVIC_SetPriority(ADC_IRQn, 1);        //Priority of one (high) since the interrupt is not enabled for long durations and is time sensitive 
  NVIC_EnableIRQ(ADC_IRQn);
  
}

//A combined function to get photoresistor data using adc
void adc_measure(){
  light_sens_ch1_ready=0;
  light_sens_ch2_ready=0;
   *AT91C_ADCC_CHER = 0x6;                          //Bit number nr. 1 and 2 (0x6) enables CH1 and CH2 in the ADC
   *AT91C_ADCC_CR = 0x2;                            //Bit 1 = Start, Starts ADC
   *AT91C_ADCC_IER = (1<<24);                       //Enable interrupt (DRDY: Data Ready Interrupt Enable) on the ADC
   int adc_read_status = 0;
   while(1)
   {
       if(light_sens_ch1_ready ==1 && adc_read_status!=1&&adc_read_status!=3)
       {
           
           adc_CDR1_value = (0xFFF&(*AT91C_ADCC_CDR1));             //Data is contained from bit 0 to 11, 0xFFF & CDR2 extracts all relevant data
           adc_read_status|=0x1;
       }
       if(light_sens_ch2_ready ==1 && adc_read_status!=2&&adc_read_status!=3)
       {
           
           adc_CDR2_value = (0xFFF&(*AT91C_ADCC_CDR2));             //Data is contained from bit 0 to 11, 0xFFF & CDR2 extracts all relevant data
           adc_read_status|=0x2;
       }
       if(adc_read_status==0x3)
       break;
   }                 //Wait untill both registers have been read interrupt on ADC
   light_sens_ch1_ready = 0;
   light_sens_ch2_ready = 0;
}

void light_measure(){
  adc_measure();
    adc_ch1_voltage = adc_CDR1_value/light_to_volt_coefficcient; 
    adc_ch2_voltage = adc_CDR2_value/light_to_volt_coefficcient;
}
