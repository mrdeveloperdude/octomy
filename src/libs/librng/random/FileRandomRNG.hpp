#ifndef FILERANDOMRNG_HPP
#define FILERANDOMRNG_HPP

#include "RNG.hpp"

#include <QFile>
/*
NOTE: This is not meant to be used directly, but rather as a subclass for /dev/xx
	  style random number sources.
*/

class FileRandomRNG : public RNG
{
private:
	QFile file;

public:
	FileRandomRNG(QString name, QString filename);

public:

	// Return true if this source is dependant on any pseudo rng
	inline bool hasPseudo() override {
		return false;
	}

	// Generates a random number on [0,0xffffffff]-interval
	// This is the main generator function that all the others are based on
	quint32 generateInt32(void) override;


	// Does nothing, this source has no seed
	virtual void init(quint32 s) override;

};

#endif
// FILERANDOMRNG_HPP
