/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.old-api.conf 14476 2006-04-18 17:36:51Z hardaker $
 */
#ifndef CPQHOSTOS_H
#define CPQHOSTOS_H
/*
 * we may use header_generic and header_simple_table from the util_funcs module
 */
config_require(util_funcs/header_generic)
config_require(util_funcs)

/*
 * function declarations 
 */
Netsnmp_Node_Handler cpqHostMib_handler;
Netsnmp_Node_Handler cpqHostInfo_handler;
void            init_cpqHostOs(void);
FindVarMethod   var_cpqHostOs;
FindVarMethod   var_cpqHoFileSysTable;
FindVarMethod   var_cpqHoFwVerTable;

#define TELNET_OTHER            1
#define TELNET_AVAILABLE        2
#define TELNET_NOTAVAILABLE     3

#endif                          /* CPQHOSTOS_H */
