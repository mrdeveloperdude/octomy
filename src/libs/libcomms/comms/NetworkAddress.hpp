#ifndef NETWORKADDRESS_HPP
#define NETWORKADDRESS_HPP


#include <QHostAddress>
#include <QVariantMap>



class NetworkAddress{
	private:
		QHostAddress mIP;
		quint16 mPort;

	public:
		explicit NetworkAddress();
		NetworkAddress(const NetworkAddress &other);
		explicit NetworkAddress(QHostAddress ip, quint16 port);
		explicit NetworkAddress(QVariantMap map);
		virtual ~NetworkAddress();

	public:
		QHostAddress ip() const;
		quint16 port() const;
		void setPort(quint16 port);
		void setIP(QHostAddress ip);
		QVariantMap toVariantMap() const;
		QString toString() const;
		void fromString(QString, bool allowOnlyAddress=true);
		bool isValid(bool allowLoopback=true, bool allowMulticast=false) const;
		bool operator==(const NetworkAddress &o) const;
		bool operator!=(const NetworkAddress &o) const;

};

const QDebug &operator<<(QDebug &d, const NetworkAddress &na);

#endif // NETWORKADDRESS_HPP
