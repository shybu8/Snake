#include <iostream>
#include <raylib.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: img-filename header-filename\n";
    return 1;
  }

  Image img = LoadImage(argv[1]);
  if (!ExportImageAsCode(img, argv[2]))
    return 1;
  return 0;
}
