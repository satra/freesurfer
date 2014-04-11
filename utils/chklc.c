/**
 * @file  chklc.c
 * @brief Routine to check .license file
 *
 */
/*
 * Original Author: Bruce Fischl
 * CVS Revision Info:
 *    $Author: rpwang $
 *    $Date: 2014/04/11 18:37:09 $
 *    $Revision: 1.18 $
 *
 * Copyright © 2011-2013 The General Hospital Corporation (Boston, MA) "MGH"
 *
 * Terms and conditions for use, reproduction, distribution and contribution
 * are found in the 'FreeSurfer Software License Agreement' contained
 * in the file 'LICENSE' found in the FreeSurfer distribution, and here:
 *
 * https://surfer.nmr.mgh.harvard.edu/fswiki/FreeSurferSoftwareLicense
 *
 * Reporting: freesurfer@nmr.mgh.harvard.edu
 *
 */

#include <unistd.h>
#include <const.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern char *crypt(const char *, const char *);

static const char* errmsg =
"--------------------------------------------------------------------------\n"
"ERROR: FreeSurfer environment FREESURFER_HOME is not defined.\n"
"  If you are outside the NMR-Martinos Center, please set this\n"
"  variable to the location where you installed FreeSurfer.\n"
"  If you are inside the NMR-Martinos Center, please source\n"
"  the standard environment. If you need to install FreeSurfer,\n"
"  go to: http://surfer.nmr.mgh.harvard.edu\n"
"--------------------------------------------------------------------------\n";

static const char* licmsg =
"--------------------------------------------------------------------------\n"
"ERROR: FreeSurfer license file %s not found.\n"
"  If you are outside the NMR-Martinos Center,\n"
"  go to http://surfer.nmr.mgh.harvard.edu to \n"
"  get a valid license file (it's free).\n"
"  If you are inside the NMR-Martinos Center,\n"
"  make sure to source the standard environment.\n"
"--------------------------------------------------------------------------\n";

#ifndef Darwin
static const char* licmsg2 =
"--------------------------------------------------------------------------\n"
"ERROR: Invalid FreeSurfer license key found in license file %s\n"
"  If you are outside the NMR-Martinos Center,\n"
"  go to http://surfer.nmr.mgh.harvard.edu to \n"
"  get a valid license file (it's free).\n"
"  If you are inside the NMR-Martinos Center,\n"
"  make sure to source the standard environment.\n"
"--------------------------------------------------------------------------\n";
#endif

void chklc(void)
{
  char  dirname[STRLEN], *cp ;
  FILE* lfile;
  char* email;
  char* magic;
  char* key;
  char* gkey;
  char* lfilename;
  char  str[STRLEN] ;

  sprintf(str, "S%sER%sRONT%sOR", "URF", "_F", "DO") ;
  if (getenv(str) != NULL) return ;

  cp = getenv("FREESURFER_HOME");
  if (cp == NULL)
  {
    fprintf(stderr,"%s",errmsg);
    exit(-1);
  }

  strncpy(dirname, cp, STRLEN) ;

  lfilename = (char*)calloc(1,512);
  email = (char*)calloc(1,512);
  magic = (char*)calloc(1,512);
  key   = (char*)calloc(1,512);
  gkey  = (char*)calloc(1,1024);

  sprintf(lfilename,"%s/.lic%s",dirname, "ense");

  lfile = fopen(lfilename,"r");
  if (lfile == NULL)
  {
    if(errno == EACCES){
      printf("\n\nERROR: FreeSurfer license file %s exists but you do not have read permission\n",lfilename);
      printf("   Try running chmod a+r %s\n\n\n",lfilename);
      exit(-1);
    }
    sprintf(lfilename,"%s/lic%s",dirname, "ense.txt");
    lfile = fopen(lfilename,"r");
  }
  if (lfile == NULL)
  {
    if(errno == EACCES){
      printf("\n\nERROR: FreeSurfer license file %s exists but you do not have read permission\n",lfilename);
      printf("   Try running chmod a+r %s\n\n\n",lfilename);
      exit(-1);
    }
    fprintf(stderr,licmsg,lfilename);
    exit(-1);
  }

  fscanf(lfile,"%s\n",email);
  fscanf(lfile,"%s\n",magic);
  fscanf(lfile,"%s\n",key);

  sprintf(gkey,"%s.%s",email,magic);

#ifndef Darwin
  {
    char* crypt_gkey = crypt(gkey,"*C*O*R*T*E*C*H*S*0*1*2*3*");
    // it seems some implementations of crypt (like Fedora18)
    // don't like the input we give it and crypt returns NULL, which crashes 
    // those systems, so check and skip
    if (crypt_gkey == NULL)
    {
      //fprintf(stderr,"chklc: crypt returned NULL!\n");
    }
    else
    {
      if (strcmp(key,crypt(gkey,crypt_gkey))!=0)
      {
        fprintf(stderr,licmsg2,lfilename);
        exit(-1);
      }
    }
  }
#endif

  free(email);
  free(magic);
  free(key);
  free(gkey);
  free(lfilename);
  fclose(lfile) ;

  return;
}

