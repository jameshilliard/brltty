/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2019 by The BRLTTY Developers.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU Lesser General Public License, as published by the Free Software
 * Foundation; either version 2.1 of the License, or (at your option) any
 * later version. Please see the file LICENSE-LGPL for details.
 *
 * Web Page: http://brltty.app/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#include "prologue.h"

#include "log.h"
#include "api_control.h"
#include "api_server.h"
#include "core.h"

#ifndef ENABLE_API
const char *const api_parameters[] = {NULL};

void
api_identify (int full) {
}

int
api_startServer (BrailleDisplay *brl, char **parameters) {
  return 0;
}

void
api_stopServer (BrailleDisplay *brl) {
}

void
api_link (BrailleDisplay *brl) {
}

void
api_unlink (BrailleDisplay *brl) {
}

void
api_suspendDriver (BrailleDisplay *brl) {
}

int
api_resumeDriver (BrailleDisplay *brl) {
  return 0;
}

int
api_claimDriver (BrailleDisplay *brl) {
  return 0;
}

void
api_releaseDriver (BrailleDisplay *brl) {
}

int
api_handleCommand (int command) {
  return 0;
}

int
api_handleKeyEvent (KeyGroup group, KeyNumber number, int press) {
  return 0;
}

int
api_flushOutput (BrailleDisplay *brl) {
  return 0;
}

void
api_updateParameter (brlapi_param_t parameter, brlapi_param_subparam_t subparam) {
}
#endif /* ENABLE_API */

static int isRunning = 0;
static int apiLinked = 0;
static int isClaimed = 0;

static void
apiIdentify (int full) {
  api_identify(full);
}

static const char *const *
apiGetParameters (void) {
  return api_parameters;
}

static int
apiStartServer (char **parameters) {
  if (api_startServer(&brl, parameters)) {
    isRunning = 1;
    return 1;
  }

  return 0;
}

static void
apiStopServer (void) {
  api_stopServer(&brl);
  isRunning = 0;
}

static int
apiIsServerRunning (void) {
  return isRunning;
}

static void
apiLink (void) {
  if (isRunning) {
    api_link(&brl);
    apiLinked = 1;
  }
}

static void
apiUnlink (void) {
  if (isRunning) {
    api_unlink(&brl);
    apiLinked = 0;
  }
}

static int
apiIsLinked (void) {
  return apiLinked;
}

static void
apiSuspendDriver (void) {
#ifdef ENABLE_API
  if (isRunning) {
    api_suspendDriver(&brl);
  } else
#endif /* ENABLE_API */

  {
    destructBrailleDriver();
  }
}

static int
apiResumeDriver (void) {
#ifdef ENABLE_API
  if (isRunning) return api_resumeDriver(&brl);
#endif /* ENABLE_API */

  return constructBrailleDriver();
}

static int
apiClaimDriver (void) {
  if (!isClaimed && isRunning) {
    if (!api_claimDriver(&brl)) return 0;
    isClaimed = 1;
  }

  return 1;
}

static void
apiReleaseDriver (void) {
  if (isClaimed) {
    api_releaseDriver(&brl);
    isClaimed = 0;
  }
}

static int
apiHandleCommand (int command) {
  if (!isRunning) return 0;
  return api_handleCommand(command);
}

static int
apiHandleKeyEvent (KeyGroup group, KeyNumber number, int press) {
  if (!isRunning) return 0;
  return api_handleKeyEvent(group, number, press);
}

static int
apiFlushOutput (void) {
  if (!isRunning) return 1;
  return api_flushOutput(&brl);
}

static void
apiUpdateParameter (brlapi_param_t parameter, brlapi_param_subparam_t subparam) {
  if (isRunning) api_updateParameter(parameter, subparam);
}

const ApiMethods api = {
  .identify = apiIdentify,
  .getParameters = apiGetParameters,

  .startServer = apiStartServer,
  .stopServer = apiStopServer,
  .isServerRunning = apiIsServerRunning,

  .link = apiLink,
  .unlink = apiUnlink,
  .isLinked = apiIsLinked,

  .suspendDriver = apiSuspendDriver,
  .resumeDriver = apiResumeDriver,

  .claimDriver = apiClaimDriver,
  .releaseDriver = apiReleaseDriver,

  .handleCommand = apiHandleCommand,
  .handleKeyEvent = apiHandleKeyEvent,

  .flushOutput = apiFlushOutput,
  .updateParameter = apiUpdateParameter
};
