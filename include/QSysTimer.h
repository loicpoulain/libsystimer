/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * This file is part of libsystimer.
 *
 * Copyright (C) 2022 Loic Poulain <loic.poulain@linaro.org>
 */

#ifndef QSYSTIMER_LINUX_H
#define QSYSTIMER_LINUX_H

#include <QtCore/qglobal.h>

#if defined(QSYSTIMER_LIBRARY)
#  define QSYSTIMERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QSYSTIMERSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QSysTimer.h>
#include <QSocketNotifier>
#include <QObject>

class Q_DECL_EXPORT QSysTimer : public QObject
{
	Q_OBJECT

public:
	QSysTimer(clockid_t clock_id = CLOCK_MONOTONIC);
	~QSysTimer();
	bool isActive() const;
	Qt::TimerType timerType() const;
	void start(std::chrono::milliseconds msec);
	void start();
	bool isSingleShot() const;
	void setSingleShot(bool singleShot);
	int remainingTime() const;
	std::chrono::milliseconds remainingTimeAsDuration() const;
	std::chrono::milliseconds intervalAsDuration() const;
	void setInterval(std::chrono::milliseconds value);
	void setInterval(int msec);
	int interval() const;
	void stop();

	virtual bool	event(QEvent *e) { return false; };
	virtual bool	eventFilter(QObject *watched, QEvent *event) { return false; };

signals:
	void timeout(QPrivateSignal);

private slots:
	void onActivation(int fd);

private:
	std::chrono::milliseconds msec;
	QSocketNotifier *notifier;
	bool m_singleshot;
	int fd;
};

#endif
