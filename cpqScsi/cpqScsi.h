/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.old-api.conf 14476 2006-04-18 17:36:51Z hardaker $
 */
#ifndef CPQSCSI_H
#define CPQSCSI_H

#define CPQMIB                  5
#define CPQMIBREVMAJOR          1
#define CPQMIBREVMINOR          38
#define CPQMIBHEALTHINDEX       10

#define CPQSCSIMIBREVMAJOR              1
#define CPQSCSIMIBREVMINOR              2
#define CPQSCSIMIBCONDITION             3

#define CPQ_REG_OTHER            1
#define CPQ_REG_OK               2
#define CPQ_REG_DEGRADED         3
#define CPQ_REG_FAILED           4

/*
 * function declarations 
 */
Netsnmp_Node_Handler   cpqScsi_handler;
void              init_cpqScsi(void);

#endif                          /* CPQSCSI_H */