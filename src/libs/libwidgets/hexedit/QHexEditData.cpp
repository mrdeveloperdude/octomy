#include "QHexEditData.hpp"

#include "uptime/MethodGate.hpp"
#include "uptime/New.hpp"


const qint64 QHexEditData::BUFFER_SIZE = 8192;

QHexEditData::QHexEditData(QIODevice *iodevice, QObject *parent): QObject(parent), _iodevice(iodevice), _length(iodevice->size()), _devicelength(iodevice->size()), _lastpos(-1), _lastaction(QHexEditData::None)
{
	OC_METHODGATE();
	qRegisterMetaType<QHexEditData::ActionType>("QHexEditData::ActionType");
	this->_modlist.append(OC_NEW ModifiedItem(0, iodevice->size(), false));
}

QHexEditData::~QHexEditData()
{
	OC_METHODGATE();
	for(ModifiedItem* mi: this->_modlist) {
		delete mi;
		mi=nullptr;
	}

	this->_modlist.clear();

	if(this->_iodevice->isOpen()) {
		this->_iodevice->close();
	}
}

QUndoStack *QHexEditData::undoStack()
{
	OC_METHODGATE();
	return &this->_undostack;
}

qint64 QHexEditData::length() const
{
	OC_METHODGATE();
	return this->_length;
}

bool QHexEditData::isReadOnly() const
{
	OC_METHODGATE();
	return !this->_iodevice->isWritable();
}

bool QHexEditData::save()
{
	OC_METHODGATE();
	return this->saveTo(this->_iodevice);
}

bool QHexEditData::saveTo(QIODevice *iodevice)
{
	OC_METHODGATE();
	if(!iodevice->isOpen()) {
		iodevice->open(QIODevice::WriteOnly);
	}

	if(!iodevice->isWritable()) {
		return false;
	}

	qint64 outseekpos = 0;
	this->_iodevice->reset();

	for(int i = 0; i < this->_modlist.length(); i++) {
		ModifiedItem* mi = this->_modlist[i];
		iodevice->seek(outseekpos);

		if(mi->modified()) {
			iodevice->write(this->_modbuffer.mid(mi->pos(), mi->length()));
		} else if(iodevice != this->_iodevice) {
			this->_iodevice->seek(mi->pos());
			QByteArray ba = this->_iodevice->read(mi->length());
			iodevice->write(ba);
		}

		outseekpos += mi->length();
	}

	return true;
}

QHexEditData *QHexEditData::fromDevice(QIODevice *iodevice)
{
	OC_FUNCTIONGATE();
	if(!iodevice->isOpen()) {
		iodevice->open(QFile::ReadWrite);
	}

	return OC_NEW QHexEditData(iodevice);
}

QHexEditData *QHexEditData::fromFile(QString filename)
{
	OC_FUNCTIONGATE();
	QFileInfo fi(filename);
	QFile* f = OC_NEW QFile(filename);

	if(fi.isWritable()) {
		f->open(QFile::ReadWrite);
	} else {
		f->open(QFile::ReadOnly);
	}

	return QHexEditData::fromDevice(f);
}

QHexEditData *QHexEditData::fromMemory(const QByteArray &ba)
{
	OC_FUNCTIONGATE();
	QBuffer* b = OC_NEW QBuffer();
	b->setData(ba);
	b->open(QFile::ReadOnly);

	return QHexEditData::fromDevice(b);
}

