/* This file is distributed under the license as specified in the LICENSE file
 * that accompanied it. If there was no LICENSE file accompanying this source,
 * it is not to be modified, redistributed or used without prior written
 * permission from original author and owner "Lennart Rolland".
 */

#ifndef KEYSTORE_HPP
#define KEYSTORE_HPP


#include "Key.hpp"
#include "PortableID.hpp"
#include "basic/GenerateRunnable.hpp"
#include "basic/AtomicBoolean.hpp"
#include "node/SimpleDataStore.hpp"
#include "KeySecurityPolicy.hpp"

#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QMutex>
#include <QSharedPointer>
#include <QString>


class KeyStore: public QObject, public SimpleDataStore
{
	Q_OBJECT
private:

	bool mDoBootstrap;
	KeySecurityPolicy mPolicy;
	QSharedPointer<Key> mLocalKey;
	QMap<QString, QSharedPointer<Key> > mAssociates;



	friend class GenerateRunnable<KeyStore>;

public:
	explicit KeyStore(QString filename="", bool doBootstrap=false, KeySecurityPolicy policy=KeySecurityPolicy(), QObject *parent=nullptr);
	virtual ~KeyStore();

	// SimpleDataStore interface
public:
	bool fromMap(QVariantMap data)  Q_DECL_OVERRIDE;
	QVariantMap toMap() Q_DECL_OVERRIDE;


private:

	// To call this, make sure to set boostrapping to true and teh ncall synchronize() or save()
	void bootstrap();
public:


	bool bootstrapEnabled();
	void setBootstrapEnabled(bool doBootstrap);

	QSharedPointer<Key> localKey();

	PortableID localPortableID();

	void dump();

	// Sign message with our private key
	QByteArray sign(const QByteArray &source);

	// Verify signature with our pub-key
	bool verify(const QByteArray &message, const QByteArray &signature);

	// Verify signature with pub-key of node recognized by fiven fingerprint id
	bool verify(const QString &fingerprint, const QByteArray &message, const QByteArray &signature);

	// Check if we have pub-key for node identified by give fingerprint ID
	bool hasPubKeyForID(const QString &id);

	// Set pub-key for node identified by given fingerprint ID to given UTF-8 encoded string containing pubkey PEM format
	void setPubKeyForID(const QString &pubkeyPEM);

	// return pub-key for node identified by give fingerprint ID
	QSharedPointer<Key> pubKeyForID(const QString &id);
signals:

	void keystoreReady(bool);
	//storeReady(!mError);


public:

	friend const QDebug &operator<<(QDebug &d, KeyStore &ks);


};



const QDebug &operator<<(QDebug &d, KeyStore &ks);

#endif // KEYSTORE_HPP
