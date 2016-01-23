#ifndef IDENTICONWIDGET_HPP
#define IDENTICONWIDGET_HPP

#include "widgets/SvgWidget.hpp"
#include <QWidget>
#include <QSvgRenderer>


class IdenticonWidget : public SvgWidget{
		Q_OBJECT
		Q_PROPERTY(QString svgURL READ svgURL WRITE setSvgURL DESIGNABLE true)

	private:
		quint64 data;

	public:
		explicit IdenticonWidget(QWidget *parent = 0);

		void setSvgURL(QString url);

	protected:

		void regenerateIdenticon();

	public:
		void setIdenticonData(quint64);

};

#endif // IDENTICONWIDGET_HPP



