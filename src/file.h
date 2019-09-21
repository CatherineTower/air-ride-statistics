#ifndef FILE_H
#define FILE_H

#define SEPARATOR '|'

typedef enum MachineName {
  dragoon,
  hydra
} MachineName;

char const *machineNameStrings[] = {
  "Dragoon",
  "Hydra"
};

typedef enum BoxColor {
  red,
  green,
  blue,
  unknownColor
} BoxColor;

char const *boxColorStrings[] = {
  "Red",
  "Green",
  "Blue",
  "Unknown color"
};

typedef enum Location {
  coral,
  houses,
  downtown,
  levels,
  volcano,
  forest,
  golf,
  cityhall,
  undervolcano,
  undercity,
  underforest,
  skyplatform
} Location;

char const *locationStrings[] = {
  "Coral",
  "Dilapidated Houses",
  "Downtown",
  "Parking Garage",
  "Volcano",
  "Forest",
  "Golf Course",
  "City Hall",
  "Under the Volcano",
  "Under the City",
  "Under the Forest",
  "Sky Platform"
};

typedef enum Event {
  noEvent,
  boxes,
  dynablade,
  fog,
  pillar,
  lighthouse,
  fakeitems,
  burning,
  meteors,
  bouncy,
  tac,
  ufo,
  zooming,
  cityhallchamber,
  restorationspot
} Event;

char const *eventStrings[] = {
  "No event",
  "All items are the same",
  "Dyna Blade is attacking",
  "A dense fog covers the city",
  "A huge pillar has appeared",
  "The lighthouse has turned on",
  "Some items are fakes",
  "The rail stations are burning",
  "Meteors are falling on the city",
  "Items are bouncy",
  "Tac stole items and is hiding",
  "A UFO has appeared",
  "Star fuel tanks are out of control",
  "City Hall's secret chamber has opened",
  "Restoration spots have appeared"
};

/* This is to hold the collected data. It takes up very little space,
   not more than a few bytes */
typedef struct DataPoint {
  MachineName name;
  BoxColor color;
  Location location;
  Event event;
  bool hadParts;
} DataPoint;

/* A simple structure to hold all of the collected data. This is used
   to minimize the need for dynamic allocation */
typedef struct DataPoints {
  size_t size;
  size_t numberUsed;
  DataPoint *data;
} DataPoints;

/* Mostly this is just for error reporting */
typedef struct DataFile {
  FILE *file;
  char const *filename;
  size_t lineNumber;
} DataFile;

/* These functions all take a pointer to the struct it initializes. I
   don't want to constrain users to dynamic allocation. glibc does
   the same thing for a lot of its functions and I think that's neat. */

/* Opens the file and sets up the DataFile structure */
int openDataFile(DataFile *file, char const *filename);

/* Reads the entire file into an in-memory data structure. Will
   report errors on stderr */
DataPoints readDataFile(DataFile *file);
void closeDataFile(DataFile *file);
void freeDataPoints(DataPoints *points);

#endif  /* FILE_H */