QHexEditData::InsertCommand* QHexEditData::internalInsert(qint64 pos, const QByteArray &ba, QHexEditData::ActionType act)
{
	OC_METHODGATE();
	if(pos < 0 || pos > this->length() || !ba.length()) {
		return nullptr;
	}

	int i=0;
	qint64 datapos;
	ModifiedItem* mi;

	if(!this->_modlist.isEmpty()) {
		mi = this->modifiedItem(pos, &datapos, &i);

		if(!mi) {
			return nullptr;
		}
	} else {
		pos = 0;
		datapos = 0;
	}

	bool optimize = false;
	ModifyList oldml, newml;
	qint64 modoffset = pos - datapos;
	qint64 buffoffset = this->updateBuffer(ba);

	if(!modoffset) {
		optimize = (act == QHexEditData::Insert) && this->canOptimize(act, pos);
		newml.append(OC_NEW ModifiedItem(buffoffset, ba.length()));
	} else {
		oldml.append(mi);
		newml.append(OC_NEW ModifiedItem(mi->pos(), modoffset, mi->modified()));
		newml.append(OC_NEW ModifiedItem(buffoffset, ba.length()));
		newml.append(OC_NEW ModifiedItem(mi->pos() + modoffset, mi->length() - modoffset, mi->modified()));
	}


	this->_length += ba.length();
	emit dataChanged(pos, ba.length(), act);
	return OC_NEW InsertCommand(i, pos, oldml, newml, this, optimize);
}

QHexEditData::RemoveCommand* QHexEditData::internalRemove(qint64 pos, qint64 len, QHexEditData::ActionType act)
{
	OC_METHODGATE();
	if(pos < 0 || !len || pos >= this->length() || pos > (this->length() - len)) {
		return nullptr;
	}

	int i;
	qint64 datapos;
	ModifiedItem* mi = this->modifiedItem(pos, &datapos, &i);

	if(!mi) {
		return nullptr;
	}

	QList<ModifiedItem*> olditems, newitems;
	qint64 remoffset = pos - datapos, remlength = len;

	if(remoffset) {
		newitems.append(OC_NEW ModifiedItem(mi->pos(), remoffset, mi->modified()));

		if((remoffset + remlength) < mi->length()) {
			newitems.append(OC_NEW ModifiedItem(mi->pos() + remoffset + remlength, mi->length() - remoffset - remlength, mi->modified()));
		}

		remlength -= qMin(remlength, mi->length() - remoffset);
		olditems.append(mi);
		i++;
	}

	while(remlength && (i < this->_modlist.length())) {
		mi = this->_modlist[i];

		if(remlength < mi->length()) {
			newitems.append(OC_NEW ModifiedItem(mi->pos() + remlength, mi->length() - remlength, mi->modified()));
		}

		remlength -= qMin(remlength, mi->length());
		olditems.append(mi);
		i++;
	}

	this->_length -= len;
	emit dataChanged(pos, len, act);
	return OC_NEW RemoveCommand(i - olditems.length(), pos, olditems, newitems, this);
}

bool QHexEditData::canOptimize(QHexEditData::ActionType at, qint64 pos)
{
	OC_METHODGATE();
	return (this->_lastaction == at) && (this->_lastpos == pos);
}

void QHexEditData::recordAction(QHexEditData::ActionType at, qint64 pos)
{
	OC_METHODGATE();
	this->_lastpos = pos;
	this->_lastaction = at;
}

qint64 QHexEditData::updateBuffer(const QByteArray &ba)
{
	OC_METHODGATE();
	qint64 pos = this->_modbuffer.length();
	this->_modbuffer.append(ba);
	return pos;
}

QHexEditData::ModifiedItem* QHexEditData::modifiedItem(qint64 pos, qint64* datapos, int *index)
{
	OC_METHODGATE();
	int i = -1;
	qint64 currpos = 0;
	ModifiedItem* mi = nullptr;

	for(i = 0; i < this->_modlist.length(); i++) {
		mi = this->_modlist[i];

		if((pos >= currpos) && (pos < (currpos + mi->length()))) {
			if(datapos) {
				*datapos = currpos;
			}

			if(index) {
				*index = i;
			}

			return mi;
		}

		currpos += mi->length();
	}

	if(mi && (pos == currpos)) { /* Return Last Item for Append */
		if(datapos) {
			*datapos = currpos;
		}

		if(index) {
			*index = i;
		}

		return mi;
	}

	return nullptr;
}
