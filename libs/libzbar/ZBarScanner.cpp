#include "ZBarScanner.hpp"

#include "../libutil/utility/widgets/PixViewer.hpp"

#include <QVideoFrame>
#include <QPixmap>
#include <QImage>
#include <QDebug>

// Depends on Qt for declaring NULL macro, so must come last
#include "zbar.h"


ZBarScanner::ZBarScanner()
	: sc(nullptr)
	, pv(nullptr)
	, mScaleDown(false)
	, mConvertToGrayscale(true)
{
	//pv=new PixViewer();
	if(nullptr!=pv) {
		pv->setWindowTitle("zbar grayscale");
	}
}


QList<ZScanResult> ZBarScanner::scan(const QVideoFrame &frame)
{
	const int w=frame.width();
	const int h=frame.height();
	// Qt recommended way to process video frames without disrupting the flow of the video
	QVideoFrame cloneFrame(frame);
	cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
	const QImage image(cloneFrame.bits(), w,h, QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));
	cloneFrame.unmap();
	return scan(image);
}



QList<ZScanResult> ZBarScanner::scan(const QPixmap &frame)
{
	return scan(frame.toImage());
}


QList<ZScanResult> ZBarScanner::scan(const QImage &image)
{
	QList<ZScanResult> out;
	if(nullptr==sc) {
		sc=new zbar::ImageScanner;
		if(nullptr!=sc) {
//TODO: Configure scanner here.
		}
	}
	if(nullptr!=sc) {
		QImage imageToUse=image;
		if(mScaleDown) {
			imageToUse=imageToUse.scaled(QSize(400,400),Qt::KeepAspectRatio);
		}
		if(mConvertToGrayscale) {
			imageToUse=imageToUse.convertToFormat(QImage::Format_Grayscale8);
		}
		if(nullptr!=pv) {
			pv->setImage(imageToUse);
			pv->show();
		}
		zbar::QZBarImage im(imageToUse);
		int found=sc->scan(im);
		if(found>0) {
			zbar::SymbolSet ss=sc->get_results();
			for(zbar::SymbolIterator b=ss.symbol_begin(), e=ss.symbol_end(); b!=e; ++b) {
				zbar::Symbol sym=*b;
				ZScanResult res;
				res.data=QString::fromStdString(sym.get_data()).trimmed();
				res.type=QString::fromStdString(sym.get_type_name()).trimmed();
				int ls=sym.get_location_size();
				if(ls>0) {
					sym.get_type_name();
					res.outline.moveTo(sym.get_location_x(0), sym.get_location_y(0));
					for(int i=1; i<ls; ++i) {
						//qDebug()<<" + "<<sym.get_location_x(i)<<"x"<<sym.get_location_y(i);
						res.outline.lineTo(sym.get_location_x(i), sym.get_location_y(i));
					}
					res.outline.lineTo(sym.get_location_x(0), sym.get_location_y(0));
				}
				//qDebug()<<"Z-SCANNED STRING: "<<str;
				qDebug()<<"Z-SYMBOL "<<sym.get_data_length()<<" = "<<res.data<<", "<<res.type<<", "<<ls;
				out<<res;
			}
		} else {

		}
		//auto pf=image.pixelFormat();
//		qDebug()<<"Z-SCANNING FRAME "<<image.width()<<"x"<<image.height() /*<<"@"<<pf*/<< " found "<<found;
	} else {
		qWarning()<<"ERROR: Could not instanciate image scanner";
	}

	return out;
}
