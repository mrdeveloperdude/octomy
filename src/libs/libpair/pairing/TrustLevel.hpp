#ifndef TRUSTLEVEL_HPP
#define TRUSTLEVEL_HPP

#include <QString>
#include <QDebug>

enum TrustLevel:quint8 {
	IGNORE, TRUST, BLOCK, REMOVE
};


QString trustLevelToString(TrustLevel level);
TrustLevel trustLevelFromString(const QString &string);


QDebug operator<<(QDebug debug, TrustLevel level);


#endif
// TRUSTLEVEL_HPP