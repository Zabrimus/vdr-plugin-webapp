#include "convm3u.h"

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Usage %s <m3u File> <config File>\n", argv[0]);
        exit(1);
    }

    auto converter = ConvM3U(argv[1], argv[2]);
    converter.convert();
}