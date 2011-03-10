/**
 * @file   transfert.h
 * @author jc <jean-charles.le-goff@epitech.eu>
 * @date   Fri Jun  4 11:32:58 2010
 * 
 * @brief  
 * 
 * 
 */

/*
sender -client
receiver - server
*/
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/evp.h>
#include <fcntl.h>
#include <libgen.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PATH_MAX 4096
#define BUF_SIZE 4096
#define MAX_SIZE 30
#define TRANS_DISPLAY 500
#define STATUT "OKAY\n"
#define STATUT_SIZE 5

#define USAGE "*** Transfert ***\nMade by JC\n\nTo receive file:\n./transfert receive port\n\nTo send file:\n./transfert send host port file\n"

/* main.c */

/* network.c */
char	*network_try_resolve_hostname(char *host);
int	network_connect(char *host, int port);
int	network_listen(int port);
/* sender.c */
int     client_side(char *host, int port, char *filename);

/* receiver.c */
int     serveur_side(int port);

/* md5.c */
void		get_md5(char *content, size_t size,
                        unsigned char md_value[EVP_MAX_MD_SIZE],
			unsigned int *md_len);
void		print_md5(unsigned char md_value[EVP_MAX_MD_SIZE],
                          unsigned int md_len);
int		compare_md5(unsigned char md_value_one[EVP_MAX_MD_SIZE],
                            unsigned char md_value_two[EVP_MAX_MD_SIZE],
                            unsigned int md_len);
