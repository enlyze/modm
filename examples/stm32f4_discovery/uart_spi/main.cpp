#include <xpcc/architecture/platform.hpp>
#include "../stm32f4_discovery.hpp"


MAIN_FUNCTION
{
	defaultSystemClock::enable();

	LedOrange::setOutput(xpcc::Gpio::High);
	LedGreen::setOutput(xpcc::Gpio::Low);
	LedRed::setOutput(xpcc::Gpio::High);
	LedBlue::setOutput(xpcc::Gpio::High);

	// Enable Uart SPI 2
	GpioOutputA2::connect(UartSpiSimpleMaster2::Mosi);
	GpioInputA3::connect(UartSpiSimpleMaster2::Miso);
	GpioOutputA4::connect(UartSpiSimpleMaster2::Sck);
	UartSpiSimpleMaster2::initialize<defaultSystemClock, 5250000, xpcc::Tolerance::Exact>();

	while (1)
	{
		UartSpiSimpleMaster2::transferBlocking(0xF0);
	}

	return 0;
}
