/*
--- COMMUNICATE BY SPI AS SLAVE WITH A MAPLE MINI IN FULL DUPLEX MODE ---

>Example programm for read data from a Master and "simultaneously"  send a data to the Master 

>Authors:  T. Flayol and F. Caminade
>Created the 31/10/2017
*/

#include <SPI.h> //ADD transferSlave function

uint8_t RxBuffer[4]; //Rx buffer
uint8_t TxBuffer[4]={0x11,0x22,0x33,0x44}; //Tx buffer



void setupSPI()
{
  // Set SPI1
  SPI.setModule(1);

  // Initialize and reset SPI1
  spi_init(SPI1);
  
  //Configure GPIO for Slave Mode 
  SPI.beginSlave();//Rx ONLY

  // Configure SPI1 - SPI MODE1- MSb Frist - 8bit Data - FULL DUPLEX  
  spi_slave_enable(SPI1, SPI_MODE1, SPI_FRAME_MSB &~SPI_SW_SLAVE | SPI_DFF_8_BIT &~SPI_CR1_BIDIMODE &~SPI_CR1_RXONLY &~SPI_CR1_CRCEN); // For FULL DUPLEX : BIDIMODE=0 and RXONLY=0

}

void setup()
{
  //Set up for serial monitor   
  Serial.begin(115200); 
  while (!Serial) {}
  delay(400);
  Serial.setTimeout(10);
  
  // Set Up SPI 
  setupSPI();

}


void loop() {
 
  
  SPI.transferSlave(RxBuffer, TxBuffer, sizeof(TxBuffer));     
  
  Serial.write(RxBuffer, sizeof(RxBuffer));
}
