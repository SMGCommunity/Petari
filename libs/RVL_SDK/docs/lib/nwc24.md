# nwc24
| Symbol | Meaning 
| ------------- | ------------- 
| :x: | Object has not yet been started. 
| :eight_pointed_black_star: | Object is in progress. 
| :white_check_mark: | Object is completed. 


| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status 
| ------------- | ------------- | ------------- | ------------- | ------------- 
| NWC24StdAPI.o | 0.0% | 0 / 10 | 0.0% | :x: 
| NWC24FileAPI.o | 0.0% | 0 / 16 | 0.0% | :x: 
| NWC24Config.o | 0.0% | 0 / 9 | 0.0% | :x: 
| NWC24Utils.o | 0.0% | 0 / 4 | 0.0% | :x: 
| NWC24Manage.o | 12.719298245614036% | 4 / 11 | 36.36363636363637% | :eight_pointed_black_star: 
| NWC24MsgObj.o | 0.0% | 0 / 11 | 0.0% | :x: 
| NWC24MBoxCtrl.o | 0.0% | 0 / 14 | 0.0% | :x: 
| NWC24Mime.o | 0.0% | 0 / 5 | 0.0% | :x: 
| NWC24Parser.o | 0.0% | 0 / 4 | 0.0% | :x: 
| NWC24MsgCommit.o | 0.0% | 0 / 19 | 0.0% | :x: 
| NWC24Schedule.o | 49.60254372019078% | 6 / 10 | 60.0% | :eight_pointed_black_star: 
| NWC24DateParser.o | 0.0% | 0 / 7 | 0.0% | :x: 
| NWC24FriendList.o | 0.0% | 0 / 2 | 0.0% | :x: 
| NWC24SecretFList.o | 0.0% | 0 / 2 | 0.0% | :x: 
| NWC24UserId.o | 0.0% | 0 / 2 | 0.0% | :x: 
| NWC24Time.o | 11.78343949044586% | 1 / 5 | 20.0% | :eight_pointed_black_star: 
| NWC24Ipc.o | 100.0% | 6 / 6 | 100.0% | :white_check_mark: 
| NWC24Download.o | 0.0% | 0 / 6 | 0.0% | :x: 
| NWC24System.o | 100.0% | 2 / 2 | 100.0% | :white_check_mark: 


# NWC24StdAPI.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| Mail_strcpy | :x: |
| Mail_strlen | :x: |
| STD_strnlen | :x: |
| Mail_memcpy | :x: |
| Mail_memset | :x: |
| Mail_strcat | :x: |
| Mail_strncat | :x: |
| convNum | :x: |
| Mail_sprintf | :x: |
| Mail_vsprintf | :x: |


# NWC24FileAPI.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24FOpen | :x: |
| NWC24iFOpenNand | :x: |
| NWC24iFOpenVF | :x: |
| NWC24FClose | :x: |
| NWC24iFCloseNand | :x: |
| NWC24FSeek | :x: |
| NWC24FRead | :x: |
| NWC24FWrite | :x: |
| NWC24FGetLength | :x: |
| NWC24FDeleteVF | :x: |
| NWC24MountVF | :x: |
| NWC24UnmountVF | :x: |
| NWC24CheckSizeVF | :x: |
| BufferedWrite | :x: |
| BufferedWriteFlush | :x: |
| BufferedRead | :x: |


# NWC24Config.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24GetMyUserId | :x: |
| NWC24iConfigOpen | :x: |
| NWC24iConfigReload | :x: |
| NWC24iConfigFlush | :x: |
| NWC24GetAccountDomain | :x: |
| NWC24GetMBoxDir | :x: |
| NWC24GetAppId | :x: |
| NWC24GetGroupId | :x: |
| CheckConfig | :x: |


# NWC24Utils.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24Data_Init | :x: |
| NWC24Data_SetDataP | :x: |
| NWC24Date_Init | :x: |
| NWC24iConvIdToStr | :x: |


# NWC24Manage.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24iRegister | :white_check_mark: |
| NWC24OpenLib | :x: |
| NWC24OpenLibInternal | :x: |
| NWC24CloseLib | :x: |
| NWC24IsMsgLibOpened | :white_check_mark: |
| NWC24IsMsgLibOpenedByTool | :white_check_mark: |
| NWC24IsMsgLibOpenBlocking | :white_check_mark: |
| NWC24BlockOpenMsgLib | :x: |
| NWC24iSetNewMsgArrived | :x: |
| NWC24GetErrorCode | :x: |
| NWC24iSetErrorCode | :x: |


