// coding: utf-8
/*
 * Copyright (c) 2017, Christopher Durand
 * Copyright (c) 2017, Niklas Hauser
 * Copyright (c) 2018, Fabian Greif
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef	XPCC_ZEROMQ_READER_HPP
#define	XPCC_ZEROMQ_READER_HPP

#include <thread>
#include <mutex>
#include <atomic>
#include <deque>

#include <zmqpp/zmqpp.hpp>

#include "../header.hpp"

#include <modm/debug/logger.hpp>
#undef MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::ERROR

namespace xpcc
{

/**
 * @brief	Reads packets from a zmqpp socket in a background thread
 *
 * @ingroup	backend
 *
 * @author	Christopher Durand <christopher.durand@rwth-aachen.de>
 * @author	strongly-typed
 */
class ZeroMQReader
{
public:
	static constexpr int PollTimeoutMs = 100;

	struct Packet
	{
		Packet(uint16_t size, const Header& inHeader) :
			header(inHeader), payload(size)
		{
		}

		xpcc::Header header;
		modm::SmartPointer payload;
	};

	ZeroMQReader(zmqpp::socket& socketIn_, std::size_t maxQueueSize_ = 1000);

	~ZeroMQReader();

	ZeroMQReader(const ZeroMQReader&) = delete;
	ZeroMQReader& operator=(const ZeroMQReader&) = delete;

	void
	start();

	void
	stop();

	bool
	isPacketAvailable() const;

	const Packet&
	getPacket() const;

	void
	dropPacket();

private:
	void
	receiveThread();

	void
	readPacket(const zmqpp::message& message);

private:
	zmqpp::socket& socketIn;

	std::deque<Packet> queue;
	mutable std::mutex queueMutex;
	const std::size_t maxQueueSize;

	std::thread thread;
	std::atomic<bool> stopThread;
};

} // modm namespace

#endif // XPCC_ZEROMQ_READER_HPP
