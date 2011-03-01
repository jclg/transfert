/**
 * @file   receiver.c
 * @author  <jc@candice>
 * @date   Fri Jun  4 15:33:08 2010
 * 
 * @brief  
 * 
 * 
 */

#include "transfert.h"

/* serveur receive */


void                    no_zombies(int i)
{
  i = 0;
  while (waitpid(0, NULL, WNOHANG) != -1);
}


int	receive_file(int cs, char filename[PATH_MAX], 
		     unsigned char md5_checksum[EVP_MAX_MD_SIZE], 
		     char file_size[MAX_SIZE])
{
  int	fd;
  int	size;
  int	total_size;
  int	total_size_max;
  char	buf[BUF_SIZE];
  int	ctr;


  /*****
	filename
	size
	md5
	content
   *****/


  /* filename */
  if (read(cs, filename, PATH_MAX) < 0)
    return (1);
  if (write(cs, STATUT, STATUT_SIZE) < 0)
    return (1);

  /* get real filename */
  filename = basename(filename);
  printf("FILENAME %s\n", filename);

  /* Filesize */
  if (read(cs, file_size, MAX_SIZE) < 0)
    return (1);
  if (write(cs, STATUT, STATUT_SIZE) < 0)
    return (1);
  printf("SIZE %s\n", file_size);
  
  /* MD5 */
  if (read(cs, md5_checksum, EVP_MAX_MD_SIZE) < 0)
    return (1);
  printf("MD5 ");
  print_md5(md5_checksum, 16);
  printf("\n");
  if (write(cs, STATUT, STATUT_SIZE) < 0)
    return (1);



  if ((fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC,
		 S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == -1)
    return (printf("Error: Can't create file...\n"));


  ctr = 0;
  total_size = 0;
  total_size_max = atoi(file_size);
  while (total_size < total_size_max)
    {
      if ((size = read(cs, buf, BUF_SIZE)) < 0)
	return (1);
      total_size += size;
      if ((ctr % TRANS_DISPLAY) == 0)
	printf("Getting %d / %s bytes\n", total_size, file_size);
      if (write(fd, buf, size) < 0)
	return (1);
      ctr++;
    }
  printf("Getting %d / %s bytes\n", total_size, file_size);
  close(fd);
  printf("Transfert over\n");
  return (0);
}

int			checking_file_integrity(char *filename, unsigned char md5_checksum[EVP_MAX_MD_SIZE])
{
  int			fd;
  struct stat           sb;
  void                  *map;
  unsigned char		md_value[EVP_MAX_MD_SIZE];
  unsigned int		md_len;

  printf("Checking file integrity...\n");
  fd = open(filename, O_RDONLY);
  if (fd == -1)
    return (printf("transfer: No such file or no permissions to read it.The file can't be checksumed.\n"));


  fstat(fd, &sb);
  map = mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (map == (void *)MAP_FAILED)
    return (printf("transfert: Mapping file failed.\n"));
  close(fd);



  printf("md5 from remote file : \t");
  print_md5(md5_checksum, 16);
  printf("\n");
  get_md5((char *)map, (size_t)sb.st_size, md_value, &md_len); 
  munmap(map, sb.st_size);

  printf("md5 from local file : \t");
  print_md5(md_value, md_len);
  printf("\n");


  if (compare_md5(md_value, md5_checksum, md_len) == 0)
    printf("File check is OK\n Transfer OK\n");
  else
    printf("File check is NOT ok\n Transfer goes wrong, the file might be corrupted\n");


  printf("Done for %s.\n\n\nWaiting for files to receive...\n", filename);
  return (0);
}


int                     server_ready(int s)
{
  char                  root_dir[PATH_MAX] = {0};
  int                   p;
  int                   cs;
  char                  filename[PATH_MAX] = {0};
  unsigned char		md5_checksum[EVP_MAX_MD_SIZE] = {0};
  char                  file_size[MAX_SIZE] = {0};
  struct sockaddr_in    client_sin;
  socklen_t             client_sin_len;

  getcwd(root_dir, PATH_MAX);
  signal(SIGCHLD, &no_zombies);
  client_sin_len = sizeof(client_sin);
  while (1)
    {
      cs = accept(s, (struct sockaddr *)&client_sin, &client_sin_len);
      printf("connection from %s  %i\n\n", inet_ntoa(client_sin.sin_addr),
             ntohs(client_sin.sin_port));
      p = fork();
      if (p == 0)
        {
	  while (1)
            {
	      receive_file(cs, filename, md5_checksum, file_size);
	      checking_file_integrity(basename(filename), md5_checksum);
              exit(0);
	    }
        }
      close(cs);
    }
  return (0);
}

int     serveur_side(int port)
{
  int	s;

  if ((s = network_listen(port)) < 0)
    return (1);
  printf("Listening on port %d...\nWaiting for files to receive...\n", port);
  server_ready(s);
  close(s);
  return (0);
}


/* end serveur receive */
