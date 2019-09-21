#ifndef FILE_H
#define FILE_H

#define isEmpty(string) ((*string) == '\n' || (*string) == '\0')

#define LINE_LENGTH 256
#define SEPARATOR '|'

typedef enum MachineName {
  dragoon,
  hydra
} MachineName;

typedef enum BoxColor {
  red,
  green,
  blue,
  unknownColor
} BoxColor;

typedef enum Location {
  coral,
  houses,
  downtown,
  levels,
  volcano,
  forest,
  golf,
  cityhall,
  underhouses,
  undervolcano,
  undercity,
  skyplatform
} Location;

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

typedef struct DataPoint {
  MachineName name;
  BoxColor color;
  Location location;
  Event event;
  bool hadParts;
} DataPoint;

typedef struct DataPoints {
  size_t size;
  size_t numberUsed;
  DataPoint *data;
} DataPoints;

typedef struct DataFile {
  FILE *file;
  char const *filename;
  size_t lineNumber;
} DataFile;

int openDataFile(DataFile *file, char const *filename);
DataPoints readDataFile(DataFile *file);

#endif  /* FILE_H */
