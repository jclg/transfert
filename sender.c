/**
 * @file   sender.c
 * @author jc <jean-charles.le-goff@epitech.eu>
 * @date   Fri Jun  4 15:37:32 2010
 * 
 * @brief  
 * 
 * 
 */

#include "transfert.h"

/* client sender */

int			send_file(int s, char *filename, void *map, int size)
{
  char                  buf[BUF_SIZE];
  unsigned char		md_value[EVP_MAX_MD_SIZE];
  unsigned int		md_len;

  /*****
   Filename
   Filesize
   MD5 Checksum
   Content
  ******/


  /* Filename */
  if (write(s, filename, strlen(filename)) < 0)
    return (1);
  if (read(s, buf, BUF_SIZE) < 0)
    return (1);


  /* Filesize */
  snprintf(buf, BUF_SIZE, "%d", (uint)size);
  if (write(s, buf, strlen(buf)) < 0)
    return (1);
  if (read(s, buf, BUF_SIZE) < 0)
    return (1);


  /* MD5 Checksum */
  get_md5((char *)map, (size_t)size, md_value, &md_len);
  printf("md5 checksum: ");
  print_md5(md_value, md_len);
  printf("\n");
  if (write(s, md_value, md_len) < 0)
    return (1);
  if (read(s, buf, BUF_SIZE) < 0)
    return (1);


  /* Content */
  printf("Transfering file \"%s\"...\n", basename(filename));
  if (write(s, (char *)map, (uint)size) < 0)
    return (1);



  return (0);
}

int     client_side(char *host, int port, char *filename)
{
  int                   s;
  struct stat           sb;
  void                  *map;
  int			fd;

  fd = open(filename, O_RDONLY);
  if (fd == -1)
    return (printf("transfer: No such file or no permissions to read it. The file can't be transfered.\n"));
  fstat(fd, &sb);
  map = mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (map == (void *)MAP_FAILED)
    return (printf("transfert: Mapping file failed.\n"));
  close(fd);

  if ((s = network_connect(host, port)) < 0)
    return (1);

  printf("Begin transfer for \"%s\"\n", basename(filename));
  send_file(s, filename, map, sb.st_size);

  munmap(map, sb.st_size);
  close(s);
  return (0);
}


/* end client sender */
