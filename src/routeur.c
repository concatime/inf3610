
/*
*********************************************************************************************************
*                                                 uC/OS-III
*                                          The Real-Time Kernel
*                                               PORT Windows
*
*
*            		          					  Guy BOIS
*                                  Polytechnique Montreal, Qc, CANADA
*                                                  09/2020
*
* File : routeur.c
*
*********************************************************************************************************
*/

#include "routeur.h"

#include "app_cfg.h"
#include "os_app_hooks.h"
#include <cpu.h>
#include <lib_mem.h>
#include <os.h>

/* Utile pour la partie 3 */
/*#include "bsp_init.h"
//#include <stdlib.h>
//#include <stdbool.h>
//#include <xil_printf.h>
//#include <xgpio.h>*/

/*/////////////////////////////////////////////////////////////////////////////////////
//								Routines d'interruptions
/////////////////////////////////////////////////////////////////////////////////////*/

/*

 À venir dans la partie 2

*/

/* Simulation du ISR TaskFitTimer_10s */

void TaskFitTimer_10s(void *data) {
	OS_ERR err;

	while (true) {
		/* À compléter */
	}
}

/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

int main(void) {

	OS_ERR os_err;
	/*   UCOS_LowLevelInit(); */

	CPU_IntInit();

	Mem_Init();   /* Initialize Memory Managment Module */
	CPU_IntDis(); /* Disable all Interrupts */
	CPU_Init();   /* Initialize the uC/CPU services */

	OSInit(&os_err);

	create_application();

	/*   Pour la partie 3 du laboratoire no 1 */
	/*   init_interrupt(); */
	/*   connect_axi(); */

	OSStart(&os_err);

	return 0;
}

void create_application() {
	int error;

	error = create_events();
	if (error != 0)
		printf("Error %d while creating events\n", error);

	error = create_tasks();
	if (error != 0)
		printf("Error %d while creating tasks\n", error);
}

