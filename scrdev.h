/*
 * BRLTTY - Access software for Unix for a blind person
 *          using a soft Braille terminal
 *
 * Copyright (C) 1995-1998 by The BRLTTY Team, All rights reserved.
 *
 * Nicolas Pitre <nico@cam.org>
 * St�phane Doyon <s.doyon@videotron.ca>
 * Nikhil Nair <nn201@cus.cam.ac.uk>
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation.  Please see the file COPYING for details.
 *
 * This software is maintained by Nicolas Pitre <nico@cam.org>.
 */

/* scrdev.h - C++ header file for the screen types library
 */

#ifndef _SCRDEV_H
#define _SCRDEV_H


extern "C"
{
#include "scr.h"
}
#include "helphdr.h"		// help file header format


// abstract base class - useful for pointers
class Screen			
{
  public:
  virtual void getstat (scrstat &) = 0;
  virtual unsigned char *getscr (winpos, unsigned char *, short) = 0;
};


// abstract base class - useful for screen source driver pointers
class RealScreen : public Screen
{
  public:
  virtual int open (void) = 0;
  virtual void close (void) = 0;
  virtual void getstat (scrstat &) = 0;
  virtual unsigned char *getscr (winpos, unsigned char *, short) = 0;
};



class FrozenScreen:public Screen
{
  scrstat stat;
  unsigned char *text;
  unsigned char *attrib;
    public:
    FrozenScreen ();
  int open (Screen *);		// called every time the screen is frozen
  void getstat (scrstat &);
  unsigned char *getscr (winpos, unsigned char *, short);
  void close (void);		// called to discard frozen screen image
};


class HelpScreen:public Screen
{
  int fd;
  short numpages;
  pageinfo *psz;
  unsigned char **page;
  unsigned char *buffer;
  short scrno;
  int gethelp (char*);
    public:
  HelpScreen ();
  void setscrno (short);
  short numscreens (void);
  int open (char* helpfile);		// called every time the help screen is opened
  void getstat (scrstat &);
  unsigned char *getscr (winpos, unsigned char *, short);
  void close (void);		// called once to close the help screen
};


/* The Live Screen type is instanciated elsewhere and choosen at link time
 * from all available screen source drivers.
 */

extern RealScreen *live;



#endif // !_SCRDEV_H
