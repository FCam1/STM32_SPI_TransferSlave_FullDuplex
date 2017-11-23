/*
 Name:    Sketch1.ino
 Created: 2017-10-31 1:19:05 PM
 Author:  ZeCurator
*/

#include <SPI.h>

uint8_t Data0Buffer[16];



// DMA has completed, transfer buffer to USB Serial (Hard locks)
void DMACompleted()
{
  Serial.write(Data0Buffer, sizeof(Data0Buffer));
}

// setup DMA to tunnel data into a buffer and execute an interruption when it's filled
void setupDMA()
{

  dma_init(DMA1); // init DMA clock domain
  dma_disable(DMA1, DMA_CH2); // Disable the DMA tube.
//
//                // DMA tube configuration for SPI1 Rx - channel 2
  dma_tube_config my_tube_cfg = {
    &SPI1->regs->DR,  // data source address
    DMA_SIZE_8BITS, // source transfer size
    Data0Buffer,    // data destination address 
    DMA_SIZE_8BITS, // destination transfer size
    sizeof(Data0Buffer),  // nr. of data to transfer
            // tube flags: auto increment dest addr, circular buffer, set tube full IRQ, very high prio:
            (DMA_CFG_CIRC | DMA_CFG_DST_INC | DMA_CFG_CMPLT_IE | DMA_CCR_PL_VERY_HIGH),
            0,  // unused
            DMA_REQ_SRC_SPI1_RX,  // Hardware DMA request source
  };

  // configure DMA channel
  int ret = dma_tube_cfg(DMA1, DMA_CH2, &my_tube_cfg);  // SPI1 Rx channel is nr. 2
  if (ret > 0) {
    Serial1.print(F("DMA configuration error: ")); Serial1.println(ret, HEX);
    Serial1.print(F("Stopped, reset is needed!"));
    while (1);  // halted
  }

  dma_attach_interrupt(DMA1, DMA_CH2, DMACompleted);  // attach an interrupt handler.
  dma_enable(DMA1, DMA_CH2);  // Enable the DMA tube. It will now begin serving requests.
}


// Setup SPI 1 to begin receiving data via DMA

void setupSPI()
{
  // Set to SPI 1
 SPI.setModule(1);

  // initialize SPI 1
  spi_init(SPI1);

  // start SPI 1 receiving
  spi_slave_enable(SPI1, SPI_MODE0, SPI_FRAME_LSB | SPI_SW_SLAVE | SPI_DFF_8_BIT | SPI_RX_ONLY);
  // enable SPI 1 RX DMA
  spi_rx_dma_enable(SPI1);

#if 0
  SPI1->regs->CR1 |= SPI_CR1_RXONLY; // Receive only
  SPI1->regs->CR1 |= SPI_CR1_BR_PCLK_DIV_2; // Baud Rate  
  SPI1->regs->CR1 |= SPI_CR1_DFF_8_BIT;
  // Sample on falling edge (second edge)
  SPI1->regs->CR1 |= SPI_CR1_CPHA; // Phase 1
  SPI1->regs->CR1 &= ~SPI_CR1_CPOL; // Polarity 0
  SPI1->regs->CR1 |= SPI_CR1_CRCEN;
  SPI1->regs->CR2 = SPI_CR2_RXNEIE; // RX status is set when buffer is available   
  SPI1->regs->CR2 |= SPI_CR2_RXDMAEN; // Enable RX DMA
  SPI1->regs->CR1 |= SPI_CR1_LSBFIRST;
  SPI1->regs->CR1 |= SPI_CR1_SSM; // Software Slave Mode (needed if NSS pin isn't used)
  SPI1->regs->CR1 |= SPI_CR1_SPE; // Enable SPI; 
#endif
}

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(115200);
 
  // Wait for USB Serial
  while (!Serial) {}
  delay(400);
  Serial.setTimeout(10);
  
  setupSPI();
  setupDMA();
}

// the loop function runs over and over again until power down or reset
void loop() {
  Serial.write("trame");
//

  delay(500);//Pour pouvoir lire le serial
  //Serial.print(timer_get_count(TIMER1));
  return;

  while (true)
  {
    if ((GPIOA->regs->IDR & 0x01) == 0)
    {

      while (true)
      {
        if ((GPIOA->regs->IDR & 0x01))
        {
          SPI.read(Data0Buffer, sizeof(Data0Buffer));
          Serial.write(Data0Buffer, sizeof(Data0Buffer));
          return;
        }
      }
    }
  }
}