int create_tasks() {

	int i;

	/* Stacks */
	static CPU_STK TaskGenerateSTK[TASK_STK_SIZE];

	/*	Pour la partie 2 du laboratoire no 1
	    static CPU_STK TaskStopSTK[TASK_STK_SIZE];
	  static CPU_STK TaskResetSTK[TASK_STK_SIZE]; */

	static CPU_STK TaskStatsSTK[TASK_STK_SIZE];

	static CPU_STK TaskComputingSTK[TASK_STK_SIZE];

	static CPU_STK TaskForwardingSTK[TASK_STK_SIZE];

	static CPU_STK TaskOutputPortSTK[NB_OUTPUT_PORTS][TASK_STK_SIZE];

	static CPU_STK TaskFitTimer_10s_STK[TASK_STK_SIZE];

	static OS_TCB TaskGenerateTCB;

	/*	Pour la partie 2 du laboratoire no 1
	  static OS_TCB TaskStopTCB;
	  static OS_TCB TaskResetTCB;
	*/

	static OS_TCB TaskStatsTCB;
	static OS_TCB TaskComputingTCB;
	static OS_TCB TaskForwardingTCB;
	static OS_TCB TaskOutputPortTCB[NB_OUTPUT_PORTS];

	static OS_TCB TaskFitTimer_10s_TCB;

	for (i = 0; i < NB_OUTPUT_PORTS; i++) {
		Port[i].id = i;
		switch (i) {
		case 0:
			Port[i].name = "Port 0";
			break;
		case 1:
			Port[i].name = "Port 1";
			break;
		case 2:
			Port[i].name = "Port 2";
			break;
		default:
			break;
		};
	}

	/* Creation des taches */
	OS_ERR err;
	OSTaskCreate(&TaskGenerateTCB, "TaskGenerate", TaskGenerate, (void *)0,
	             TaskGeneratePRIO, &TaskGenerateSTK[0u], TASK_STK_SIZE / 2,
	             TASK_STK_SIZE, 1, 0, (void *)0,
	             (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);

	/* Création de tâches à compléter */
	OSTaskCreate();

	OSTaskCreate();

	OSTaskCreate();

	for (i = 0; i < NB_OUTPUT_PORTS; i++) {
		OSTaskCreate();
	};

	OSTaskCreate();

	return 0;
}

int create_events() {
	OS_ERR err;
	int i;

	/* Creation des semaphores pour synchronisation */
	OSSemCreate(&semStop, "semStop", 0, &err);
	OSSemCreate(&semStats, "semStats", 0, &err);
	OSSemCreate(&semReset, "semReset", 0, &err);

	/* Mutex pour sections critiques n.b. le print */
	OSMutexCreate(&mutRejete, "mutRejete", &err);
	OSMutexCreate(&mutAlloc, "mutAlloc", &err);

	/* Creation des files */
	OSQCreate(&inputQ, "inputQ", 1024, &err);

	OSQCreate(&lowQ, "lowQ", 1024, &err);
	OSQCreate(&mediumQ, "mediumQ", 1024, &err);
	OSQCreate(&highQ, "highQ", 1024, &err);

	for (i = 0; i < NB_OUTPUT_PORTS; i++) {
		OSQCreate(&MBPort[i], "MailBox", 1, &err);
	}

	return 0;
}

/*/////////////////////////////////////////////////////////////////////////////////////
//									TASKS
/////////////////////////////////////////////////////////////////////////////////////*/

/*
 *********************************************************************************************************
 *											  TaskGeneratePacket
 *  - Génère des paquets et les envoie dans la InputQ.
 *  - À des fins de développement de votre application, vous pouvez
 **temporairement* modifier la variable "shouldSlowthingsDown" à true pour ne
 *générer que quelques paquets par seconde, et ainsi pouvoir déboguer le flot de
 *vos paquets de manière plus saine d'esprit. Cependant, la correction sera
 *effectuée avec cette variable à false.
 *********************************************************************************************************
 */
void TaskGenerate(void *data) {
	srand(42);
	OS_ERR err;
	CPU_TS ts;
	bool isGenPhase =
	    false; /* Indique si on est dans la phase de generation ou non */
	const bool shouldSlowThingsDown = false; /* Variable à modifier */
	int packGenQty = (rand() % 250);
	while (true) {
		if (isGenPhase) {
			OSMutexPend(&mutAlloc, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
			Packet *packet = malloc(sizeof(Packet));
			OSMutexPost(&mutAlloc, OS_OPT_POST_NONE, &err);

			packet->src = rand() * (UINT32_MAX / RAND_MAX);
			packet->dst = rand() * (UINT32_MAX / RAND_MAX);
			packet->type = rand() % NB_PACKET_TYPE;

			int i;
			for (i = 0; i < ARRAY_SIZE(packet->data); ++i)
				packet->data[i] = (unsigned int)rand();
			packet->data[0] = nbPacketCrees;

			nbPacketCrees++;

			/* Pour débug */
			printf("GENERATE : ********Generation du Paquet # %d ******** \n",
			       nbPacketCrees);
			printf("ADD %x \n", packet);
			printf("	** src : %x \n", packet->src);
			printf("	** dst : %x \n", packet->dst);
			printf("	** type : %d \n", packet->type);

			OSQPost(&inputQ, packet, sizeof(packet),
			        OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);

			if (err == OS_ERR_Q_MAX) {
				printf("GENERATE: Paquet rejete a l'entree car la FIFO est pleine !\n");
				OSMutexPend(&mutAlloc, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
				free(packet);
				OSMutexPost(&mutAlloc, OS_OPT_POST_NONE, &err);
			}

			if (shouldSlowThingsDown) {
				OSTimeDlyHMSM(0, 0, 0, 200 + rand() % 600, OS_OPT_TIME_HMSM_STRICT,
				              &err);
			} else {
				OSTimeDlyHMSM(0, 0, 0, 2, OS_OPT_TIME_HMSM_STRICT, &err);

				if ((nbPacketCrees % packGenQty) ==
				    0) /* On genère jusqu'à 250 paquets par phase de géneration */
				{
					isGenPhase = false;
				}
			}
		} else {
			/* OSTimeDlyHMSM(0, 0, 0, 500, OSTimeDlyHMSM, &err); */ /* Pour
			                                                           l'exécution sur
			                                                           Zedboard */
			OSTimeDlyHMSM(0, 0, 5, 0, OS_OPT_TIME_HMSM_STRICT,
			              &err); /* Pour la simulation sur Windows */
			isGenPhase = true;
			packGenQty = (rand() % 250);
			printf("GENERATE: Generation de %d paquets durant les %d prochaines "
			       "millisecondes\n",
			       packGenQty, packGenQty * 2);
		}
	}
}

/*
 *********************************************************************************************************
 *											  TaskStop
 *  -Stoppe le routeur une fois que 100 paquets ont été rejetés pour mauvais CRC
 *  -Ne doit pas stopper la tâche d'affichage des statistiques.
 *********************************************************************************************************
 */

void TaskStop(void *data) {
	OS_ERR err;
	CPU_TS ts;
	int i;
	while (true) {

		/* À compléter */
	}
}

/*
 *********************************************************************************************************
 *											  TaskReset
 *  -Remet le compteur de mauvaises sources à 0
 *  -Si le routeur était arrêté, le redémarre
 *********************************************************************************************************
 */
void TaskReset(void *data) {
	OS_ERR err;
	CPU_TS ts;
	int i;
	while (true) {

		/* À compléter */
	}
}

/*
 *********************************************************************************************************
 *											  TaskComputing
 *  -Vérifie si les paquets sont conformes (CRC,Adresse Source)
 *  -Dispatche les paquets dans des files (HIGH,MEDIUM,LOW)
 *
 *********************************************************************************************************
 */
void TaskComputing(void *pdata) {
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;
	Packet *packet = NULL;

	while (true) {

		/* À compléter */
	}
}

/*
 *********************************************************************************************************
 *											  TaskForwarding
 *  -traite la priorité des paquets : si un paquet de haute priorité est prêt,
 *   on l'envoie à l'aide de la fonction dispatch_packet, sinon on regarde les
 *paquets de moins haute priorité
 *********************************************************************************************************
 */
void TaskForwarding(void *pdata) {
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;
	Packet *packet = NULL;

	while (1) {
		/* Si paquet vidéo prêt */
		packet = OSQPend(&highQ); /* À compléter */
		if (err == OS_ERR_NONE) {
			/* Envoi du paquet */
			dispatch_packet(packet);
			printf("\n--TaskForwarding:  paquets %d envoyés\n\n", ++nbPacketTraites);
		} else {

			/* À compléter */
		}

		OSTimeDly(1, OS_OPT_TIME_DLY, &err); /* Pour simulation sous Windows */
	}
}

/*
 *********************************************************************************************************
 *											  Fonction Dispatch
 *  -Envoie le paquet passé en paramètre vers la mailbox correspondante à son
 *adressage destination
 *********************************************************************************************************
 */
void dispatch_packet(Packet *packet) {
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;

	/* Test sur la destination du paquet */

	/* À compléter */
}

/*
 *********************************************************************************************************
 *											  TaskPrint
 *  -Affiche les infos des paquets arrivés à destination et libere la mémoire
 *allouée
 *********************************************************************************************************
 */
void TaskOutputPort(void *data) {
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;
	Packet *packet = NULL;
	Info_Port info = *(Info_Port *)data;

	while (1) {

		/* À compléter */
	}
}

/*
 *********************************************************************************************************
 *                                              TaskStats
 *  -Est déclenchée lorsque le gpio_isr() libère le sémaphore
 *  -Lorsque déclenchée, imprime les statistiques du routeur à cet instant
 *********************************************************************************************************
 */
void TaskStats(void *pdata) {
	OS_ERR err;
	CPU_TS ts;

	while (1) {

		/* À compléter RDV unilatéral avec TaskFitTimer_10s */

		printf(
		    "\n------------------ Affichage des statistiques ------------------\n");
		printf("Nb de packets total traités : %d\n", nbPacketCrees);
		printf("Nb de packets total traités : %d\n", nbPacketTraites);
		printf("Nb de packets rejetés pour mauvaise source : %d\n",
		       nbPacketSourceRejete);
	}
}

void err_msg(char *entete, uint8_t err) {
	if (err != 0) {
		printf(entete);
		printf(": Une erreur est retournée : code %d \n", err);
	}
}
