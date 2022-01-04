/*
 * Copyright (c) 2018, Texas Instruments Incorporated - http://www.ti.com/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \addtogroup cc13xx-cc26xx-platform
 * @{
 *
 * \file
 *        Implementation of the dbg module for CC13xx/CC26xx, used by stdio.
 *        The dbg module is implemented by writing to UART0.
 * \author
 *        Edvard Pettersen <e.pettersen@ti.com>
 */
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include "sys/cc.h"
/*---------------------------------------------------------------------------*/
#include "uart0-arch.h"
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stddef.h>
#include <string.h>
/*---------------------------------------------------------------------------*/
int
dbg_putchar(int c)
{
  unsigned char ch;
  int num_bytes;

  ch = (unsigned char)c;

  num_bytes = (int)uart0_write(&ch, 1);
  return (num_bytes > 0)
         ? num_bytes
         : 0;
}
/*---------------------------------------------------------------------------*/
unsigned int
dbg_send_bytes(const unsigned char *seq, unsigned int len)
{
  size_t seq_len;
  size_t max_len;
  int num_bytes;

  seq_len = strlen((const char *)seq);
  max_len = MIN(seq_len, (size_t)len);

  if(max_len == 0) {
    return 0;
  }

  //@Siva -- Added for proper printing in CCS
  /* Find the number of new line characters in the sequence. */
  size_t n = 0;
  for(size_t i = 0; i < max_len; i++)
    if(*(seq + i) == '\n')
      ++n;

  if(n > 0) {
    /*
     * Create the second sequence by inserting a carriage return character
     * before any new line one in the first sequence.
     */
    unsigned char ch, seq2[max_len + n];
    size_t j = 0;
    for(size_t i = 0; i < max_len; i++) {
      ch = *(seq + i);
      if(ch != '\n')
        seq2[j] = ch;
      else {
        seq2[j] = '\r';
        seq2[++j] = '\n';
      }
      ++j;
    }
    num_bytes = (int)uart0_write((const unsigned char *)seq2, max_len + n);
  }
  else
    //@Siva -- End of Added for proper printing in CCS
    num_bytes = (int)uart0_write(seq, max_len);



  return (num_bytes > 0)
         ? num_bytes
         : 0;
}
/*---------------------------------------------------------------------------*/
/** @} */
