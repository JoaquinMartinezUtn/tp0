#include "client.h"

int main(void) {
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();
	log_info(logger, "Hola! Soy un log");

	config = iniciar_config();
	leer_consola(logger);
	conexion = crear_conexion(config_get_string_value(config, "IP"), config_get_string_value(config, "PUERTO"));
	enviar_mensaje(config_get_string_value(config, "CLAVE"), conexion);
	paquete(conexion);

	terminar_programa(conexion, logger, config);
	printf("\n####CLIENT-PROGRAM-FINISHED###\n");
}

t_log* iniciar_logger(void) {
	t_log* nuevo_logger = log_create("logs/cliente.log","CLIENT_LOG",1,LOG_LEVEL_INFO);
	if(nuevo_logger == NULL) {
		perror("Error, create new log.");
		exit(EXIT_FAILURE);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void) {
	t_config* nuevo_config = config_create("cliente.config");
	if(nuevo_config == NULL) {
		perror("Error, create config");
		exit(EXIT_FAILURE);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger) {
	char* leido;
    do {
        leido = readline("> ");
        if (strlen(leido) > 0) {
            log_info(logger, "%s", leido);
            free(leido);
        }
    } while (strlen(leido) > 0);
}

void paquete(int conexion) {
    t_paquete *paquete  = crear_paquete();
    char *leido;
    do {
        leido = readline("> ");
        if (strlen(leido) > 0) {
            agregar_a_paquete(paquete, leido, strlen(leido) + 1);
            free(leido);
        }
    } while (strlen(leido) > 0);

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);

}

void terminar_programa(int conexion, t_log* logger, t_config* config) {
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
