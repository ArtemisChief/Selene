#pragma once
#include <QObject>
#include <QThread>

class MicrosecondTimer : public QObject{

	Q_OBJECT

public:

	MicrosecondTimer();

	void Start(const int microsecond);

	void Stop();

private:

	QThread thread_;
	
	bool should_stop_;

signals:

	void StartTiming(const int microsecond);
	void TimeOut();

private slots:

	void Timing(const int microsecond);

};