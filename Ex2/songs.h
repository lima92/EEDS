#include "tones.h"

typedef struct Song{
	uint8_t length;
	uint8_t note[];
} Song;

Song LISA = {TODO, {G4, A5, B5, C5, D5, D5, D5, D5, E5, E5, E5, E5, F5, F5, F5, F5}}