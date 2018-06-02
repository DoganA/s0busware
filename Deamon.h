/*
 *    Dateiname: Deamon.h
 *
 *       Author: Dogan Alkan
 * Matrikel-Nr.: s772991
 * Beschreibung: Es wurde im Rahmen der Bachelorarbeit erstellt.
 *               Funktionsdefinitionen, um ein Deamon, also einen LLinux Dienstprogramm zu erstellen.
 */
#include <syslog.h>

#ifndef _DEAMON_H_
	#define _DEAMON_H_

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef DEAMON_H_
   #define DEAMON_H_

#define DEAMON_NAME	"Bachelorarbeit"
#define DAEMON_VERSION	"v1.0"
#define DAEMON_BUILD "1"

void daemonize(char *rundir, char *pidfile);
void daemonShutdown();
void signal_handler(int sig);

#endif /* SRC_DEAMONIZER_H_ */