# NWC24MsgObj.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24InitMsgObj | :x: |
| NWC24SetMsgToId | :x: |
| NWC24SetMsgText | :x: |
| NWC24SetMsgAttached | :x: |
| NWC24SetMsgTag | :x: |
| NWC24SetMsgAltName | :x: |
| NWC24SetMsgMBNoReply | :x: |
| NWC24SetMsgMBDelay | :x: |
| NWC24SetMsgLedPattern | :x: |
| NWC24GetMsgSize | :x: |
| NWC24SetMsgDesignatedTime | :x: |


# NWC24MBoxCtrl.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24iOpenMBox | :x: |
| NWC24iMBoxOpenNewMsg | :x: |
| NWC24iMBoxCloseMsg | :x: |
| NWC24iMBoxCancelMsg | :x: |
| NWC24iMBoxAddMsgObj | :x: |
| NWC24iMBoxFlushHeader | :x: |
| NWC24iMBoxCheck | :x: |
| DeleteMsg | :x: |
| DuplicationCheck | :x: |
| GetCachedMBCHeader | :x: |
| AddMBCEntry | :x: |
| ClearMBCEntry | :x: |
| MountVFMBox | :x: |
| CopyMsgObjToMBCFmt | :x: |


# NWC24Mime.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| Base64Encode | :x: |
| NWC24Base64Encode | :x: |
| NWC24InitBase64Table | :x: |
| QEncode | :x: |
| NWC24EncodeQuotedPrintable | :x: |


# NWC24Parser.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24GetMIMETypeStr | :x: |
| NWC24iGetMIMETypeSuffix | :x: |
| NWC24GetCharsetStr | :x: |
| NWC24GetEncodingStr | :x: |


# NWC24MsgCommit.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24CommitMsg | :x: |
| NWC24CommitMsgInternal | :x: |
| CorrectHeadersForMBDelay | :x: |
| CheckMsgObject | :x: |
| CheckMsgBoxSpace | :x: |
| SynthesizeAddrStr | :x: |
| WriteSMTP_MAILFROM | :x: |
| WriteSMTP_RCPTTO | :x: |
| WriteFromField | :x: |
| WriteToField | :x: |
| WriteDateField | :x: |
| WriteXWiiAppIdField | :x: |
| WriteXWiiFaceField | :x: |
| WriteXWiiAltNameField | :x: |
| WriteMIMEAttachHeader | :x: |
| WriteContentTypeField | :x: |
| WritePlainText | :x: |
| WriteBase64Data | :x: |
| WriteQPData | :x: |


# NWC24Schedule.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24SuspendScheduler | :white_check_mark: |
| NWC24ResumeScheduler | :white_check_mark: |
| NWC24iSetScriptMode | :white_check_mark: |
| NWC24iRequestGenerateUserId | :x: |
| NWC24iTrySuspendForOpenLib | :x: |
| NWC24iResumeForCloseLib | :x: |
| ExecSuspendScheduler | :white_check_mark: |
| ExecTrySuspendScheduler | :x: |
| ExecResumeScheduler | :white_check_mark: |
| ExecNoParamCommand | :white_check_mark: |


# NWC24DateParser.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24iDateToMinutes | :x: |
| NWC24iMinutesToDate | :x: |
| NWC24iEpochSecondsToDate | :x: |
| NWC24iEpochSecondsToMinutes | :x: |
| NWC24iDateToOSCalendarTime | :x: |
| ConvertDateToDays | :x: |
| ConvertDaysToDate | :x: |


# NWC24FriendList.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24iOpenFriendList | :x: |
| GetCachedFLHeader | :x: |


# NWC24SecretFList.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24iOpenSecretFriendList | :x: |
| GetCachedSecretFLHeader | :x: |


# NWC24UserId.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24iCheckUserIdCRC | :x: |
| getUnScrambleId | :x: |


# NWC24Time.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24iGetUniversalTimeMinute | :x: |
| NWC24iGetUniversalTime | :x: |
| NWC24iGetTimeDifference | :x: |
| NWC24iSetRtcCounter | :x: |
| NWC24iSynchronizeRtcCounter | :white_check_mark: |


# NWC24Ipc.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24iOpenResourceManager | :white_check_mark: |
| NWC24iCloseResourceManager | :white_check_mark: |
| NWC24iIoctlResourceManager | :white_check_mark: |
| NWC24iIoctlResourceManagerAsync | :white_check_mark: |
| NWC24iIsAsyncRequestPending | :white_check_mark: |
| CallbackAsyncIpc | :white_check_mark: |


# NWC24Download.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24CheckDlTask | :x: |
| NWC24iOpenDlTaskList | :x: |
| NWC24iCloseDlTaskList | :x: |
| NWC24iCheckDlHeaderConsistency | :x: |
| NWC24iLoadDlHeader | :x: |
| DeleteDlTask | :x: |


# NWC24System.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| NWC24iPrepareShutdown | :white_check_mark: |
| NWC24Shutdown | :white_check_mark: |


