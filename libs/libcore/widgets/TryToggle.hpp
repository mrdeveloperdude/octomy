#ifndef TRYTOGGLE_HPP
#define TRYTOGGLE_HPP

#include <QWidget>

#include <QTimer>
#include <QDebug>

namespace Ui {
	class TryToggle;
}

enum TryToggleState{
	OFF,TRYING,ON
};

QString ToggleStateToSTring(TryToggleState s);

class TryToggle : public QWidget
{
		Q_OBJECT
	private:
		QTimer timer;
		TryToggleState state;
		QString t1,t2,t3;

	public:
		explicit TryToggle(QWidget *parent = 0);
		~TryToggle();

	public:
		void setText(QString, QString, QString);

	private:
		void updateText();

	public slots:
		void setState(const TryToggleState s);

		void animateClick();
	private slots:
		void onTimeout();

		void on_pushButtonToggle_toggled(bool checked);

	private:
		Ui::TryToggle *ui;
	signals:
		void stateChanged(const TryToggleState s);
};


QDebug operator<<(QDebug d, const TryToggleState &s);


#endif // TRYTOGGLE_HPP