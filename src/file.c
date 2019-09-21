#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#include "file.h"

#define isEmpty(string) ((*string) == '\n' || (*string) == '\0')
#define eprintf(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)

#define LINE_LENGTH 256

static void removeComment(char const *string);
static void removeNewline(char const *string);
static int DataPointFromString(DataPoint *data, char *string);
static void reallocateDataPoints(DataPoints *points);
static char *isolateToken(char *string);
static int readMachineType(DataPoint *data, char const *token);
static int readBoxColor(DataPoint *data, char const *token);
static int readLocation(DataPoint *data, char const *token);
static int readEvent(DataPoint *data, char const *token);
static int readOtherPartsFound(DataPoint *data, char const *token);

static void removeComment(char const *string) {
  char *ch = strchr(string, '#');
  if(ch) {
    *ch = '\0';
  }
}

static void removeNewline(char const *string) {
  char *ch = strchr(string, '\n');
  if(ch) {
    *ch = '\0';
  }
}

int openDataFile(DataFile *file, char const *filename) {
  assert(filename != NULL);

  file->file = fopen(filename, "r");
  if(file->file == NULL) {
    return 1;
  }

  file->filename = strdup(filename);
  file->lineNumber = 0;
  return 0;
}

DataPoints readDataFile(DataFile *file) {
  assert(file != NULL);

  /* Gonna start with 256, that's a good starting point */
  DataPoints res;
  res.size = 256;
  res.numberUsed = 0;
  res.data = calloc(sizeof(DataPoint), 256);
  assert(res.data != NULL);

  while(!feof(file->file)) {
    if(res.numberUsed == res.size) {
      reallocateDataPoints(&res);
    }

    file->lineNumber++;
    char lineRead[LINE_LENGTH] = {};
    if(fgets(lineRead, sizeof(lineRead), file->file) == NULL) {
      break;
    }
    removeNewline(lineRead);
    removeComment(lineRead);
    if(isEmpty(lineRead)) {
      continue;
    }

    if(DataPointFromString(&res.data[res.numberUsed], lineRead) != 0) {
      eprintf("Error in file %s on line %lu\n", file->filename, file->lineNumber);
      continue;
    }

    res.numberUsed++;
  }
  return res;
}

void closeDataFile(DataFile *file) {
  assert(file != NULL);
  fclose(file->file);
  free((char *) file->filename);
}

static void reallocateDataPoints(DataPoints *points) {
  points->data = realloc(points->data, sizeof(DataPoint) * points->size * 2);
  assert(points->data != NULL);

  for(size_t i = 0; i < points->size; i++) {
    points->data[i + points->size] = (DataPoint) {};
  }
  points->size *= 2;
}

void freeDataPoints(DataPoints *points) {
  assert(points != NULL);
  free(points->data);
}

/* This will mangle the string, heads up */
static int DataPointFromString(DataPoint *data, char *string) {
  /* Get machine type */
  char *nextField = strchr(string, SEPARATOR);
  if(nextField == NULL) {
    return 1;
  }
  nextField++;

  char *token = isolateToken(string);
  if(readMachineType(data, token) != 0) {
    return 1;
  }

  /* Get box color */
  char *nextToken = nextField;
  nextField = strchr(nextField, SEPARATOR);
  if(nextField == NULL) {
    return 1;
  }
  nextField++;

  token = isolateToken(nextToken);
  if(readBoxColor(data, token) != 0) {
    return 1;
  }

  /* Get location */
  nextToken = nextField;
  nextField = strchr(nextField, SEPARATOR);
  if(nextField == NULL) {
    return 1;
  }
  nextField++;

  token = isolateToken(nextToken);
  if(readLocation(data, token) != 0) {
    return 1;
  }

  /* Get event */
  nextToken = nextField;
  nextField = strchr(nextField, SEPARATOR);
  if(nextField == NULL) {
    return 1;
  }
  nextField++;

  token = isolateToken(nextToken);
  if(readEvent(data, token) != 0) {
    return 1;
  }

  /* Get other parts found */
  nextToken = nextField;
  nextField = strchr(nextField, SEPARATOR);
  /* Should be NULL this time, it's the end of the data */
  if(nextField != NULL) {
    return 1;
  }

  token = isolateToken(nextToken);
  if(readOtherPartsFound(data, token) != 0) {
    return 1;
  }
  /* All done! That wasn't too hard, was it? */
  return 0;
}

static char *isolateToken(char *string) {
  char *beginningOfToken = string;
  while(isspace(*beginningOfToken)) {
    beginningOfToken++;
  }

  char *endOfToken = beginningOfToken;
  while(isalpha(*endOfToken)) {
    endOfToken++;
  }
  *endOfToken = '\0';
  return beginningOfToken;
}



