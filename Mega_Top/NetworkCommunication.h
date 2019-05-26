//this file is simply used as a bride between the c code and arduino code so it may be easily called.
#ifndef __NETWORK_COMMUNICATION_H__
#define __NETWORK_COMMUNICATION_H__

#ifdef __cplusplus
extern "C" {
#endif

void remoteCommunication();
void sendLocalMessage(byte startByte, byte task, byte ID, byte data, byte endByte);
void sendRemoteMessage(byte startByte, byte task, byte ID, int data, byte endByte);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __NETWORK_COMMUNICATION_H__ */
