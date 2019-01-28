/****************************************************************************
 **
 **  Copyright (C) 2013 - 2019 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>

#include <TinyGPS++.h>

#include <NeguPiDaemon.h>
#include <NeguPiLogger.h>
#include <NeguPiMillis.h>

using namespace NeguPi;

const unsigned char UBLOX_INIT[] = {
  0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B, // GxGLL off
  0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39, // GxGSV off
  0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x05, 0x47, // GxVTG off
  0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32, // GxGSA off
  0x24, 0x50, 0x55, 0x42, 0x58, 0x2c, 0x34, 0x31, 0x2c, 0x31, 0x2c, 0x33, 0x2c, 0x33, 0x2c, 0x33,
  0x38, 0x34, 0x30, 0x30, 0x2c, 0x30, 0x2a, 0x32, 0x34, 0x0d, 0x0a
};

TinyGPSPlus gps;
int gpsfd = -1;

void read_gps()
{
	while (1) {
		char buffer;
		while (read(gpsfd, &buffer, 1)==1) {
			std::cout << buffer;

			if (gps.encode(buffer)) {
				if (gps.location.isUpdated() && gps.date.isUpdated() && gps.time.isUpdated()) {
					std::cout << "g ";
					std::cout << NeguPi::Millis::instance()->get() << " ";
					std::cout << gps.satellites.value() << " ";
					std::cout << gps.location.lat() << " ";
					std::cout << gps.location.lng() << " ";
					std::cout << std::endl;

					gps.location.rawLat();
					gps.date.value();
					gps.time.value();
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main(int argc, char * argv[])
{
	NeguPi::Millis::instance();

	//-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively

	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	gpsfd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (gpsfd == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}

	std::cout << gpsfd << std::endl;

	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(gpsfd, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	//tcflush(gpsfd, TCIFLUSH);
	tcsetattr(gpsfd, TCSANOW, &options);

    write(gpsfd, (const void*)UBLOX_INIT, sizeof(UBLOX_INIT));

    /*
    const unsigned char cmd[] = "$PUBX,41,1,3,3,38400,0*24\r\n";
    std::cout << sizeof(cmd) << std::endl;
	write(gpsfd, cmd, sizeof(cmd));
    */

	close(gpsfd);

	gpsfd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (gpsfd == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}

	std::cout << gpsfd << std::endl;

	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	tcgetattr(gpsfd, &options);
	options.c_cflag = B38400 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	//tcflush(gpsfd, TCIFLUSH);
	tcsetattr(gpsfd, TCSANOW, &options);

	std::thread gps_thread(read_gps);

	while (1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	//----- CLOSE THE UART -----
	close(gpsfd);

	return 0;
}
