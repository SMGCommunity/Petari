# Speaker
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| SpkSpeakerCtrl.o | 55.497382198952884% | 15 / 21 | 71.42857142857143% | :eight_pointed_black_star: 
| SpkSystem.o | 70.3125% | 11 / 14 | 78.57142857142857% | :eight_pointed_black_star: 
| SpkMixingBuffer.o | 55.952380952380956% | 3 / 4 | 75.0% | :eight_pointed_black_star: 
| SpkWave.o | 100.0% | 7 / 7 | 100.0% | :white_check_mark: 
| SpkTable.o | 18.181818181818183% | 1 / 2 | 50.0% | :eight_pointed_black_star: 
| SpkData.o | 100.0% | 4 / 4 | 100.0% | :white_check_mark: 
| SpkSound.o | 31.11413043478261% | 13 / 22 | 59.09090909090909% | :eight_pointed_black_star: 


# SpkSpeakerCtrl.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| setMixingBuffer__14SpkSpeakerCtrlFP15SpkMixingBuffer | :white_check_mark: |
| setup__14SpkSpeakerCtrlFv | :x: |
| connect__14SpkSpeakerCtrlFl | :x: |
| disconnect__14SpkSpeakerCtrlFl | :x: |
| setSpeakerOn__14SpkSpeakerCtrlFl | :white_check_mark: |
| setSpeakerOnCallback__14SpkSpeakerCtrlFll | :white_check_mark: |
| setSpeakerPlay__14SpkSpeakerCtrlFl | :white_check_mark: |
| startPlayCallback__14SpkSpeakerCtrlFll | :x: |
| setSpeakerOff__14SpkSpeakerCtrlFl | :white_check_mark: |
| retryConnection__14SpkSpeakerCtrlFl | :x: |
| reconnect__14SpkSpeakerCtrlFl | :white_check_mark: |
| framework__14SpkSpeakerCtrlFv | :white_check_mark: |
| reconnectProcess__14SpkSpeakerCtrlFl | :white_check_mark: |
| initReconnect__14SpkSpeakerCtrlFl | :white_check_mark: |
| continuousUsingProcess__14SpkSpeakerCtrlFl | :white_check_mark: |
| updateSpeaker__14SpkSpeakerCtrlFP7OSAlarmP9OSContext | :x: |
| isEnable__14SpkSpeakerCtrlFl | :white_check_mark: |
| extensionProcess__14SpkSpeakerCtrlFll | :white_check_mark: |
| getDeviceVolume__14SpkSpeakerCtrlFl | :white_check_mark: |
| __sinit_\SpkSpeakerCtrl_cpp | :white_check_mark: |
| __arraydtor$3716 | :white_check_mark: |


# SpkSystem.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__9SpkSystemFP7JKRHeap | :white_check_mark: |
| __dt__18JSUList&lt;8SpkSound&gt;Fv | :white_check_mark: |
| setResource__9SpkSystemFP10JKRArchiveUsUs | :white_check_mark: |
| framework__9SpkSystemFv | :white_check_mark: |
| startSound__9SpkSystemFllP14SpkSoundHandle | :white_check_mark: |
| newSoundMemPool__9SpkSystemFl | :white_check_mark: |
| connect__9SpkSystemFl | :white_check_mark: |
| disconnect__9SpkSystemFl | :white_check_mark: |
| reconnect__9SpkSystemFl | :white_check_mark: |
| extensionProcess__9SpkSystemFll | :white_check_mark: |
| getDeviceVolume__9SpkSystemFl | :white_check_mark: |
| __sinit_\SpkSystem_cpp | :x: |
| __ct__21JASMemPool&lt;8SpkSound&gt;Fv | :x: |
| __dt__21JASMemPool&lt;8SpkSound&gt;Fv | :x: |


# SpkMixingBuffer.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__15SpkMixingBufferFP7JKRHeap | :white_check_mark: |
| mix__15SpkMixingBufferFlPslfl | :x: |
| getSamples__15SpkMixingBufferCFl | :white_check_mark: |
| update__15SpkMixingBufferFl | :white_check_mark: |


# SpkWave.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__7SpkWaveFv | :white_check_mark: |
| setResource__7SpkWaveFPv | :white_check_mark: |
| getWaveSize__7SpkWaveCFl | :white_check_mark: |
| getLoopStartPos__7SpkWaveCFl | :white_check_mark: |
| getLoopEndPos__7SpkWaveCFl | :white_check_mark: |
| getWave__7SpkWaveCFl | :white_check_mark: |
| getWaveData__7SpkWaveCFl | :white_check_mark: |


# SpkTable.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__8SpkTableFv | :white_check_mark: |
| setResource__8SpkTableFPv | :x: |


# SpkData.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__7SpkDataFP10JKRArchive | :white_check_mark: |
| loadTable__7SpkDataFUs | :white_check_mark: |
| loadWave__7SpkDataFUs | :white_check_mark: |
| isValid__7SpkDataCFv | :white_check_mark: |


# SpkSound.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| releaseSound__14SpkSoundHandleFv | :white_check_mark: |
| __ct__14SpkSoundVolumeFv | :white_check_mark: |
| setRelease__14SpkSoundVolumeFl | :white_check_mark: |
| setFadeOut__14SpkSoundVolumeFl | :white_check_mark: |
| calc__14SpkSoundVolumeFRb | :x: |
| __ct__8SpkSoundFv | :white_check_mark: |
| __dt__8SpkSoundFv | :x: |
| update__8SpkSoundFf | :x: |
| stop__8SpkSoundFl | :white_check_mark: |
| attachHandle__8SpkSoundFP14SpkSoundHandle | :white_check_mark: |
| releaseHandle__8SpkSoundFv | :white_check_mark: |
| start__8SpkSoundFll | :white_check_mark: |
| unlock__8SpkSoundFv | :white_check_mark: |
| setWaveData__8SpkSoundFl | :x: |
| __ct__14SpkSoundHolderFv | :white_check_mark: |
| startSound__14SpkSoundHolderFllP14SpkSoundHandle | :x: |
| update__14SpkSoundHolderFl | :white_check_mark: |
| updateEachSound__14SpkSoundHolderFl | :x: |
| freeDeadSound__14SpkSoundHolderFl | :x: |
| framework__14SpkSoundHolderFv | :x: |
| __ct__18JSUList&lt;8SpkSound&gt;Fv | :white_check_mark: |
| __sinit_\SpkSound_cpp | :x: |


