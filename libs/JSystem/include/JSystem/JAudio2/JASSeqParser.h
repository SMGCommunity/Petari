#pragma once

class JASTrack;

class JASSeqParser {
public:
	typedef s32 (JASSeqParser::*Command)(const void *, void *, JASTrack *, u32 *);

	JASSeqParser();
	
	virtual ~JASSeqParser();
	virtual int parse(JASTrack *);
	virtual void execNoteOnMidi(JASTrack *, u32, u32, u32);
	virtual void execNoteOnGate(JASTrack *, u32, u32, u32, u32);
	virtual void execNoteOff(JASTrack *, u32);
	virtual void execCommand(JASTrack *, JASSeqParser::Command, u32, u32 *);
};