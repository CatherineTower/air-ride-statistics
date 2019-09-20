# air-ride-statistics
Statistical analysis of finding Dragoon and Hydra parts in Kirby Air Ride

I had this idea when I was playing Kirby Air Ride as a child that the
special parts would appear under specific circumstances. When I picked
up the game again recently, I found that my childhood beliefs didn't
hold at all. So I decided to do something about it.

## Data File Format

The format of the data file is a pipe-separated table which supports
single-line comments. The proper format is as follows:

```
# This is a comment
MachineName | BoxColor | LocationFound | EventsOccurring | HadOtherParts?
```

The fields can have as much whitespace as you want before or after the
pipes, excluding newlines.

## Contributing

If you've got the game, feel free to create your own data files and
send them in. The more data, the better. In addition, I only know what
I know about statistics, so if you see any good ways this can be
improved, let me know.
