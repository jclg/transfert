/**
 * @file   md5.c
 * @author  <jc@candice>
 * @date   Fri Jun  4 15:52:41 2010
 * 
 * @brief  
 * 
 * 
 */


#include "transfert.h"


void		get_md5(char *content, size_t size, 
			unsigned char md_value[EVP_MAX_MD_SIZE], 
			unsigned int *md_len)
{
  EVP_MD_CTX	mdctx;

  EVP_DigestInit(&mdctx, EVP_md5());
  EVP_DigestUpdate(&mdctx, content, size);
  EVP_DigestFinal_ex(&mdctx, md_value, md_len);
  EVP_MD_CTX_cleanup(&mdctx);
}

void		print_md5(unsigned char md_value[EVP_MAX_MD_SIZE], 
			  unsigned int md_len)
{
  unsigned int	i;

  for(i = 0; i < md_len; i++)
    printf("%02x", md_value[i]);
}

int		compare_md5(unsigned char md_value_one[EVP_MAX_MD_SIZE], 
			    unsigned char md_value_two[EVP_MAX_MD_SIZE], 
			    unsigned int md_len)
{
  unsigned int	i;

  for(i = 0; i < md_len; i++)
    {
      if (md_value_one[i] != md_value_two[i])
	return (1);
    }
  return (0);
}