static int readMachineType(DataPoint *data, char const *token) {
  if(!strcasecmp(token, "dragoon")) {
    data->name = dragoon;
  } else if(!strcasecmp(token, "hydra")) {
    data->name = hydra;
  } else {
    return 1;
  }
  return 0;
}

static int readBoxColor(DataPoint *data, char const *token) {
  if(!strcasecmp(token, "red")) {
    data->color = red;
  } else if(!strcasecmp(token, "green")) {
    data->color = green;
  } else if(!strcasecmp(token, "blue")) {
    data->color = blue;
  } else if(!strcasecmp(token, "none")) {
    data->color = unknownColor;
  } else {
    return 1;
  }
  return 0;
}

static int readLocation(DataPoint *data, char const *token) {
  if(!strcasecmp(token, "coral")) {
    data->location = coral;
  } else if(!strcasecmp(token, "houses")) {
    data->location = houses;
  } else if(!strcasecmp(token, "downtown")) {
    data->location = downtown;
  } else if(!strcasecmp(token, "levels")) {
    data->location = levels;
  } else if(!strcasecmp(token, "volcano")) {
    data->location = volcano;
  } else if(!strcasecmp(token, "forest")) {
    data->location = forest;
  } else if(!strcasecmp(token, "golf")) {
    data->location = golf;
  } else if(!strcasecmp(token, "cityhall")) {
    data->location = cityhall;
  } else if(!strcasecmp(token, "undervolcano")) {
    data->location = undervolcano;
  } else if(!strcasecmp(token, "undercity")) {
    data->location = undercity;
  } else if(!strcasecmp(token, "underforest")) {
    data->location = underforest;
  } else if(!strcasecmp(token, "skyplatform")) {
    data->location = skyplatform;
  } else {
    return 1;
  }
  return 0;
}

static int readEvent(DataPoint *data, char const *token) {
  if(!strcasecmp(token, "none")) {
    data->event = noEvent;
  } else if(!strcasecmp(token, "boxes")) {
    data->event = boxes;
  } else if(!strcasecmp(token, "dynablade")) {
    data->event = dynablade;
  } else if(!strcasecmp(token, "fog")) {
    data->event = fog;
  } else if(!strcasecmp(token, "pillar")) {
    data->event = pillar;
  } else if(!strcasecmp(token, "lighthouse")) {
    data->event = lighthouse;
  } else if(!strcasecmp(token, "fakeitems")) {
    data->event = fakeitems;
  } else if(!strcasecmp(token, "burning")) {
    data->event = burning;
  } else if(!strcasecmp(token, "meteors")) {
    data->event = meteors;
  } else if(!strcasecmp(token, "bouncy")) {
    data->event = bouncy;
  } else if(!strcasecmp(token, "tac")) {
    data->event = tac;
  } else if(!strcasecmp(token, "ufo")) {
    data->event = ufo;
  } else if(!strcasecmp(token, "zooming")) {
    data->event = zooming;
  } else if(!strcasecmp(token, "cityhallchamber")) {
    data->event = cityhallchamber;
  } else if(!strcasecmp(token, "restorationspot")) {
    data->event = restorationspot;
  } else {
    return 1;
  }
  return 0;
}

static int readOtherPartsFound(DataPoint *data, char const *token) {
  if(!strcasecmp(token, "no")) {
    data->hadParts = false;
  } else if(!strcasecmp(token, "yes")) {
    data->hadParts = true;
  } else {
    return 1;
  }
  return 0;
}

/* I'm gonna throw in a unit test here */
#ifdef UNITTEST_FILE

int main(void) {
  DataPoint test = {};
  char *testString = strdup("hydra | red | cityhall | boxes | no\n");
  assert(DataPointFromString(&test, testString) == 0);
  assert(test.name == hydra);
  assert(test.color == red);
  assert(test.location == cityhall);
  assert(test.event == boxes);
  assert(test.hadParts == false);

  free(testString);

  DataFile file = {};
  assert(openDataFile(&file, "../data/specialParts") == 0);
  DataPoints points = readDataFile(&file);
  closeDataFile(&file);

  size_t locations[12] = {};
  for(size_t i = 0; i < points.numberUsed; i++) {
    locations[points.data[i].location]++;
  }
  printf("\n");

  for(size_t i = 0; i < 12; i++) {
    printf("%s: %lu\n", locationStrings[i], locations[i]);
  }

  freeDataPoints(&points);
  return 0;
}

#endif  /* !UNITTEST_FILE */