//  return value:   0 - failed
//                  1 - passed
//  if failed, error msg will be returned in msg. make sure msg is pre-allocated with enough space
int chklc2(char* msg)
{
  char  dirname[STRLEN], *cp ;
  FILE* lfile;
  char* email;
  char* magic;
  char* key;
  char* gkey;
  char* lfilename;
  char  str[STRLEN] ;

  sprintf(str, "S%sER%sRONT%sOR", "URF", "_F", "DO") ;
  if (getenv(str) != NULL) return 1;

  cp = getenv("FREESURFER_HOME");
  if (cp == NULL)
  {
    fprintf(stderr,"%s",errmsg);
    if (msg)
      sprintf(msg, "%s", errmsg);
    return 0;
  }

  strncpy(dirname, cp, STRLEN) ;

  lfilename = (char*)calloc(1,512);
  email = (char*)calloc(1,512);
  magic = (char*)calloc(1,512);
  key   = (char*)calloc(1,512);
  gkey  = (char*)calloc(1,1024);

  sprintf(lfilename,"%s/.lic%s",dirname, "ense");

  lfile = fopen(lfilename,"r");
  char permission_msg[] =
    "---------------------------------------------------------------------------\n"
    "ERROR: FreeSurfer license file %s exists \n"
    "but you do not have read permission. Try running\n\n"
    "  chmod a+r %s\n"
    "---------------------------------------------------------------------------\n";
  if (lfile == NULL)
  {
    if(errno == EACCES){
      fprintf(stderr,permission_msg,lfilename,lfilename);
      if (msg)
        sprintf(msg,permission_msg,lfilename,lfilename);
      return 0;
    }
    sprintf(lfilename,"%s/lic%s",dirname, "ense.txt");
    lfile = fopen(lfilename,"r");
  }
  if (lfile == NULL)
  {
    if(errno == EACCES){
      fprintf(stderr,permission_msg,lfilename,lfilename);
      if (msg)
        sprintf(msg,permission_msg,lfilename,lfilename);
      return 0;
    }
    fprintf(stderr,licmsg,lfilename);
    if (msg)
      sprintf(msg,licmsg,lfilename);
    return 0;
  }

  fscanf(lfile,"%s\n",email);
  fscanf(lfile,"%s\n",magic);
  fscanf(lfile,"%s\n",key);

  sprintf(gkey,"%s.%s",email,magic);

#ifndef Darwin
  {
    char* crypt_gkey = crypt(gkey,"*C*O*R*T*E*C*H*S*0*1*2*3*");
    // it seems some implementations of crypt (like Fedora18)
    // don't like the input we give it and crypt returns NULL, which crashes
    // those systems, so check and skip
    if (crypt_gkey == NULL)
    {
      //fprintf(stderr,"chklc: crypt returned NULL!\n");
    }
    else
    {
      if (strcmp(key,crypt(gkey,crypt_gkey))!=0)
      {
        fprintf(stderr,licmsg2,lfilename);
        if (msg)
          sprintf(msg, licmsg2, lfilename);
        return 0;
      }
    }
  }
#endif

  free(email);
  free(magic);
  free(key);
  free(gkey);
  free(lfilename);
  fclose(lfile) ;

  return 1;
}

