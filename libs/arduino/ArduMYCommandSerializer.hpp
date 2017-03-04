#ifndef ARDUMYCOMMANDSERIALIZER_HPP
#define ARDUMYCOMMANDSERIALIZER_HPP

#include "ArduMYParserState.hpp"
#include "MagicDetector.hpp"
#include "ArduMYActuatorValueSerializer.hpp"
#include "ArduMYActuatorConfigSerializer.hpp"
#include "ArduMYActuatorSet.hpp"

class ArduMYCommandSerializer
{
public:
	ArduMYActuatorSet &actuators;
	ArduMYParserState currentCommand;
	const uint8_t magic[4];
	uint8_t byteIndex;
	bool mSentCommandByte;
	ArduMYActuatorConfigSerializer actuatorConfigSerializer;
	int16_t actuatorConfigIndex;
	ArduMYActuatorValueSerializer actuatorValuesSerializer;

public:
	ArduMYCommandSerializer(ArduMYActuatorSet &actuators);

public:
	bool isReadyForNewCommand();
	void startSync();
	void startActuatorCount();
	void startActuatorConfig(uint8_t index);
	void startActuatorValues();

public:
	// Return true if there is more data remaining to be read
	bool hasMoreData();

	// Get the next byte of serial data. NOTE: only valid when hasMoreData() returns true, if not will return 0x00
	uint8_t nextByte();
};

#endif // ARDUMYCOMMANDSERIALIZER_HPP
