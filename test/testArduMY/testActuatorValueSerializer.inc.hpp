#ifndef TEST_INCLUDED

#include "test.inc.hpp"

/**
 * @brief testActuatorValueSerializerInclude
 * @param inSet
 *
 * Attempt to serialize input actuator value and then parse it back to make sure the value is intact
 *
 */
void testActuatorValueSerializerInclude(ArduMYActuatorSet &inSet)
{
#endif

	//qDebug()<<"BEFORE A--------";
	//logLines(ardumyActuatorSetToString(inSet));
	ArduMYActuatorValueSerializer serializer;
	serializer.setSet(inSet);
	ArduMYActuatorSet outSet;

	const auto inSize=inSet.size();
	// The out-set must match in size as this would be carried out by the set size command in a real world implementation
	outSet.setSize(inSize);
	// We also copy the representations of the sets as that would have been carried out in a series of set config commands in a real world implementation
	for(size_t i =0; i<inSize; ++i) {
		outSet[i].config=inSet[i].config;
		outSet[i].state.flags=inSet[i].state.flags;
		QCOMPARE(outSet[i].config, inSet[i].config);
		QCOMPARE(outSet[i].state.flags, inSet[i].state.flags);
		outSet[i].state.setDirty(false); // Don't spoil the test!
		outSet[i].config.setDirty(false); // Don't spoil the test!
		outSet[i].state.value.quadWord=0x00;
	}
	//qDebug()<<"BEFORE B--------";	logLines(actuatorSetToString(outSet));
//Converter cv;

	ArduMYActuatorValueParser parser;
	parser.setSet(outSet);

//ENABLED_ACTUATOR_BITS, ACTUATOR_VALUE_BATCHES, END_OF_OP
	const uint16_t enableBytes=(inSize+7)/8;
	uint8_t enanbledActuatorCount=0;
	for(uint16_t i=0; i< enableBytes; ++i) {
		//qDebug()<<"I: "<<(i+1)<<" / "<<enableBytes;
		QCOMPARE((uint8_t)serializer.step, (uint8_t)ArduMYActuatorValuesParserStep::ENABLED_ACTUATOR_BITS);
		QCOMPARE((uint8_t)parser.step, (uint8_t)ArduMYActuatorValuesParserStep::ENABLED_ACTUATOR_BITS);
		//QVERIFY(serializer.currentActuatorIndex
		QVERIFY(serializer.hasMoreData());
		//QCOMPARE(serializer.byteIndex, i); // NOTE: This test will fail because byteIndex is not used in Serializer
		QCOMPARE(parser.byteIndex, i);
		const uint8_t byte=serializer.nextByte();
		//qDebug()<<"Enable-Byte: "<<QString("%1").arg(byte,8,2,QChar('0'))<<"("<<byte<<")";
		// Count the enabled bits
		for(uint8_t b=0; b<8; ++b) {
			const uint8_t mask=(1<<b);
			const uint8_t value=byte&mask;
			if(value>0) {
				enanbledActuatorCount++;
			}
		}
		const bool res=parser.parse(byte);
		Q_UNUSED(res);

		//    QCOMPARE(res,false);

		//QCOMPARE(parser.enableBits[i], serializer.enableBits[i]);// NOTE: This test will fail because enableBits are not used in Serializer

	}

/*
	qDebug()<<"";
	qDebug()<<"------ enabled actuators: "<<enanbledActuatorCount<<", enableBytes: "<<enableBytes<<" parser.enabledActuatorCount"<<parser.enabledActuatorCount<<" serializer.enabledActuatorCount"<<serializer.enabledActuatorCount;
	qDebug()<<"";
*/
	QCOMPARE(parser.enabledActuatorCount, serializer.enabledActuatorCount);

	QCOMPARE(parser.byteIndex, (uint16_t)0);

	QByteArray baOut;

	int ix=0;
	if(enanbledActuatorCount>0) {
		//qDebug()<<"enanbledActuatorCount="<<enanbledActuatorCount<<", serializer.step="<<ardumyActuatorValueParserStepToString(serializer.step)<<", parser.step="<<ardumyActuatorValueParserStepToString(parser.step);
		QCOMPARE((uint8_t)serializer.step, (uint8_t)ArduMYActuatorValuesParserStep::ACTUATOR_VALUE_BATCHES);
		QCOMPARE((uint8_t)parser.step, (uint8_t)ArduMYActuatorValuesParserStep::ACTUATOR_VALUE_BATCHES);

		//QCOMPARE(parser.currentActuatorIndex, serializer.currentActuatorIndex);

		uint32_t loopCounter=0;
		while(true) {
			if(!serializer.hasMoreData()) {
				//qDebug()<<"XX SERIALIZER: END OF DATA REACHED @ "<<loopCounter;
				break;
			}
			QCOMPARE((uint8_t)serializer.step, (uint8_t)ArduMYActuatorValuesParserStep::ACTUATOR_VALUE_BATCHES);
			QCOMPARE((uint8_t)parser.step, (uint8_t)ArduMYActuatorValuesParserStep::ACTUATOR_VALUE_BATCHES);


			const uint8_t byte=serializer.nextByte();
			baOut[ix++]=byte;

			auto lca=parser.currentActuatorIndex;
			Q_UNUSED(lca);
			const bool res=parser.parse(byte);
			//qDebug()<<" + Value-Byte: "<<byte<<" SERIALIZER:"<<serializer.currentActuatorIndex<<" PARSER:"<<parser.currentActuatorIndex;
			if(res) {
				//qDebug()<<"XX PARSER: END OF OP REACHED @ "<<loopCounter<<" after "<<(lca+1) <<" actuators";
				break;
			}
			if(loopCounter++ > 1000000) {
				QFAIL("ERROR: LOOP NEVER ENDED");
				break;
			}
		}

		//qDebug()<<"";	qDebug()<<"---------------- LOOPS COUNTED: "<<loopCounter;	qDebug()<<"";

//	QVERIFY(!serializer.hasMoreData());

	} else {
		QCOMPARE((uint8_t)serializer.step, (uint8_t)ArduMYActuatorValuesParserStep::END_OF_OP);
		QCOMPARE((uint8_t)parser.step, (uint8_t)ArduMYActuatorValuesParserStep::END_OF_OP);
	}

	QCOMPARE(inSet.size(), outSet.size());
	QCOMPARE(inSet.size(), inSize);

	/*
	for(uint8_t byte:baOut) {
		qDebug()<<"ALL BYTES: "<<byte<<"(" << QString::number(byte,2)<<")";
	}
	*/

	for(size_t i =0; i<inSize; ++i) {
		const ArduMYActuator &a=inSet[i];
		const ArduMYActuator &b=outSet[i];
		// Only the dirty ones were copied!
		if(a.state.isDirty()) {
			//qDebug()<<"For actuator "<<(i+1)<<"/"<< inSize<<"SERIALIZER(A): "<<ardumyActuatorValueToString(a.state.value, a.config.representation)<<", PARSER(B): "<<ardumyActuatorValueToString(b.state.value, b.config.representation);
			if(ArduMYActuatorValueRepresentation::VALREP_BIT!=a.config.representation || true) {
				if(a.state.value != b.state.value) {
					Converter cva;
					Converter cvb;
					cva.uint64=a.state.value.quadWord;
					cvb.uint64=b.state.value.quadWord;
					qWarning()<<"ERROR: Actuator value is DIFFERENT! ";
					for(int j=0; j<8; ++j) {
						//qDebug()<<" + BYTE-CMP"<<cva.uint8[j]<< " vs. "<<cvb.uint8[j];
					}
				}


				QCOMPARE(a.state.value, b.state.value);
				QCOMPARE(a.state, b.state);
				QCOMPARE(a.config, b.config);
				QCOMPARE(a, b);
			}
		}
	}

	//qDebug()<<"AFTER A--------"; 	logLines(actuatorSetToString(inSet));
	//qDebug()<<"AFTER B--------";	logLines(actuatorSetToString(outSet));
	//QCOMPARE(inSet,outSet);


#ifndef TEST_INCLUDED
}
#endif











































