//Authors : T.Flayol and F.Caminade
//Based on read function
//Read data from Rx buffer and write data to Tx buffer as SLAVE 
//To ADD in SPI.cpp
void SPIClass::transferSlave(uint8 *RXbuf, uint8 *TXbuf,  uint32 len)
{
    if ( len == 0 ) return;
    spi_rx_reg(_currentSetting->spi_d);		// clear the RX buffer in case a byte is waiting on it.
    spi_reg_map * regs = _currentSetting->spi_d->regs;
    // start sequence: write byte 0
    regs->DR = *TXbuf++;						// write in the TX buffer
    // main loop
    while ( (--len) ) {
        while( !(regs->SR & SPI_SR_TXE) );		// wait for TXE flag
        noInterrupts();							// go atomic level - avoid interrupts to surely get the previously received data
        regs->DR = *TXbuf++;						// write the next data item to be transmitted into the SPI_DR register. This clears the TXE flag.
        while ( !(regs->SR & SPI_SR_RXNE) );	// wait till data is available in the DR register
        *RXbuf++ = (uint8)(regs->DR);				// read and store the received byte. This clears the RXNE flag.
        interrupts();							// let systick do its job
    }
    // read remaining last byte
    while ( !(regs->SR & SPI_SR_RXNE) );	// wait till data is available in the Rx register
    *RXbuf = (uint8)(regs->DR);				// read and store the received byte
}





