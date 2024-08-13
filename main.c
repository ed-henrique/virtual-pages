#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NDEBUG
#define PAGESIZE 4096

int main() {
  FILE *f;
  long fileSize, virtualPageCount, virtualPageOffsetDiff;

  if ((f = fopen("./big-file.txt", "r")) == NULL) {
    printf("ERROR: could not open file!\n");
    exit(1);
  }

  fseek(f, 0, SEEK_END);
  fileSize = ftell(f);
  fseek(f, 0, SEEK_SET);

  virtualPageCount = fileSize / PAGESIZE;
  virtualPageOffsetDiff = fileSize % PAGESIZE;

  printf("Our big-file.txt has %ld bytes.\n\n", fileSize);
  printf(
    "This is equivalent to %ld pages in virtual memory and an offset difference of %ld.\n\n",
    virtualPageCount,
    virtualPageOffsetDiff
  );

  char *data = 0;
  data = malloc(fileSize);
  if (!data) {
    printf("ERROR: could not allocate memory!\n");
    exit(1);
  }

  long readSize = fread(data, 1, fileSize, f);
  if (readSize != fileSize) {
    printf("ERROR: read size is different from file size!\n");
    exit(1);
  }

  char *lastDataAddress = data + fileSize - 1;

  uintptr_t dataAddressAsInt = (uintptr_t) data;
  uintptr_t dataAddressPageNumber = dataAddressAsInt >> 12;
  uintptr_t dataAddressPageOffset = dataAddressAsInt & 0xFFF;

  uintptr_t lastDataAddressAsInt = (uintptr_t) lastDataAddress;
  uintptr_t lastDataAddressPageNumber = lastDataAddressAsInt >> 12;
  uintptr_t lastDataAddressPageOffset = lastDataAddressAsInt & 0xFFF;

  // Assert that the last 5 bytes of data are equal to the
  // last 5 bytes of our big file
  assert(*(lastDataAddress - 4) == 'k');
  assert(*(lastDataAddress - 3) == 's');
  assert(*(lastDataAddress - 2) == '.');
  assert(*(lastDataAddress - 1) == '\r');
  assert(*(lastDataAddress)     == '\n');

  printf("Format (HEX):            PAGE_NUMBER PAGE_OFFSET\n");
  printf(
    "Data address (HEX):      0x%lx %03lx\n",
    dataAddressPageNumber,
    dataAddressPageOffset
  );
  printf(
    "Last data address (HEX): 0x%lx %03lx\n",
    lastDataAddressPageNumber,
    lastDataAddressPageOffset
  );

  printf("\nFormat (DEC):            PAGE_NUMBER PAGE_OFFSET\n");
  printf(
    "Data address (DEC):      %ld %03ld\n",
    dataAddressPageNumber,
    dataAddressPageOffset
  );
  printf(
    "Last data address (DEC): %ld %03ld\n",
    lastDataAddressPageNumber,
    lastDataAddressPageOffset
  );

  fclose(f);
  free(data);
  free(lastDataAddress);

  return 0;
}
