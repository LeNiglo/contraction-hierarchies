#include "parse_csv.h"

int main(int argc, char** argv) {
  RoadData data = ParseCsvFile(argv[1]);
}
