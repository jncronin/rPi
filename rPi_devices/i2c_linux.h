/*
 * i2c_linux.h
 *
 *  Created on: Feb 2, 2013
 *      Author: jonathanhendrix
 */

#ifndef I2C_LINUX_H_
#define I2C_LINUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;

class i2c {
public:
	/**
	 * Setup the i2c bus and initialize it to disabled.
	 * @param bus
	 * @return a i2c object address
	 */
	static i2c& setup(unsigned short bus) {
		switch (bus)
		{
		case 0: return setup0();
		case 1: return setup1();
		case 2: return setup2();
		default: return setup0();
		}
	}

	/**
	 * Send a command to a slave device
	 * on this bus.
	 * @param slaveaddr
	 * @param cmd
	 * @param data_addr
	 * @param data_size
	 */
	unsigned command(unsigned char slaveaddr,
			unsigned char cmd,
			const unsigned char* data_addr=0,
			unsigned data_size=0)
	{
		set_slave(slaveaddr);
		unsigned char* buffer = new unsigned char[1 + data_size];
		buffer[0] = cmd;
		memcpy(&buffer[1], data_addr, data_size);
		if ((write(fd_, &cmd, 1+data_size)) != 1+data_size) {
			cerr << "Error writing to i2c slave" << endl;
			return 0;
		}
		delete buffer;
		return data_size;
	}

	/**
	 * Query a register on a slave device. Return
	 * a one-byte reply
	 * @param slaveaddr
	 * @param slave_register
	 * @return reply
	 */
	unsigned char query(unsigned char slaveaddr,
			unsigned char slave_register)
	{
		unsigned char buffer;
		if (query(slaveaddr, slave_register, &buffer, 1) == 1)
		{
			return buffer;
		}
		else
		{
			return 0;
		}
	}


	/**
	 * Query multiple registers on a slave device. Return
	 * the multi-byte reply
	 * @param slaveaddr
	 * @param slave_register
	 * @param buffer
	 * @param buffer_size
	 * @return number of bytes read
	 */
	unsigned query(unsigned char slaveaddr,
			unsigned char slave_register,
			unsigned char* buffer,
			unsigned buffer_size)
	{
		set_slave(slaveaddr);
		if ((write(fd_, &slave_register, 1)) != 1) { // Send register we want to read from
			cerr << "Error writing to i2c slave" << endl;
			return 0;
		}

		if (read(fd_, buffer, buffer_size) != buffer_size) {		// Read back data into buf[]
			cerr << "Unable to read from slave" << endl;
			return 0;
		}
		return buffer_size;
	}

private:

	/**
	 * Register offsets from the BAR.
	 */
	enum registers_t {
		CNTRL_REG = 0x00,
		STATUS_REG = 0x01,
		DLEN_REG = 0x02,
		SLAVE_ADDR_REG = 0x03,
		FIFO_REG = 0x04,
		CLOCK_DIV_REG = 0x05,
		DATA_DELAY_REG = 0x06,
		CLKT_REG = 0x07
	};


	/**
	 * Construct an i2c bus using
	 * a base address register
	 */
	i2c(const string& filename)
	: fd_ ()
	{
		if ((fd_ = open(filename.c_str(), O_RDWR)) < 0) {
			// Open port for reading and writing
			cerr << "Failed to open i2c port" << endl;
			exit(1);
		}
	}


	void set_slave(int address)
	{
		if (ioctl(fd_, I2C_SLAVE, address) < 0) {
			// Set the port options and set the address of the device we wish to speak to
			cerr << "Unable to get bus access to talk to slave" << endl;
			exit(1);
		}
	}

	int fd_;

	i2c(const i2c&); // no copy
	i2c& operator=(const i2c&); // no assign

	/**
	 * Setup the i2c bus 0 and initialize it to disabled.
	 */
	static i2c& setup0() {
		static i2c i2c0("/dev/i2c-0");
		return i2c0;
	}
	/**
	 * Setup the i2c bus 1 and initialize it to disabled.
	 */
	static i2c& setup1() {
		static i2c i2c1("/dev/i2c-1");
		return i2c1;
	}
	/**
	 * Setup the i2c bus 2 and initialize it to disabled.
	 */
	static i2c& setup2() {
		static i2c i2c2("/dev/i2c-2");  // Not available on RPi
		return i2c2;
	}
};



#endif /* I2C_LINUX_H_ */
