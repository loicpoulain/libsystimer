/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * This file is part of libsystimer
 *
 * Copyright (C) 2022 Loic Poulain <loic.poulain@linaro.org>
 */

#include <QSysTimer.h>
#include <QtGlobal>
#include <QSocketNotifier>

#include <systimer.h>

QSysTimer::QSysTimer(clockid_t clock_id)
	: m_singleshot(false)
{
	this->fd = systimer_create(clock_id);
	Q_ASSERT(this->fd != -1);
	this->notifier = new QSocketNotifier(this->fd, QSocketNotifier::Read, this);
	Q_ASSERT(this->notifier);
	connect(this->notifier, &QSocketNotifier::activated, this, &QSysTimer::onActivation);
}

QSysTimer::~QSysTimer()
{
	disconnect(this->notifier, &QSocketNotifier::activated, this, &QSysTimer::onActivation);
	delete this->notifier;
	systimer_destroy(this->fd);
}

bool QSysTimer::isActive() const
{
	return systimer_active(this->fd);
}

Qt::TimerType QSysTimer::timerType() const
{
	return Qt::PreciseTimer;
}

void QSysTimer::start(std::chrono::milliseconds msec)
{
	this->msec = msec;
	if (systimer_start(this->fd, msec.count()))
		throw QString(strerror(errno));
}

void QSysTimer::start()
{
	if (systimer_start(this->fd, this->msec.count()))
		throw QString(strerror(errno));
}

void QSysTimer::stop()
{
	systimer_stop(this->fd);
}

int QSysTimer::interval() const
{
	return this->msec.count();
}

void QSysTimer::setInterval(int msec)
{
	this->msec = std::chrono::milliseconds(msec);
}

void QSysTimer::setInterval(std::chrono::milliseconds value)
{
	this->msec = value;
}

std::chrono::milliseconds QSysTimer::intervalAsDuration() const
{
	return this->msec;
}

int QSysTimer::remainingTime() const
{
	return systimer_value(this->fd);
}

std::chrono::milliseconds QSysTimer::remainingTimeAsDuration() const
{
	return std::chrono::milliseconds(systimer_value(this->fd));
}

void QSysTimer::setSingleShot(bool singleShot)
{
	this->m_singleshot = singleShot;
}

bool QSysTimer::isSingleShot() const
{
	return this->m_singleshot;
}

void QSysTimer::onActivation(int fd)
{
	if (fd != this->fd)
		return;

	if (systimer_expired(this->fd)) {
		if (!this->isSingleShot())
			systimer_start(this->fd, this->msec.count());
		emit this->timeout(QSysTimer::QPrivateSignal());
	}
}