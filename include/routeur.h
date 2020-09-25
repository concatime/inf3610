/*
 * router.h
 *
 *  Created on: 26 July 2020
 *      Author: Guy BOIS
 */

#ifndef ROUTEUR_H
#define ROUTEUR_H

#include <inttypes.h>
#include <os.h>
#include <stdbool.h>
#include <stdlib.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define TASK_STK_SIZE 8192

/* ************************************************
 *                TASK PRIOS
 **************************************************/

#define TaskGeneratePRIO 5
#define TaskStopPRIO 21
#define TaskResetPRIO 20
#define TaskStatsPRIO 3
#define TaskComputingPRIO 11
#define TaskForwardingPRIO 10
#define TaskOutputPortPRIO 12

#define TaskFitTimer_10s_PRIO 3

#define WAITFOR3TICKS 3

/* Routing info. */

#define NB_OUTPUT_PORTS 3

#define INT1_LOW 0x00000000
#define INT1_HIGH 0x3FFFFFFF
#define INT2_LOW 0x40000000
#define INT2_HIGH 0x7FFFFFFF
#define INT3_LOW 0x80000000
#define INT3_HIGH 0xBFFFFFFF
#define INT_BC_LOW 0xC0000000
#define INT_BC_HIGH 0xFFFFFFFF

/* Reject source info. */
#define REJECT_LOW1 0x10000000
#define REJECT_HIGH1 0x17FFFFFF
#define REJECT_LOW2 0x50000000
#define REJECT_HIGH2 0x57FFFFFF
#define REJECT_LOW3 0x60000000
#define REJECT_HIGH3 0x67FFFFFF
#define REJECT_LOW4 0xD0000000
#define REJECT_HIGH4 0xD7FFFFFF

typedef struct {
	int id;
	char *name;
} Info_Port;

Info_Port Port[NB_OUTPUT_PORTS];

typedef enum {
	PACKET_VIDEO,
	PACKET_AUDIO,
	PACKET_AUTRE,
	NB_PACKET_TYPE
} PACKET_TYPE;

typedef struct {
	unsigned int src;
	unsigned int dst;
	PACKET_TYPE type;
	unsigned int data[13];
} Packet;

/* ************************************************
 *                  Queues
 **************************************************/

OS_Q inputQ;
OS_Q lowQ;
OS_Q mediumQ;
OS_Q highQ;
OS_Q MBPort[NB_OUTPUT_PORTS];

/* ************************************************
 *                  Semaphores
 **************************************************/

OS_SEM semStop;
OS_SEM semStats;
OS_SEM semReset;

/* ************************************************
 *                  Mutexes
 **************************************************/
OS_MUTEX mutRejete;
OS_MUTEX mutPrint;
OS_MUTEX mutAlloc;

/* DECLARATION DES COMPTEURS */
int nbPacketCrees = 0;        /* Nb de packets total créés */
int nbPacketTraites = 0;      /* Nb de paquets envoyés sur une interface */
int nbPacketSourceRejete = 0; /* Nb de packets rejetés pour mauvaise source */

/* ************************************************
 *              TASK PROTOTYPES
 **************************************************/

void TaskGenerate(void *data); /* Function prototypes of tasks */
void TaskStats(void *data);
void TaskStop(void *data);
void TaskReset(void *data);
void TaskComputing(void *data);
void TaskForwarding(void *data);
void TaskVerify(void *data); /* Pour plus tard */
void TaskOutputPort(void *data);
void Taskgpio_isr(void *data);

void dispatch_packet(Packet *packet);

void create_application();
int create_tasks();
int create_events();
void err_msg(char *, uint8_t);

#endif /* ROUTEUR_H */
